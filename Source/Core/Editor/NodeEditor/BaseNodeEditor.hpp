#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <imgui.h>
#include "GraphTypes.hpp"

namespace TulparEngine::EngineEditor {
    class BaseNodeEditor {
    public:
        virtual ~BaseNodeEditor() {}

        // Graph manipulation
        virtual void AddLink(NodeID fromId, NodeID toId, int startAttr, int endAttr) = 0;
        virtual void RemoveLinkByID(int linkId) = 0;
        virtual const std::unordered_map<NodeID, EditorNode>& GetNodes() const = 0;
        virtual const std::vector<EditorLink>& GetLinks() const = 0;

        // File operations
        virtual bool LoadFromFile(const std::string& filePath) = 0;
        virtual bool SaveToFile(const std::string& filePath) = 0;

        // Node position updates
        virtual void UpdateNodePositions() = 0;
        virtual void RemoveNode(NodeID nodeId) = 0;

        // Pin queries
        virtual bool IsPinOutput(const EditorNode& node, int pinIndex) const = 0;
        virtual bool IsFlowPin(const EditorNode& node, int pinIndex) const = 0;

        // Access the graphs
        virtual EditorGraph* GetCurrentGraph() = 0;
        virtual EditorGraph* GetMainGraph() = 0;
        virtual const EditorGraph* GetCurrentGraph() const = 0;
        virtual const EditorGraph* GetMainGraph() const = 0;
    };

}