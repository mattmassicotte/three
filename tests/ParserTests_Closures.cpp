#include "ParserTestBase.h"

class ParserTest_Closures : public ParserTestBase {
};

TEST_F(ParserTest_Closures, InlineClosureWithoutTypes) {
    Three::ASTNode* node;

    node = this->parse("def foo({} c)\nend\ndef test()\nfoo(do {})\nend\n");
    node = node->childAtIndex(1); // def test
    node = node->childAtIndex(0); // foo(..)
    node = node->childAtIndex(0);

    ASSERT_EQ("Closure", node->name());
}

TEST_F(ParserTest_Closures, InlineClosureWithTypes) {
    Three::ASTNode* node;

    node = this->parse("def foo({Int} c)\nend\ndef test()\nfoo(do (Int x) {})\nend\n");
    node = node->childAtIndex(1); // def test
    node = node->childAtIndex(0); // foo(..)
    node = node->childAtIndex(0);

    ASSERT_EQ("Closure", node->name());
}

TEST_F(ParserTest_Closures, ClosureInFunction) {
    Three::ASTNode* node;

    node = this->parse("include \"stdio.h\"\n\ndef main(;Int)\nInt x\nInt y\n{Int, Int; Int} closure\nx = 42\ny = 43\nclosure = do (Int a, Int b; Int; x) {\nx = a + b + y\nreturn a + b\n}\n printf(\"x = %d, value = %d\\n\", x, closure(1,2))\nprintf(\"x = %d\\n\", x)\nend\n");

    // include statement
    ASSERT_INCLUDE("stdio.h", node->childAtIndex(0));

    node = node->childAtIndex(1);

    // Variable declarations
    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", node->childAtIndex(0));
    ASSERT_VARIABLE_DECLARATION("Int", 0, "y", node->childAtIndex(1));

    Three::VariableDeclarationNode* varDecl = dynamic_cast<Three::VariableDeclarationNode*>(node->childAtIndex(2));
    ASSERT_VARIABLE_DECLARATION("", 0, "closure", varDecl);
    ASSERT_EQ(2, varDecl->variable()->type().referencedType()->childCount());
    ASSERT_EQ("Int", varDecl->variable()->type().referencedType()->childAtIndex(0).name());
    ASSERT_EQ(0, varDecl->variable()->type().referencedType()->childAtIndex(0).indirectionDepth());
    ASSERT_EQ("Int", varDecl->variable()->type().referencedType()->childAtIndex(1).name());
    ASSERT_EQ(0, varDecl->variable()->type().referencedType()->childAtIndex(1).indirectionDepth());

    ASSERT_OPERATOR("=", node->childAtIndex(3));
    ASSERT_OPERATOR("=", node->childAtIndex(4));
    ASSERT_OPERATOR("=", node->childAtIndex(5));

    Three::ClosureNode* closure = dynamic_cast<Three::ClosureNode*>(node->childAtIndex(5)->childAtIndex(1));
    ASSERT_EQ("Closure", closure->name());
}
