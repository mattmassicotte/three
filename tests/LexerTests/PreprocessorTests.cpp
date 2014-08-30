#include "compiler/Preprocessing/Preprocessor.h"
#include "runtime/platform.h"

#include <assert.h>
#include <gtest/gtest.h>

#define ASSERT_NEXT_TOKEN(exp_type, exp_value) do {\
Three::NewToken tmp = this->next(); \
EXPECT_EQ(Three::NewToken::Type::exp_type, tmp.type()); \
ASSERT_EQ(exp_value, tmp.str()); \
} while(0)

class PreprocessorTest : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    std::string preprocess(const char* input) {
        std::string inputString(input);
        std::istringstream stream(inputString);

        return Three::Preprocessor::process(&stream);
    }
};

TEST_F(PreprocessorTest, IfEndDirective) {
    std::string output = this->preprocess("%if true\nabc\n%end\n");

    ASSERT_EQ("abc\n", output);
}

TEST_F(PreprocessorTest, IfElseEndDirective) {
    std::string output = this->preprocess("%if false\nabc\n%else\ndef\n%end\n");

    ASSERT_EQ("def\n", output);
}

TEST_F(PreprocessorTest, PreprocessedString) {
    std::string output = this->preprocess("include \"abc\"\n");

    ASSERT_EQ("include \"abc\"\n", output);
}

TEST_F(PreprocessorTest, PreprocessorPlatformSelect) {
#if THREE_PLATFORM_DARWIN
    std::string output = this->preprocess("%if darwin\nabc\n%else\ndef\n%end\n");
#else
    std::string output = "unimplemented";
#endif

    ASSERT_EQ("abc\n", output);
}
