#include "CSourceIndexer.h"
#include "runtime/platform.h"

#include <assert.h>
#include <clang-c/Index.h>
#include <iostream>
#include <fstream>

static int abortQuery(CXClientData clientData, void* reserved);
static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved);
static CXIdxClientFile enteredMainFile(CXClientData client_data, CXFile mainFile, void *reserved);
static CXIdxClientFile includedFile(CXClientData clientData, const CXIdxIncludedFileInfo* includedFileInfo);

static void indexDeclaration(CXClientData clientData, const CXIdxDeclInfo* declInfo);
static Three::CompoundField FieldFromIndex(const std::string& name, const CXIdxDeclInfo* declInfo);
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

#if THREE_PLATFORM_DARWIN
        std::string xcodePath = executeCommand("xcrun --show-sdk-path");

        if (xcodePath.length() > 0) {
            xcodePath = xcodePath.substr(0, xcodePath.length() - 1);
            _includePaths->push_back(xcodePath + "/usr/include");
            _includePaths->push_back(xcodePath + "/usr/local/include");
        }
#elif THREE_PLATFORM_LINUX
        // TODO: bogus include paths for gcc 4.8
        _includePaths->push_back("/usr/include/x86_64-linux-gnu");
        _includePaths->push_back("/usr/lib/gcc/x86_64-linux-gnu/4.8/include");
        _includePaths->push_back("/usr/lib/gcc/x86_64-linux-gnu/4.8/include-fixed");

        // and for clang 3.3
        _includePaths->push_back("/usr/local/bin/../lib/clang/3.3/include");
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

    CSourceIndexer::CSourceIndexer() : verbose(false) , _currentCompoundType(NULL) {
        _module = new Three::Module();

        _module->addDataType(new DataType(DataType::Flavor::Scalar, "Void"));
        _module->addDataType(new DataType(DataType::Flavor::Scalar, "Int"));
        _module->addDataType(new DataType(DataType::Flavor::Scalar, "Char"));
    }

    CSourceIndexer::~CSourceIndexer() {
    }

    bool CSourceIndexer::indexFileAtPath(const std::string& path) {
        CXIndex       index  = clang_createIndex(1, 0);
        CXIndexAction action = clang_IndexAction_create(index);

        IndexerCallbacks callbacks;

        callbacks.abortQuery = abortQuery;
        callbacks.diagnostic = diagnostic;
        callbacks.enteredMainFile = enteredMainFile;
        callbacks.ppIncludedFile = includedFile;
        callbacks.importedASTFile = NULL;
        callbacks.startedTranslationUnit = NULL;
        callbacks.indexDeclaration = indexDeclaration;
        callbacks.indexEntityReference = NULL;

        assert(path.length() > 0);

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
                                           path.c_str(),
                                           args,
                                           argCount,
                                           NULL,
                                           0,
                                           NULL,
                                           CXTranslationUnit_DetailedPreprocessingRecord);

        free(args);

        clang_IndexAction_dispose(action);
        clang_disposeIndex(index);

        return result == 0;
    }

    Module* CSourceIndexer::module() const {
        return _module;
    }

    void CSourceIndexer::addFunction(const std::string& name) {
        Function* fn;

        _currentCompoundType = nullptr;
        _fields.clear();

        if (_module->functionForName(name)) {
            _module->removeFunctionForName(name);
            std::cout << "[Indexer] Redefining function '" << name << "' " << std::endl;
        }

        fn = new Function();
        fn->setName(name);

        // TODO: this is bogus!!! But, all functions need a return type at minimum.
        fn->setReturnType(TypeReference::ref(_module, "Void", 0));

        _module->addFunction(name, fn);
    }

    void CSourceIndexer::addType(const std::string& name, DataType::Flavor flavor) {
        DataType* type;

        if (verbose) {
            std::cout << "[Indexer] creating type '" << name << "'" << std::endl;
        }

        // first, check to see if we've already defined a type with this name
        type = _module->dataTypeForName(name);
        if (type) {
            // now, if this type is just the struct version of a scalar, we can ignore it safely
            // If we are now defining a new type with a struct prefix, then we can just remove
            // the existing type
            if (!type->cSourcePrependStructKeyword() && flavor == DataType::Flavor::Structure) {
                return;
            }

            _module->removeDataTypeForName(name);
            std::cout << "[Indexer] Redefining type '" << name << "' " << flavor << std::endl;
            //assert(0 && "Redefintion of type during indexing");
        }

        // everything is good to define a new type
        type = new DataType(flavor, name);

        if (type->isCompound()) {
            _currentCompoundType = type;

            for (const CompoundField& field : _fields) {
                type->addChild(TypeReference(type, field.indirection, field.dimensions), field.name);
            }
        } else {
            _currentCompoundType = nullptr;
        }

        _fields.clear(); // remove all buffered fields in both cases

        if (flavor == DataType::Flavor::Structure) {
            type->setCSourcePrependStructKeyword(true);
        }

        _module->addDataType(type);
    }

    void CSourceIndexer::addVariable(const std::string& name) {
        _currentCompoundType = nullptr;
        _fields.clear();

        // TODO: this is wack
        _module->addConstant(name, name);
    }

    void CSourceIndexer::addConstant(const std::string& name) {
        _currentCompoundType = nullptr;
        _fields.clear();

        _module->addConstant(name, name);
    }

    void CSourceIndexer::addField(const CompoundField& field) {
        if (!_currentCompoundType) {
            _fields.push_back(field);
            return;
        }

        DataType* type;

        type = _module->dataTypeForName(field.typeName);

        if (!type) {
            std::cout << "[Indexer] unable to look up field '" << field.name << "' type '" << field.typeName << "'" << std::endl;
            return;
        }

        _currentCompoundType->addChild(TypeReference(type, field.indirection, field.dimensions), field.name);
    }
}

static int abortQuery(CXClientData clientData, void* reserved) {
    return 0;
}

static void diagnostic(CXClientData clientData, CXDiagnosticSet diagnosticSet, void* reserved) {
    unsigned diagnosticCount = clang_getNumDiagnosticsInSet(diagnosticSet);

    for (unsigned i = 0; i < diagnosticCount; ++i) {
        CXDiagnostic diagnostic = clang_getDiagnosticInSet(diagnosticSet, i);

        CXString cxstring = clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions());

        std::string string = std::string(clang_getCString(cxstring));

        clang_disposeString(cxstring);

        // This works around an annoying clang bug that will warn, unnecessarily, when using "#pragma once"
        if (string.find("#pragma once in main file", 0, 25) != std::string::npos) {
            continue;
        }

        std::cout << "C parse error: " << string << std::endl;
    }
}

static CXIdxClientFile enteredMainFile(CXClientData client_data, CXFile mainFile, void *reserved) {
    // std::cout << "Indexing main " << std::endl;

    return nullptr;
}

static CXIdxClientFile includedFile(CXClientData clientData, const CXIdxIncludedFileInfo* includedFileInfo) {
    CXFile file = includedFileInfo->file;

    // std::cout << "Indexing include '" << clang_getCString(clang_getFileName(file)) << "'" << std::endl;

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
            index->addType(name, Three::DataType::Flavor::Scalar);
            break;
        case CXIdxEntity_EnumConstant:
            index->addConstant(name);
            break;
        case CXIdxEntity_Function:
            index->addFunction(name);
            break;
        case CXIdxEntity_Variable:
            index->addVariable(name);
            break;
        case CXIdxEntity_Enum:
            break;
        case CXIdxEntity_Struct:
            if (name.length() > 0) {
                index->addType(name, Three::DataType::Flavor::Structure);
            }
            break;
        case CXIdxEntity_Union:
            if (name.length() > 0) {
                index->addType(name, Three::DataType::Flavor::Union);
            }
            break;
        case CXIdxEntity_Field:
            index->addField(FieldFromIndex(name, declInfo));
            break;
        default:
            break;
    }
}

static Three::CompoundField FieldFromIndex(const std::string& name, const CXIdxDeclInfo* declInfo) {
    CXType type = clang_getCursorType(declInfo->cursor);

    uint32_t indirection;

    // TODO: this isn't right
    indirection = type.kind == CXType_Pointer ? 1 : 0;

    // clang_getCString(clang_getTypeSpelling(type))

    Three::CompoundField field;

    field.name = name;
    field.typeName = std::string("Int");
    field.indirection = indirection;

    return field;
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
