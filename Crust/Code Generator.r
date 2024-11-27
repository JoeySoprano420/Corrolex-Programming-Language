pub fn generate_code(ast: ASTNode) -> String {
    match ast {
        ASTNode::Module { name, body } => {
            let mut code = format!("// Module: {}\n", name);
            for node in body {
                code.push_str(&generate_code(node));
            }
            code
        }
        ASTNode::Function { name, params, body } => {
            let mut code = format!("fn {}({}) {{\n", name, params.join(", "));
            for node in body {
                code.push_str(&generate_code(node));
            }
            code.push_str("}\n");
            code
        }
        ASTNode::Assignment { variable, value } => {
            format!("let {} = {};\n", variable, value)
        }
        ASTNode::Conditional { condition, actions } => {
            let mut code = format!("if ({}) {{\n", condition);
            for action in actions {
                code.push_str(&generate_code(action));
            }
            code.push_str("}\n");
            code
        }
        ASTNode::Expression { expression } => {
            format!("{}\n", expression)
        }
    }
}
