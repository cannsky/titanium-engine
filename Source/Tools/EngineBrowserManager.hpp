#pragma once

#ifndef ENGINEBROWSERMANAGER_HPP
#define ENGINEBROWSERMANAGER_HPP

#include <string>
#include <memory>
#ifdef QT_ENABLED
#include <QApplication>
#include <QWebEngineView>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include <QEvent>
#endif

#include "EngineDebugManager.hpp"
#include "EngineVulkanManager.hpp"
#include "EditorAssistantChatUI.hpp"

namespace TulparEngine::Tools {

    class EngineBrowserManager {
    public:
        static EngineBrowserManager& GetInstance() {
            static EngineBrowserManager instance;
            return instance;
        }
        #ifdef QT_ENABLED
        void Init();             // Initializes the browser manager and loads the page
        void Run();              // Processes events to keep the app responsive
        void Cleanup();          // Cleans up resources

        QImage CaptureWebPage(); // Returns the captured image
        void RenderOffscreenAndGenerateVkImage();
        VkImage CreateVulkanImageFromQImage(const QImage& img);

        void OnMouseClicked();
        void OnKeyPressed(int key); // Accept Qt::Key code instead of string

        VkImage GetVkImage() const { return currentVKImage; }
        VkImageView GetVkImageView() const { return currentImageView; }
        VkDescriptorSet GetDescriptorSet() const { return currentDescriptorSet; }
        #endif

    private:
        #ifdef QT_ENABLED
        EngineBrowserManager();
        ~EngineBrowserManager();
        
        void RenderOffscreen(); // Renders the web page offscreen to a QImage

        std::unique_ptr<QApplication> app;
        std::unique_ptr<QWebEngineView> webView;
        QImage currentImage; // Holds the latest rendered image
        VkImage currentVKImage;
        VkImageView currentImageView;
        VkDescriptorSet currentDescriptorSet;
        std::unique_ptr<QTimer> renderTimer;
        #endif
    };
}

#endif // ENGINEBROWSERMANAGER_HPP