#include "StateMachineEditor.hpp"
#include <imnodes.h>

namespace TulparEngine::EngineEditor {

    StateMachineEditor::StateMachineEditor(const std::string& smName)
        : name(smName)
    {
        graph = std::make_unique<EditorGraph>(GraphType::StateMachine);
        manager = std::make_unique<GraphManager>(*graph);
    }

    void StateMachineEditor::AddLink(NodeID fromId, NodeID toId, int startAttr, int endAttr) {
        manager->AddLink(fromId, toId, startAttr, endAttr);
    }

    void StateMachineEditor::RemoveLinkByID(int linkId) {
        manager->RemoveLinkByID(linkId);
    }

    const std::unordered_map<NodeID, EditorNode>& StateMachineEditor::GetNodes() const {
        return manager->GetNodes();
    }

    const std::vector<EditorLink>& StateMachineEditor::GetLinks() const {
        return manager->GetLinks();
    }

    bool StateMachineEditor::LoadFromFile(const std::string& filePath) {
        // Not implemented here
        return true;
    }

    bool StateMachineEditor::SaveToFile(const std::string& filePath) {
        // Not implemented here
        return true;
    }

    void StateMachineEditor::UpdateNodePositions() {
        for (auto& [id, node] : graph->nodes) {
            node.position = ImNodes::GetNodeEditorSpacePos(id);
        }
    }

    void StateMachineEditor::RemoveNode(NodeID nodeId) {
        manager->RemoveNode(nodeId);
    }

    bool StateMachineEditor::IsPinOutput(const EditorNode& node, int pinIndex) const {
        // In a state machine, let's assume output pin is always pinIndex == 1
        return pinIndex == 1;
    }

    bool StateMachineEditor::IsFlowPin(const EditorNode& node, int pinIndex) const {
        // Assume all pins in a state machine are flow pins
        return true;
    }

    EditorGraph* StateMachineEditor::GetCurrentGraph() {
        return graph.get();
    }

    EditorGraph* StateMachineEditor::GetMainGraph() {
        return graph.get();
    }

    const EditorGraph* StateMachineEditor::GetCurrentGraph() const {
        return graph.get();
    }

    const EditorGraph* StateMachineEditor::GetMainGraph() const {
        return graph.get();
    }

}