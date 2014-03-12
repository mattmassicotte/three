#include "FunctionDefinitionNode.h"
#include "../../Parser.h"
#include "../../Constructs/Variable.h"
#include "../Control/EnsureNode.h"

#include <assert.h>
#include <sstream>

namespace Three {
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        FunctionDefinitionNode* node = new FunctionDefinitionNode();

        node->_ensureBlock = nullptr;
        node->setVisibility(parser.context()->visibility());

        // take (and then clear) current annotations
        node->setAnnotations(parser.currentScope()->annotations());
        parser.currentScope()->clearAnnotations();

        node->_function = parser.parseFunctionSignature();
        assert(node->_function);

        parser.parseNewline();

        // define the function in the current module
        parser.currentModule()->addFunction(node->_function->fullyQualifiedName(), node->_function);

        // define a variable for the function, so it can be referred to
        Variable* var = new Variable();

        // use the fullyQualifiedName here, so when it is codegen'ed, we get the
        // right symbol
        var->setName(node->_function->fullyQualifiedName());

        // TODO: this createType usage is a leak
        var->setType(TypeReference(node->_function->createType(), 1));

        parser.currentScope()->addVariable(node->_function->fullyQualifiedName(), var);

        // push in a new scope for the function body
        parser.pushScope(new Scope(node->_function->fullyQualifiedName()));

        // define new variables for all parameters
        node->_function->eachParameterWithLast([&] (Variable* v, bool last) {
            parser.currentScope()->addVariable(v->name(), v);
        });

        // parse until we find an end or an ensure
        parser.parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::KeywordEnd:
                    return true;
                case Token::Type::KeywordEnsure:
                    node->_ensureBlock = EnsureNode::parse(parser);
                    return true;
                default:
                    node->addChild(parser.parseStatement());
                    return false;
            }
        });

        if (!node->_ensureBlock) {
            // the ensure parses the end, so we'll only have one if we didn't
            // encounter an ensure
            assert(parser.next().type() == Token::Type::KeywordEnd);
        }

        parser.popScope();
        parser.parseNewline();

        return node;
    }

    std::string FunctionDefinitionNode::name() const {
        return "FunctionDefinition";
    }

    std::string FunctionDefinitionNode::str() const {
        std::stringstream s;

        s << this->name() << ": " << this->function()->str();

        return s.str();
    }

    void FunctionDefinitionNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    Function* FunctionDefinitionNode::function() const {
        return _function;
    }

    ASTNode* FunctionDefinitionNode::ensureClause() const {
        return _ensureBlock;
    }

    void FunctionDefinitionNode::codeGen(CSourceContext& context) {
        context.adjustCurrentForVisibility(this->visibility(), [&] (CSource* source) {
            *source << this->function()->codeGen();
            source->printLine(";");
        });

        context << this->function()->codeGen();
        context.current()->printLineAndIndent(" {");

        this->codeGenChildren(context);

        context.current()->outdentAndPrintLine("}");
        context.current()->printLine(""); // add an extra newline for separation
    }
}
