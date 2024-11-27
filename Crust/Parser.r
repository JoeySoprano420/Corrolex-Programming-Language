#[derive(Debug)]
pub enum ASTNode {
    Module { name: String, body: Vec<ASTNode> },
    Function { name: String, params: Vec<String>, body: Vec<ASTNode> },
    Assignment { variable: String, value: String },
    Conditional { condition: String, actions: Vec<ASTNode> },
    Expression { expression: String },
}

pub fn parse(tokens: Vec<Token>) -> ASTNode {
    let mut iter = tokens.iter();
    let mut ast = Vec::new();

    while let Some(token) = iter.next() {
        match token {
            Token::Keyword(k) if k == "module" => {
                let name = match iter.next() {
                    Some(Token::Identifier(id)) => id,
                    _ => panic!("Expected module name"),
                };
                ast.push(ASTNode::Module {
                    name: name.clone(),
                    body: parse_module_body(&mut iter),
                });
            }
            _ => continue,
        }
    }

    ASTNode::Module {
        name: "root".to_string(),
        body: ast,
    }
}

fn parse_module_body<'a>(iter: &mut std::slice::Iter<'a, Token>) -> Vec<ASTNode> {
    let mut body = Vec::new();

    while let Some(token) = iter.next() {
        match token {
            Token::Keyword(k) if k == "function" => {
                let name = match iter.next() {
                    Some(Token::Identifier(id)) => id,
                    _ => panic!("Expected function name"),
                };
                body.push(ASTNode::Function {
                    name: name.clone(),
                    params: vec![],
                    body: vec![], // Placeholder, add more parsing logic here.
                });
            }
            _ => continue,
        }
    }

    body
}
