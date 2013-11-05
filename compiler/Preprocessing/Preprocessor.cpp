#include "Preprocessor.h"
#include "runtime/platform.h"

namespace Three {
    std::string Preprocessor::process(std::istream* stream) {
        SubtokenLexer lexer(stream);
        Preprocessor  preprocessor(&lexer);

        preprocessor.process();

        return preprocessor.processedOutput();
    }

    Preprocessor::Preprocessor(SubtokenLexer* lexer) : _lexer(lexer) {
    }

    std::string Preprocessor::processedOutput() {
        return _processedStream.str();
    }

    void Preprocessor::process() {
        while (true) {
            Subtoken t = _lexer->peek();

            // std::cout << t.type() << std::endl;

            switch (t.type()) {
                case Subtoken::Type::Undefined:
                case Subtoken::Type::EndOfInput:
                    return;
                case Subtoken::Type::Comment:
                    // filter out comments
                    _lexer->next();
                    break;
                case Subtoken::Type::DirectiveIf:
                    this->parseIf();
                    break;
                default:
                    _processedStream << t.str();
                    _lexer->next();
                    break;
                }
        }
    }

    void Preprocessor::parseIf() {
        bool active = false;

        assert(_lexer->next().type() == Subtoken::Type::DirectiveIf);
        assert(_lexer->next().type() == Subtoken::Type::Whitespace);

        if (_lexer->peek().str() == "true") {
            active = true;
        } else if (_lexer->peek().str() == "false") {
            active = false;
        } else if (_lexer->peek().str() == "darwin") {
#if THREE_PLATFORM_DARWIN
            active = true;
#else
            active = false;
#endif
        } else if (_lexer->peek().str() == "linux") {
#if THREE_PLATFORM_LINUX
            active = true;
#else
            active = false;
#endif
        } else {
            std::cout << "Unhandled preprocessor if condition" << _lexer->peek().str() << std::endl;
            assert(0);
        }

        _lexer->next();
        assert(_lexer->next().type() == Subtoken::Type::Newline);

        while (true) {
            Subtoken t = _lexer->peek();

            if (t.type() == Subtoken::Type::Undefined || t.type() == Subtoken::Type::EndOfInput) {
                break;
            }

            // keep going until else or end
            if (t.type() == Subtoken::Type::DirectiveEnd || t.type() == Subtoken::Type::DirectiveElse) {
                break;
            }

            if (active) {
                _processedStream << t.str();
            }

            _lexer->next();
        }

        if (_lexer->peek().type() == Subtoken::Type::DirectiveElse) {
            _lexer->next();
            assert(_lexer->next().type() == Subtoken::Type::Newline);

            while (true) {
                Subtoken t = _lexer->peek();

                if (t.type() == Subtoken::Type::Undefined || t.type() == Subtoken::Type::EndOfInput) {
                    break;
                }

                // keep going until else or end
                if (t.type() == Subtoken::Type::DirectiveEnd) {
                    break;
                }

                if (!active) {
                    _processedStream << t.str();
                }

                _lexer->next();
            }
        }

        assert(_lexer->next().type() == Subtoken::Type::DirectiveEnd);
        assert(_lexer->next().type() == Subtoken::Type::Newline);
    }
}
