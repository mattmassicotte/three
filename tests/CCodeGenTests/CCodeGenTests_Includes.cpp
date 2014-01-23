#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Includes : public ParserTestBase {
};

TEST_F(CCodeGenTests_Includes, IncludeNode) {
    ASTNode* node = this->parse("include \"stdio.h\"\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <stdio.h>\n\n", visitor.internalHeaderString());
}

TEST_F(CCodeGenTests_Includes, PublicIncludeNode) {
    ASTNode* node = this->parse("public\ninclude \"stdio.h\"\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("", visitor.internalHeaderString());
    EXPECT_EQ("#include <stdio.h>\n#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("", visitor.bodyString());
}
