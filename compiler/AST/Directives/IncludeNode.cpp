#include "IncludeNode.h"
#include "../../CSourceIndexer.h"
#include "compiler/Parser/Parser.h"
#include "compiler/Lexer/Lexer.h"

#include <assert.h>
#include <sstream>

namespace Three {
    IncludeNode* IncludeNode::parse(Parser& parser) {
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

    std::string IncludeNode::parseSearchPathArgument(Parser& parser) {
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
