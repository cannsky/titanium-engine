#pragma once
#include "GraphTypes.hpp"
#include <string>

namespace TulparEngine::EngineEditor {

    // NodeFactory creates common node types for convenience.
    class NodeFactory {
    public:
        static EditorNode CreateFunctionCallNode(const std::string& functionName, ImVec2 pos = ImVec2(0,0));
        static EditorNode CreateVariableGetterNode(const std::string& varName, ImVec2 pos = ImVec2(0,0));
        static EditorNode CreateVariableSetterNode(const std::string& varName, ImVec2 pos = ImVec2(0,0));
        static EditorNode CreateArithmeticNode(const std::string& op, ImVec2 pos = ImVec2(0,0));
        static EditorNode CreateFunctionInputNode(const std::string& functionName, ImVec2 pos = ImVec2(0,0));
        static EditorNode CreateFunctionOutputNode(const std::string& functionName, ImVec2 pos = ImVec2(0,0));
        static EditorNode CreateLogNode(ImVec2 pos = ImVec2(0,0));
        static EditorNode CreateStateNode(const std::string& stateName, ImVec2 pos = ImVec2(0,0));
    };

}