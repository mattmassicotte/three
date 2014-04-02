#include "../NewParserTestsBase.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/AST/Annotations/Annotations.h"

class NewParserTests_Annotations : public NewParserTestsBase {
};

TEST_F(NewParserTests_Annotations, GlobalConstBoolean) {
    ASTNode* node = this->parseNode("@const Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    ASSERT_EQ(NewDataType::Boolean, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, GlobalConstPointerToBoolean) {
    ASTNode* node = this->parseNode("@const *Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().access());
    ASSERT_EQ(NewDataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(NewParserTests_Annotations, GlobalPointerToConstBoolean) {
    ASTNode* node = this->parseNode("* @const Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().access());
    ASSERT_EQ(NewDataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(NewParserTests_Annotations, GlobalConstPointerToConstBoolean) {
    ASTNode* node = this->parseNode("@const * @const Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().access());
    ASSERT_EQ(NewDataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(NewParserTests_Annotations, GlobalRestrictedPointer) {
    ASTNode* node = this->parseNode("@restrict *Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().access());
    ASSERT_TRUE(node->dataType().potentiallyAliased());
    ASSERT_EQ(NewDataType::Integer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(NewParserTests_Annotations, GlobalVolatileBool) {
    ASTNode* node = this->parseNode("@volatile Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Boolean, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().access());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().volatility());
}

TEST_F(NewParserTests_Annotations, GlobalVolatileNoneBool) {
    ASTNode* node = this->parseNode("@volatile(none) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Boolean, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::None, node->dataType().volatility());
}

TEST_F(NewParserTests_Annotations, GlobalVolatileReadBool) {
    ASTNode* node = this->parseNode("@volatile(r) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Boolean, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().volatility());
}

TEST_F(NewParserTests_Annotations, GlobalVolatileWriteBool) {
    ASTNode* node = this->parseNode("@volatile(w) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Boolean, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::Write, node->dataType().volatility());
}

TEST_F(NewParserTests_Annotations, GlobalVolatileReadWriteBool) {
    ASTNode* node = this->parseNode("@volatile(rw) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Boolean, node->dataType().kind());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().volatility());
}

TEST_F(NewParserTests_Annotations, GlobalAccessReadShortBool) {
    ASTNode* node = this->parseNode("@access(r) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Access::None, node->dataType().volatility());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, GlobalAccessReadBool) {
    ASTNode* node = this->parseNode("@access(read) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Access::None, node->dataType().volatility());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, GlobalAccessWriteShortBool) {
    ASTNode* node = this->parseNode("@access(w) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Access::Write, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, GlobalAccessWriteBool) {
    ASTNode* node = this->parseNode("@access(write) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Access::Write, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, GlobalAccessReadWriteShortBool) {
    ASTNode* node = this->parseNode("@access(rw) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, GlobalAccessReadWriteBool) {
    ASTNode* node = this->parseNode("@access(readwrite) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, GlobalAccessNoneWriteBool) {
    ASTNode* node = this->parseNode("@access(none) Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Access::None, node->dataType().access());
}

TEST_F(NewParserTests_Annotations, BriefAnnotation) {
    ASTNode* node = this->parseNode("@brief Some description\n");

    TextAnnotationNode* brief = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Brief Annotation", brief->nodeName());
    ASSERT_EQ("Some description", brief->text());
}

TEST_F(NewParserTests_Annotations, SummaryAnnotation) {
    ASTNode* node = this->parseNode("@summary Some longer description\n");

    TextAnnotationNode* summary = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Summary Annotation", summary->nodeName());
    ASSERT_EQ("Some longer description", summary->text());
}

TEST_F(NewParserTests_Annotations, BriefThenSummary) {
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

TEST_F(NewParserTests_Annotations, ParamAnnotation) {
    ASTNode* node = this->parseNode("@param A function parameter\n");

    TextAnnotationNode* summary = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Param Annotation", summary->nodeName());
    ASSERT_EQ("A function parameter", summary->text());
}

TEST_F(NewParserTests_Annotations, ParamWithAdditionalAnnotations) {
    ASTNode* node = this->parseNode("@param @read(1) A function parameter\n");

    TextAnnotationNode* param = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Param Annotation", param->nodeName());
    ASSERT_EQ("A function parameter", param->text());

    ReadAnnotationNode* readNode = dynamic_cast<ReadAnnotationNode*>(param->childAtIndex(0));

    ASSERT_EQ("Read Annotation", readNode->nodeName());
}

TEST_F(NewParserTests_Annotations, ReturnAnnotation) {
    ASTNode* node = this->parseNode("@return return value\n");

    TextAnnotationNode* returnNode = dynamic_cast<TextAnnotationNode*>(node->childAtIndex(0));

    ASSERT_EQ("Return Annotation", returnNode->nodeName());
    ASSERT_EQ("return value", returnNode->text());
}

TEST_F(NewParserTests_Annotations, NoReturnAnnotation) {
    ASTNode* node = this->parseNode("@noreturn");

    ASSERT_EQ("No Return Annotation", node->childAtIndex(0)->nodeName());
}
