#include "DebugNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/Scope.h"

#include <assert.h>
#include <sstream>

namespace Three {
	DebugNode* DebugNode::parse(Parser& parser) {
		assert(parser.helper()->nextIf(Token::Type::KeywordDebug));

		DebugNode* node = new DebugNode();

		if (!parser.helper()->parseNewline()) {
			assert(0 && "Message: new line expected after debug definition");
		}

		parser.context()->pushScope();

		bool parsedSuccessfully = true;

		// TODO: the success conditions need to be improved here
		parser.helper()->parseUntilEnd([&] () {
			bool success = false;

			if (!node->addChild(parser.parseTopLevelNode(&success))) {
				parsedSuccessfully = success;
				return true;
			}

			return false;
		});

		if (!parsedSuccessfully) {
			delete node;

			return nullptr;
		}

		parser.context()->popScope();

		return node;
	}

	std::string DebugNode::nodeName() const {
		return "Debug";
	}

	std::string DebugNode::name() const {
		return this->nodeName();
	}
}
