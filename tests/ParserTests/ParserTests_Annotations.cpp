#include "../ParserTestsBase.h"
#include "compiler/constructs/DataType.h"
#include "compiler/AST/Annotations/Annotations.h"

class ParserTests_Annotations : public ParserTestsBase {
};

TEST_F(ParserTests_Annotations, DISABLED_GlobalConstBoolean) {
    ASTNode* node = this->parseNode("@const Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    ASSERT_EQ(DataType::Boolean, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalConstPointerToBoolean) {
    ASTNode* node = this->parseNode("@const *Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_EQ(DataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalPointerToConstBoolean) {
    ASTNode* node = this->parseNode("* @const Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_EQ(DataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalConstPointerToConstBoolean) {
    ASTNode* node = this->parseNode("@const * @const Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_EQ(DataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalRestrictedPointer) {
    ASTNode* node = this->parseNode("@restrict *Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_TRUE(node->dataType().potentiallyAliased());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalVolatileBool) {
    ASTNode* node = this->parseNode("@volatile Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Boolean, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_EQ(DataType::Access::ReadWrite, node->dataType().volatility());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalVolatileNoneBool) {
    ASTNode* node = this->parseNode("@volatile(none) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Boolean, node->dataType().kind());
    ASSERT_EQ(DataType::Access::None, node->dataType().volatility());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalVolatileReadBool) {
    ASTNode* node = this->parseNode("@volatile(r) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Boolean, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().volatility());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalVolatileWriteBool) {
    ASTNode* node = this->parseNode("@volatile(w) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Boolean, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Write, node->dataType().volatility());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalVolatileReadWriteBool) {
    ASTNode* node = this->parseNode("@volatile(rw) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Boolean, node->dataType().kind());
    ASSERT_EQ(DataType::Access::ReadWrite, node->dataType().volatility());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalAccessReadShortBool) {
    ASTNode* node = this->parseNode("@access(r) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Access::None, node->dataType().volatility());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalAccessReadBool) {
    ASTNode* node = this->parseNode("@access(read) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Access::None, node->dataType().volatility());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalAccessWriteShortBool) {
    ASTNode* node = this->parseNode("@access(w) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Access::Write, node->dataType().access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalAccessWriteBool) {
    ASTNode* node = this->parseNode("@access(write) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Access::Write, node->dataType().access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalAccessReadWriteShortBool) {
    ASTNode* node = this->parseNode("@access(rw) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Access::ReadWrite, node->dataType().access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalAccessReadWriteBool) {
    ASTNode* node = this->parseNode("@access(readwrite) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Access::ReadWrite, node->dataType().access());
}

TEST_F(ParserTests_Annotations, DISABLED_GlobalAccessNoneWriteBool) {
    ASTNode* node = this->parseNode("@access(none) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Access::None, node->dataType().access());
}

TEST_F(ParserTests_Annotations, BriefAnnotation) {
    ASTNode* node = this->parseNode("@brief Some description\n");

    TextAnnotationNode* brief = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Brief Annotation", brief->nodeName());
    ASSERT_EQ("Some description", brief->text());
}

TEST_F(ParserTests_Annotations, SummaryAnnotation) {
    ASTNode* node = this->parseNode("@summary Some longer description\n");

    TextAnnotationNode* summary = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Summary Annotation", summary->nodeName());
    ASSERT_EQ("Some longer description", summary->text());
}

TEST_F(ParserTests_Annotations, BriefThenSummary) {
    ASTNode* node = this->parseNode("@brief Some description\n"
                                    "@summary Some longer description\n");

    TextAnnotationNode* textNode;

    textNode = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Brief Annotation", textNode->nodeName());
    ASSERT_EQ("Some description", textNode->text());

    textNode = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(1));

    ASSERT_EQ("Summary Annotation", textNode->nodeName());
    ASSERT_EQ("Some longer description", textNode->text());
}

TEST_F(ParserTests_Annotations, ParamAnnotation) {
    ASTNode* node = this->parseNode("@param A function parameter\n");

    TextAnnotationNode* summary = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Param Annotation", summary->nodeName());
    ASSERT_EQ("A function parameter", summary->text());
}

TEST_F(ParserTests_Annotations, ParamWithAdditionalAnnotations) {
    ASTNode* node = this->parseNode("@param @read(1) A function parameter\n");

    TextAnnotationNode* param = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Param Annotation", param->nodeName());
    ASSERT_EQ("A function parameter", param->text());

    ReadAnnotationNode* readNode = dynamic_cast<ReadAnnotationNode*>(param->childAtIndex(0));

    ASSERT_EQ("Read Annotation", readNode->nodeName());
}

TEST_F(ParserTests_Annotations, ReturnAnnotation) {
    ASTNode* node = this->parseNode("@return return value\n");

    TextAnnotationNode* returnNode = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Return Annotation", returnNode->nodeName());
    ASSERT_EQ("return value", returnNode->text());
}

TEST_F(ParserTests_Annotations, NoReturnAnnotation) {
    ASTNode* node = this->parseNode("@noreturn");

    ASSERT_EQ("No Return Annotation", node->childAtIndex(0)->nodeName());
}
