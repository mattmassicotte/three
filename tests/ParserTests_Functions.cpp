#include "ParserTestBase.h"

class ParserTest_Functions : public ParserTestBase {
};

TEST_F(ParserTest_Functions, ClosureInFunctionSignature) {
    Language::ASTNode* node;

    node = this->parse("def test({Int} closure)\nclosure(1)\nend\n");
    node = node->childAtIndex(0);
}

TEST_F(ParserTest_Functions, ClosureWithLabelledParameterInFunctionSignature) {
    Language::ASTNode* node;

    node = this->parse("def test({Int value} closure)\nclosure(1)\nend\n");
    node = node->childAtIndex(0);
}
