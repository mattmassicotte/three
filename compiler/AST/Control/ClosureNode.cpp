#include "ClosureNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/Scope.h"

#include <assert.h>

namespace Three {
    ClosureNode* ClosureNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordClosure));

        ClosureNode* node = new ClosureNode();

        node->_name = parser.context()->scope()->scopedName("closure");

        std::vector<std::string> referenceNames;

        // this is optional for closures that are (Void; Void)
        if (parser.helper()->peek().type() == Token::Type::PunctuationOpenParen) {
            node->_dataType = parser.parseFunctionType(true, &referenceNames);

            // this will (correctly) parse as a function signature
            assert(node->_dataType.kind() == DataType::Kind::Function);
        }

        parser.context()->pushScope();
        parser.context()->scope()->setCapturing(true);

        // define variables for the closure params
        node->_dataType.eachParameterWithLast([&] (const DataType& paramType, bool last) {
            // TODO: this should probably create and hold a reference to a variable
            parser.context()->scope()->defineVariableTypeForName(paramType, paramType.label());
        });

        // setup our environment type
        DataType envStruct = DataType(DataType::Kind::CStructPrefixedStructure);
        envStruct.setName(node->_name + "_env");

        // handle references
        for (const std::string& name : referenceNames) {
            Variable* v = parser.context()->variableForName(name);

            // this must be writable, because that's the point
            DataType type = v->type;
            type.setAccess(DataType::Access::ReadWrite);

            envStruct.addSubtype(DataType::wrapInPointer(type, name));

            // track the variable as referenced in this scope
            parser.context()->scope()->addReferencedVariable(name);

            node->_referencedVariables.push_back(v);
        }

        // fix up the type, which was a function
        node->_dataType.setKind(DataType::Kind::Closure);
        node->_dataType.setLabel(node->_name);

        if (!parser.helper()->nextIf(Token::Type::PunctuationOpenBrace)) {
            assert(0 && "Message: Closure body should start with an open brace");
        }

        parser.helper()->parseUntil(true, [&] (const Token& token) {
            if (token.type() == Token::Type::PunctuationCloseBrace) {
                return true;
            }

            ASTNode* child = parser.parseStatement();
            if (!child) {
                return true; // we're done
            }

            node->addChild(child);

            return false;
        });

        for (const std::string& name : parser.context()->scope()->capturedVariables()) {
            Variable* v = parser.context()->variableForName(name);

            DataType capturedType = v->type;
            capturedType.setAccess(DataType::Access::Read); // make contant
            capturedType.setLabel(v->name.to_s());

            // add it to the closure's environment struct
            envStruct.addSubtype(capturedType);

            node->_capturedVariables.push_back(v);
        }

        node->environmentType = DataType::wrapInPointer(envStruct);
        node->environmentType.setLabel("self_env");

        // insert that type as the first parameter
        node->_dataType.parameters.insert(node->_dataType.parameters.cbegin(), node->environmentType);

        parser.context()->popScope();

        return node;
    }

    std::string ClosureNode::nodeName() const {
        return "Closure";
    }

    DataType ClosureNode::dataType() const {
        return _dataType;
    }
    std::string ClosureNode::name() const {
        return _name;
    }

    std::string ClosureNode::closureName() const {
        return _name;
    }

    bool ClosureNode::hasReferences() const {
        return _referencedVariables.size() > 0;
    }

    DataType ClosureNode::environmentStructureType() const {
        return this->environmentType.subtypeAtIndex(0);
    }

    void ClosureNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void ClosureNode::eachCapturedVariable(std::function<void (Variable*, bool, bool)> func) const {
        uint32_t refSize    = this->_referencedVariables.size();
        uint32_t closedSize = this->_capturedVariables.size();

        for (uint32_t i = 0; i < refSize; ++i) {
            Variable* v = this->_referencedVariables.at(i);

            func(v, true, i == (refSize - 1) && closedSize == 0);
        }

        uint32_t i = 0;
        for (Variable* v : this->_capturedVariables) {

            func(v, false, i == (closedSize - 1));
            i += 1;
        }
    }
}
