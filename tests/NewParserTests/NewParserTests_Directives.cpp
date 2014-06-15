#include "../ParserTestsBase.h"

class ParserTests_Directives : public ParserTestsBase {
};

TEST_F(ParserTests_Directives, Default) {
    Three::ParseContext* ctx = this->parse("Int a");

    ASSERT_EQ(TranslationUnit::Visibility::Default, ctx->visibility());
}

TEST_F(ParserTests_Directives, Public) {
    Three::ParseContext* ctx = this->parse("public\nInt a");

    ASSERT_EQ(TranslationUnit::Visibility::External, ctx->visibility());
    ASSERT_EQ("Visibility", ctx->rootNode()->childAtIndex(0)->nodeName());

    VisibilityNode* visNode = dynamic_cast<VisibilityNode*>(ctx->rootNode()->childAtIndex(0));
    ASSERT_EQ(TranslationUnit::Visibility::External, visNode->type());
}

TEST_F(ParserTests_Directives, Private) {
    Three::ParseContext* ctx = this->parse("private\nInt a");

    ASSERT_EQ(TranslationUnit::Visibility::None, ctx->visibility());
    ASSERT_EQ("Visibility", ctx->rootNode()->childAtIndex(0)->nodeName());

    VisibilityNode* visNode = dynamic_cast<VisibilityNode*>(ctx->rootNode()->childAtIndex(0));
    ASSERT_EQ(TranslationUnit::Visibility::None, visNode->type());
}

TEST_F(ParserTests_Directives, Namespace) {
    ASTNode* node = this->parseNode("namespace Foo\n"
                                    "  def test(Int x)\n"
                                    "  end\n"
                                    "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Namespace", node->nodeName());
    ASSERT_EQ("Foo", dynamic_cast<NamespaceNode*>(node)->name());
}
