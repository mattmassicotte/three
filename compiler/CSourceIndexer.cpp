#include "CSourceIndexer.h"

#include <assert.h>
#include <clang-c/Index.h>
#include <iostream>

static int abortQuery(CXClientData clientData, void* reserved);
static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved);
static void indexDeclaration(CXClientData clientData, const CXIdxDeclInfo* declInfo);

namespace Three {
    CSourceIndexer::CSourceIndexer() {
    }

    CSourceIndexer::~CSourceIndexer() {
    }

    bool CSourceIndexer::indexFileAtPath(const char* path) {
        CXIndex       index  = clang_createIndex(1, 0);
        CXIndexAction action = clang_IndexAction_create(index);

        IndexerCallbacks callbacks;

        callbacks.abortQuery = abortQuery;
        callbacks.diagnostic = diagnostic;
        callbacks.enteredMainFile = NULL;
        callbacks.ppIncludedFile = NULL;
        callbacks.importedASTFile = NULL;
        callbacks.startedTranslationUnit = NULL;
        callbacks.indexDeclaration = indexDeclaration;
        callbacks.indexEntityReference = NULL;

        assert(path);

        int result = clang_indexSourceFile(action,
                                           this,
                                           &callbacks,
                                           sizeof(IndexerCallbacks),
                                           CXIndexOpt_SuppressRedundantRefs,
                                           path,
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           NULL,
                                           0);

        clang_IndexAction_dispose(action);
        clang_disposeIndex(index);

        return result == 0;
    }
}

static int abortQuery(CXClientData clientData, void* reserved) {
    return 0;
}

static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved) {
}

static void indexDeclaration(CXClientData clientData, const CXIdxDeclInfo* declInfo) {
    switch(declInfo->entityInfo->kind) {
        case CXIdxEntity_Typedef:
        case CXIdxEntity_Function:
        case CXIdxEntity_Variable:
        case CXIdxEntity_Enum:
        case CXIdxEntity_Struct:
        case CXIdxEntity_Union:
            if (declInfo->entityInfo->name) {
                std::cout << "indexDeclaration: " << declInfo->entityInfo->name << std::endl;
            }
            break;
        default:
            break;
    }
    
}
