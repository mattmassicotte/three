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

TEST_F(ParserTests_Directives, NestedNamespace) {
    ASTNode* node = this->parseNode("namespace Foo::Bar\n"
                                    "  def test(Int x)\n"
                                    "  end\n"
                                    "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Namespace", node->nodeName());

    NamespaceNode* nsNode = dynamic_cast<NamespaceNode*>(node);
    ASSERT_EQ(2, nsNode->qualifiedName().components.size());
    ASSERT_EQ("Foo", nsNode->qualifiedName().components.at(0));
    ASSERT_EQ("Bar", nsNode->qualifiedName().components.at(1));
}

TEST_F(ParserTests_Directives, EmptyNamespace) {
    ASTNode* node = this->parseNode("namespace Foo\n"
                                    "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Namespace", node->nodeName());
    ASSERT_EQ("Foo", dynamic_cast<NamespaceNode*>(node)->name());
}

TEST_F(ParserTests_Directives, EmptyNamespaceWithWhitespace) {
    ASTNode* node = this->parseNode("namespace Foo\n"
                                    "\n"
                                    "end\n");

    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Namespace", node->nodeName());
    ASSERT_EQ("Foo", dynamic_cast<NamespaceNode*>(node)->name());
}

