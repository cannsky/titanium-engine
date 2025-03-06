#pragma once
#include "NodeRenderer.hpp"
#include <unordered_map>

namespace TulparEngine::EngineEditor {

    // A registry to map VisualScriptNodeType to a renderer
    class VSNodeRendererRegistry {
    public:
        static VSNodeRendererRegistry& Instance() {
            static VSNodeRendererRegistry instance;
            return instance;
        }

        void RegisterRenderer(VisualScriptNodeType type, NodeRenderer* renderer) {
            renderers[type] = renderer;
        }

        NodeRenderer* GetRenderer(VisualScriptNodeType type) {
            auto it = renderers.find(type);
            if (it != renderers.end()) return it->second;
            return nullptr;
        }

    private:
        std::unordered_map<VisualScriptNodeType, NodeRenderer*> renderers;
    };

    // Example renderers for various node types:
    class FunctionInputNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            // Example: just render a simple input pin and an output flow pin
            // Input nodes might have a single output flow pin
            // Input pins:
            // None, because it's a function entry point.

            // Output pin (flow):
            // We'll use node.id and a pin index to compute attribute IDs.
            ImNodes::BeginOutputAttribute(node.id * 100); 
            ImGui::Text("Out");
            ImNodes::EndOutputAttribute();
        }
    };

    class FunctionOutputNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            // Typically a function output might have an input flow pin
            ImNodes::BeginInputAttribute(node.id * 100);
            ImGui::Text("In");
            ImNodes::EndInputAttribute();
        }
    };

    class VariableGetterNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            // Variable Getter might have no input pins, but one output value pin
            ImNodes::BeginOutputAttribute(node.id * 100); 
            ImGui::Text("Value");
            ImNodes::EndOutputAttribute();
        }
    };

    class VariableSetterNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            // Variable Setter might have an input for value and a flow input and output
            // For simplicity, just show one input and one output
            ImNodes::BeginInputAttribute(node.id * 100);
            ImGui::Text("In (Flow)");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id * 100 + 1);
            ImGui::Text("Out (Flow)");
            ImNodes::EndOutputAttribute();

            ImNodes::BeginInputAttribute(node.id * 100 + 2);
            ImGui::Text("Value");
            ImNodes::EndInputAttribute();
        }
    };

    class ArithmeticOpNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            // Arithmetic node example: two inputs and one output
            ImNodes::BeginInputAttribute(node.id * 100);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.id * 100 + 1);
            ImGui::Text("B");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id * 100 + 2);
            ImGui::Text("Result");
            ImNodes::EndOutputAttribute();
        }
    };

    class FunctionCallNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            // Function call: one input flow, one output flow, plus possibly argument pins
            ImNodes::BeginInputAttribute(node.id * 100);
            ImGui::Text("In (Flow)");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id * 100 + 1);
            ImGui::Text("Out (Flow)");
            ImNodes::EndOutputAttribute();

            // For arguments, you would add more attributes based on node.inputArguments/outputArguments
        }
    };

    class LogNodeRenderer : public NodeRenderer {
    public:
        void RenderNode(const EditorNode& node) override {
            ImNodes::BeginInputAttribute(node.id * 100);
            ImGui::Text("In (Flow)");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id * 100 + 1);
            ImGui::Text("Out (Flow)");
            ImNodes::EndOutputAttribute();
        }
    };

    // Initialize the registry somewhere (e.g. in a function you call at startup)
    inline void RegisterVSNodeRenderers() {
        VSNodeRendererRegistry::Instance().RegisterRenderer(VisualScriptNodeType::FunctionInput, new FunctionInputNodeRenderer());
        VSNodeRendererRegistry::Instance().RegisterRenderer(VisualScriptNodeType::FunctionOutput, new FunctionOutputNodeRenderer());
        VSNodeRendererRegistry::Instance().RegisterRenderer(VisualScriptNodeType::VariableGetter, new VariableGetterNodeRenderer());
        VSNodeRendererRegistry::Instance().RegisterRenderer(VisualScriptNodeType::VariableSetter, new VariableSetterNodeRenderer());
        VSNodeRendererRegistry::Instance().RegisterRenderer(VisualScriptNodeType::ArithmeticOp, new ArithmeticOpNodeRenderer());
        VSNodeRendererRegistry::Instance().RegisterRenderer(VisualScriptNodeType::FunctionCall, new FunctionCallNodeRenderer());
        VSNodeRendererRegistry::Instance().RegisterRenderer(VisualScriptNodeType::Log, new LogNodeRenderer());
    }

}