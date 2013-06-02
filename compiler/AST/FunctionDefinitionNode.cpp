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
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        // at each point we could have:
        // - a ';'
        // - a Type + identifier, followed by a ';'
        // - a Type + identifier, followed by a ','
        // - a ')'
        while (true) {
            Token t = parser.peek();

            if (t.str().at(0) == ';' || t.type() == Token::Type::PunctuationCloseParen) {
                break;
            }

            TypeReference type = parser.parseType();

            node->_function->addParameter(parser.next().str(), type);

            // a ',' means another paramter was specified
            if (parser.peek().str().at(0) == ',') {
                parser.next();
                continue;
            }
        }

        // now, check for the return type
        if (parser.peek().str().at(0) == ';' && parser.peek(2).type() != Token::Type::PunctuationCloseParen) {
            // move past the ';'
            assert(parser.next().str().at(0) == ';');

            node->_function->setReturnType(parser.parseType());
        } else {
            node->_function->setReturnType(TypeReference::ref(parser.currentModule(), "Void", 0));
        }

        // parse the close paren
        assert(parser.next().type() == Token::Type::PunctuationCloseParen);
        assert(parser.next().type() == Token::Type::Newline);

        // push in a new scope for the function body
        parser.pushScope(new Scope(node->_function->name()));

        // and now, parse the body
        while (true) {
            if (parser.peek().type() == Token::Type::KeywordEnd) {
                break;
            }

            node->addChild(parser.parseStatement());
        }

        parser.popScope();

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

        f->returnType().codeGenCSource(context.current(), "");

        context.current()->print(" ");
        context.current()->print(f->name());
        context.current()->print("(");

        f->eachParameterWithLast([=, &context] (Variable* var, bool last) {
            var->type().codeGenCSource(context.current(), var->name());
            if (!last) {
                context.current()->print(", ");
            }
        });
        
        if (f->parameterCount() == 0) {
            context.current()->print("void");
        }

        context.current()->printLineAndIndent(") {");

        this->codeGenCSourceForChildren(context);

        context.current()->outdentAndPrintLine("}");
    }
}
