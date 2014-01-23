#include "../ParserTestBase.h"

class ParserTest_CompoundTypes : public ParserTestBase {
};

TEST_F(ParserTest_CompoundTypes, EnumWithValues) {
    ASTNode* node;

    node = this->parse("enum MyEnum\nvalue = 1\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ("Enumeration", node->nodeName());
}

TEST_F(ParserTest_CompoundTypes, UsingEnumAsType) {
    ASTNode* node;

    node = this->parse("enum MyEnum\n"
                       "  value\n"
                       "end\n"
                       "def test(MyEnum x)\n"
                       "end\n");

    Three::FunctionDefinitionNode* defNode = dynamic_cast<Three::FunctionDefinitionNode*>(node->childAtIndex(1));

    ASSERT_VARIABLE("MyEnum", 0, "x", defNode->function()->parameterAtIndex(0));
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
    node = node->childAtIndex(2)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_DATA_TYPE("Int", node->nodeType().referencedType());
    ASSERT_EQ(0, node->nodeType().indirectionDepth());

    ASSERT_EQ("MemberAccess", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("MemberAccess", node->childAtIndex(0)->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_CompoundTypes, InvokeFunctionPointerMember) {
    ASTNode* node;

    node = this->parse("struct MyStruct\n*(*Void arg; *Void) fn_ptr\nend\ndef test()\nMyStruct s\ns.fn_ptr(null)\nend\n");
    node = node->childAtIndex(1)->childAtIndex(1);

    ASSERT_OPERATOR("()", node);
    
    Three::FunctionCallOperatorNode* callNode;

    callNode = dynamic_cast<Three::FunctionCallOperatorNode*>(node);

    ASSERT_EQ(1, callNode->childCount());
    ASSERT_EQ("NullLiteral", callNode->childAtIndex(0)->name());
    ASSERT_EQ("MemberAccess", callNode->receiver()->nodeName());
}

TEST_F(ParserTest_CompoundTypes, StructureDefinitionWithPacking) {
    Three::ASTNode* node;

    node = this->parse("struct:2 MyStructure\nInt x\nInt y\nend\n");

    ASSERT_STRUCT("MyStructure", 2, node->childAtIndex(0));
}

TEST_F(ParserTest_CompoundTypes, StructureDefinitionWithoutPacking) {
    Three::ASTNode* node;

    node = this->parse("struct MyStructure\nInt x\nInt y\nend\n");

    ASSERT_STRUCT("MyStructure", 0, node->childAtIndex(0));
}

TEST_F(ParserTest_CompoundTypes, StructureDefinitionUsedAsType) {
    Three::ASTNode* node;

    node = this->parse("struct MyStructure\nInt x\nend\ndef foo()\nMyStructure a\nend\n");

    ASSERT_STRUCT("MyStructure", 0, node->childAtIndex(0));

    node = node->childAtIndex(1);

    ASSERT_VARIABLE_DECLARATION("MyStructure",  0, "a", node->childAtIndex(0));
}
