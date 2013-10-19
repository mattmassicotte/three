#include "compiler/AST.h"

#include <gtest/gtest.h>

using namespace Three;
using namespace Three;

#define ASSERT_IMPORT(path_value, node) do {\
Three::ImportNode* tmp = dynamic_cast<Three::ImportNode*>(node); \
ASSERT_EQ("Import", tmp->name()); \
ASSERT_EQ(path_value, tmp->path()); \
} while(0)

#define ASSERT_STRING_LITERAL_NODE(str_value, node) do {\
Three::StringLiteralNode* tmp = dynamic_cast<Three::StringLiteralNode*>(node); \
ASSERT_EQ("StringLiteral", tmp->name()); \
ASSERT_EQ(str_value, tmp->stringValue()); \
} while(0)

#define ASSERT_INTEGER_LITERAL_NODE(num_value, node) do {\
Three::IntegerLiteralNode* tmp = dynamic_cast<Three::IntegerLiteralNode*>(node); \
ASSERT_EQ("IntegerLiteral", tmp->name()); \
ASSERT_EQ(num_value, tmp->value()); \
} while(0)

#define ASSERT_BOOLEAN_LITERAL_NODE(bool_value, node) do { \
Three::BooleanLiteralNode* tmp = dynamic_cast<Three::BooleanLiteralNode*>(node); \
ASSERT_EQ("BooleanLiteral", tmp->name()); \
ASSERT_EQ(bool_value, tmp->value()); \
} while(0)

#define ASSERT_RETURN_NODE(node) do {\
Three::ReturnNode* tmp = dynamic_cast<Three::ReturnNode*>(node); \
ASSERT_EQ("Return", tmp->name()); \
} while(0)

#define ASSERT_OPERATOR(op_value, node) do { \
Three::OperatorNode* tmp = dynamic_cast<Three::OperatorNode*>(node); \
ASSERT_EQ("Operator", tmp->name()); \
ASSERT_EQ(op_value, tmp->op()); \
} while(0)

#define ASSERT_LOOP(node) do { \
Three::LoopNode* tmp = dynamic_cast<Three::LoopNode*>(node); \
ASSERT_EQ("Loop", tmp->name()); \
} while(0)

#define ASSERT_DATA_TYPE(dt_name, obj) do {\
ASSERT_EQ(dt_name, obj->fullyQualifiedName()); \
} while(0)

#define ASSERT_VARIABLE(var_type, var_indirection, var_name, obj) do { \
Three::Variable* tmp = obj; \
ASSERT_DATA_TYPE(var_type, tmp->type().referencedType()); \
ASSERT_EQ(var_indirection, tmp->type().indirectionDepth()); \
ASSERT_EQ(var_name, tmp->name()); \
} while(0)

#define ASSERT_VARIABLE_NODE(var_type, var_indirection, var_name, obj) do {\
Three::VariableNode* tmp = dynamic_cast<Three::VariableNode*>(obj); \
ASSERT_EQ("Variable", tmp->name()); \
ASSERT_DATA_TYPE(var_type, tmp->variable()->type().referencedType()); \
ASSERT_EQ(var_indirection, tmp->variable()->type().indirectionDepth()); \
ASSERT_EQ(var_name, tmp->variable()->name()); \
} while(0)

#define ASSERT_VARIABLE_DECLARATION(var_type, var_indirection, var_name, obj) do {\
Three::VariableDeclarationNode* tmp = dynamic_cast<Three::VariableDeclarationNode*>(obj); \
ASSERT_EQ("VariableDeclaration", tmp->name()); \
ASSERT_DATA_TYPE(var_type, tmp->variable()->type().referencedType()); \
ASSERT_EQ(var_indirection, tmp->variable()->type().indirectionDepth()); \
ASSERT_EQ(var_name, tmp->variable()->name()); \
} while(0)

#define ASSERT_MODULE(mod_name, obj) do {\
Three::ModuleNode* tmp = dynamic_cast<Three::ModuleNode*>(obj); \
ASSERT_EQ("Module", tmp->nodeName()); \
ASSERT_EQ(mod_name, tmp->moduleName()); \
} while(0)

#define ASSERT_LINKAGE(lib_name, obj) do {\
Three::LinkageNode* tmp = dynamic_cast<Three::LinkageNode*>(obj); \
ASSERT_EQ("Linkage", tmp->nodeName()); \
ASSERT_EQ(lib_name, tmp->libraryName()); \
} while(0)

#define ASSERT_INCLUDE(include_name, obj) do {\
Three::IncludeNode* tmp = dynamic_cast<Three::IncludeNode*>(obj); \
ASSERT_EQ("Include", tmp->nodeName()); \
ASSERT_EQ(include_name, tmp->headerName()); \
} while(0)

#define ASSERT_STRUCT(struct_name, struct_packing, obj) do {\
Three::StructureNode* tmp = dynamic_cast<Three::StructureNode*>(obj); \
ASSERT_EQ("Structure", tmp->nodeName()); \
ASSERT_EQ(struct_name, tmp->structureName()); \
ASSERT_EQ(struct_packing, tmp->packing()); \
} while(0)

#define ASSERT_FUNCTION_DEFINITION(name, obj) do { \
Three::FunctionDefinitionNode* tmp = dynamic_cast<Three::FunctionDefinitionNode*>(obj); \
ASSERT_TRUE(tmp != NULL); \
ASSERT_EQ("FunctionDefinition", tmp->nodeName()); \
ASSERT_EQ(name, tmp->function()->fullyQualifiedName()); \
} while(0)

#define ASSERT_FUNCTION_CALL(name, obj) do { \
Three::FunctionCallNode* tmp = dynamic_cast<Three::FunctionCallNode*>(obj); \
ASSERT_TRUE(tmp != NULL); \
ASSERT_EQ("FunctionCall", tmp->nodeName()); \
ASSERT_EQ(name, tmp->functionName()); \
} while(0)
