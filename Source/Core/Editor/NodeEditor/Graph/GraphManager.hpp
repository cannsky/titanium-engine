#pragma once
#include "GraphTypes.hpp"

namespace TulparEngine::EngineEditor {

    class GraphManager {
    public:
        explicit GraphManager(EditorGraph& graph);

        NodeID AddNode(const EditorNode& nodeTemplate);
        void RemoveNode(NodeID id);

        void AddLink(NodeID fromNode, NodeID toNode, int startAttr, int endAttr);
        void RemoveLinkByID(int linkId);

        bool AddVariable(const std::string& name, VariableType type, std::variant<bool, int, std::string> initialValue);
        EditorVariable* GetVariable(const std::string& name);

        const std::unordered_map<NodeID, EditorNode>& GetNodes() const;
        const std::vector<EditorLink>& GetLinks() const;

        // If you need other utility methods, add them here.

    private:
        EditorGraph& graph;
        NodeID nextNodeId = 1;
        int nextLinkId = 1;
    };

}