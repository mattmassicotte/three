#pragma once

#include "../ASTNode.h"

namespace Three {
	class DebugNode : public ASTNode {
	public:
		static DebugNode* parse(Parser& parser);

	public:
		std::string nodeName() const;
		std::string name() const;
	};
}
