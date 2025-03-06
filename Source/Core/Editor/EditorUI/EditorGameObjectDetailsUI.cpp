#include "EditorGameObjectDetailsUI.hpp"
#include "EditorSceneHierarchyUI.hpp"
#include "imgui.h"
#include "EngineGraphicsManager.hpp"
#include "EngineEntityManager.hpp"
#include <typeinfo>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace TulparEngine::EngineEditor {

    bool showGameObjectDetailsPanel = true; // Controls the visibility of the entire panel
    bool showGameObjectDetails = true; // Controls whether details are visible
    bool isEntityUpdated = false;

    EditorGameObjectDetailsUI::EditorGameObjectDetailsUI() {
        // Constructor implementation
    }

    EditorGameObjectDetailsUI::~EditorGameObjectDetailsUI() {
        // Destructor implementation
    }

    void EditorGameObjectDetailsUI::renderGui() {
        if (!showGameObjectDetailsPanel) return;

        float topBarHeight = 30.0f;
        float panelWidth = 400.0f;
        panelHeight = ImGui::GetIO().DisplaySize.y - topBarHeight;
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - panelWidth, topBarHeight));
        ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

        ImGui::Begin("Game Object Details", &showGameObjectDetailsPanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginTabBar("##DetailsChatTabBar")) {
            if (ImGui::BeginTabItem("Details")) {
                // Existing code for GameObject Details
                renderGameObjectDetails();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Chat")) {
                // Render the Chat UI
                EditorAssistantChatUI::GetInstance().renderGui();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void EditorGameObjectDetailsUI::renderGameObjectDetails() {
        if (ImGui::Button(showGameObjectDetails ? "Collapse" : "Expand")) {
            showGameObjectDetails = !showGameObjectDetails;
        }

        if (showGameObjectDetails && EditorSceneHierarchyUI::GetInstance().selectedEntityID != 0) {
            EntityID entityID = EditorSceneHierarchyUI::GetInstance().selectedEntityID;

            // Check if the selected entity has changed
            if (entityID != selectedEntityID || isEntityUpdated) {
                selectedEntityID = entityID;
                components = EngineEntityManager::GetInstance().GetEntityComponents(selectedEntityID);
                addableComponents = EngineEntityManager::GetInstance().GetAddableComponentTypes(selectedEntityID);
                isEntityUpdated = false;
            }

            ImGui::Text("Selected Object:");
            std::string entityName = "Entity " + std::to_string(selectedEntityID);

            // Get name component if available
            for (const auto& component : components) {
                if (component->GetType() == typeid(EngineNameComponent)) {
                    auto nameComponent = std::dynamic_pointer_cast<EngineNameComponent>(component);
                    if (nameComponent) {
                        entityName = nameComponent->name;
                        break;
                    }
                }
            }
            ImGui::Text("Name: %s", entityName.c_str());

            // Display and update the transform component if present
            for (auto& component : components) {
                if (component->GetType() == typeid(EngineTransformComponent)) {
                    auto transformComponent = std::dynamic_pointer_cast<EngineTransformComponent>(component);

                    glm::vec3 position = transformComponent->translation;
                    if (ImGui::InputFloat3("Position", glm::value_ptr(position))) {
                        transformComponent->translation = position;
                    }

                    glm::vec3 rotation = transformComponent->rotation;
                    if (ImGui::InputFloat3("Rotation", glm::value_ptr(rotation))) {
                        transformComponent->rotation = rotation;
                    }

                    glm::vec3 scale = transformComponent->scale;
                    if (ImGui::InputFloat3("Scale", glm::value_ptr(scale))) {
                        transformComponent->scale = scale;
                    }
                }
            }

            ImGui::Separator();
            ImGui::Text("Components:");

            // Display existing components
            for (const auto& component : components) {
                std::string componentName = EngineEntityManager::GetInstance().GetComponentTypeName(component->GetType());
                ImGui::Text("%s", componentName.c_str());
            }

            if (!addableComponents.empty()) {
                if (ImGui::Button("Add Component")) {
                    ImGui::OpenPopup("AddComponentPopup");
                }

                if (ImGui::BeginPopup("AddComponentPopup")) {
                    for (const auto& componentType : addableComponents) {
                        std::string componentName = EngineEntityManager::GetInstance().GetComponentTypeName(componentType);
                        if (ImGui::MenuItem(componentName.c_str())) {
                            EngineEntityManager::GetInstance().AddComponentToEntity(selectedEntityID, componentType);
                            components = EngineEntityManager::GetInstance().GetEntityComponents(selectedEntityID);  // Update components list
                            isEntityUpdated = true;
                        }
                    }
                    ImGui::EndPopup();
                }
            } else {
                ImGui::Text("No components can be added.");
            }
        }
    }
}