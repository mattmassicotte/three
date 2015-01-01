#include "../ParserTestsBase.h"
#include "compiler/constructs/DataType.h"

class ParserTests_CustomTypes : public ParserTestsBase {
};

TEST_F(ParserTests_CustomTypes, Structure) {
    ASTNode* node = this->parseNode("struct MyStruct\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    ASSERT_EQ(DataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(DataType::Access::Read, structNode->definedType().access());
    ASSERT_EQ(0, structNode->definedType().subtypeCount());
    ASSERT_EQ("MyStruct", structNode->definedType().name());

    ASSERT_EQ(DataType::Kind::Structure, this->context()->typeKindWithName("MyStruct"));
}

TEST_F(ParserTests_CustomTypes, NamespacedStructure) {
    ASTNode* node = this->parseNode("namespace Foo\n"
                                    "  struct MyStruct\n"
                                    "  end\n"
                                    "end\n");

    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Namespace", node->nodeName());

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ("Foo_3_MyStruct", structNode->fullName());
    ASSERT_EQ("Foo_3_MyStruct", structNode->definedType().name());
}

TEST_F(ParserTests_CustomTypes, NamespacedStructureUsedWithoutFullName) {
    ASTNode* node = this->parseNode("namespace Foo\n"
                                    "  struct MyStruct\n"
                                    "  end\n"
                                    "  MyStruct global\n"
                                    "end\n");

    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Namespace", node->nodeName());
    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Structure Definition", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Variable Declaration", node->childAtIndex(1)->nodeName());

    // this is essentially testing that it is possible to parse a namespaced type
    auto var = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(1));
    ASSERT_EQ("global", var->name());
    ASSERT_EQ(DataType::Kind::Structure, var->dataType().kind());
    ASSERT_EQ("Foo_3_MyStruct", var->dataType().name());
}

TEST_F(ParserTests_CustomTypes, NamespacedStructureUsedWithFullName) {
    ASTNode* node = this->parseNode("namespace Foo\n"
                                    "  struct MyStruct\n"
                                    "  end\n"
                                    "end\n"
                                    "Foo::MyStruct global\n");

    ASSERT_EQ(2, node->childCount());
    ASSERT_EQ("Namespace", node->childAtIndex(0)->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Variable Declaration", node->nodeName());

    // this is essentially testing that it is possible to parse a namespaced type
    auto var = dynamic_cast<VariableDeclarationNode*>(node);
    ASSERT_EQ("global", var->name());
    ASSERT_EQ(DataType::Kind::Structure, var->dataType().kind());
    ASSERT_EQ("Foo_3_MyStruct", var->dataType().name());
}

TEST_F(ParserTests_CustomTypes, StructureWithPacking) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(DataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(0, structNode->definedType().subtypeCount());

    ASSERT_EQ(DataType::Kind::Structure, this->context()->typeKindWithName("MyStruct"));
}

TEST_F(ParserTests_CustomTypes, StructureWithOneTypedMember) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "  Int a\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(DataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(1, structNode->definedType().subtypeCount());
    ASSERT_EQ(DataType::Kind::Integer, structNode->definedType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Access::ReadWrite, structNode->definedType().subtypeAtIndex(0).access());

    ASSERT_EQ(1, structNode->childCount());

    auto member = dynamic_cast<CompositeTypeMemberNode*>(structNode->childAtIndex(0));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("a", member->name());

    ASSERT_EQ(DataType::Kind::Structure, this->context()->typeKindWithName("MyStruct"));
}

TEST_F(ParserTests_CustomTypes, StructureWithTwoTypedMembers) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "  Int a\n"
                                    "  Float b\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(DataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(2, structNode->definedType().subtypeCount());
    ASSERT_EQ(DataType::Kind::Integer, structNode->definedType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Float, structNode->definedType().subtypeAtIndex(1).kind());

    ASSERT_EQ(2, structNode->childCount());

    auto member = dynamic_cast<CompositeTypeMemberNode*>(structNode->childAtIndex(0));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("a", member->name());

    member = dynamic_cast<CompositeTypeMemberNode*>(structNode->childAtIndex(1));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("b", member->name());
}

TEST_F(ParserTests_CustomTypes, StructureWithUntypedMembers) {
    ASTNode* node = this->parseNode("struct:4 MyStruct\n"
                                    "  a\n"
                                    "  b\n"
                                    "end\n");

    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(4, structNode->packing());

    ASSERT_EQ(DataType::Kind::Structure, structNode->definedType().kind());
    ASSERT_EQ(2, structNode->definedType().subtypeCount());
    ASSERT_EQ(DataType::Kind::Undefined, structNode->definedType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Undefined, structNode->definedType().subtypeAtIndex(1).kind());

    ASSERT_EQ(2, structNode->childCount());

    auto member = dynamic_cast<CompositeTypeMemberNode*>(structNode->childAtIndex(0));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("a", member->name());

    member = dynamic_cast<CompositeTypeMemberNode*>(structNode->childAtIndex(1));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("b", member->name());
}

TEST_F(ParserTests_CustomTypes, Enumeration) {
    ASTNode* node = this->parseNode("enum MyEnum\n"
                                    "  Value\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    EnumerationNode* enumNode = dynamic_cast<EnumerationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Enumeration", enumNode->nodeName());
    ASSERT_EQ("MyEnum", enumNode->name());
    ASSERT_EQ(0, enumNode->size());

    ASSERT_EQ(DataType::Kind::Enumeration, enumNode->definedType().kind());
    ASSERT_EQ(0, enumNode->definedType().subtypeCount());
}

TEST_F(ParserTests_CustomTypes, EnumerationWithSize) {
    ASTNode* node = this->parseNode("enum:4 MyEnum\n"
                                    "end\n");

    EnumerationNode* enumNode = dynamic_cast<EnumerationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Enumeration", enumNode->nodeName());
    ASSERT_EQ("MyEnum", enumNode->name());
    ASSERT_EQ(4, enumNode->size());

    ASSERT_EQ(DataType::Kind::Enumeration, enumNode->definedType().kind());
    ASSERT_EQ(0, enumNode->definedType().subtypeCount());
}

TEST_F(ParserTests_CustomTypes, EnumerationWithOneMember) {
    ASTNode* node = this->parseNode("enum MyEnum\n"
                                    "  Value\n"
                                    "end\n");

    EnumerationNode* enumNode = dynamic_cast<EnumerationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Enumeration", enumNode->nodeName());
    ASSERT_EQ("MyEnum", enumNode->name());
    ASSERT_EQ(0, enumNode->size());

    ASSERT_EQ(DataType::Kind::Enumeration, enumNode->definedType().kind());

    ASSERT_EQ(1, enumNode->childCount());
}

TEST_F(ParserTests_CustomTypes, NamespacedEnumeration) {
    ASTNode* node = this->parseNode("namespace Foo\n"
                                    "  enum MyEnum\n"
                                    "    Value\n"
                                    "  end\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Namespace", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    EnumerationNode* enumNode = dynamic_cast<EnumerationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Enumeration", enumNode->nodeName());
    ASSERT_EQ("MyEnum", enumNode->name());
    ASSERT_EQ(0, enumNode->size());

    ASSERT_EQ(DataType::Kind::Enumeration, enumNode->definedType().kind());

    ASSERT_EQ(1, enumNode->childCount());

    auto member = dynamic_cast<CompositeTypeMemberNode*>(enumNode->childAtIndex(0));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("Value", member->name());
    ASSERT_EQ("Value", member->qualifiedName().to_s());
}

TEST_F(ParserTests_CustomTypes, Union) {
    ASTNode* node = this->parseNode("union MyUnion\n"
                                    "end\n");

    UnionNode* unionNode = dynamic_cast<UnionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Union", unionNode->nodeName());
    ASSERT_EQ("MyUnion", unionNode->name());

    ASSERT_EQ(DataType::Kind::Union, unionNode->definedType().kind());
    ASSERT_EQ(0, unionNode->definedType().subtypeCount());
}

TEST_F(ParserTests_CustomTypes, UnionWithOneTypedMember) {
    ASTNode* node = this->parseNode("union MyUnion\n"
                                    "  Int a\n"
                                    "end\n");

    UnionNode* unionNode = dynamic_cast<UnionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Union", unionNode->nodeName());
    ASSERT_EQ("MyUnion", unionNode->name());

    ASSERT_EQ(DataType::Kind::Union, unionNode->definedType().kind());
    ASSERT_EQ(1, unionNode->definedType().subtypeCount());
    ASSERT_EQ(DataType::Kind::Integer, unionNode->definedType().subtypeAtIndex(0).kind());

    ASSERT_EQ(1, unionNode->childCount());

    auto member = dynamic_cast<CompositeTypeMemberNode*>(unionNode->childAtIndex(0));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("a", member->name());
}

TEST_F(ParserTests_CustomTypes, UnionWithTwoTypedMembers) {
    ASTNode* node = this->parseNode("union MyUnion\n"
                                    "  Int a\n"
                                    "  Float b\n"
                                    "end\n");

    UnionNode* unionNode = dynamic_cast<UnionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Union", unionNode->nodeName());
    ASSERT_EQ("MyUnion", unionNode->name());

    ASSERT_EQ(DataType::Kind::Union, unionNode->definedType().kind());
    ASSERT_EQ(2, unionNode->definedType().subtypeCount());
    ASSERT_EQ(DataType::Kind::Integer, unionNode->definedType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Float, unionNode->definedType().subtypeAtIndex(1).kind());

    ASSERT_EQ(2, unionNode->childCount());

    auto member = dynamic_cast<CompositeTypeMemberNode*>(unionNode->childAtIndex(0));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("a", member->name());

    member = dynamic_cast<CompositeTypeMemberNode*>(unionNode->childAtIndex(1));
    ASSERT_EQ("Composite Type Member", member->nodeName());
    ASSERT_EQ("b", member->name());
}
