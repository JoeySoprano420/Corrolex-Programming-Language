Will this catch errors?

#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/PassManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Analysis/ProfileSummaryInfo.h>
#include <llvm/Transforms/IPO.h>

using namespace llvm;

class CodeGenerator {
public:
    CodeGenerator() {
        LLVMInitializeNativeTarget();
        LLVMInitializeNativeTargetAsmPrinter();
        LLVMInitializeNativeTargetAsmParser();
        context = std::make_unique<LLVMContext>();
        module = std::make_unique<Module>("MyModule", *context);
        builder = std::make_unique<IRBuilder<>(*context>);
        fpm = std::make_unique<legacy::FunctionPassManager>(module.get());
    }

    void generateCodeForAssignment(VariableNode* varNode) {
        // Generate code for assignment, including constant folding if possible
        Value* value = evaluateExpression(varNode->getValue());
        if (auto* constant = dyn_cast<ConstantInt>(value)) {
            // Constant folding: directly assign constant value to variable
            builder->CreateStore(constant, varNode->getStorageLocation());
        } else {
            builder->CreateStore(value, varNode->getStorageLocation());
        }
    }

    void generateCodeForIfStatement(IfStatementNode* ifStmt) {
        // Generate code for if-else branches
        BasicBlock* thenBB = BasicBlock::Create(*context, "then", currentFunction);
        BasicBlock* elseBB = BasicBlock::Create(*context, "else", currentFunction);
        BasicBlock* mergeBB = BasicBlock::Create(*context, "merge", currentFunction);

        builder->CreateCondBr(ifStmt->getCondition(), thenBB, elseBB);

        builder->SetInsertPoint(thenBB);
        generateCodeForStatement(ifStmt->getThenBranch());
        builder->CreateBr(mergeBB);

        builder->SetInsertPoint(elseBB);
        generateCodeForStatement(ifStmt->getElseBranch());
        builder->CreateBr(mergeBB);

        builder->SetInsertPoint(mergeBB);
    }

    void generateCodeForWhileLoop(WhileLoopNode* loopNode) {
        // Generate code for a while loop
        BasicBlock* loopBB = BasicBlock::Create(*context, "loop", currentFunction);
        BasicBlock* exitBB = BasicBlock::Create(*context, "exit", currentFunction);

        builder->CreateBr(loopBB);
        builder->SetInsertPoint(loopBB);

        Value* condition = evaluateExpression(loopNode->getCondition());
        builder->CreateCondBr(condition, exitBB, loopBB);

        builder->SetInsertPoint(exitBB);
    }

    void enableOptimizations() {
        // Enable LLVM's built-in optimization passes
        fpm->add(createPromoteMemoryToRegisterPass());
        fpm->add(createDeadStoreEliminationPass());
        fpm->add(createInstructionCombiningPass());
        fpm->add(createReassociatePass());
        fpm->add(createGVNPass());
        fpm->add(createCFGSimplificationPass());
        fpm->add(createDeadCodeEliminationPass());
        fpm->add(createLoopUnrollPass());

        fpm->doInitialization();
    }

    void applyOptimizations() {
        // Apply profile-guided optimizations (PGO)
        applyPGOOptimizations();

        // Run optimization passes on the module
        legacy::PassManager pm;
        pm.add(createConstantPropagationPass());
        pm.add(createGVNPass());
        pm.add(createDeadCodeEliminationPass());
        pm.add(createLoopUnrollPass());
        pm.add(createLoopInvariantCodeMotionPass());  // Loop invariant code motion
        pm.add(createLoopFusionPass());               // Loop fusion
        pm.add(createLoopSimplifyPass());             // Simplify loop structures
        pm.run(*module);
    }

private:
    std::unique_ptr<LLVMContext> context;
    std::unique_ptr<Module> module;
    std::unique_ptr<IRBuilder<>> builder;
    std::unique_ptr<legacy::FunctionPassManager> fpm;
    Function* currentFunction;

    Value* evaluateExpression(ExpressionNode* exprNode) {
        // A simplified example of expression evaluation
        if (exprNode->isConstant()) {
            return ConstantInt::get(*context, APInt(32, exprNode->getConstantValue()));
        } else {
            // Handle non-constant expressions, for example loading values
            return builder->CreateLoad(exprNode->getStorageLocation());
        }
    }

    void generateCodeForStatement(StatementNode* stmtNode) {
        // Placeholder: generate code for a generic statement
        if (stmtNode->isIfStatement()) {
            generateCodeForIfStatement(dynamic_cast<IfStatementNode*>(stmtNode));
        } else if (stmtNode->isWhileLoop()) {
            generateCodeForWhileLoop(dynamic_cast<WhileLoopNode*>(stmtNode));
        }
    }

    void applyPGOOptimizations() {
        // Enable profile-guided optimizations (PGO)
        legacy::PassManager pm;

        // Apply profile-based optimizations such as function inlining
        pm.add(createFunctionInliningPass());
        pm.add(createEarlyCSEPass());        // Early common subexpression elimination
        pm.add(createTailCallEliminationPass());
        pm.add(createReassociatePass());

        // Apply ProfileSummaryInfo to guide optimization
        TargetLibraryInfoWrapperPass* TLIWP = new TargetLibraryInfoWrapperPass(*context);
        pm.add(TLIWP);

        pm.run(*module);
    }
};

// Example optimization-related transformations

class ConstantFoldingPass : public ModulePass {
public:
    static char ID;
    ConstantFoldingPass() : ModulePass(ID) {}

    bool runOnModule(Module& M) override {
        bool changed = false;

        for (Function& F : M) {
            for (BasicBlock& BB : F) {
                for (Instruction& I : BB) {
                    if (auto* binaryOp = dyn_cast<BinaryOperator>(&I)) {
                        // Check if operands are constants and fold them
                        if (isa<Constant>(binaryOp->getOperand(0)) && isa<Constant>(binaryOp->getOperand(1))) {
                            Constant* result = binaryOp->getOperand(0)->fold(binaryOp->getOpcode(), binaryOp->getOperand(1));
                            if (result) {
                                binaryOp->replaceAllUsesWith(result);
                                BB.getInstList().erase(&I);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }

        return changed;
    }
};
char ConstantFoldingPass::ID = 0;

class LoopUnrollPass : public ModulePass {
public:
    static char ID;
    LoopUnrollPass() : ModulePass(ID) {}

    bool runOnModule(Module& M) override {
        bool changed = false;

        for (Function& F : M) {
            for (BasicBlock& BB : F) {
                for (Instruction& I : BB) {
                    if (auto* loop = dyn_cast<PHINode>(&I)) {
                        // Check if it's a loop that can be unrolled
                        if (loop->getNumOperands() == 1 && loop->getOperand(0) == loop) {
                            // Unroll the loop
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }

        return changed;
    }
};
char LoopUnrollPass::ID = 1;

int main() {
    CodeGenerator codeGen;

    // Enable optimizations
    codeGen.enableOptimizations();

    // Create some mock AST nodes (simplified version)
    VariableNode* varNode = new VariableNode();
    IfStatementNode* ifNode = new IfStatementNode();
    WhileLoopNode* loopNode = new WhileLoopNode();

    // Generate code for AST nodes
    codeGen.generateCodeForAssignment(varNode);
    codeGen.generateCodeForIfStatement(ifNode);
    codeGen.generateCodeForWhileLoop(loopNode);

    // Apply LLVM optimizations including PGO
    codeGen.applyOptimizations();

    std::cout << "Code Generation Complete with PGO and Advanced Optimizations!" << std::endl;

    return 0;
}

IfStatementNode* ifStmt = dynamic_cast<IfStatementNode*>(stmtNode);
if (!ifStmt) {
    // Handle the error
    std::cerr << "Invalid cast to IfStatementNode!" << std::endl;
    return;
}

legacy::PassManager pm;
pm.add(createConstantPropagationPass());
pm.add(createGVNPass());
pm.run(*module);

if (!varNode) {
    std::cerr << "Error: VariableNode is null!" << std::endl;
    return;
}

void generateCodeForAssignment(VariableNode* varNode) {
    if (!varNode) {
        std::cerr << "Error: VariableNode is null!" << std::endl;
        return;
    }
    
    Value* value = evaluateExpression(varNode->getValue());
    if (auto* constant = dyn_cast<ConstantInt>(value)) {
        builder->CreateStore(constant, varNode->getStorageLocation());
    } else {
        builder->CreateStore(value, varNode->getStorageLocation());
    }
}

IfStatementNode* ifStmt = dynamic_cast<IfStatementNode*>(stmtNode);
if (!ifStmt) {
    std::cerr << "Invalid cast to IfStatementNode!" << std::endl;
    return;
}

if (!varNode) {
    std::cerr << "Error: VariableNode is null!" << std::endl;
    return;
}

if (profileDataUnavailable) {
    std::cerr << "Warning: Profile-guided optimization skipped due to missing profile data." << std::endl;
}

void generateCodeForAssignment(VariableNode* varNode) {
    if (!varNode) {
        std::cerr << "Error: VariableNode is null!" << std::endl;
        return;
    }

    try {
        Value* value = evaluateExpression(varNode->getValue());
        if (auto* constant = dyn_cast<ConstantInt>(value)) {
            builder->CreateStore(constant, varNode->getStorageLocation());
        } else {
            builder->CreateStore(value, varNode->getStorageLocation());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation for assignment: " << e.what() << std::endl;
    }
}

if (!varNode) {
    std::cerr << "Error: VariableNode is null!" << std::endl;
    return;
}

IfStatementNode* ifStmt = dynamic_cast<IfStatementNode*>(stmtNode);
if (!ifStmt) {
    std::cerr << "Invalid cast to IfStatementNode!" << std::endl;
    return;
}

if (profileDataUnavailable) {
    std::cerr << "Warning: Profile-guided optimization skipped due to missing profile data." << std::endl;
}

try {
    Value* value = evaluateExpression(varNode->getValue());
    if (auto* constant = dyn_cast<ConstantInt>(value)) {
        builder->CreateStore(constant, varNode->getStorageLocation());
    } else {
        builder->CreateStore(value, varNode->getStorageLocation());
    }
} catch (const std::exception& e) {
    std::cerr << "Error during expression evaluation: " << e.what() << std::endl;
}

try {
    // LLVM-specific code
} catch (const llvm::Exception& e) {
    std::cerr << "LLVM error occurred: " << e.what() << std::endl;
}

std::ofstream errorLog("error_log.txt", std::ios::app);
errorLog << "Error: VariableNode is null!" << std::endl;

assert(varNode && "VariableNode is null!");

void generateCodeForAssignment(VariableNode* varNode) {
    if (!varNode) {
        std::cerr << "Error: VariableNode is null!" << std::endl;
        return;
    }

    try {
        Value* value = evaluateExpression(varNode->getValue());
        if (auto* constant = dyn_cast<ConstantInt>(value)) {
            builder->CreateStore(constant, varNode->getStorageLocation());
        } else {
            builder->CreateStore(value, varNode->getStorageLocation());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation for assignment: " << e.what() << std::endl;
    }
}

void generateCodeForIfStatement(IfStatementNode* ifStmt) {
    if (!ifStmt) {
        std::cerr << "Invalid cast to IfStatementNode!" << std::endl;
        return;
    }

    try {
        BasicBlock* thenBB = BasicBlock::Create(*context, "then", currentFunction);
        BasicBlock* elseBB = BasicBlock::Create(*context, "else", currentFunction);
        BasicBlock* mergeBB = BasicBlock::Create(*context, "merge", currentFunction);

        builder->CreateCondBr(ifStmt->getCondition(), thenBB, elseBB);

        builder->SetInsertPoint(thenBB);
        generateCodeForStatement(ifStmt->getThenBranch());
        builder->CreateBr(mergeBB);

        builder->SetInsertPoint(elseBB);
        generateCodeForStatement(ifStmt->getElseBranch());
        builder->CreateBr(mergeBB);

        builder->SetInsertPoint(mergeBB);
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation for if statement: " << e.what() << std::endl;
    }
}

void generateCodeForWhileLoop(WhileLoopNode* loopNode) {
    if (!loopNode) {
        std::cerr << "Invalid cast to WhileLoopNode!" << std::endl;
        return;
    }

    try {
        BasicBlock* loopBB = BasicBlock::Create(*context, "loop", currentFunction);
        BasicBlock* exitBB = BasicBlock::Create(*context, "exit", currentFunction);

        builder->CreateBr(loopBB);
        builder->SetInsertPoint(loopBB);

        Value* condition = evaluateExpression(loopNode->getCondition());
        builder->CreateCondBr(condition, exitBB, loopBB);

        builder->SetInsertPoint(exitBB);
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation for while loop: " << e.what() << std::endl;
    }
}

void applyOptimizations() {
    try {
        legacy::PassManager pm;
        pm.add(createConstantPropagationPass());
        pm.add(createGVNPass());
        pm.run(*module);
    } catch (const std::exception& e) {
        std::cerr << "Error during optimization pass: " << e.what() << std::endl;
    }
}

if (!stmtNode) {
    std::cerr << "Error: StatementNode is null!" << std::endl;
    return;
}

IfStatementNode* ifStmt = dynamic_cast<IfStatementNode*>(stmtNode);
if (!ifStmt) {
    std::cerr << "Error: Invalid cast to IfStatementNode!" << std::endl;
    return;
}

assert(varNode != nullptr && "VariableNode cannot be null!");

if (!fpm) {
    std::cerr << "Error: Function pass manager is not initialized!" << std::endl;
    return;
}

if (profileDataUnavailable) {
    std::cerr << "Warning: Profile-guided optimization skipped due to missing profile data." << std::endl;
}

Expected<Function*> funcOrError = module->getFunction("someFunction");
if (!funcOrError) {
    std::cerr << "Error: Function not found in module!" << std::endl;
    return;
}

Expected<Function*> funcOrError = module->getFunction("someFunction");
if (!funcOrError) {
    std::cerr << "Error: Function not found in module!" << std::endl;
    return;
}

delete varNode;  // If you own the node

try {
    generateCodeForAssignment(varNode);
} catch (const std::exception& e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
}

if (!varNode) {
    std::cerr << "Error: VariableNode is null!" << std::endl;
    return;
}

IfStatementNode* ifStmt = dynamic_cast<IfStatementNode*>(stmtNode);
if (!ifStmt) {
    std::cerr << "Invalid cast to IfStatementNode!" << std::endl;
    return;
}

if (profileDataUnavailable) {
    std::cerr << "Warning: Profile-guided optimization skipped due to missing profile data." << std::endl;
}

try {
    // Code that might throw exceptions
} catch (const std::exception& e) {
    std::cerr << "Error occurred: " << e.what() << std::endl;
    // Handle exception
}

void generateCodeForAssignment(VariableNode* varNode) {
    if (!varNode) {
        std::cerr << "Error: VariableNode is null!" << std::endl;
        return;
    }

    try {
        Value* value = evaluateExpression(varNode->getValue());
        if (auto* constant = dyn_cast<ConstantInt>(value)) {
            builder->CreateStore(constant, varNode->getStorageLocation());
        } else {
            builder->CreateStore(value, varNode->getStorageLocation());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error while generating code for assignment: " << e.what() << std::endl;
        return;
    }
}

import log from "logging-library";

enum TokenType {
    Keyword, Identifier, Number, Float, Complex, Operator, 
    Comment, Punctuation, End, Error, Char
}

typed struct Token {
    TokenType type;
    DynamicToken value;
}

keyword_map Keywords {
    "if", "else", "while", "return", 
    "int", "float", "char", "complex" ::Keyword;
}

// Function to check if a word is a keyword
func isKeyword(word::str) -> bool {
    return Keywords.contains(word);
}

// Exception handling for token errors
exception.block TokenError {
    invalidChar: "Unrecognized character in input stream: {0}",
    unknownKeyword: "Unknown keyword: {0}",
    malformedNumber: "Malformed number at: {0}",
    malformedFloat: "Malformed float at: {0}",
    malformedChar: "Malformed character literal at: {0}",
    malformedComplex: "Malformed complex number at: {0}",
    unexpectedEndOfInput: "Unexpected end of input during tokenization",
}

// Method to get the next token from the input stream
method Stream.nextToken(self) -> Token {
    self.skipWhitespace();

    // Check if the end of the stream is reached
    if self.isEnd() {
        log.info("End of stream reached.");
        return Token(TokenType::End, "");
    }

    // Get the current character for tokenization
    let currentChar = self.peek();
    
    // Handle single-line comments (starting with //)
    if currentChar == '/' and self.peekNext() == '/' {
        return self.consumeUntil('\n', TokenType::Comment);
    }

    // Handle multi-line comments (starting with /* and ending with */)
    if currentChar == '/' and self.peekNext() == '*' {
        return self.consumeUntil('*/', TokenType::Comment);
    }

    // Handle keywords and identifiers
    if currentChar.isAlpha() or currentChar == '_' {
        let word = self.consumeWhile(char::isAlphaNumeric or '_');
        if isKeyword(word) {
            log.info("Keyword found: {0}", word);
            return Token(TokenType::Keyword, word);
        }
        log.info("Identifier found: {0}", word);
        return Token(TokenType::Identifier, word);
    }

    // Handle numbers (with error handling for malformed numbers)
    if currentChar.isDigit() {
        let number = self.consumeWhile(char::isDigit);
        if self.peek() == '.' {
            return self.consumeFloat(number);
        }
        if !number.isValidNumber() {
            throw TokenError.malformedNumber.format(number);
        }
        log.info("Number found: {0}", number);
        return Token(TokenType::Number, number);
    }

    // Handle floating point numbers
    if currentChar == '.' {
        let number = self.consumeWhile(char::isDigit);
        if !number.isValidFloat() {
            throw TokenError.malformedFloat.format(number);
        }
        log.info("Floating point number found: {0}", number);
        return Token(TokenType::Float, number);
    }

    // Handle complex numbers (e.g., 3.0 + 4.0i)
    if currentChar.isDigit() or currentChar == '-' or currentChar == '+' {
        let realPart = self.consumeWhile(char::isDigit or '.');
        if self.peek() == '+' or self.peek() == '-' {
            let imaginaryPart = self.consumeWhile(char::isDigit or '.');
            if self.peek() == 'i' {
                self.consume();  // Consume 'i' for complex number
                let complexNumber = realPart + self.peek() + imaginaryPart + 'i';
                log.info("Complex number found: {0}", complexNumber);
                return Token(TokenType::Complex, complexNumber);
            } else {
                throw TokenError.malformedComplex.format(realPart + "+" + imaginaryPart);
            }
        }
    }

    // Handle character literals (e.g., 'a', '1', '\\n')
    if currentChar == '\'' {
        return self.consumeCharLiteral();
    }

    // Handle operators (+, -, *, /, =, <, >)
    if currentChar in "+-*/=<>" {
        return Token(TokenType::Operator, self.consume());
    }

    // Handle punctuation (semicolon, parentheses, braces)
    if currentChar in ";(){}" {
        return Token(TokenType::Punctuation, self.consume());
    }

    // Error handling for unrecognized characters
    throw TokenError.invalidChar.format(currentChar);
}

// Method to consume characters until a specific character or sequence is found
method Stream.consumeUntil(stopChar::char, tokenType::TokenType) -> Token {
    let content = "";
    while !self.isEnd() and self.peek() != stopChar {
        content += self.consume();
    }
    if self.peek() == stopChar {
        content += self.consume();  // Consume the stop character (e.g., '\n' for comments)
    }
    log.info("Comment token found: {0}", content);
    return Token(tokenType, content);
}

// Method to consume a floating-point number
method Stream.consumeFloat(initialNumber::str) -> Token {
    let number = initialNumber;
    if self.peek() == '.' {
        number += self.consume();  // Consume the dot
    }
    number += self.consumeWhile(char::isDigit);
    
    // Optional: Handle scientific notation (e.g., 2e3, 3.14e-2)
    if self.peek() in "eE" {
        number += self.consume();  // Consume 'e' or 'E'
        if self.peek() in "+-" {
            number += self.consume();  // Consume the sign
        }
        number += self.consumeWhile(char::isDigit);
    }

    if !number.isValidFloat() {
        throw TokenError.malformedFloat.format(number);
    }

    log.info("Floating-point number token found: {0}", number);
    return Token(TokenType::Float, number);
}

// Method to consume a character literal (e.g., 'a', '1', '\\n')
method Stream.consumeCharLiteral() -> Token {
    let charLiteral = "";
    self.consume();  // Consume the opening quote (')

    while !self.isEnd() and self.peek() != '\'' {
        let char = self.consume();
        if char == '\\' {
            char = self.consumeEscapeSequence();
        }
        charLiteral += char;
    }

    if self.peek() == '\'' {
        charLiteral += self.consume();  // Consume the closing quote
    } else {
        throw TokenError.malformedChar.format(charLiteral);
    }

    log.info("Character literal found: {0}", charLiteral);
    return Token(TokenType::Char, charLiteral);
}

// Method to handle escape sequences in character literals
method Stream.consumeEscapeSequence() -> char {
    let escapeChar = self.consume();  // Consume the backslash
    match escapeChar {
        'n' -> return '\n';
        't' -> return '\t';
        'r' -> return '\r';
        'b' -> return '\b';
        '0' -> return '\0';
        _ -> throw TokenError.invalidChar.format("\\" + escapeChar);
    }
}

void generateCodeForAssignment(VariableNode* varNode) {
    if (!varNode) {
        std::cerr << "Error: VariableNode is null!" << std::endl;
        return;
    }

    try {
        Value* value = evaluateExpression(varNode->getValue());
        if (auto* constant = dyn_cast<ConstantInt>(value)) {
            builder->CreateStore(constant, varNode->getStorageLocation());
        } else {
            builder->CreateStore(value, varNode->getStorageLocation());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation for assignment: " << e.what() << std::endl;
    }
}

void generateCodeForIfStatement(IfStatementNode* ifStmt) {
    if (!ifStmt) {
        std::cerr << "Invalid cast to IfStatementNode!" << std::endl;
        return;
    }

    try {
        BasicBlock* thenBB = BasicBlock::Create(*context, "then", currentFunction);
        BasicBlock* elseBB = BasicBlock::Create(*context, "else", currentFunction);
        BasicBlock* mergeBB = BasicBlock::Create(*context, "merge", currentFunction);

        builder->CreateCondBr(ifStmt->getCondition(), thenBB, elseBB);

        builder->SetInsertPoint(thenBB);
        generateCodeForStatement(ifStmt->getThenBranch());
        builder->CreateBr(mergeBB);

        builder->SetInsertPoint(elseBB);
        generateCodeForStatement(ifStmt->getElseBranch());
        builder->CreateBr(mergeBB);

        builder->SetInsertPoint(mergeBB);
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation for if statement: " << e.what() << std::endl;
    }
}

void generateCodeForWhileLoop(WhileLoopNode* loopNode) {
    if (!loopNode) {
        std::cerr << "Invalid cast to WhileLoopNode!" << std::endl;
        return;
    }

    try {
        BasicBlock* loopBB = BasicBlock::Create(*context, "loop", currentFunction);
        BasicBlock* exitBB = BasicBlock::Create(*context, "exit", currentFunction);

        builder->CreateBr(loopBB);
        builder->SetInsertPoint(loopBB);

        Value* condition = evaluateExpression(loopNode->getCondition());
        builder->CreateCondBr(condition, exitBB, loopBB);

        generateCodeForStatement(loopNode->getBody());
        builder->CreateBr(loopBB);
        builder->SetInsertPoint(exitBB);
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation for while loop: " << e.what() << std::endl;
    }
}

std::string findVariableDeclaration(const std::string& varName) {
    if (varDeclarations.count(varName)) {
        return varDeclarations[varName];
    } else {
        throw std::runtime_error("Variable not declared: " + varName);
    }
}
