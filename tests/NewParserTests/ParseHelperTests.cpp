#include "compiler/Lexer/Lexer.h"
#include "compiler/Parser/ParseHelper.h"

#include <assert.h>
#include <gtest/gtest.h>

class ParseHelperTests : public testing::Test {
protected:
    virtual void SetUp() {
        _stream = nullptr;
        _lexer  = nullptr;
        _helper = nullptr;
    }

    virtual void TearDown() {
        delete _helper;
        delete _lexer;
        delete _stream;
    }

    void parse(const char* input) {
        std::string inputString(input);

        assert(!_stream);
        _stream = new std::istringstream(inputString);

        assert(!_lexer);
        _lexer = new Three::Lexer(_stream);

        assert(!_helper);
        _helper = new Three::ParseHelper(_lexer);
    }

    Three::ParseHelper* helper() const {
        return _helper;
    }

private:
    std::istringstream* _stream;
    Three::Lexer* _lexer;
    Three::ParseHelper* _helper;
};

TEST_F(ParseHelperTests, TestPeek) {
    this->parse("a b c");

    ASSERT_EQ(Three::Token::Type::Identifier, this->helper()->peek(3).type());
    ASSERT_EQ("c", this->helper()->peek(3).str());
}

TEST_F(ParseHelperTests, NextIfStringOnMatch) {
    this->parse("1 b c");

    ASSERT_TRUE(this->helper()->nextIf("1"));
    ASSERT_EQ("b", this->helper()->peek().str());
}

TEST_F(ParseHelperTests, NextIfStringOnMiss) {
    this->parse("a b c");

    ASSERT_FALSE(this->helper()->nextIf("1"));
    ASSERT_EQ("a", this->helper()->peek().str());
}
