#pragma once
#include "BaseNodeEditor.hpp"
#include "GraphManager.hpp"
#include <memory>
#include <string>

namespace TulparEngine::EngineEditor {
    class StateMachineEditor : public BaseNodeEditor {
    public:
        StateMachineEditor(const std::string& smName);

        // Overridden from BaseNodeEditor
        void AddLink(NodeID fromId, NodeID toId, int startAttr, int endAttr) override;
        void RemoveLinkByID(int linkId) override;
        const std::unordered_map<NodeID, EditorNode>& GetNodes() const override;
        const std::vector<EditorLink>& GetLinks() const override;

        bool LoadFromFile(const std::string& filePath) override;
        bool SaveToFile(const std::string& filePath) override;

        void UpdateNodePositions() override;
        void RemoveNode(NodeID nodeId) override;

        bool IsPinOutput(const EditorNode& node, int pinIndex) const override;
        bool IsFlowPin(const EditorNode& node, int pinIndex) const override;

        EditorGraph* GetCurrentGraph() override;
        EditorGraph* GetMainGraph() override;
        const EditorGraph* GetCurrentGraph() const override;
        const EditorGraph* GetMainGraph() const override;

    private:
        std::string name;
        std::unique_ptr<EditorGraph> graph;
        std::unique_ptr<GraphManager> manager;
    };
}