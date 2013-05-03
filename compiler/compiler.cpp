#include "Token.h"
#include "Lexer.h"

#include <iostream>
#include <fstream>
#include <string>


#include <assert.h>

void printFile(std::istream& stream) {
    char buffer;
    int  i = 0;
    
    while (!stream.eof()) {
        buffer = stream.get();

        if (buffer == -1) {
            break;
        }

        std::cout << i << ": " << buffer << " " << (int)buffer << std::endl;
        i++;
    }
}

int main () {
    std::ifstream inputFile("test.txt");

    if (!inputFile.is_open()) {
        return -1;
    }

    Language::Lexer lexer(&inputFile);

    while (1) {
        Language::Token token = lexer.nextToken();
        if (token.isEnd()) {
            std::cout << "End reached" << std::endl;
            break;
        }

        std::cout << token.type() << " " << token.str() << std::endl;
    }

    return 0;
}
