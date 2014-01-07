#include "../compiler/Parser.h"
#include "../compiler/CodeGen/CSourceContext.h"
#include "../compiler/AST/ASTNode.h"
#include "../compiler/CSourceIndexer.h"
#include "REPL.h"

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <fstream>

#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    bool debug;
    bool printAST;
    bool trace;

    std::string outputFile;
    std::string compilerPath;
} build_options_t;

void getOptions(build_options_t* options, int argc, char** argv) {
    int32_t c;

    static struct option longopts[] = {
        { "debug",     no_argument,       NULL, 'd' },
        { "help",      no_argument,       NULL, 'h' },
        { "compiler",  required_argument, NULL, 'm' },
        { "output",    required_argument, NULL, 'o' },
        { "print",     no_argument,       NULL, 'p' },
        { "trace",     no_argument,       NULL, 't' },
        { "version",   no_argument,       NULL, 'v' },
        { NULL,        0,                 NULL, 0   }
    };

    options->debug    = false;
    options->printAST = false;
    options->trace    = false;
    options->compilerPath = std::string("cc");

    while ((c = getopt_long(argc, argv, "dhm:o:ptv", longopts, NULL)) != -1) {
        switch (c) {
            case 'd':
                options->debug = true;
                break;
            case 'm':
                options->compilerPath = std::string(optarg);
                break;
            case 'o':
                options->outputFile = std::string(optarg);
                break;
            case 'p':
                options->printAST = true;
                break;
            case 't':
                options->trace = true;
                break;
            case 'v':
                printf("Three Release: %s\n\n", "0.1");
                exit(0);
                break;
            case 'h':
            default:
                printf("usage: three [options] [--] [file] [arguments]\n");
                printf("\n");
                printf("  -d (--debug)      disable optimizations\n");
                printf("  -h (--help)       print this help message and exit\n");
                printf("  -m (--compiler)   path to C compiler (default: cc)\n");
                printf("  -o (--output)     name of output file\n");
                printf("  -p (--print)      print out the AST representation for the input file\n");
                printf("  -t (--trace)      trace compilation steps\n");
                printf("  -v (--version)    print versions\n");
                exit(0);
                break;
        }
    }
}

bool createCOutputs(Three::ParsingContext* context, const std::string& basePath) {
    Three::CSourceContext outputContext;

    context->rootNode()->codeGen(outputContext);
    
    std::ofstream bodyFile(basePath + ".c");

    // make sure to import the headers
    outputContext.internalDeclarations()->addHeader(true, basePath + ".h");
    outputContext.declarations()->addHeader(true, basePath + "_internal.h");

    bodyFile << "// Declarations" << std::endl;
    bodyFile << outputContext.declarations()->renderToString();
    bodyFile << std::endl << "// Definitions" << std::endl;
    bodyFile << outputContext.body()->renderToString();

    std::ofstream internalHeaderFile(basePath + "_internal.h");

    internalHeaderFile << outputContext.internalDeclarations()->renderToStringWithIncludeGuard("__" + basePath + "_internal__");

    std::ofstream externalHeaderFile(basePath + ".h");

    externalHeaderFile << outputContext.externalDeclarations()->renderToStringWithIncludeGuard("__" + basePath + "__");

    return true;
}

std::vector<std::string> defaultCIncludePaths(void) {
    std::vector<std::string> paths = *Three::CSourceIndexer::defaultCIncludePaths();

    paths.push_back("/usr/local/include/three/runtime");

    return paths;
}

std::vector<std::string> splitString(const std::string& string, const std::string& delimiter) {
    std::vector<std::string> parts;

    std::string::const_iterator substart = string.begin();
    std::string::const_iterator subend;

    for (;;) {
        subend = std::search(substart, string.end(), delimiter.begin(), delimiter.end());
        std::string temp(substart, subend);

        if (!temp.empty()) {
            parts.push_back(temp);
        }

        if (subend == string.end()) {
            break;
        }

        substart = subend + delimiter.size();
    }

    return parts;
}

bool createPath(const std::string& path) {
    std::vector<std::string> components = splitString(path, "/");

    std::string partial;
    for (const std::string& component : components) {
        partial += component + "/";

        if (mkdir(partial.c_str(), 00777) != 0) {
            if (errno != EEXIST) {
                return false;
            }
        }
    }

    return true;
}

bool compileCSource(const std::string& cCompilerPath, const std::string& cSourcePath, const std::string& outputPath) {
    std::stringstream s;

    s << cCompilerPath;
    s << " -std=c11";
    s << " -o '" << outputPath << "'" ;

    s << " -I.";

    for (const std::string& string : defaultCIncludePaths()) {
        s << " '-I" << string << "'";
    }

    s << " -c '" << cSourcePath << "'";

    // std::cout << s.str() << std::endl;

    return system(s.str().c_str()) == 0;
}

void adjustOutputFileName(build_options_t* options, const std::string& inputFile) {
    // create a sensible output path default, if none was supplied
    if (options->outputFile.length() == 0) {
        if (inputFile.substr(inputFile.length() - 2, 2) == ".3") {
            options->outputFile = inputFile.substr(0, inputFile.length() - 2);
        } else {
            options->outputFile = std::string("three_output");
        }
    }
}

int buildCSources(const std::string& inputFile) {
    Three::ParsingContext* parsingContext = Three::Parser::contextFromFile(inputFile);

    std::string output = inputFile.substr(0, inputFile.length() - 2);
    createCOutputs(parsingContext, output);

    return 0;
}

int linkExecutable(const std::string& linkerPath, const std::string& inputFile, const std::string& outputPath) {
    std::stringstream s;

    s << linkerPath;
    s << " -L/usr/local/lib -lthree_runtime";

    s << " -o '" << outputPath << "'" ;

    s << " '" << inputFile << "'";

    return system(s.str().c_str()) == 0;
}

int buildExecutable(build_options_t* options, const std::string& inputFile, Three::ParsingContext* context) {
    createCOutputs(context, options->outputFile);
    compileCSource(options->compilerPath, options->outputFile + ".c", options->outputFile + ".o");

    linkExecutable(options->compilerPath, options->outputFile + ".o", options->outputFile);

    return 1;
}

bool buildLibrary(const std::vector<std::string>& inputs, const std::string& outputPath) {
    // The output needs to be removed first
    unlink(outputPath.c_str());

    std::stringstream s;

    s << "ar crs";
    s << " '" << outputPath << "'";

    for (const std::string& string : inputs) {
        s << " '" << string << "'";
    }

    return system(s.str().c_str()) == 0;
}

int buildModule(build_options_t* options, Three::ParsingContext* context) {
    std::vector<std::string> pathComponents = splitString(context->currentModule()->name, "_3_");

    std::string path;

    for (const std::string& component : pathComponents) {
        path += component + "/";
    }

    if (!createPath(path)) {
        std::cerr << "Unable to create path" << std::endl;
        return 1;
    }

    for (const std::string& submodule : context->currentModule()->importedModules) {
        std::cout << "[Compile] building subcomponent '" << submodule << "'" << std::endl;

        buildCSources(submodule + ".3");
        compileCSource(options->compilerPath, submodule + ".c", submodule + ".o");
    }

    std::vector<std::string> objects;

    // now move the headers
    for (const std::string& submodule : context->currentModule()->importedModules) {
        rename((submodule + ".c").c_str(), (path + "/" + submodule + ".c").c_str());
        rename((submodule + ".h").c_str(), (path + "/" + submodule + ".h").c_str());
        rename((submodule + "_internal.h").c_str(), (path + "/" + submodule + "_internal.h").c_str());

        objects.push_back(submodule + ".o");
    }

    buildLibrary(objects, path + "/" + "lib" + options->outputFile + ".a");

    for (const std::string& object_file : objects) {
        unlink(object_file.c_str());
    }

    return 0;
}

int processInput(build_options_t* options, const std::string& inputFile) {
    Three::ParsingContext* parsingContext = Three::Parser::contextFromFile(inputFile);

    if (options->printAST) {
        std::cout << parsingContext->rootNode()->recursiveStr() << std::endl;
        return 0;
    }

    adjustOutputFileName(options, inputFile);

    if (parsingContext->currentModule()->hasMainFunction()) {
        std::cout << "[Compile] Building executable" << std::endl;
        return buildExecutable(options, inputFile, parsingContext);
    }

    return buildModule(options, parsingContext);
}

int main(int argc, char** argv) {
    build_options_t options;

    // read options, and then adjust the inputs
    getOptions(&options, argc, argv);
    argc -= optind;
    argv += optind;

    if (argc == 0) {
        return Three::REPL::start();
    }

    if (argc > 1) {
        std::cerr << "[Compile] Two many inputs given" << std::endl;
        return 2;
    }

    processInput(&options, std::string(argv[0]));

    return 0;
}
