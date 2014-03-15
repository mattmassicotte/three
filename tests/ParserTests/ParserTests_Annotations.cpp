#include "../ParserTestBase.h"

class ParserTest_Annotations : public ParserTestBase {
};

TEST_F(ParserTest_Annotations, NoReturnAnnotation) {
    ASTNode* node;

    node = this->parse("@noreturn\ndef test()\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ("Annotation", node->name());
}