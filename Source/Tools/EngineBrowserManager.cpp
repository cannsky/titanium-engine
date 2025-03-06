#include "EngineBrowserManager.hpp"

namespace TulparEngine::Tools {
    #ifdef QT_ENABLED
    EngineBrowserManager::EngineBrowserManager() : app(nullptr), webView(nullptr) {}

    EngineBrowserManager::~EngineBrowserManager() {
        #ifdef QT_ENABLED
        Cleanup();
        #endif
    }

    void EngineBrowserManager::Init() {
        // Define dummy command-line arguments
        int argc = 1;
        const char* argv[] = { "EngineBrowserManagerApp" };

        // Initialize QApplication with the dummy arguments
        app = std::make_unique<QApplication>(argc, const_cast<char**>(argv));

        // Create the QWebEngineView instance for offscreen rendering
        webView = std::make_unique<QWebEngineView>();
        webView->setAttribute(Qt::WA_DontShowOnScreen, false); // Prevents showing on the screen
        webView->setUrl(QUrl("http://localhost:3000")); // Replace with your desired URL

        // Ensure the widget is part of the application but not shown to the user
        webView->show(); // Shows it in a way that triggers rendering without displaying
        webView->setFocusPolicy(Qt::StrongFocus);

        // Connect to the loadFinished signal to render once the page is loaded
        QObject::connect(webView->page(), &QWebEnginePage::loadFinished, [this](bool success) {
            if (success) {
                EngineDebugManager::log("Web page loaded successfully.", DebugCategory::BrowserManager);
                RenderOffscreenAndGenerateVkImage();
            } else {
                EngineDebugManager::log("Failed to load the webpage.", DebugCategory::BrowserManager);
            }
        });

        renderTimer = std::make_unique<QTimer>();
        renderTimer->setInterval(250); // Set interval to 5 seconds
        QObject::connect(renderTimer.get(), &QTimer::timeout, [this]() {
            RenderOffscreenAndGenerateVkImage();
        });
        renderTimer->start();
    }

    void EngineBrowserManager::RenderOffscreen() {
        if (webView) {
            webView->resize(400, EngineEditor::EditorAssistantChatUI::GetInstance().GetImageSize().y); // Resize the widget to the desired dimensions
            QImage image(webView->size(), QImage::Format_RGBA8888);
            image.fill(Qt::transparent); // Clear the image

            QPainter painter(&image);
            webView->render(&painter);
            painter.end();

            currentImage = image;
        }
    }

    QImage EngineBrowserManager::CaptureWebPage() {
        if (currentImage.isNull()) {
            EngineDebugManager::log("Current image is empty!", DebugCategory::BrowserManager);
        }
        return currentImage;
    }

    void EngineBrowserManager::Run() {
        if (app) {
            app->processEvents(); // Process events to keep the app responsive
        }
    }

    void EngineBrowserManager::Cleanup() {
        if (webView) {
            webView.reset();
        }
        if (app) {
            app->quit();
            app.reset();
        }
    }

    void EngineBrowserManager::RenderOffscreenAndGenerateVkImage() {
        // Render the page to an offscreen QImage
        RenderOffscreen();

        if (currentImage.isNull()) {
            EngineDebugManager::log("Current image is empty!", DebugCategory::BrowserManager);
            return;
        }

        // Create a Vulkan image from the captured QImage
        EngineVulkanManager& vulkanManager = EngineVulkanManager::GetInstance();
        VkImage vkImage = vulkanManager.CreateVulkanImageFromQImage(currentImage);

        currentVKImage = vkImage;

        if (vkImage == VK_NULL_HANDLE) {
            EngineDebugManager::log("Failed to create Vulkan image from QImage!", DebugCategory::BrowserManager);
        }

        currentImageView = vulkanManager.createImageView(vkImage, VK_FORMAT_R8G8B8A8_UNORM);

        vulkanManager.allocateTextureDescriptorSet(currentDescriptorSet);
        vulkanManager.updateTextureDescriptorSet(currentDescriptorSet, currentImageView);
    }

    void EngineBrowserManager::OnMouseClicked() {
        ImVec2 mousePos = EngineEditor::EditorAssistantChatUI::GetInstance().GetMousePos();
        ImVec2 imageSize = EngineEditor::EditorAssistantChatUI::GetInstance().GetImageSize();
        ImVec2 imagePos = EngineEditor::EditorAssistantChatUI::GetInstance().GetImagePos();

        // Calculate the relative position within the image
        double xRatio = (mousePos.x - imagePos.x) / imageSize.x;
        double yRatio = (mousePos.y - imagePos.y) / imageSize.y;

        // Convert ratios to page coordinates
        int pageWidth = webView->size().width();
        int pageHeight = webView->size().height();
        int pageX = static_cast<int>(xRatio * pageWidth);
        int pageY = static_cast<int>(yRatio * pageHeight);

        // JavaScript to simulate mouse events at the calculated position
        QString jsCode = QString(
            "var elem = document.elementFromPoint(%1, %2);"
            "if(elem) {"
            "   elem.focus();"
            "   var rect = elem.getBoundingClientRect();"
            "   var x = %1 - rect.left;"
            "   var y = %2 - rect.top;"
            "   var eventOptions = {clientX: %1, clientY: %2, bubbles: true};"
            "   elem.dispatchEvent(new MouseEvent('mouseover', eventOptions));"
            "   elem.dispatchEvent(new MouseEvent('mousemove', eventOptions));"
            "   elem.dispatchEvent(new MouseEvent('mousedown', eventOptions));"
            "   elem.dispatchEvent(new MouseEvent('mouseup', eventOptions));"
            "   elem.dispatchEvent(new MouseEvent('click', eventOptions));"
            "}"
        ).arg(pageX).arg(pageY);

        webView->page()->runJavaScript(jsCode);
    }

    void EngineBrowserManager::OnKeyPressed(int key) {
        QString keyName;

        if (key >= Qt::Key_A && key <= Qt::Key_Z) {
            keyName = QChar(key).toLower(); // Convert to lowercase character
        } else {
            switch (key) {
                case Qt::Key_Space:
                    keyName = " ";
                    break;
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    keyName = "Enter";
                    break;
                case Qt::Key_Escape:
                    keyName = "Escape";
                    break;
                // Add more keys as needed
                default:
                    EngineDebugManager::log("Unsupported key code", DebugCategory::BrowserManager);
                    return;
            }
        }

        // Ensure the web view has focus
        webView->setFocus();

        // Simulate keydown event
        QString jsKeyDown = QString("document.dispatchEvent(new KeyboardEvent('keydown', {key: '%1', bubbles: true, cancelable: true}));").arg(keyName);
        webView->page()->runJavaScript(jsKeyDown);

        // Simulate keyup event
        QString jsKeyUp = QString("document.dispatchEvent(new KeyboardEvent('keyup', {key: '%1', bubbles: true, cancelable: true}));").arg(keyName);
        webView->page()->runJavaScript(jsKeyUp);

        EngineDebugManager::log("Key pressed", DebugCategory::BrowserManager);
    }
    #endif
}