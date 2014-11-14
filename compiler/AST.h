#pragma once

#include "AST/Annotations/AnnotationNode.h"
#include "AST/Atomics/AbortStatementNode.h"
#include "AST/Atomics/AtomicStatementNode.h"
#include "AST/Atomics/AtomicExpressionNode.h"
#include "AST/Atomics/BarrierNode.h"
#include "AST/Control/BreakNode.h"
#include "AST/Control/CaseNode.h"
#include "AST/Control/ContinueNode.h"
#include "AST/Control/ForNode.h"
#include "AST/Control/LoopNode.h"
#include "AST/Control/SwitchNode.h"
#include "AST/Control/IfNode.h"
#include "AST/Control/ReturnNode.h"
#include "AST/Control/ClosureNode.h"
#include "AST/Directives/DebugNode.h"
#include "AST/Directives/ImportNode.h"
#include "AST/Directives/ModuleImportNode.h"
#include "AST/Directives/IncludeNode.h"
#include "AST/Directives/LinkageNode.h"
#include "AST/Directives/VisibilityNode.h"
#include "AST/Literals/BooleanLiteralNode.h"
#include "AST/Literals/CharacterLiteralNode.h"
#include "AST/Literals/IntegerLiteralNode.h"
#include "AST/Literals/NullLiteralNode.h"
#include "AST/Literals/RealLiteralNode.h"
#include "AST/Literals/StringLiteralNode.h"
#include "AST/MetaFunctions/Metafunctions.h"
#include "AST/Operators/Operators.h"
#include "AST/RootNode.h"
#include "AST/Top-Level/FunctionDeclarationNode.h"
#include "AST/Top-Level/FunctionDefinitionNode.h"
#include "AST/Top-Level/NamespaceNode.h"
#include "AST/Top-Level/ModuleNode.h"
#include "AST/Top-Level/TypeDefinitionNode.h"
#include "AST/Top-Level/EnumerationNode.h"
#include "AST/Top-Level/StructureNode.h"
#include "AST/Top-Level/UnionNode.h"
#include "AST/Top-Level/CMacroNode.h"
#include "AST/Variables/Variables.h"
