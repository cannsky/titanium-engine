#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <imgui.h>
#include <absl/container/flat_hash_map.h>

namespace TulparEngine::EngineEditor {
    using NodeID = int;

    enum class EditorNodeType {
        State,
        VariableGetter,
        ArithmeticOp,
        FunctionCall,
        FunctionInput,
        FunctionOutput,
        VariableSetter,
        Log
    };

    enum class GraphType {
        StateMachine,
        VisualScript
    };

    enum class VisualScriptNodeType {
        None,
        FunctionInput,
        FunctionOutput,
        VariableGetter,
        VariableSetter,
        ArithmeticOp,
        FunctionCall,
        Log
    };

    struct FunctionArgument {
        std::string name;
        std::string type; // "bool", "int", "string"
    };

    struct EditorNode {
        NodeID id = 0;
        std::string name;
        ImVec2 position = ImVec2(0,0);
        EditorNodeType type = EditorNodeType::State;

        std::string variableName; 
        std::string functionName; 
        std::string op;

        VisualScriptNodeType vsType = VisualScriptNodeType::None;
        std::vector<FunctionArgument> inputArguments;
        std::vector<FunctionArgument> outputArguments;
    };

    struct EditorLink {
        int id = 0;
        NodeID fromNode = 0;
        NodeID toNode = 0;
        int startAttribute = 0;
        int endAttribute = 0;
    };

    // Unified variable type
    enum class VariableType {
        Bool,
        Int,
        String
    };

    struct EditorVariable {
        std::string name;
        VariableType type;
        std::variant<bool, int, std::string> value;
    };

    struct EditorGraph {
        GraphType graphType;
        std::unordered_map<NodeID, EditorNode> nodes;
        std::vector<EditorLink> links;

        // Unified variable storage
        absl::flat_hash_map<std::string, EditorVariable> variables;

        EditorGraph(GraphType t) : graphType(t) {}
    };
}