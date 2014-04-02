#include "IncludeNode.h"
#include "../../CSourceIndexer.h"
#include "compiler/Parser/NewParser.h"
#include "compiler/Lexer/Lexer.h"

#include <assert.h>
#include <sstream>

namespace Three {
    // IncludeNode* IncludeNode::parse(OldParser& parser) {
    //     IncludeNode* node = new IncludeNode();
    // 
    //     assert(parser.next().type() == Token::Type::KeywordInclude);
    // 
    //     assert(parser.peek().type() == Token::Type::LiteralString);
    //     node->_headerName = parser.next().str();
    //     node->_visibility = parser.context()->visibility();
    // 
    //     parser.parseNewline();
    // 
    //     parser.currentModule()->addCIncludePath(node->_headerName);
    // 
    //     CSourceIndexer index;
    // 
    //     std::string fullPath = CSourceIndexer::resolveCHeaderPath(node->_headerName);
    //     
    //     if (!index.indexFileAtPath(fullPath)) {
    //         std::cout << "Unable to index c header '" << node->_headerName << "' => '" << fullPath << "'" << std::endl;
    //         assert(0);
    //     }
    // 
    //     parser.currentModule()->addModule(node->_headerName, index.module());
    // 
    //     index.module()->eachFunction([&] (const Function* function) {
    //         assert(function);
    // 
    //         // define a variable for the function, so it can be referred to
    //         // TODO: copied right from FunctionDefinitionNode.cpp
    //         Variable* var = new Variable();
    // 
    //         var->setName(function->fullyQualifiedName());
    //         var->setType(TypeReference(function->createType(), 1));
    // 
    //         parser.currentScope()->addVariable(function->fullyQualifiedName(), var);
    //     });
    // 
    //     return node;
    // }

    IncludeNode* IncludeNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordInclude));

        IncludeNode* node = new IncludeNode();

        node->_visibility = parser.context()->visibility();

        if (parser.helper()->peek().type() == Token::Type::LiteralString) {
            node->_argument = parser.helper()->next().strTrimmingFirstAndLast();
            node->_useSearchPaths = false;
        } else {
            node->_useSearchPaths = true;
            node->_argument = IncludeNode::parseSearchPathArgument(parser);
        }

        if (node->_argument.length() == 0) {
            assert(0 && "Message: Unable to parse include directive argument");
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: Include directive must be followed by a newline");
        }

        if (parser.context()->skipIncludes) {
            return node;
        }

        // now, actually index the source
        if (!CSourceIndexer::resolvePathAndIndex(node->_argument, node->_useSearchPaths, parser.context())) {
            assert(0 && "Message: Unable to index C source from include directive");
        }

        return node;
    }

    std::string IncludeNode::parseSearchPathArgument(NewParser& parser) {
        if (!parser.helper()->nextIf(Token::Type::OperatorLessThan)) {
            assert(0 && "Message: Include argument must start with \" or <");
            return "";
        }

        std::stringstream s;

        parser.helper()->lexer()->setFilterWhitespace(false);
        parser.helper()->parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::OperatorGreaterThan:
                    return true;
                case Token::Type::Newline:
                    assert(0 && "Message: include argument invalid");
                    return true;
                default:
                    break;
            }

            s << parser.helper()->next().str();

            return false;
        });
        parser.helper()->lexer()->setFilterWhitespace(true);

        if (!parser.helper()->nextIf(Token::Type::OperatorGreaterThan)) {
            assert(0 && "Message: Include argument must end with >");
            return "";
        }

        return s.str();
    }

    std::string IncludeNode::name() const {
        return "Include";
    }

    std::string IncludeNode::headerName() const {
        return this->argument();
    }

    bool IncludeNode::relative() const {
        return false;
    }

    bool IncludeNode::useSearchPaths() const {
        return _useSearchPaths;
    }
}
