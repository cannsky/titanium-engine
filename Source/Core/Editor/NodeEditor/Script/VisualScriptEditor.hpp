#pragma once
#include "BaseNodeEditor.hpp"
#include "GraphManager.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace TulparEngine::EngineEditor {
    class VisualScriptEditor : public BaseNodeEditor {
    public:
        VisualScriptEditor(const std::string& scriptName);

        void NewFunction(const std::string& functionName);

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

        void SwitchToGraph(const std::string& graphName);

        // New methods so NodeEditorLeftPanelUI can list / switch "functions"
        std::vector<std::string> GetFunctionNames() const;
        const std::string& GetCurrentGraphName() const;
        GraphManager* GetMainManager();

    private:
        std::string mainScriptName;
        std::string currentGraphName;

        // Each function (graph) in a VisualScriptEditor
        std::unordered_map<std::string, std::unique_ptr<EditorGraph>> graphs;
        std::unordered_map<std::string, std::unique_ptr<GraphManager>> managers;

        // Helper: Get the current GraphManager
        GraphManager* GetCurrentManager();
    };
}