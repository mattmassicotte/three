#include "../NewParserTestsBase.h"
#include "compiler/AST/Directives/ImportNode.h"

class NewParserTests_ExternalFiles : public NewParserTestsBase {
protected:
    virtual void SetUp() {
        NewParserTestsBase::SetUp();
        this->setSkipIncludes(true);
    }
};

TEST_F(NewParserTests_ExternalFiles, ImportStatement) {
    ASTNode* node = this->parseNode("import Foo\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Import", node->nodeName());
    ASSERT_EQ("Foo", dynamic_cast<ImportNode*>(node)->argument());
}

TEST_F(NewParserTests_ExternalFiles, ImportStatementWithMultiPartIdentifier) {
    ASTNode* node = this->parseNode("import Foo::Bar::Baz\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Import", node->nodeName());
    ASSERT_EQ("Foo::Bar::Baz", dynamic_cast<ImportNode*>(node)->argument());
}

TEST_F(NewParserTests_ExternalFiles, Include) {
    ASTNode* node = this->parseNode("include \"/a/b/c.h\"\n");

    IncludeNode* includeNode = dynamic_cast<IncludeNode*>(node->childAtIndex(0));

    ASSERT_EQ("Include", includeNode->nodeName());
    ASSERT_EQ("/a/b/c.h", includeNode->argument());
    ASSERT_FALSE(includeNode->useSearchPaths());
}

TEST_F(NewParserTests_ExternalFiles, IncludeWithSearchPaths) {
    ASTNode* node = this->parseNode("include <file.h>\n");

    IncludeNode* includeNode = dynamic_cast<IncludeNode*>(node->childAtIndex(0));

    ASSERT_EQ("Include", includeNode->nodeName());
    ASSERT_EQ("file.h", includeNode->argument());
    ASSERT_TRUE(includeNode->useSearchPaths());
}
