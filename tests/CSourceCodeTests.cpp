#include "compiler/CodeGen/CSource.h"
#include "compiler/AST.h"

#include <assert.h>
#include <gtest/gtest.h>

class CSourceCodeGenTest : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(CSourceCodeGenTest, SingleLinePrint) {
    Three::CSource source;

    source << "Something";

    ASSERT_EQ("Something", source.renderToString());
}

TEST_F(CSourceCodeGenTest, BasicIndentationControl) {
    Three::CSource source;

    source.printLine("if (x) {");
    source.increaseIndentation();
    source.printLine("y();");
    source.decreaseIndentation();
    source.printLine("}");

    ASSERT_EQ("if (x) {\n    y();\n}\n", source.renderToString());
}

TEST_F(CSourceCodeGenTest, LineBasedIndentationControl) {
    Three::CSource source;

    source.printLineAndIndent("if (x) {");
    source.printLine("y();");
    source.outdentAndPrintLine("}");

    ASSERT_EQ("if (x) {\n    y();\n}\n", source.renderToString());
}

TEST_F(CSourceCodeGenTest, FormLineFollowedByLineEnding) {
    Three::CSource source;

    source << "a";
    source << "b";
    source << "c";
    source.printLine(";");

    ASSERT_EQ("abc;\n", source.renderToString());
}

TEST_F(CSourceCodeGenTest, PreviousLineWriting) {
    Three::CSource source;

    source.printLine("a;");
    source << "b";
    source << "c";
    source.printPreviousLine("previous;");
    source.printLine(";");

    ASSERT_EQ("a;\nprevious;\nbc;\n", source.renderToString());
}
