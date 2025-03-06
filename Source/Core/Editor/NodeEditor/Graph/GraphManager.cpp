#include "GraphManager.hpp"
#include <algorithm>

namespace TulparEngine::EngineEditor {

    GraphManager::GraphManager(EditorGraph& g) : graph(g) {}

    NodeID GraphManager::AddNode(const EditorNode& nodeTemplate) {
        EditorNode n = nodeTemplate;
        n.id = nextNodeId++;
        graph.nodes[n.id] = n;
        return n.id;
    }

    void GraphManager::RemoveNode(NodeID id) {
        graph.links.erase(std::remove_if(graph.links.begin(), graph.links.end(),
                                         [id](const EditorLink& l){
                                             return l.fromNode == id || l.toNode == id;
                                         }), graph.links.end());
        graph.nodes.erase(id);
    }

    void GraphManager::AddLink(NodeID fromNode, NodeID toNode, int startAttr, int endAttr) {
        // Check if link already exists
        for (auto& link : graph.links) {
            if (link.fromNode == fromNode && link.toNode == toNode &&
                link.startAttribute == startAttr && link.endAttribute == endAttr) {
                return;
            }
        }

        EditorLink l;
        l.id = nextLinkId++;
        l.fromNode = fromNode;
        l.toNode = toNode;
        l.startAttribute = startAttr;
        l.endAttribute = endAttr;
        graph.links.push_back(l);
    }

    void GraphManager::RemoveLinkByID(int linkId) {
        graph.links.erase(std::remove_if(graph.links.begin(), graph.links.end(),
                                         [linkId](const EditorLink& l){return l.id == linkId;}), graph.links.end());
    }

    bool GraphManager::AddVariable(const std::string& name, VariableType type, std::variant<bool, int, std::string> initialValue) {
        if (graph.variables.find(name) != graph.variables.end()) return false;
        EditorVariable var;
        var.name = name;
        var.type = type;
        var.value = initialValue;
        graph.variables[name] = var;
        return true;
    }

    // Retrieve a variable by name
    EditorVariable* GraphManager::GetVariable(const std::string& name) {
        auto it = graph.variables.find(name);
        if (it != graph.variables.end()) return &it->second;
        return nullptr;
    }

    const std::unordered_map<NodeID, EditorNode>& GraphManager::GetNodes() const {
        return graph.nodes;
    }

    const std::vector<EditorLink>& GraphManager::GetLinks() const {
        return graph.links;
    }

}