#include "CSourceIndexer.h"

#include <assert.h>
#include <clang-c/Index.h>
#include <iostream>
#include <fstream>

static int abortQuery(CXClientData clientData, void* reserved);
static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved);
static void indexDeclaration(CXClientData clientData, const CXIdxDeclInfo* declInfo);

static std::string executeCommand(const char* cmd);

static std::vector<std::string>* _includePaths = NULL;

namespace Three {
    std::vector<std::string>* CSourceIndexer::defaultCIncludePaths() {
        if (_includePaths) {
            return _includePaths;
        }

        _includePaths = new std::vector<std::string>();

        _includePaths->push_back("/usr/include");
        _includePaths->push_back("/usr/local/include");

#if defined(__APPLE__)
        std::string xcodePath = executeCommand("xcrun --show-sdk-path");

        if (xcodePath.length() > 0) {
            xcodePath = xcodePath.substr(0, xcodePath.length() - 1);
            _includePaths->push_back(xcodePath + "/usr/include");
            _includePaths->push_back(xcodePath + "/usr/local/include");
        }
#endif

        return _includePaths;
    }

    std::string CSourceIndexer::resolveCHeaderPath(const std::string& partialPath) {
        for (const std::string& path : *CSourceIndexer::defaultCIncludePaths()) {
            std::string fullPath = path + "/" + partialPath;

            // constructing an ifstream checks if we can open the file for reading
            std::ifstream file(fullPath);

            if (file) {
                return fullPath;
            }
        }

        return partialPath;
    }

    CSourceIndexer::CSourceIndexer() {
        _module = new Three::Module();
    }

    CSourceIndexer::~CSourceIndexer() {
    }

    bool CSourceIndexer::indexFileAtPath(const std::string& path) {
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

        assert(path.length() > 0);

        std::string fullPath = CSourceIndexer::resolveCHeaderPath(path);

        int result = clang_indexSourceFile(action,
                                           this,
                                           &callbacks,
                                           sizeof(IndexerCallbacks),
                                           CXIndexOpt_SuppressRedundantRefs,
                                           fullPath.c_str(),
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

    Module* CSourceIndexer::module() const {
        return _module;
    }

    void CSourceIndexer::addFunction(const std::string& name) {
        Language::Function* fn;

        fn = new Language::Function();
        fn->setName(name);

        _module->addFunction(name, fn);
    }
}

static int abortQuery(CXClientData clientData, void* reserved) {
    return 0;
}

static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved) {
}

static void indexDeclaration(CXClientData clientData, const CXIdxDeclInfo* declInfo) {
    Three::CSourceIndexer* index = static_cast<Three::CSourceIndexer*>(clientData);
    std::string name;

    if (declInfo->entityInfo->name) {
        name = std::string(declInfo->entityInfo->name);
    }

    switch(declInfo->entityInfo->kind) {
        case CXIdxEntity_Typedef:
            break;
        case CXIdxEntity_Function:
            index->addFunction(name);
            break;
        case CXIdxEntity_Variable:
            break;
        case CXIdxEntity_Enum:
            break;
        case CXIdxEntity_Struct:
            break;
        case CXIdxEntity_Union:
            break;
        default:
            break;
    }
}

std::string executeCommand(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        return "";
    }

    char buffer[128];
    std::string result = "";

    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            result += buffer;
        }
    }

    pclose(pipe);

    return result;
}
