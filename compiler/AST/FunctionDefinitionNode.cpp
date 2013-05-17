#include "FunctionDefinitionNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        FunctionDefinitionNode* node;
        Token t;

        node = new FunctionDefinitionNode();
        node->_function = new Function();

        // parse the function name
        t = parser.next();
        assert(t.type() == Token::Type::Identifier);

        node->_function->setName(t.str());

        // move past the opening '('
        assert(parser.next().str().at(0) == '(');

        // at each point we could have:
        // - a ';'
        // - a Type + identifier, followed by a ';'
        // - a Type + identifier, followed by a ','
        // - a ')'
        while (true) {
            Token t = parser.peek();

            if (t.str().at(0) == ';' || t.str().at(0) == ')') {
                break;
            }

            Variable* v = new Variable();

            v->setDataType(DataType::parse(parser));
            
            assert(parser.peek().type() == Token::Type::Identifier);
            v->setName(parser.next().str());

            node->_function->addParameter(v);

            // a ',' means another paramter was specified
            if (parser.peek().str().at(0) == ',') {
                parser.next();
                continue;
            }
        }

        // now, check for the return type
        if (parser.peek().str().at(0) == ';' && parser.peek(2).str().at(0) != ')') {
            // move past the ';'
            assert(parser.next().str().at(0) == ';');

            node->_function->setReturnType(DataType::parse(parser));
        } else {
            node->_function->setReturnType(DataType());
        }

        // parse the close paren
        assert(parser.next().str().at(0) == ')');
        assert(parser.next().type() == Token::Type::Newline);

        // and now, parse the body
        while (true) {
            if (parser.peek().type() == Token::Type::KeywordEnd) {
                break;
            }

            node->addChild(parser.parseStatement());
        }

        assert(parser.next().type() == Token::Type::KeywordEnd);
        assert(parser.next().type() == Token::Type::Newline);

        node->setStatement(true);

        return node;
    }

    std::string FunctionDefinitionNode::name() const {
        return "FunctionDefinition";
    }

    Function* FunctionDefinitionNode::function() const {
        return _function;
    }

    std::string FunctionDefinitionNode::str() const {
        std::stringstream s;

        s << this->name() << ": " << this->function()->str();

        return s.str();
    }

    void FunctionDefinitionNode::codeGenCSource(CSourceContext& context) {
        Function* f = this->function();

        assert(f);
        f->returnType().codeGenCSource(context);

        context.print(" ");
        context.print(f->name());
        context.print("(");

        f->eachParameter([=, &context] (Variable* var, uint32_t index) {
            var->dataType().codeGenCSource(context);
            context.print(" ");
            context.print(var->name());

            if (index < (f->parameterCount() - 1)) {
                context.print(", ");
            }
        });
        
        if (f->parameterCount() == 0) {
            context.print("void");
        }

        context.print(") {");
        context.increaseIndentation();
        context.printNewLine();

        this->codeGenCSourceForChildren(context);

        context.decreaseIndentation();
        context.printNewLine();
        context.print("}");
        context.printNewLine();
    }
}
