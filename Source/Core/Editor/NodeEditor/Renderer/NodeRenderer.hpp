#pragma once
#include "GraphTypes.hpp"
#include <imgui.h>
#include <imnodes.h>

namespace TulparEngine::EngineEditor {
    // Abstract base class for rendering a single node type.
    class NodeRenderer {
    public:
        virtual ~NodeRenderer() {}
        virtual void RenderNode(const EditorNode& node) = 0;
    };
}