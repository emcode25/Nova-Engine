#include <Nova/ui.hpp>

#include <Nova/components.hpp>
#include <Nova/const.hpp>
#include <Nova/objects.hpp>
#include <Nova/utils.hpp>

#include <imgui/imgui.h>

void Nova::EditorUI::MainMenu(GLFWwindow* window, const flecs::world& ecs, std::vector<flecs::entity>& objs)
{
    bool quit = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //TODO: Implement a save system
            ImGui::MenuItem("Save",  "Ctrl-S");
            ImGui::MenuItem("Save as...", "Ctrl-Shift-S");

            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt-F4", &quit))
            {
                glfwSetWindowShouldClose(window, quit);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Object"))
        {
            if (ImGui::BeginMenu("New Object"))
            {
                if (ImGui::MenuItem("Cube"))
                {
                    auto cube = Nova::createCube(ecs);
                    cube.set_doc_name("Cube");

                    objs.push_back(cube);
                }

                if (ImGui::MenuItem("Camera"))
                {
                    auto cam = Nova::createCamera(ecs);
                    cam.set_doc_name("Camera");

                    objs.push_back(cam);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void Nova::EditorUI::ShowObjectProperties(flecs::entity& obj)
{
    if(ImGui::Begin("Object Components"))
    {
        auto activeTransform = obj.get_ref<Nova::Component::Transform>();
        
        if(ImGui::CollapsingHeader("Transform"))
        {
            ImGui::DragFloat3("Position", &(activeTransform->position(0)), 0.05f);
            ImGui::DragFloat3("Rotation", &(activeTransform->rotation(0)), 1.0f);
            ImGui::DragFloat3("Scale", &(activeTransform->scale(0)), 0.1f);
        }

        if(obj.has<Nova::Component::Camera>() && ImGui::CollapsingHeader("Camera"))
        {
            auto camProps = obj.get_ref<Nova::Component::Camera>();

            ImGui::SliderFloat("FOV", &(camProps->fov), 1.0f, 45.0f, "%.1f");
            ImGui::DragFloat("Near", &(camProps->zNear), 0.1f, 0.0f, 0.0f, "%.1f");
            ImGui::DragFloat("Far", &(camProps->zFar), 0.1f, 0.0f, 0.0f, "%.1f");
        }
    }

    ImGui::End();
}

void Nova::EditorUI::ShowObjectList(std::vector<flecs::entity>& objs, flecs::entity& activeObj)
{
    ImGui::Begin("Object List", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
    
    static std::string name;
    if (ImGui::BeginListBox("Objects", ImGui::GetWindowContentRegionMax()))
    {
        for (auto e : objs)
        {
            const bool selected = (activeObj == e);
            name = e.doc_name();
            name += "##";
            name += e.raw_id();
            if (ImGui::Selectable(name.c_str(), selected))
            {
                //TODO: Allow multiple objects to be selected at once
                //This allows the object to be manipulated (one at a time)
                activeObj = e;
            }

            //Highlight selected object in list
            if (selected)
            {
                ImGui::SetItemDefaultFocus();

                //TODO: Highlight object in window
            }

            //Rename Objects (right-click)
            if (ImGui::BeginPopupContextItem())
            {
                char rename[Nova::CONST::OBJECT_NAME_CHARACTER_LIMIT];
                ImGui::Text("Rename:");
                ImGui::InputText("##rename", rename, IM_ARRAYSIZE(rename));

                if (ImGui::Button("Close"))
                {
                    //Check for a new name if the window is closed
                    if(e.doc_name() != rename)
                    {
                        e.set_doc_name(rename);
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::EndListBox();
    }
    
    ImGui::End();
}