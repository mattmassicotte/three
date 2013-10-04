#include "CSourceIndexer.h"

#include <assert.h>
#include <clang-c/Index.h>
#include <iostream>
#include <fstream>

static int abortQuery(CXClientData clientData, void* reserved);
static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved);
static CXIdxClientFile includedFile(CXClientData clientData, const CXIdxIncludedFileInfo* includedFileInfo);
                                    
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

    std::vector<std::string> CSourceIndexer::defaultCCompilerArguments() {
        std::vector<std::string> arguments;

        for (const std::string& path : *CSourceIndexer::defaultCIncludePaths()) {
            std::string includeString;

            includeString = "-I" + path;

            arguments.push_back(includeString);
        }

        return arguments;
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
        callbacks.ppIncludedFile = includedFile;
        callbacks.importedASTFile = NULL;
        callbacks.startedTranslationUnit = NULL;
        callbacks.indexDeclaration = indexDeclaration;
        callbacks.indexEntityReference = NULL;

        assert(path.length() > 0);

        std::string fullPath = CSourceIndexer::resolveCHeaderPath(path);

        std::vector<std::string> arguments = CSourceIndexer::defaultCCompilerArguments();
        unsigned argCount = arguments.size();
        
        const char** args = (const char**)malloc(argCount * sizeof(char*));

        for (int i = 0; i < argCount; ++i) {
            args[i] = arguments.at(i).c_str();
        }

        int result = clang_indexSourceFile(action,
                                           this,
                                           &callbacks,
                                           sizeof(IndexerCallbacks),
                                           CXIndexOpt_SuppressRedundantRefs,
                                           fullPath.c_str(),
                                           args,
                                           argCount,
                                           NULL,
                                           0,
                                           NULL,
                                           0);

        free(args);

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

    void CSourceIndexer::addType(const std::string& name, Language::DataType::Flavor flavor) {
        Language::DataType* type;

        // first, check to see if we've already defined a type with this name
        type = _module->dataTypeForName(name);
        if (type) {
            // now, if this type is just the struct version of a scalar, we can ignore it safely
            // If we are now defining a new type with a struct prefix, then we can just remove
            // the existing type
            if (!type->cSourcePrependStructKeyword() && flavor == Language::DataType::Flavor::Structure) {
                return;
            }

            assert(0 && "Redefintion of type during indexing");
        }

        // everything is good to define a new type
        type = new Language::DataType(flavor, name);

        if (flavor == Language::DataType::Flavor::Structure) {
            type->setCSourcePrependStructKeyword(true);
        }

        _module->addDataType(type);
    }
}

static int abortQuery(CXClientData clientData, void* reserved) {
    return 0;
}

static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved) {
    unsigned diagnosticCount = clang_getNumDiagnosticsInSet(diagnosticSet);

    for (unsigned i = 0; i < diagnosticCount; ++i) {
        CXDiagnostic diagnostic = clang_getDiagnosticInSet(diagnosticSet, i);

        CXString string = clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions());

        std::cout << "C parse error: " << clang_getCString(string) << std::endl;
    }
}

static CXIdxClientFile includedFile(CXClientData clientData, const CXIdxIncludedFileInfo* includedFileInfo) {
    CXFile file = includedFileInfo->file;

    //std::cout << "Indexing include '" << clang_getCString(clang_getFileName(file)) << "'" << std::endl;

    return nullptr;
}

static void indexDeclaration(CXClientData clientData, const CXIdxDeclInfo* declInfo) {
    Three::CSourceIndexer* index = static_cast<Three::CSourceIndexer*>(clientData);
    std::string name;

    if (declInfo->entityInfo->name) {
        name = std::string(declInfo->entityInfo->name);
    }

    switch(declInfo->entityInfo->kind) {
        case CXIdxEntity_Typedef:
            index->addType(name, Language::DataType::Flavor::Scalar);
            break;
        case CXIdxEntity_Function:
            index->addFunction(name);
            break;
        case CXIdxEntity_Variable:
            break;
        case CXIdxEntity_Enum:
            break;
        case CXIdxEntity_Struct:
            if (name.length() > 0) {
                index->addType(name, Language::DataType::Flavor::Structure);
            }
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
