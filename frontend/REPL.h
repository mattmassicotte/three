#pragma once

#include <string>
#include <sstream>

namespace Three {
    class REPL {
    public:
        static int start();
        static void prepareTerminal();

        static std::string ansiRed(const std::string& str);
        static std::string ansiCursorBack(unsigned int distance);
        static std::string ansiClearLine();

    public:
        int loop();
        std::string characterLoop();
        int nextCharacter() const;
        std::string currentFunction() const;

        void printPrompt() const;
        void clearLine(size_t length) const;
        void processEscape();
        void printOutput(const std::string& str) const;

        std::string colorizeInput(const std::string& str) const;
        std::string evaluate(const std::string& str);
    };
}
