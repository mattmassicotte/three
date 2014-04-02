#include "../NewParserTestsBase.h"
#include "compiler/constructs/NewDataType.h"

class NewParserTests_CustomTypes : public NewParserTestsBase {
};

TEST_F(NewParserTests_CustomTypes, Structure) {
    ASTNode* node = this->parseNode("struct MyStruct\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    ASSERT_EQ(NewDataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(0, structNode->definedType().subtypeCount());

    ASSERT_EQ(NewDataType::Kind::Structure, this->context()->typeKindWithName("MyStruct"));
}

TEST_F(NewParserTests_CustomTypes, StructureWithPacking) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(NewDataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(0, structNode->definedType().subtypeCount());

    ASSERT_EQ(NewDataType::Kind::Structure, this->context()->typeKindWithName("MyStruct"));
}

TEST_F(NewParserTests_CustomTypes, StructureWithOneTypedMember) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "  Int a\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(NewDataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(1, structNode->definedType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Integer, structNode->definedType().subtypeAtIndex(0).kind());

    ASSERT_EQ(1, structNode->childCount());

    VariableDeclarationNode* member = dynamic_cast<VariableDeclarationNode*>(structNode->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("a", member->name());

    ASSERT_EQ(NewDataType::Kind::Structure, this->context()->typeKindWithName("MyStruct"));
}

TEST_F(NewParserTests_CustomTypes, StructureWithTwoTypedMembers) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "  Int a\n"
                                    "  Float b\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(NewDataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(2, structNode->definedType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Integer, structNode->definedType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Kind::Float, structNode->definedType().subtypeAtIndex(1).kind());

    ASSERT_EQ(2, structNode->childCount());

    VariableDeclarationNode* member = dynamic_cast<VariableDeclarationNode*>(structNode->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("a", member->name());

    member = dynamic_cast<VariableDeclarationNode*>(structNode->childAtIndex(1));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("b", member->name());
}

TEST_F(NewParserTests_CustomTypes, StructureWithUntypedMembers) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "  a\n"
                                    "  b\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(NewDataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(2, structNode->definedType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Undefined, structNode->definedType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Kind::Undefined, structNode->definedType().subtypeAtIndex(1).kind());

    ASSERT_EQ(2, structNode->childCount());

    VariableDeclarationNode* member = dynamic_cast<VariableDeclarationNode*>(structNode->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("a", member->name());

    member = dynamic_cast<VariableDeclarationNode*>(structNode->childAtIndex(1));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("b", member->name());
}

TEST_F(NewParserTests_CustomTypes, Enumeration) {
    ASTNode* node = this->parseNode("enum MyEnum\n"
                                    "end\n");

    EnumerationNode* enumNode = dynamic_cast<EnumerationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Enumeration", enumNode->nodeName());
    ASSERT_EQ("MyEnum", enumNode->name());
    ASSERT_EQ(0, enumNode->size());

    ASSERT_EQ(NewDataType::Kind::Enumeration, enumNode->definedType().kind());
    ASSERT_EQ(0, enumNode->definedType().subtypeCount());
}

TEST_F(NewParserTests_CustomTypes, EnumerationWithSize) {
    ASTNode* node = this->parseNode("enum:4 MyEnum\n"
                                    "end\n");

    EnumerationNode* enumNode = dynamic_cast<EnumerationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Enumeration", enumNode->nodeName());
    ASSERT_EQ("MyEnum", enumNode->name());
    ASSERT_EQ(4, enumNode->size());

    ASSERT_EQ(NewDataType::Kind::Enumeration, enumNode->definedType().kind());
    ASSERT_EQ(0, enumNode->definedType().subtypeCount());
}

TEST_F(NewParserTests_CustomTypes, EnumerationWithOneMember) {
    ASTNode* node = this->parseNode("enum MyEnum\n"
                                    "  Value\n"
                                    "end\n");

    EnumerationNode* enumNode = dynamic_cast<EnumerationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Enumeration", enumNode->nodeName());
    ASSERT_EQ("MyEnum", enumNode->name());
    ASSERT_EQ(0, enumNode->size());

    ASSERT_EQ(NewDataType::Kind::Enumeration, enumNode->definedType().kind());

    ASSERT_EQ(1, enumNode->childCount());
}

TEST_F(NewParserTests_CustomTypes, Union) {
    ASTNode* node = this->parseNode("union MyUnion\n"
                                    "end\n");

    UnionNode* unionNode = dynamic_cast<UnionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Union", unionNode->nodeName());
    ASSERT_EQ("MyUnion", unionNode->name());

    ASSERT_EQ(NewDataType::Kind::Union, unionNode->definedType().kind());
    ASSERT_EQ(0, unionNode->definedType().subtypeCount());
}

TEST_F(NewParserTests_CustomTypes, UnionWithOneTypedMember) {
    ASTNode* node = this->parseNode("union MyUnion\n"
                                    "  Int a\n"
                                    "end\n");

    UnionNode* unionNode = dynamic_cast<UnionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Union", unionNode->nodeName());
    ASSERT_EQ("MyUnion", unionNode->name());

    ASSERT_EQ(NewDataType::Kind::Union, unionNode->definedType().kind());
    ASSERT_EQ(1, unionNode->definedType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Integer, unionNode->definedType().subtypeAtIndex(0).kind());

    ASSERT_EQ(1, unionNode->childCount());

    VariableDeclarationNode* member = dynamic_cast<VariableDeclarationNode*>(unionNode->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("a", member->name());
}

TEST_F(NewParserTests_CustomTypes, UnionWithTwoTypedMembers) {
    ASTNode* node = this->parseNode("union MyUnion\n"
                                    "  Int a\n"
                                    "  Float b\n"
                                    "end\n");

    UnionNode* unionNode = dynamic_cast<UnionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Union", unionNode->nodeName());
    ASSERT_EQ("MyUnion", unionNode->name());

    ASSERT_EQ(NewDataType::Kind::Union, unionNode->definedType().kind());
    ASSERT_EQ(2, unionNode->definedType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Integer, unionNode->definedType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Kind::Float, unionNode->definedType().subtypeAtIndex(1).kind());

    ASSERT_EQ(2, unionNode->childCount());

    VariableDeclarationNode* member = dynamic_cast<VariableDeclarationNode*>(unionNode->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("a", member->name());

    member = dynamic_cast<VariableDeclarationNode*>(unionNode->childAtIndex(1));
    ASSERT_EQ("Variable Declaration", member->nodeName());
    ASSERT_EQ("b", member->name());
}
