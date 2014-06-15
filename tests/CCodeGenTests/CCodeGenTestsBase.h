#include "../ParserTestsBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTestsBase : public ParserTestsBase {
protected:
    virtual void SetUp() {
        ParserTestsBase::SetUp();
        _visitor = nullptr;
    }

    virtual void TearDown() {
        ParserTestsBase::TearDown();
        delete _visitor;
    }

    CCodeGenVisitor* visit(const char* input) {
        Three::ParseContext* c = this->parseWithBodies(input);

        assert(c);

        c->postProcessAST();

        assert(!_visitor);
        _visitor = new CCodeGenVisitor();

        c->rootNode()->accept(*_visitor);

        return _visitor;
    }

    CCodeGenVisitor* _visitor;
};
