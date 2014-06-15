#include "CCodeGenTestsBase.h"

class CCodeGenTests_Includes : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Includes, IncludeNode) {
    Three::CCodeGenVisitor* visitor = this->visit("include <stdio.h>\n");

    EXPECT_EQ("#include <stdio.h>\n\n", visitor->internalHeaderString());
}

TEST_F(CCodeGenTests_Includes, PublicIncludeNode) {
    Three::CCodeGenVisitor* visitor = this->visit("public\ninclude <stdio.h>\n");

    EXPECT_EQ("", visitor->internalHeaderString());
    EXPECT_EQ("#include <stdio.h>\n#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("", visitor->bodyString());
}
