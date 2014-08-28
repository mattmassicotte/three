#include "CCodeGenVisitor.h"
#include "compiler/AST.h"
#include "compiler/CodeGen/CTypeCodeGenerator.h"

#include <iostream>

namespace Three {
    CCodeGenVisitor::CCodeGenVisitor() {
        _externalHeaderSource.addHeader(false, "three/runtime/types.h");

        _currentSource = &_bodySource;
    }

    void CCodeGenVisitor::visit(ASTNode& node) {
        this->visitChildren(node);
    }

    void CCodeGenVisitor::visit(ExternalSourceNode& node) {
        this->sourceForVisibility(node.visibility(), [&] (CSource& source) {
            source.addHeader(node.relative(), node.headerName());
        });
    }

    void CCodeGenVisitor::visit(FunctionDefinitionNode& node) {
        // codegen state
        _tmpReturnValueCounter = 0;

        _currentFunctionType = node.functionType();
        std::string functionString = CTypeCodeGenerator::codeGenFunction(_currentFunctionType, node.fullName());

        // Function Prototype
        this->sourceForVisibility(node.visibility(), [&] (CSource& source) {
            if (node.functionType().returnType().kind() == NewDataType::Kind::Tuple) {
                this->createDefinitionForTuple(node.functionType().returnType(), source);
            }

            source << functionString;
            source.printLine(";");
        });

        // Function Definition
        _currentSource = &_bodySource;

        _currentSource->print(functionString);
        _currentSource->printLineAndIndent(" {");

        this->visitChildren(node);

        _currentSource->outdentAndPrintLine("}");
        _currentSource->printLine(""); // add an extra newline for separation
    }

    void CCodeGenVisitor::visit(VariableDeclarationNode& node) {
        std::string variableDeclString = CTypeCodeGenerator::codeGen(node.dataType(), node.name());

        // we only have to make a declaration if this is a global
        if (node.global) {
            this->sourceForVisibility(node.visibility, [&] (CSource& source) {
                // special-case, as none is defined differently
                if (node.visibility == TranslationUnit::Visibility::None) {
                    (*_currentSource) << "static ";
                    return;
                }

                source << "extern " << variableDeclString;
                source.printLine(";");
            });
        }

        (*_currentSource) << variableDeclString;

        if (node.initializerExpression()) {
            _currentSource->print(" = ");
            node.initializerExpression()->accept(*this);
        }
    }

    void CCodeGenVisitor::visit(ClosureNode& node) {
        // The codegen for closures is complex.  We need:
        // - the environment structure definition
        // - the body function
        // - the environment capture
        // - the actual closure creation

        CSource dedicatedSource;

        // comment
        dedicatedSource.print("// ");
        dedicatedSource.printLine(node.closureName());

        // environment definition
        this->closureEnvironmentStructure(node, dedicatedSource);

        // closure body
        CSource* temp = _currentSource;
        _currentSource = &dedicatedSource;

        std::string functionString = CTypeCodeGenerator::codeGenFunction(node.dataType(), node.closureName());

        (*_currentSource) << "static " << functionString;

        _currentSource->printLineAndIndent(" {");

        this->visitChildren(node);

        _currentSource->outdentAndPrintLine("}");

        (*_currentSource) << "THREE_CHECK_CLOSURE_FUNCTION(" << node.name();
        _currentSource->printLine(");");
        _currentSource->printLine("");

        _currentSource = temp;

        // the closure instantiation
        _currentSource->printPreviousLine(this->closureEnvironmentCapture(node));

        (*_currentSource) << "THREE_MAKE_CLOSURE(" << node.closureName() << ", ";

        if (node.hasReferences()) {
            (*_currentSource) << "THREE_CLOSURE_FLAGS_HAS_REFERENCES";
        } else {
            (*_currentSource) << "THREE_CLOSURE_FLAGS_DEFAULT";
        }

        (*_currentSource) << ")";

        // Doing this last ensures that closures are defined in the opposite
        // order they appear.
        _declaractionsSource << dedicatedSource.renderToString();
    }

    void CCodeGenVisitor::visit(IntegerLiteralNode& node) {
        std::stringstream s;

        // TODO: this is kinda stupid just to convert into a string...
        s << node.value();

        (*_currentSource) << s.str();
    }

    void CCodeGenVisitor::visit(BinaryOperatorNode& node) {
        node.childAtIndex(0)->accept(*this);
        (*_currentSource) << " " << node.op() << " ";
        node.childAtIndex(1)->accept(*this);
    }

    void CCodeGenVisitor::visit(VariableNode& node) {
        assert(node.newVariable());

        (*_currentSource) << node.newVariable()->name;
    }

    void CCodeGenVisitor::visit(CapturedVariableNode& node) {
        assert(node.newVariable());

        (*_currentSource) << "(self_env->" << node.newVariable()->name << ")";
    }

    void CCodeGenVisitor::visit(ReferencedVariableNode& node) {
        assert(node.newVariable());

        (*_currentSource) << "*(self_env->" << node.newVariable()->name << ")";
    }

    void CCodeGenVisitor::visit(FunctionCallOperatorNode& node) {
        NewDataType receiverType = node.receiver()->dataType();

        if (receiverType.kind() == NewDataType::Kind::Closure) {
            *_currentSource << "THREE_CALL_CLOSURE(";
            // This is kind of abusing the CTypeCodeGenerator API a little. But, this bit of code-gen
            // is pretty weird.
            *_currentSource << CTypeCodeGenerator::codeGenFunctionType(receiverType, "*");
            *_currentSource << ", ";
            node.receiver()->accept(*this);
        
            if (node.childCount() > 0) {
                *_currentSource << ", ";
            }
        } else {
            node.receiver()->accept(*this);
            *_currentSource << "(";
        }

        node.eachChildWithLast([&] (ASTNode* child, bool last) {
            child->accept(*this);
            if (!last) {
                *_currentSource << ", ";
            }
        });

        *_currentSource << ")";
    }

    void CCodeGenVisitor::visit(MethodCallOperatorNode& node) {
        assert(node.receiverDataType().subtypeCount() == 1);

        *_currentSource << node.receiverDataType().subtypeAtIndex(0).name() + "_3_" + node.name();
        *_currentSource << "(";

        node.receiver()->accept(*this);

        node.eachChildWithLast([&] (ASTNode* child, bool last) {
            *_currentSource << ", ";
            child->accept(*this);
        });

        *_currentSource << ")";
    }

    void CCodeGenVisitor::visit(UnaryOperatorNode& node) {
        *_currentSource << "(";
        *_currentSource << node.op();

        node.childAtIndex(0)->accept(*this);

        *_currentSource << ")";
    }

    void CCodeGenVisitor::visit(StringLiteralNode& node) {
        *_currentSource << "\"" << node.stringValue() << "\"";
    }

    void CCodeGenVisitor::visit(SwitchNode& node) {
        node.eachChild([&] (ASTNode* child, uint32_t index) {
            CaseNode* caseNode = dynamic_cast<CaseNode*>(child);

            if (index != 0) {
                *_currentSource << "} else ";
            }

            *_currentSource << "if (";
            node.argument()->accept(*this);
            *_currentSource << " == ";
            caseNode->argument()->accept(*this);
            _currentSource->printLineAndIndent(") {");

            caseNode->accept(*this);

            _currentSource->decreaseIndentation();
        });

        if (node.elseNode()) {
            _currentSource->printLineAndIndent("} else {");

            node.elseNode()->accept(*this);

            _currentSource->decreaseIndentation();
        }

        _currentSource->printLine("}");
    }

    void CCodeGenVisitor::visit(IfNode& node) {
        *_currentSource << "if ";

        // Some nodes need to add parentheses themselves to ensure
        // the correct of evaluation
        if (!node.condition()->includesParentheses()) {
            *_currentSource << "(";
        }

        node.condition()->accept(*this);

        if (!node.condition()->includesParentheses()) {
            *_currentSource << ")";
        }

        _currentSource->printLineAndIndent(" {");

        this->visitChildren(node);

        if (node.elseStatement()) {
            _currentSource->outdentAndPrintLine("} else {");
            _currentSource->increaseIndentation();

            this->visitChildren(*node.elseStatement());
        }

        _currentSource->outdentAndPrintLine("}");
    }

    void CCodeGenVisitor::visit(StructureNode& node) {
        // TODO: this is not right, but it greatly simplifies the header management needed.

        if (node.visibility == TranslationUnit::Visibility::External) {
            this->sourceForVisibility(TranslationUnit::Visibility::External, [&node] (CSource& source) {
                // create an opaque structure declaration
                source.print("typedef struct ");
                source.print(node.name());
                source.print(" ");
                source.print(node.name());
                source.printLine(";");
                source.printLine(""); 
            });
        }

        this->sourceForVisibility(TranslationUnit::Visibility::Internal, [&] (CSource& source) {
            if (node.packing() != 0) {
                std::stringstream s;

                // TODO: this is kinda stupid just to convert into a string...
                s << node.packing();

                source.printLine("#pragma pack(push)");
                source.printLine("#pragma pack(" + s.str() + ")");
            }

            source.print("typedef struct ");
            source.print(node.name());
            source.printLineAndIndent(" {");

            CSource* oldSource = _currentSource;
            _currentSource = &source;
            this->visitChildren(node, true);
            _currentSource = oldSource;

            source.outdentAndPrintLine("} " + node.name() + ";");

            if (node.packing() != 0) {
                source.printLine("#pragma pack(pop)");
            }

            source.printLine(""); // padding
        });
    }

    void CCodeGenVisitor::visit(EnumerationNode& node) {
        this->sourceForVisibility(node.visibility, [&] (CSource& source) {
            source.printLineAndIndent("enum {");

            node.eachMemberWithLast([&] (const std::string& memberName, bool last) {
                source << node.name() << "_3_" << memberName;

                if (!last) {
                    source << ",";
                }

                source.printLine("");
            });

            source.outdentAndPrintLine("};");
            source << "typedef uint32_t " << node.name();
            source.printLine(";");
            source.printLine("");
        });
    }

    void CCodeGenVisitor::visit(MemberAccessNode& node) {
        node.childAtIndex(0)->accept(*this);

        if (node.indirect()) {
            *_currentSource << "->";
        } else {
            *_currentSource << ".";
        }

        *_currentSource << node.memberName();
    }

    void CCodeGenVisitor::visit(TernaryOperatorNode& node) {
        assert(node.childCount() == 3);

        if (node.op() == "?") {
            node.childAtIndex(0)->accept(*this);
            *_currentSource << " ? ";
            node.childAtIndex(1)->accept(*this);
            *_currentSource << " : ";
            node.childAtIndex(2)->accept(*this);
            return;
        }

        assert(0 && "codegen for ternary operator not supported");
    }

    void CCodeGenVisitor::visit(DestructuredAssignmentOperatorNode& node) {
        assert(node.childCount() == 2);

        std::string tmpVariableName = "tmp_return";

        // TODO: make this the data type of the right hand side
        NewDataType tupleType = node.childAtIndex(1)->dataType();
        assert(tupleType.kind() == NewDataType::Kind::Tuple);

        // part one, a temporary variable to hold the right-hand side value
        *_currentSource << CTypeCodeGenerator::codeGen(tupleType, tmpVariableName);
        *_currentSource << " = ";

        // assign it to the right hand side
        node.childAtIndex(1)->accept(*this);

        _currentSource->printLine(";");

        // now, carry out the parallel assignment
        ASTNode* tuple = node.childAtIndex(0);

        tuple->eachChild([&] (ASTNode* child, uint32_t idx) {
            child->accept(*this);

            *_currentSource << " = ";

            *_currentSource << tmpVariableName << "." << tupleType.subtypeAtIndex(idx).label();

            // This is a hack: this node is defined as a statement.
            if (idx < tuple->childCount() - 1) {
                _currentSource->printLine(";");
            }
        });
    }

    void CCodeGenVisitor::visit(IndexerNode& node) {
        assert(node.childCount() == 2);

        node.childAtIndex(0)->accept(*this);

        *_currentSource << "[";

        // index expression
        node.childAtIndex(1)->accept(*this);

        *_currentSource << "]";
    }

    void CCodeGenVisitor::visit(ReturnNode& node) {
        // codegen state
        _tmpReturnValueCounter++;

        // this is the easy case - no children
        if (node.childCount() == 0) {
            if (node.endsTransaction()) {
                this->endCurrentTransaction(node.transactionName());
            }

            *_currentSource << "return";
            return;
        }

        if (!node.endsTransaction()) {
            *_currentSource << "return ";
            this->returnStatementArgument(node);

            return;
        }

        // if this return is impliciting ending a transaction,
        // we need to buffer the return value, so all arguments
        // of the return are evaluated before the transaction
        // is complete
        std::stringstream s;

        s << "tmp_return_value_" << _tmpReturnValueCounter;

        std::string varName = s.str();

        // we need a temp variable
        *_currentSource << CTypeCodeGenerator::codeGen(node.childAtIndex(0)->dataType(), varName);

        *_currentSource << " = ";
        this->returnStatementArgument(node);
        _currentSource->printLine(";");

        if (node.endsTransaction()) {
            this->endCurrentTransaction(node.transactionName());
        }

        *_currentSource << "return " << varName;
    }

    void CCodeGenVisitor::visit(NullLiteralNode& node) {
        *_currentSource << "NULL";
    }

    void CCodeGenVisitor::visit(BooleanLiteralNode& node) {
        *_currentSource << (node.value() ? "true" : "false");
    }

    void CCodeGenVisitor::visit(TupleNode& node) {
        
        std::string structName = CTypeCodeGenerator::codeGen(node.dataType());

        // first a cast to the struct type, which must be defined
        *_currentSource << "(" << structName << ")";

        *_currentSource << "{";

        node.eachChildWithLast([&] (ASTNode* child, bool last) {
            child->accept(*this);

            if (!last) {
                *_currentSource << ", ";
            }
        });

        *_currentSource << "}";
    }

    void CCodeGenVisitor::visit(LoopNode& node) {
        // infinite loop
        if (!node.condition()) {
            _currentSource->printLineAndIndent("for (;;) {");

            this->visitChildren(node);

            _currentSource->outdentAndPrintLine("}");

            return;
        }

        // do-while
        if (node.evaluateConditionAtEnd()) {
            _currentSource->printLineAndIndent("do {");

            this->visitChildren(node);

            _currentSource->print("} while (");
            node.condition()->accept(*this);
            _currentSource->outdentAndPrintLine(");");

            return;
        }

        // regular while
        _currentSource->print("while (");
        node.condition()->accept(*this);
        _currentSource->printLineAndIndent(") {");

        this->visitChildren(node);

        _currentSource->outdentAndPrintLine("}");
    }

    void CCodeGenVisitor::visit(ForNode& node) {
        *_currentSource << "for (";

        this->forStartExpression(node);
        *_currentSource << "; ";

        this->forCondition(node);
        *_currentSource << "; ";

        this->forLoopExpression(node);

        _currentSource->printLineAndIndent(") {");
        this->visitChildren(node);
        _currentSource->outdentAndPrintLine("}");
    }

    void CCodeGenVisitor::visit(BreakNode& node) {
        *_currentSource << "break";
    }

    void CCodeGenVisitor::visit(ContinueNode& node) {
        *_currentSource << "continue";
    }

    void CCodeGenVisitor::visit(SizeofNode& node) {
        *_currentSource << "sizeof(";
        *_currentSource << CTypeCodeGenerator::codeGen(node.dataTypeArgument);
        *_currentSource << ")";
    }

    void CCodeGenVisitor::visit(CastNode& node) {
        assert(node.childCount() == 1);

        *_currentSource << "(";
        *_currentSource << CTypeCodeGenerator::codeGen(node.dataTypeArgument);
        *_currentSource << ")";

        *_currentSource << "(";
        node.childAtIndex(0)->accept(*this);
        *_currentSource << ")";
    }

    void CCodeGenVisitor::visit(NextargNode& node) {
        *_currentSource << "va_arg(";
        node.childAtIndex(0)->accept(*this);
        *_currentSource << ", ";
        *_currentSource << CTypeCodeGenerator::codeGen(node.dataTypeArgument);
        *_currentSource << ")";
    }

    void CCodeGenVisitor::visit(AbortStatementNode& node) {
        this->prepareForTransactions();

        *_currentSource << "three_transaction_abort(&";
        *_currentSource << node.transactionName();
        *_currentSource << ")";
    }

    void CCodeGenVisitor::visit(AtomicExpressionNode& node) {
        this->prepareForAtomicExpressions();

        assert(node.childCount() == 1);

        _declaractionsSource << "THREE_CHECK_ATOMIC(";
        _declaractionsSource << CTypeCodeGenerator::codeGen(node.op()->dataType());
        _declaractionsSource.printLine(");");

        // we need to inspect the operation inside the expression to figure out
        // what code we actually need to emit

        std::string functionName = this->c11AtomicFunctionForFullOperation(node.op()->op());
        if (functionName != "") {
            *_currentSource << functionName;
            *_currentSource << "(";
            this->atomicVariable(node.op());
            *_currentSource << ", ";
            node.op()->childAtIndex(1)->accept(*this);
            *_currentSource << ", ";
            *_currentSource << this->c11MemoryOrderString(node.ordering());
            *_currentSource << ")";

            return;
        }

        if (this->c11AtomicFunctionIsLoadOperation(node.op()->op())) {
            *_currentSource << "atomic_load_explicit(";
            this->atomicVariable(node.op());
            *_currentSource << ", ";
            *_currentSource << this->c11MemoryOrderString(node.ordering());
            *_currentSource << ") ";

            *_currentSource << node.op()->op();
            *_currentSource << " ";

            node.op()->childAtIndex(1)->accept(*this);

            return;
        }

        assert(0 && "Atomic expression codegen failure");
    }

    void CCodeGenVisitor::visit(AtomicStatementNode& node) {
        this->prepareForTransactions();

        this->transactionAllocation(node.transactionName());

        *_currentSource << "if (three_transaction_begin(&" << node.transactionName();
        _currentSource->printLineAndIndent(")) {");

        this->visitChildren(node);

        // This is unnecessary when the last statement is a return
        if (node.lastChild()->nodeName() != "Return") {
            this->endCurrentTransaction(node.transactionName());
        }

        if (node.elseClause()) {
            _currentSource->outdentAndPrintLine("} else {");
            _currentSource->increaseIndentation();

            node.elseClause()->accept(*this);
        } else {
            _currentSource->outdentAndPrintLine("} else {");

            _declaractionsSource.addHeader(false, "assert.h");

            _currentSource->increaseIndentation();

            // assert(0 && "transaction 'tx_1' failed without any fallback path");
            *_currentSource << "assert(0 && \"transaction '" << node.transactionName();
            *_currentSource << "' failed without any fallback path\"";
            _currentSource->printLine(");");
        }

        _currentSource->outdentAndPrintLine("}");
    }

    void CCodeGenVisitor::visit(BarrierNode& node) {
        this->prepareForAtomicExpressions();

        *_currentSource << "atomic_thread_fence(";
        *_currentSource << this->c11MemoryOrderString(node.ordering());
        _currentSource->printLine(");");
    }

    void CCodeGenVisitor::visit(ValueNode& node) {
        *_currentSource << node.value();
    }

    void CCodeGenVisitor::visit(CMacroNode& node) {
        *_currentSource << node.value();
    }

    // For Loop Support
    void CCodeGenVisitor::forCondition(ForNode& node) {
        if (node.condition()) {
            node.condition()->accept(*this);
            return;
        }

        assert(node.rangeStartExpression() && node.rangeEndExpression());

        *_currentSource << "(";
        *_currentSource << node.rangeLoopVariable()->name;
        *_currentSource << " < ";

        node.rangeEndExpression()->accept(*this);

        *_currentSource << ") && (";

        node.rangeStartExpression()->accept(*this);

        *_currentSource << " < ";

        node.rangeEndExpression()->accept(*this);

        *_currentSource << ")";
    }

    void CCodeGenVisitor::forStartExpression(ForNode& node) {
        node.startExpression()->accept(*this);

        if (node.rangeEndExpression()) {
            *_currentSource << " = ";
            node.rangeStartExpression()->accept(*this);
        }
    }

    void CCodeGenVisitor::forLoopExpression(ForNode& node) {
        if (node.loopExpression()) {
            node.loopExpression()->accept(*this);
            return;
        }

        assert(node.rangeStartExpression() && node.rangeEndExpression());

        *_currentSource << "++" << node.rangeLoopVariable()->name;
    }

    void CCodeGenVisitor::returnStatementArgument(ReturnNode& node) {
        this->visitChildren(node);
    }

    // Transactions Support
    void CCodeGenVisitor::prepareForAtomicExpressions() {
        _declaractionsSource.addHeader(false, "three/runtime/atomic.h");
    }

    void CCodeGenVisitor::prepareForTransactions() {
        _declaractionsSource.addHeader(false, "three/runtime/transactional_memory.h");
    }

    void CCodeGenVisitor::transactionAllocation(const std::string& name) {
        *_currentSource << "three_transaction_t ";
        *_currentSource << name;
        *_currentSource << " = THREE_MAKE_DEFAULT_TRANSACTION()";
        _currentSource->printLine(";");
    }

    void CCodeGenVisitor::endCurrentTransaction(const std::string& name) {
        *_currentSource << "three_transaction_end(";
        *_currentSource << "&" << name;
        _currentSource->printLine(");");
    }

    std::string CCodeGenVisitor::c11MemoryOrderString(AtomicNode::Ordering order) const {
        switch (order) {
            case AtomicNode::Ordering::None:
                return "memory_order_relaxed";
            case AtomicNode::Ordering::Consume:
                return "memory_order_consume";
            case AtomicNode::Ordering::Acquire:
                return "memory_order_acquire";
            case AtomicNode::Ordering::Release:
                return "memory_order_release";
            case AtomicNode::Ordering::AcquireRelease:
                return "memory_order_acq_rel";
            case AtomicNode::Ordering::SequentiallyConsistent:
                return "memory_order_seq_cst";
            default:
                break;
        }

        assert(0 && "Unhandled memory ordering type");

        return "";
    }

    void CCodeGenVisitor::atomicVariable(OperatorNode* op) {
        *_currentSource << "(_Atomic(";
        *_currentSource << CTypeCodeGenerator::codeGen(op->dataType());
        *_currentSource << ")*)";
        *_currentSource << "&";

        op->childAtIndex(0)->accept(*this);
    }

    std::string CCodeGenVisitor::c11AtomicFunctionForFullOperation(const std::string& op) {
        if (op == "+=") {
            return "atomic_fetch_add_explicit";
        } else if (op == "-=") {
            return "atomic_fetch_sub_explicit";
        }

        return "";
    }

    bool CCodeGenVisitor::c11AtomicFunctionIsLoadOperation(const std::string& op) {
        // >, >=, <, <=, ==
        return op == ">" || op == ">=" || op == "<" || op == "<=" || op == "==";
    }

    void CCodeGenVisitor::closureEnvironmentStructure(ClosureNode& node, CSource& source) {
        source << "struct " << node.name() << "_env";
        source.printLineAndIndent(" {");

        for (const NewDataType& type : node.environmentStructureType().subtypes) {
            source << CTypeCodeGenerator::codeGen(type, type.label());
            source.printLine(";");
        }

        source.outdentAndPrintLine("};");
    }

    std::string CCodeGenVisitor::closureEnvironmentCapture(ClosureNode& node) {
        std::stringstream s;

        s << "THREE_CAPTURE_ENV(" << node.name() << "_env";

        node.eachCapturedVariable([&s] (NewVariable* v, bool ref, bool last) {
            s << ", ";

            if (ref) {
                s << "&";
            }

            s << v->name;
        });

        s << ");";

        return s.str();
    }

    void CCodeGenVisitor::createDefinitionForTuple(const NewDataType& tupleType, CSource& source) {
        source.printLineAndIndent("typedef struct {");

        tupleType.eachSubtypeWithLast([&] (const NewDataType& type, bool last) {
            source << CTypeCodeGenerator::codeGen(type);
            source << " " << type.label();

            source.printLine(";");
        });

        std::string structName = CTypeCodeGenerator::codeGenTupleStructName(tupleType);

        source.outdentAndPrintLine("} " + structName + ";");
    }

    // Utilities
    void CCodeGenVisitor::visitChildren(ASTNode& node, bool statements) {
        node.eachChild([&] (ASTNode* child, uint32_t _) {
            child->accept(*this);
            if (statements || child->statement()) {
                _currentSource->printLine(";");
            }
        });
    }

    // CSource Handling
    std::string CCodeGenVisitor::externalHeaderString() {
        return _externalHeaderSource.renderToString();
    }

    std::string CCodeGenVisitor::internalHeaderString() {
        return _internalHeaderSource.renderToString();
    }

    std::string CCodeGenVisitor::declarationsString() {
        return _declaractionsSource.renderToString();
    }

    std::string CCodeGenVisitor::bodyString() {
        return _bodySource.renderToString();
    }

    void CCodeGenVisitor::addInternalHeader(bool relative, const std::string& path) {
        _internalHeaderSource.addHeader(relative, path);
    }

    void CCodeGenVisitor::addDeclarationsHeader(bool relative, const std::string& path) {
        _declaractionsSource.addHeader(relative, path);
    }

    void CCodeGenVisitor::sourceForVisibility(TranslationUnit::Visibility v, std::function<void (CSource&)> func) {
        switch (v) {
            case TranslationUnit::Visibility::None:
                func(_declaractionsSource);
                break;
            case TranslationUnit::Visibility::Internal:
            case TranslationUnit::Visibility::Default:
                func(_internalHeaderSource);
                break;
            case TranslationUnit::Visibility::External:
                func(_externalHeaderSource);
                break;
            default:
                break;
        }
    }
}
