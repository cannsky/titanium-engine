#pragma once

namespace TulparEngine::EngineEditor {
    class NodeEditorUIUtils {
    public:
        static int AttributeID(int nodeId, int pinIndex) {
            return nodeId * 100 + pinIndex;
        }
    };
}