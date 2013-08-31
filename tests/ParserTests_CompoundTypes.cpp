#include "ParserTestBase.h"

class ParserTest_CompoundTypes : public ParserTestBase {
};

TEST_F(ParserTest_CompoundTypes, EnumWithValues) {
    Language::ASTNode* node;

    node = this->parse("enum MyEnum\nvalue = 1\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ("Enumeration", node->nodeName());
}
