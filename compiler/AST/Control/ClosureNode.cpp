#include "ClosureNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ClosureNode* ClosureNode::parse(Parser& parser) {
        ClosureNode* node = new ClosureNode();

        // first thing is first, create a unique function and environment
        // name for this closure
        node->_name = parser.currentScope()->makeScopedName("closure");
        node->_environmentName = node->_name + "_env";

        // now, we need to parse the closure signature
        assert(parser.next().type() == Token::Type::KeywordClosure);
        std::vector<std::string> params;
        std::vector<std::string> references;
        if (parser.peek().type() == Token::Type::PunctuationOpenParen) {
            node->_type = parser.parseFunctionType(0, &params, &references);
        } else {
            node->_type = TypeReference::ref(parser.currentModule(), "(Void; Void)", 0);
        }

        // Closure can capture two different kinds of variables - references and values.  They
        // are both handled differently.  And, they affect the scoping rules as well.
        parser.pushScope(new Scope(node->_name));
        parser.currentScope()->setClosingScope(true);

        // First, look up the referenced variables.  Since they have to be 
        // explicitly declared they are the easy ones
        for (const std::string& name : references) {
            Variable* v = parser.currentScope()->variableForName(name);

            assert(v && "Referenced variables must already be declared in scope");
            node->_referencedVariables.push_back(v);

            // make this new referenced variable in scope
            parser.currentScope()->addVariable(v->name(), v);
            parser.currentScope()->setReferencedVariable(name);
        }

        // We need to create a structure to hold the environment to be closed over.
        // That is made up of referenced variables (which we know), and closed variables
        // which we don't know about until we parse the closure body.  That makes this
        // slightly complex.
        DataType* environmentType = new DataType(DataType::Flavor::Structure, "struct " + node->_environmentName);

        for (Variable* v : node->_referencedVariables) {
            environmentType->addChild(v->type(), v->name());
        }

        parser.currentModule()->addDataType(environmentType);

        // Closures are actually function definitions, so we need
        // to create a function object to hold them.  However, the type
        // of that function is not identical to the closure's declared type.
        node->_function = new Function();
        node->_function->setName(node->_name);
        node->_function->setReturnType(node->_type.referencedType()->returnType());

        // the first parameter is the pointer to the closure structure itself
        node->_function->addParameter("self_env", TypeReference(environmentType, 1));

        // Create a new parameter to match the signature, and add a matching variable to the current
        // scope.  Have to be careful with Void types here, since there will not be a matching param.
        node->_type.referencedType()->eachChild([=] (const TypeReference& paramType, const std::string& name, uint32_t index) {
            if (params.size() == 0) {
                return;
            }

            Variable* v = new Variable();

            v->setType(paramType);
            // TODO: I think this can be set to "name"
            v->setName(params.at(index));

            node->_function->addParameter(v->name(), v->type());
            parser.currentScope()->addVariable(v->name(), v);
        });

        // With all that done, we are now ready to parse the closure's body
        assert(parser.next().str() == "{");
        if (parser.peek().type() == Token::Type::Newline) {
            parser.parseNewline();
        }

        // and now, parse the body
        while (true) {
            if (parser.peek().str() == "}") {
                break;
            }

            node->addChild(parser.parseStatement());
        }

        assert(parser.next().str() == "}");

        // Body has been parsed.  Before we pop the scope, inspect its closed variables.  We need to complete the environment type 
        // by filling in these values.
        // TODO: this is a hack and doesn't work for nested closures (at least)
        node->_capturedVariables = parser.currentScope()->_closedVariables;

        // now pop the scope to complete the hack
        parser.popScope();

        return node;
    }

    std::string ClosureNode::name() const {
        return "Closure";
    }

    // void ClosureNode::codeGenCSourceStructDefinition(CSourceContext& context) {
    //     CSource* source = context.current();
    //     
    //     // struct to hold the closure function and environment
    //     source->printLineAndIndent("struct " + this->_environmentName + " {");
    // 
    //     this->eachCapturedVariable([=] (Variable* v, bool ref, bool last) {
    //         TypeReference refType = v->type();
    // 
    //         if (ref) {
    //             // grab a copy of the referenced variable's type, bump up the indirection
    //             // and render it
    //             refType.incrementIndirectionDepth();
    //         }
    // 
    //         refType.codeGenCSource(source, v->name());
    //         source->printLine(";");
    //     });
    // 
    //     source->outdentAndPrintLine("};");
    //     // source->decreaseIndentation();
    //     // source->printNewLine();
    //     // source->printLine("};");
    // }

    void ClosureNode::eachCapturedVariable(std::function<void (Variable*, bool, bool)> func) const {
        uint32_t refSize    = this->_referencedVariables.size();
        uint32_t closedSize = this->_capturedVariables.size();

        for (uint32_t i = 0; i < refSize; ++i) {
            Variable* v = this->_referencedVariables.at(i);

            func(v, true, i == (refSize - 1) && closedSize == 0);
        }

        for (uint32_t i = 0; i < closedSize; ++i) {
            Variable* v = this->_capturedVariables.at(i);

            func(v, false, i == (closedSize - 1));
        }
    }

    // void ClosureNode::codeGenCSource(CSourceContext& context) {
    //     context.setCurrent(CSourceContext::Section::Declarations);
    // 
    //     this->codeGenCSourceStructDefinition(context);
    //     context << "static ";
    //     this->_function->codeGenCSource(context, [=, &context] () {
    //         // this->codeGenCSourceForChildren(context);
    //     });
    // 
    //     context.setCurrent(CSourceContext::Section::Body);
    // 
    //     // create the environment capture
    //     // struct main_closure_1_env main_closure_1_env_value = (struct main_closure_1_env){&x, y};
    //     std::stringstream stream;
    // 
    //     stream << "struct " << this->_environmentName << " ";
    //     stream << this->_environmentName << "_value";
    //     stream << " = (struct " << this->_environmentName << "){";
    // 
    //     this->eachCapturedVariable([=, &context, &stream] (Variable* v, bool ref, bool last) {
    //         if (ref) {
    //             stream << "&";
    //         }
    // 
    //         stream << v->name();
    // 
    //         if (!last) {
    //             stream << ", ";
    //         }
    //     });
    // 
    //     stream << "};";
    //     context.current()->printPreviousLine(stream.str());
    // 
    //     // cast the to generic type
    //     context.current()->print("THREE_MAKE_CLOSURE(");
    //     context.current()->print(this->_name);
    //     context.current()->print(")");
    // }

    void ClosureNode::codeGen(CSourceContext& context) {
        assert(0 && "Not implemented yet");
    }
}
