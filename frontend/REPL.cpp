#include "REPL.h"

#include <iostream>
#include <termios.h>
#include <stdio.h>

namespace Three {
    int REPL::start() {
        REPL repl;

        REPL::prepareTerminal();

        return repl.loop();
    }

    void REPL::prepareTerminal() {
        struct termios tty;

        tcgetattr(fileno(stdin), &tty);

        // disable echoing, so we can control what the user sees
        tty.c_lflag &= ~ECHO;

        // turn off buffering, so we can intercept characters before return is pressed
        tty.c_lflag &= ~ICANON;

        tcsetattr(fileno(stdin), TCSANOW, &tty);
    }

    std::string REPL::ansiRed(const std::string& str) {
        std::stringstream s;

        s << "\e[31m";
        s << str;
        s << "\e[0m";

        return s.str();
    }

    std::string REPL::ansiCursorBack(unsigned int distance) {
        std::stringstream s;

        s << "\e[";
        s << distance;
        s << "D";

        return s.str();
    }

    std::string REPL::ansiClearLine() {
        return "\e[K";
    }

    int REPL::loop() {
        while (1) {
            this->printPrompt();
            std::string input = this->characterLoop();
            std::string output = this->evaluate(input);

            if (output.length() > 0) {
                std::cout << output << std::endl;
            }
        }

        return 1;
    }

    std::string REPL::characterLoop() {
        size_t lastLineLength = 0;
        std::stringstream currentLine;

        while (1) {
            // get the next character, and store it in our input buffer
            int character = this->nextCharacter();

            switch (character) {
                case '\e':
                    this->processEscape();
                    continue;
                default:
                    currentLine << (char)character;
                    break;
            }

            // convert the buffer to a string, and process the tokens for syntax highlighting
            std::string input = currentLine.str();
            std::string highlightedInput = this->colorizeInput(input);

            // clear the line, so we can re-print the newly highlighted output
            this->clearLine(lastLineLength);
            this->printOutput(highlightedInput);

            // if our last character was a return, return the input
            if (character == '\n') {
                return input;
            }

            lastLineLength = input.length();
        }

        return "";
    }

    int REPL::nextCharacter() const {
        return std::getchar();
    }

    std::string REPL::currentFunction() const {
        return "function";
    }

    void REPL::printPrompt() const {
        std::cout << "3:" << this->currentFunction() << ":" << "0" << "> ";
    }

    void REPL::clearLine(size_t length) const {
        if (length == 0) {
            return;
        }

        // back up that number of characters
        std::cout << REPL::ansiCursorBack(length);

        // clear to the end of the line
        std::cout << REPL::ansiClearLine();
    }

    void REPL::processEscape() {
    }

    void REPL::printOutput(const std::string& str) const {
        std::cout << str;
    }

    std::string REPL::colorizeInput(const std::string& str) const {
        return REPL::ansiRed(str);
    }

    std::string REPL::evaluate(const std::string& str) {
        return "<no result>";
    }
}
