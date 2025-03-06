#include "NodeFactory.hpp"

namespace TulparEngine::EngineEditor {

    EditorNode NodeFactory::CreateFunctionCallNode(const std::string& functionName, ImVec2 pos) {
        EditorNode node;
        node.name = "Call: " + functionName;
        node.type = EditorNodeType::FunctionCall;
        node.vsType = VisualScriptNodeType::FunctionCall;
        node.functionName = functionName;
        node.position = pos;
        return node;
    }

    EditorNode NodeFactory::CreateVariableGetterNode(const std::string& varName, ImVec2 pos) {
        EditorNode node;
        node.name = "Get " + varName;
        node.type = EditorNodeType::VariableGetter;
        node.vsType = VisualScriptNodeType::VariableGetter;
        node.variableName = varName;
        node.position = pos;
        return node;
    }

    EditorNode NodeFactory::CreateVariableSetterNode(const std::string& varName, ImVec2 pos) {
        EditorNode node;
        node.name = "Set " + varName;
        node.type = EditorNodeType::VariableSetter;
        node.vsType = VisualScriptNodeType::VariableSetter;
        node.variableName = varName;
        node.position = pos;
        return node;
    }

    EditorNode NodeFactory::CreateArithmeticNode(const std::string& op, ImVec2 pos) {
        EditorNode node;
        node.name = op;
        node.type = EditorNodeType::ArithmeticOp;
        node.vsType = VisualScriptNodeType::ArithmeticOp;
        node.op = op;
        node.position = pos;
        return node;
    }

    EditorNode NodeFactory::CreateFunctionInputNode(const std::string& functionName, ImVec2 pos) {
        EditorNode node;
        node.name = functionName + " Input";
        node.type = EditorNodeType::FunctionInput;
        node.vsType = VisualScriptNodeType::FunctionInput;
        node.functionName = functionName;
        node.position = pos;
        return node;
    }

    EditorNode NodeFactory::CreateFunctionOutputNode(const std::string& functionName, ImVec2 pos) {
        EditorNode node;
        node.name = functionName + " Output";
        node.type = EditorNodeType::FunctionOutput;
        node.vsType = VisualScriptNodeType::FunctionOutput;
        node.functionName = functionName;
        node.position = pos;
        return node;
    }

    EditorNode NodeFactory::CreateLogNode(ImVec2 pos) {
        EditorNode node;
        node.name = "Log";
        node.type = EditorNodeType::Log;
        node.vsType = VisualScriptNodeType::Log;
        node.position = pos;
        return node;
    }

    EditorNode NodeFactory::CreateStateNode(const std::string& stateName, ImVec2 pos) {
        EditorNode node;
        node.name = stateName;
        node.type = EditorNodeType::State;
        node.vsType = VisualScriptNodeType::None; // Not used in state machine
        node.position = pos;
        return node;
    }

}