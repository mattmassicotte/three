#include "../ParserTestBase.h"

class ParserTest_Directives : public ParserTestBase {
};

//TEST_F(ParserTest_Directives, VersionDirective) {
//    ASTNode* node;
//
//    node = this->parse("version(platform)\nwhen mach\ndef mach()\nend\nwhen unix\ndef unix()\nend\nend\n");
//    node = node->childAtIndex(0);
//
//    ASSERT_EQ("Annotation", node->name());
//}
