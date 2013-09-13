#include "FunctionDefinitionNode.h"
#include "../Parser.h"
#include "../Constructs/Variable.h"

#include <assert.h>
#include <sstream>

namespace Language {
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        FunctionDefinitionNode* node = new FunctionDefinitionNode();

        // take (and then clear) current annotations
        node->setAnnotations(parser.currentScope()->annotations());
        parser.currentScope()->clearAnnotations();

        node->_function = parser.parseFunctionSignature();
        assert(node->_function);

        parser.parseNewline();

        // define the function in the current module
        parser.currentModule()->addFunction(node->_function->name(), node->_function);

        // define a variable for the function, so it can be referred to
        Variable* var = new Variable();

        // use the fullyQualifiedName here, so when it is codegen'ed, we get the
        // right symbol
        var->setName(node->_function->fullyQualifiedName());

        // TODO: this createType usage is a leak
        var->setType(TypeReference(node->_function->createType(), 1));

        parser.currentScope()->addVariable(node->_function->name(), var);

        // push in a new scope for the function body
        parser.pushScope(new Scope(node->_function->name()));
        

        // define new variables for all parameters
        node->_function->eachParameterWithLast([&] (Variable* v, bool last) {
            parser.currentScope()->addVariable(v->name(), v);
        });

        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        parser.popScope();
        parser.parseNewline();

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

        context.setCurrent(CSourceContext::Section::Declarations);

        for (Annotation* annotation : this->annotations()) {
            context << annotation->CSourceString() << " ";
        }

        f->codeGenCSource(context, NULL);

        context.setCurrent(CSourceContext::Section::Body);

        for (Annotation* annotation : this->annotations()) {
            context << annotation->CSourceString() << " ";
        }

        f->codeGenCSource(context, [&] () {
            this->codeGenCSourceForChildren(context);
        });
    }
}
