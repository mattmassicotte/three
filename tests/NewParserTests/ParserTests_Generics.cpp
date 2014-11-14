#include "../ParserTestsBase.h"
#include "compiler/constructs/DataType.h"

class ParserTests_Generics : public ParserTestsBase {
};

TEST_F(ParserTests_Generics, OneParamGenericStructure) {
    ASTNode* node = this->parseNode("struct MyStruct<T>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();

    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_TRUE(type.isGeneric());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::Generic, type.kind());
    ASSERT_EQ("T", type.name());
    ASSERT_EQ(0, type.widthSpecifier());
}

TEST_F(ParserTests_Generics, TwoParamsGenericStructure) {
    ASTNode* node = this->parseNode("struct MyStruct<T, U>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();

    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_TRUE(type.isGeneric());
    ASSERT_EQ(2, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::Generic, type.kind());
    ASSERT_EQ("T", type.name());
    ASSERT_EQ(0, type.widthSpecifier());

    type = structNode->definedType().genericParameterAtIndex(1);
    ASSERT_EQ(DataType::Kind::Generic, type.kind());
    ASSERT_EQ("U", type.name());
    ASSERT_EQ(0, type.widthSpecifier());
}

TEST_F(ParserTests_Generics, SizeConstrainedParam) {
    ASTNode* node = this->parseNode("struct MyStruct<T:32>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();
    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::Generic, type.kind());
    ASSERT_EQ("T", type.name());
    ASSERT_EQ(32, type.widthSpecifier());
}

TEST_F(ParserTests_Generics, PointerConstrainedParam) {
    ASTNode* node = this->parseNode("struct MyStruct<*T>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();
    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::GenericPointer, type.kind());
    ASSERT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Generic, type.subtypeAtIndex(0).kind());
    ASSERT_EQ("T", type.subtypeAtIndex(0).name());
}

TEST_F(ParserTests_Generics, NullablePointerConstrainedParam) {
    ASTNode* node = this->parseNode("struct MyStruct<*?T>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();
    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::GenericNullablePointer, type.kind());
    ASSERT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Generic, type.subtypeAtIndex(0).kind());
    ASSERT_EQ("T", type.subtypeAtIndex(0).name());
}

TEST_F(ParserTests_Generics, QuestionOnlyNullablePointerConstrainedParam) {
    ASTNode* node = this->parseNode("struct MyStruct<T?>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();
    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::GenericNullablePointer, type.kind());
    ASSERT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Generic, type.subtypeAtIndex(0).kind());
    ASSERT_EQ("T", type.subtypeAtIndex(0).name());
}

TEST_F(ParserTests_Generics, MutableConstrainedParam) {
    ASTNode* node = this->parseNode("struct MyStruct<T!>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();
    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::Generic, type.kind());
    ASSERT_EQ(DataType::Access::ReadWrite, type.access());
    ASSERT_EQ("T", type.name());
    ASSERT_EQ(0, type.widthSpecifier());
}

TEST_F(ParserTests_Generics, ArrayConstrainedParam) {
    ASTNode* node = this->parseNode("struct MyStruct<[2]T>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();
    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::GenericArray, type.kind());
    ASSERT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(2, type.arrayCount());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Generic, type.subtypeAtIndex(0).kind());
    ASSERT_EQ("T", type.subtypeAtIndex(0).name());
}

TEST_F(ParserTests_Generics, UnsizedArrayConstrainedParam) {
    ASTNode* node = this->parseNode("struct MyStruct<[]T>\n"
                                    "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    StructureNode* structNode = dynamic_cast<StructureNode*>(node->childAtIndex(0));

    ASSERT_EQ("Structure Definition", structNode->nodeName());
    ASSERT_EQ("MyStruct", structNode->name());
    ASSERT_EQ(0, structNode->packing());

    DataType type = structNode->definedType();
    ASSERT_EQ(DataType::Kind::Structure, type.kind());
    ASSERT_EQ(1, type.genericParameterCount());

    type = structNode->definedType().genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::GenericArray, type.kind());
    ASSERT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(0, type.arrayCount());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Generic, type.subtypeAtIndex(0).kind());
    ASSERT_EQ("T", type.subtypeAtIndex(0).name());
}

TEST_F(ParserTests_Generics, OneParamGenericFunction) {
    ASTNode* node = this->parseNode("def foo<T>()\nend\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(1, node->childCount());
    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    DataType type = func->functionType();
    ASSERT_EQ(DataType::Kind::Function, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(0, type.parameterCount());
    ASSERT_EQ(DataType::Void, type.returnType().kind());

    ASSERT_TRUE(type.isGeneric());
    ASSERT_EQ(1, type.genericParameterCount());

    type = type.genericParameterAtIndex(0);
    ASSERT_EQ(DataType::Kind::Generic, type.kind());
    ASSERT_EQ("T", type.name());
    ASSERT_EQ(0, type.widthSpecifier());
}
