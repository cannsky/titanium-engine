#pragma once

#include "imgui.h"
#include <vector>
#include <string>
#include <vulkan/vulkan.h>
#ifdef QT_ENABLED
#include <QImage>
#include <QApplication>
#include <QOpenGLContext>
#include <QPainter>
#include <QWebEngineSettings>
#endif

#include "EngineBrowserManager.hpp"

namespace TulparEngine::EngineEditor {

    class EditorAssistantChatUI {
    public:
        static EditorAssistantChatUI& GetInstance() {
            static EditorAssistantChatUI instance;
            return instance;
        }

        void renderGui();

        void cleanup();

        void Run();

        ImVec2 GetImageSize() const { return imageSize; }
        ImVec2 GetImagePos() const { return imagePos; }
        ImVec2 GetMousePos() const { return mousePos; }
        bool GetIsMouseOverImage() const { return isMouseOverImage; }

    private:
        EditorAssistantChatUI();
        ~EditorAssistantChatUI();

        EditorAssistantChatUI(const EditorAssistantChatUI&) = delete;
        void operator=(const EditorAssistantChatUI&) = delete;

        ImVec2 mousePos;
        ImVec2 imageSize;
        ImVec2 imagePos;

        bool isMouseClicked;
        bool isMouseOverImage;

        void CheckIsMouseClickingImage();
    };

}