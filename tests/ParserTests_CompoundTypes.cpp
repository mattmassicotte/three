#include "ParserTestBase.h"

class ParserTest_CompoundTypes : public ParserTestBase {
};

TEST_F(ParserTest_CompoundTypes, EnumWithValues) {
    ASTNode* node;

    node = this->parse("enum MyEnum\nvalue = 1\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ("Enumeration", node->nodeName());
}

TEST_F(ParserTest_CompoundTypes, TypeOfUnaryAddressOfMemberAccess) {
    ASTNode* node;

    node = this->parse("struct MyStruct\nInt a\nend\ndef test()\n*MyStruct s\n&s->a\nend\n");
    node = node->childAtIndex(1)->childAtIndex(1);

    ASSERT_OPERATOR("&", node);
    ASSERT_EQ("MemberAccess", node->childAtIndex(0)->nodeName());
    ASSERT_DATA_TYPE("Int", node->nodeType().referencedType());
    ASSERT_EQ(0, node->nodeType().indirectionDepth());
}

TEST_F(ParserTest_CompoundTypes, NestedStruct) {
    ASTNode* node;

    node = this->parse("struct A\nInt in\nend\nstruct B\nA out\nend\ndef test(B b)\nb.out.in = 5\nend\n");

    // TODO
    std::cout << node->recursiveStr() << std::endl;
}

TEST_F(ParserTest_CompoundTypes, InvokeFunctionPointerMember) {
    ASTNode* node;

    node = this->parse("struct MyStruct\n*(*Void arg; *Void) fn_ptr\nend\ndef test()\nMyStruct s\ns.fn_ptr(null)\nend\n");

    // TODO
    std::cout << node->recursiveStr() << std::endl;
}