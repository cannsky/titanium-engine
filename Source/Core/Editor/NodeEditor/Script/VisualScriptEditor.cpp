#include "VisualScriptEditor.hpp"
#include <imnodes.h>

namespace TulparEngine::EngineEditor {

    VisualScriptEditor::VisualScriptEditor(const std::string& scriptName)
        : mainScriptName(scriptName), currentGraphName(scriptName)
    {
        auto g = std::make_unique<EditorGraph>(GraphType::VisualScript);
        managers[scriptName] = std::make_unique<GraphManager>(*g);
        graphs[scriptName] = std::move(g);

        // Example initialization of default nodes for the main function:
        EditorNode inputNode;
        inputNode.name = scriptName + " Input";
        inputNode.type = EditorNodeType::FunctionInput;
        inputNode.vsType = VisualScriptNodeType::FunctionInput;
        GetMainManager()->AddNode(inputNode);

        EditorNode outputNode;
        outputNode.name = scriptName + " Output";
        outputNode.type = EditorNodeType::FunctionOutput;
        outputNode.vsType = VisualScriptNodeType::FunctionOutput;
        GetMainManager()->AddNode(outputNode);
    }

    void VisualScriptEditor::NewFunction(const std::string& functionName) {
        // If a graph with this name already exists, skip
        if (graphs.find(functionName) != graphs.end()) return;

        auto g = std::make_unique<EditorGraph>(GraphType::VisualScript);
        managers[functionName] = std::make_unique<GraphManager>(*g);
        graphs[functionName] = std::move(g);

        // For each new function, add an input and output node
        EditorNode inputNode;
        inputNode.name = functionName + " Input";
        inputNode.type = EditorNodeType::FunctionInput;
        inputNode.vsType = VisualScriptNodeType::FunctionInput;
        managers[functionName]->AddNode(inputNode);

        EditorNode outputNode;
        outputNode.name = functionName + " Output";
        outputNode.type = EditorNodeType::FunctionOutput;
        outputNode.vsType = VisualScriptNodeType::FunctionOutput;
        managers[functionName]->AddNode(outputNode);
    }

    void VisualScriptEditor::AddLink(NodeID fromId, NodeID toId, int startAttr, int endAttr) {
        GetCurrentManager()->AddLink(fromId, toId, startAttr, endAttr);
    }

    void VisualScriptEditor::RemoveLinkByID(int linkId) {
        GetCurrentManager()->RemoveLinkByID(linkId);
    }

    const std::unordered_map<NodeID, EditorNode>& VisualScriptEditor::GetNodes() const {
        return managers.at(currentGraphName)->GetNodes();
    }

    const std::vector<EditorLink>& VisualScriptEditor::GetLinks() const {
        return managers.at(currentGraphName)->GetLinks();
    }

    bool VisualScriptEditor::LoadFromFile(const std::string& filePath) {
        // Loading logic not implemented
        return true;
    }

    bool VisualScriptEditor::SaveToFile(const std::string& filePath) {
        // Saving logic not implemented
        return true;
    }

    void VisualScriptEditor::UpdateNodePositions() {
        for (auto& [id, node] : graphs.at(currentGraphName)->nodes) {
            node.position = ImNodes::GetNodeEditorSpacePos(id);
        }
    }

    void VisualScriptEditor::RemoveNode(NodeID nodeId) {
        GetCurrentManager()->RemoveNode(nodeId);
    }

    bool VisualScriptEditor::IsPinOutput(const EditorNode& node, int pinIndex) const {
        // Example logic, adapt as desired
        if (node.type == EditorNodeType::FunctionInput) {
            return (pinIndex == 0);
        }
        else if (node.type == EditorNodeType::FunctionOutput) {
            return false;
        }
        else if (node.type == EditorNodeType::VariableGetter) {
            return (pinIndex == 0);
        }
        else if (node.type == EditorNodeType::VariableSetter) {
            return (pinIndex == 1);
        }
        else if (node.type == EditorNodeType::ArithmeticOp) {
            return (pinIndex == 2);
        }
        else if (node.type == EditorNodeType::FunctionCall) {
            return (pinIndex == 1);
        }
        else if (node.type == EditorNodeType::State) {
            return (pinIndex == 1);
        }
        else if (node.type == EditorNodeType::Log) {
            return (pinIndex == 1);
        }
        return false;
    }

    bool VisualScriptEditor::IsFlowPin(const EditorNode& node, int pinIndex) const {
        // Example logic, adapt as desired
        switch (node.type) {
            case EditorNodeType::FunctionInput:
            case EditorNodeType::FunctionOutput:
            case EditorNodeType::FunctionCall:
            case EditorNodeType::State:
                return true;
            case EditorNodeType::VariableGetter:
                return false;
            case EditorNodeType::VariableSetter:
                return (pinIndex == 0 || pinIndex == 1);
            case EditorNodeType::ArithmeticOp:
                return (pinIndex == 0 || pinIndex == 1);
            case EditorNodeType::Log:
                return (pinIndex == 1);
        }
        return false;
    }

    EditorGraph* VisualScriptEditor::GetCurrentGraph() {
        return graphs.at(currentGraphName).get();
    }

    EditorGraph* VisualScriptEditor::GetMainGraph() {
        return graphs.at(mainScriptName).get();
    }

    const EditorGraph* VisualScriptEditor::GetCurrentGraph() const {
        return graphs.at(currentGraphName).get();
    }

    const EditorGraph* VisualScriptEditor::GetMainGraph() const {
        return graphs.at(mainScriptName).get();
    }

    void VisualScriptEditor::SwitchToGraph(const std::string& graphName) {
        if (graphs.find(graphName) != graphs.end()) {
            currentGraphName = graphName;
        }
    }

    GraphManager* VisualScriptEditor::GetCurrentManager() {
        return managers.at(currentGraphName).get();
    }

    GraphManager* VisualScriptEditor::GetMainManager() {
        return managers.at(mainScriptName).get();
    }

    std::vector<std::string> VisualScriptEditor::GetFunctionNames() const {
        std::vector<std::string> result;
        result.reserve(graphs.size());
        for (auto& kv : graphs) {
            result.push_back(kv.first);
        }
        return result;
    }

    const std::string& VisualScriptEditor::GetCurrentGraphName() const {
        return currentGraphName;
    }

}