#include "compiler/CodeGen/CSource.h"
#include "compiler/AST.h"

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
    Three::DataType intType(Three::DataType::Flavor::Scalar, "int");
    Three::TypeReference ref(&intType, 1);

    Three::CSource source;

    ref.codeGenCSource(&source, "x");

    ASSERT_EQ("int* x", source.renderToString());
}

TEST_F(CSourceCodeGenTest, FunctionDeclaration) {
    Three::DataType intType(Three::DataType::Flavor::Scalar, "int");
    Three::DataType functionType(Three::DataType::Flavor::Function, "my_function");
    
    functionType.addChild(Three::TypeReference(&intType, 0), "a");
    functionType.addChild(Three::TypeReference(&intType, 1), "b");
    functionType.setReturnType(Three::TypeReference(&intType, 2));
    
    Three::TypeReference ref(&functionType, 0);

    Three::CSource source;

    ref.codeGenCSource(&source, "x");

    ASSERT_EQ("int** (x)(int, int*)", source.renderToString());
}

TEST_F(CSourceCodeGenTest, FunctionReference) {
    Three::DataType intType(Three::DataType::Flavor::Scalar, "int");
    Three::DataType functionType(Three::DataType::Flavor::Function, "my_function");
    
    functionType.addChild(Three::TypeReference(&intType, 0), "a");
    functionType.addChild(Three::TypeReference(&intType, 1), "b");
    functionType.setReturnType(Three::TypeReference(&intType, 2));
    
    Three::TypeReference ref(&functionType, 1);

    Three::CSource source;

    ref.codeGenCSource(&source, "");

    ASSERT_EQ("int** (*)(int, int*)", source.renderToString());
}

TEST_F(CSourceCodeGenTest, IfStatementWithSingleVariableCondition) {
    Three::DataType intType(Three::DataType::Flavor::Scalar, "int");

    Three::Variable* variable = new Three::Variable();
    variable->setName("a");
    variable->setType(Three::TypeReference(&intType, 0));

    Three::VariableNode* variableNode = new Three::VariableNode();
    variableNode->setVariable(variable);

    Three::IfNode* ifNode = new Three::IfNode();
    ifNode->setCondition(variableNode);

    Three::CSourceContext context;

    ifNode->codeGenCSource(context);

    ASSERT_EQ("if (a) {\n}\n", context.body()->renderToString());
}

TEST_F(CSourceCodeGenTest, AddressOfOperator) {
    Three::DataType intType(Three::DataType::Flavor::Scalar, "int");

    Three::Variable* variable = new Three::Variable();
    variable->setName("a");
    variable->setType(Three::TypeReference(&intType, 0));

    Three::VariableNode* variableNode = new Three::VariableNode();
    variableNode->setVariable(variable);

    Three::OperatorNode* opNode = new Three::OperatorNode();
    opNode->setOp("&");
    opNode->setStatement(false);
    opNode->addChild(variableNode);

    Three::CSourceContext context;

    opNode->codeGenCSource(context);

    ASSERT_EQ("(&a)", context.body()->renderToString());
}
