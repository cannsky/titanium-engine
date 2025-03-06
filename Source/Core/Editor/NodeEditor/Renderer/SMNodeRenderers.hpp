#pragma once
#include "NodeRenderer.hpp"
#include <unordered_map>

namespace TulparEngine::EngineEditor {

    enum class StateMachineNodeType {
        StateNode
        // Add other node types if needed
    };

    class SMNodeRendererRegistry {
    public:
        static SMNodeRendererRegistry& Instance() {
            static SMNodeRendererRegistry instance;
            return instance;
        }

        void RegisterRenderer(StateMachineNodeType type, NodeRenderer* renderer) {
            renderers[type] = renderer;
        }

        NodeRenderer* GetRenderer(StateMachineNodeType type) {
            auto it = renderers.find(type);
            if (it != renderers.end()) return it->second;
            return nullptr;
        }

    private:
        std::unordered_map<StateMachineNodeType, NodeRenderer*> renderers;
    };

    class StateNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            // State nodes typically have 1 input (flow) and 1 output (flow)
            ImNodes::BeginInputAttribute(node.id * 100);
            ImGui::Text("In");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id * 100 + 1);
            ImGui::Text("Out");
            ImNodes::EndOutputAttribute();
        }
    };

    inline void RegisterSMNodeRenderers() {
        SMNodeRendererRegistry::Instance().RegisterRenderer(StateMachineNodeType::StateNode, new StateNodeRenderer());
    }

}