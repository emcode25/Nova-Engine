#include <Nova/ui.hpp>

#define NOVA_HPP_NO_HEADERS
#include <Nova/nova.hpp>
#undef NOVA_HPP_NO_HEADERS

#include <Nova/components.hpp>
#include <Nova/const.hpp>
#include <Nova/objects.hpp>
#include <Nova/utils.hpp>

#include <imgui/imgui.h>
#include <nfd.h>

void Nova::EditorUI::MainMenu(GLFWwindow* window, const flecs::world& ecs, std::vector<flecs::entity>& objs)
{
    static bool texWindowOpen = false;
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
            if (ImGui::MenuItem("Import Texture..."))
            {
                texWindowOpen = true;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Lighting Mode"))
            {
                if (ImGui::MenuItem("Default"))
                {
                    Nova::activeProgram = Nova::forwardShader.getProgram();
                }

                if (ImGui::MenuItem("Unlit"))
                {
                    Nova::activeProgram = Nova::unlitShader.getProgram();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Object"))
        {
            if (ImGui::BeginMenu("New Object"))
            {
                if (ImGui::MenuItem("Cube"))
                {
                    auto cube = Nova::createCube();
                    cube.set_doc_name("Cube");

                    objs.push_back(cube);
                }

                if (ImGui::MenuItem("Camera"))
                {
                    auto cam = Nova::createCamera();
                    cam.set_doc_name("Camera");

                    objs.push_back(cam);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }

    if(texWindowOpen)
    {
        ImGui::Begin("Import Texture", &texWindowOpen);

        //Path text box
        static nfdresult_t res;
        static char texPath[Nova::CONST::OBJECT_NAME_CHARACTER_LIMIT];
        ImGui::Text("Path:");
        ImGui::InputText("##path", texPath, IM_ARRAYSIZE(texPath));

        //Three dots box
        //If box pressed, get path
        if (ImGui::Button("Browse"))
        {
            char* path = NULL;
            res = NFD_OpenDialog(NULL, NULL, &path);
            strncpy(texPath, path, Nova::CONST::OBJECT_NAME_CHARACTER_LIMIT);
            free(path);
        }

        //Texture name box
        static char name[Nova::CONST::OBJECT_NAME_CHARACTER_LIMIT];
        ImGui::Text("Name:");
        ImGui::InputText("##new_tex_name", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsNoBlank);
        //TODO: Handle when no name is given

        //Type dropdown
        const char* texTypeNames[] = { "Diffuse", "Specular" };
        static Nova::TexType typeSelected = Nova::TexType::DIFFUSE;
        const char* comboPreview = texTypeNames[static_cast<int>(typeSelected)];

        if (ImGui::BeginCombo("Texture Type", comboPreview))
        {
            for (int n = 0; n < IM_ARRAYSIZE(texTypeNames); n++)
            {
                const bool isSelected = (static_cast<int>(typeSelected) == n);
                if (ImGui::Selectable(texTypeNames[n], isSelected))
                {
                    typeSelected = static_cast<TexType>(n);
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        //Accept/close button
        if (ImGui::Button("Finish"))
        {
            if (res == NFD_OKAY)
            {
                Nova::Texture* containerTexture = Nova::loadTexture(name, texPath, typeSelected, globalTextures);
            }

            texWindowOpen = false;
        }

        ImGui::End();
    }
}

//Displays the combo tool for texture selection
//Returns true when a selection has occurred, indexSelected will reflect what was selected
bool textureNameCombo(int& indexSelected, std::vector<Nova::Texture*>& textureSet)
{
    if (indexSelected < 0 || indexSelected > textureSet.size())
    {
        return false;
    }

    bool selectionHappened = false;
    if (ImGui::BeginCombo("Texture", textureSet[indexSelected]->name.c_str()))
    {
        for (int i = 0; i < textureSet.size(); i++)
        {
            const bool isSelected = (indexSelected == i);
            if (ImGui::Selectable(textureSet[i]->name.c_str(), isSelected))
            {
                indexSelected = i;
                selectionHappened = true;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    return selectionHappened;
}

//Returns the index if a texture is found, -1 otherwise
int findTextureIndex(Nova::Texture* texture, std::vector<Nova::Texture*>& textureSet)
{
    for (int i = 0; i < textureSet.size(); ++i)
    {
        if (textureSet[i] == texture)
        {
            return i;
        }
    }

    return -1;
}

void Nova::EditorUI::ShowObjectProperties(flecs::entity& obj)
{
    static auto prevObj = obj;
    bool newObj = (prevObj != obj) ? true : false;

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

        if(obj.has<Nova::Component::Mesh>() && ImGui::CollapsingHeader("Mesh"))
        {
            auto meshProps = obj.get_ref<Nova::Component::Mesh>();
            static std::vector<int> indexSelected(Nova::CONST::OPENGL_SHADER_TEXTURE_MAX);

            for (int i = 0; i < meshProps->textures.size(); ++i)
            {
                Nova::Texture* texSelected = meshProps->textures[i];
                indexSelected[i] = findTextureIndex(texSelected, globalTextures);

                //Recheck index for texture if a new object is found
                if (newObj)
                {
                    indexSelected[i] = findTextureIndex(texSelected, globalTextures);
                }

                if (textureNameCombo(indexSelected[i], globalTextures))
                {
                    meshProps->textures[i] = globalTextures[indexSelected[i]];
                }
            }
        }

        if (obj.has<Nova::Component::PointLight>() && ImGui::CollapsingHeader("Light"))
        {
            auto lightProps = obj.get_ref<Nova::Component::PointLight>();

            ImGui::DragFloat3("Ambient", &(lightProps->base.ambient(0)), 0.005f, 0.0f, 1.0f);
            ImGui::DragFloat3("Diffuse", &(lightProps->base.diffuse(0)), 0.005f, 0.0f, 1.0f);
            ImGui::DragFloat3("Specular", &(lightProps->base.specular(0)), 0.005f, 0.0f, 1.0f);

            ImGui::DragFloat("Constant", &(lightProps->constant), 1.0f);
            ImGui::DragFloat("Linear", &(lightProps->linear), 0.05f);
            ImGui::DragFloat("Quadratic", &(lightProps->quadratic), 0.01f);
        }
    }

    ImGui::End();

    prevObj = obj;
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
            }

            //Rename Objects (right-click)
            if (ImGui::BeginPopupContextItem())
            {
                static char rename[Nova::CONST::OBJECT_NAME_CHARACTER_LIMIT];
                ImGui::Text("Rename:");
                ImGui::InputText("##rename", rename, IM_ARRAYSIZE(rename));

                if (ImGui::Button("Close"))
                {
                    //New name if the string is non-empty
                    if(strcmp(rename, "") != 0)
                    {
                        e.set_doc_name(rename);
                    }

                    std::strcpy(rename, "");
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::EndListBox();
    }
    
    ImGui::End();
}