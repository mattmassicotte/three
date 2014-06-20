#include "CSourceEmitter.h"
#include "compiler/Operations/CCodeGenVisitor.h"
#include "compiler/Parser/ParseContext.h"
#include "compiler/AST/RootNode.h"

#include <assert.h>
#include <fstream>

namespace Three {
    bool CSourceEmitter::createSourcesAtPath(const ParseContext& context, const std::string& outputBasePath) {
        Three::CCodeGenVisitor visitor;

        context.rootNode()->accept(visitor);

        std::ofstream bodyFile(outputBasePath + ".c");

        assert(bodyFile);

        // make sure to import the headers
        visitor.addInternalHeader(true, outputBasePath + ".h");
        visitor.addDeclarationsHeader(true, outputBasePath + "_internal.h");

        bodyFile << "// Declarations" << std::endl;
        bodyFile << visitor.declarationsString();
        bodyFile << std::endl << "// Definitions" << std::endl;
        bodyFile << visitor.bodyString();

        std::ofstream internalHeaderFile(outputBasePath + "_internal.h");

        internalHeaderFile << Three::CSource::includeGuard("");
        internalHeaderFile << visitor.internalHeaderString();

        std::ofstream externalHeaderFile(outputBasePath + ".h");

        externalHeaderFile << Three::CSource::includeGuard("");
        externalHeaderFile << visitor.externalHeaderString();

        return true;
    }

    std::string CSourceEmitter::pathWithoutExtension(const std::string& path) {
        return path.substr(0, path.length() - 2);
    }
}
