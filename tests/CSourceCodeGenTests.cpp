#include "compiler/CodeGen/CSource.h"
#include "compiler/Constructs/TypeReference.h"
#include "compiler/Constructs/DataType.h"

#include <assert.h>
#include <gtest/gtest.h>

class CSourceCodeGenTest : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(CSourceCodeGenTest, ScalarDeclaration) {
    Language::DataType intType(Language::DataType::Flavor::Scalar, "int");
    Language::TypeReference ref(&intType, 1);

    Language::CSource source;

    ref.codeGenCSource(&source, "x");

    ASSERT_EQ("int* x", source.renderToString());
}

TEST_F(CSourceCodeGenTest, FunctionDeclaration) {
    Language::DataType intType(Language::DataType::Flavor::Scalar, "int");
    Language::DataType functionType(Language::DataType::Flavor::Function, "my_function");
    
    functionType.addChild(Language::TypeReference(&intType, 0));
    functionType.addChild(Language::TypeReference(&intType, 1));
    functionType.setReturnType(Language::TypeReference(&intType, 2));
    
    Language::TypeReference ref(&functionType, 0);

    Language::CSource source;

    ref.codeGenCSource(&source, "x");

    ASSERT_EQ("int** (x)(int, int*)", source.renderToString());
}

TEST_F(CSourceCodeGenTest, FunctionReference) {
    Language::DataType intType(Language::DataType::Flavor::Scalar, "int");
    Language::DataType functionType(Language::DataType::Flavor::Function, "my_function");
    
    functionType.addChild(Language::TypeReference(&intType, 0));
    functionType.addChild(Language::TypeReference(&intType, 1));
    functionType.setReturnType(Language::TypeReference(&intType, 2));
    
    Language::TypeReference ref(&functionType, 1);

    Language::CSource source;

    ref.codeGenCSource(&source, "");

    ASSERT_EQ("int** (*)(int, int*)", source.renderToString());
}
