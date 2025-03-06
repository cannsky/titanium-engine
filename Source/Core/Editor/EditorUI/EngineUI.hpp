#pragma once

namespace TulparEngine::EngineEditor {
    class EngineUI {
    public:
        // Pure virtual function
        virtual void renderGui() = 0;

        // Virtual destructor (recommended for abstract classes)
        virtual ~EngineUI() = default;
    };
}