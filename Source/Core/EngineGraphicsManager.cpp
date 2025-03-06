#include "EngineGraphicsManager.hpp"

namespace TulparEngine {

    struct GlobalUniformBufferObject {
        glm::mat4 projectionView{1.0f};
        glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 2.0f};
        glm::vec3 lightPosition{-1.0f};
        alignas(16) glm::vec4 lightColor{1.0f};
    };

    EngineGraphicsManager::EngineGraphicsManager() : uniformBuffers(EngineSwapChain::MAX_FRAMES_IN_FLIGHT) {}

    EngineGraphicsManager::~EngineGraphicsManager() {  }

    void EngineGraphicsManager::Init() {

        globalPool = EngineDescriptorPool::Builder()
            .setMaxSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

        // Add maybe | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT later

        for (int i = 0; i < uniformBuffers.size(); i++) {
            uniformBuffers[i] = std::make_unique<EngineBuffer>(
                sizeof(GlobalUniformBufferObject),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );
            uniformBuffers[i]->map();
        }

        auto globalSetLayout = EngineDescriptorSetLayout::Builder()
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        globalDescriptorSets.resize(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uniformBuffers[i]->descriptorInfo();
            EngineDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        // Create render system with the render pass
        renderSystem = std::make_unique<EngineRenderSystem>(renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

        // Create static render system with the render pass
        staticRenderSystem = std::make_unique<EngineStaticRenderSystem>(renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

        // camera.setViewDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.0f, 1.0f));
        camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

        // Create a viewer object
        viewerObject = EngineGameObject::createUniqueGameObject();

        viewerObject->transform.translation.z = 2.5f;
    }

    void EngineGraphicsManager::run() {
        EngineInputManager::GetInstance().moveInPlaneXZ(EngineLoopManager::getDeltaTime(), *viewerObject);
        camera.setViewYXZ(viewerObject->transform.translation, viewerObject->transform.rotation);

        float aspect = renderer.getAspectRatio();
        // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
        if (auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();

            FrameInfo frameInfo = {frameIndex, EngineLoopManager::getDeltaTime(), commandBuffer, camera, globalDescriptorSets[frameIndex]};

            GlobalUniformBufferObject ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            uniformBuffers[frameIndex]->writeToBuffer(&ubo);
            uniformBuffers[frameIndex]->flush();
            
            renderer.beginSwapChainRenderPass(commandBuffer);

            renderSystem->renderGameObjects(frameInfo);
            staticRenderSystem->renderGameObjects(frameInfo);

            TulparEngine::EngineEditor::EditorUIManager::GetInstance().renderGui(commandBuffer);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }

    void EngineGraphicsManager::cleanup() {

    }

    void EngineGraphicsManager::createCamera() {
        
    }
}