#include <Nova/systems.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Nova/types.hpp>
#include <Nova/const.hpp>
#include <Nova/engine.hpp>
#include <Nova/utils.hpp>

void Nova::ObjectRenderSystem(flecs::iter& it)
{
    static auto cam = editorCamera.getCameraProperties();

    //Window size must be accounted for as well due to resize
    Nova::Int windowWidth = 0, windowHeight = 0;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    //This rotation calculation is needed once per render system call 
    //Translation MUST ALSO BE APPLIED to work properly
    //TODO: Fix roll rotation on camera
    //Transform to view space
    Nova::Matrix4 view = editorCamera.viewMatrix();

    //Project into clip space
    Nova::Matrix4 proj = Nova::makePerspective(
        static_cast<Nova::Float>(windowWidth) / static_cast<Nova::Float>(windowHeight),
        cam.fov * Nova::CONST::DEG_TO_RAD, cam.zNear, cam.zFar); //Do not forget about integer division

    //Loop setup
    //Activate program and send matrices to shaders
    glUseProgram(activeProgram);

    Nova::UInt modelLoc = glGetUniformLocation(activeProgram, "model");
    Nova::UInt viewLoc = glGetUniformLocation(activeProgram, "view");
    Nova::UInt projLoc = glGetUniformLocation(activeProgram, "proj");

    Nova::UInt viewPosLoc = glGetUniformLocation(activeProgram, "viewPos");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

    glUniform3fv(viewPosLoc, 1, editorCamera.getPosition().data());

    while (it.next())
    {
        auto transforms = it.field<const Nova::Component::Transform>(0);
        auto meshes = it.field<const Nova::Component::Mesh>(1);

        //Iterate through each object
        for (auto i : it)
        {
            //Modify transform properties (locally)
            Nova::Component::Transform transform = transforms[i];
            Nova::Component::Mesh mesh = meshes[i];

            //Perform world space transformations
            Nova::Affine3 model = Nova::Affine3::Identity();
            model.translate(transform.position);
            model.rotate(Nova::rotateFromEuler(transform.rotation));
            model.scale(transform.scale);

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());

            //Attach textures
            for (int j = 0; j < mesh.textures.size(); ++j)
            {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, mesh.textures[j]->texture);
            }

            //Render the mesh
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        }
    }

    //Active object gets another pass to run the geometry shader
    if (activeObj.has<Nova::Component::Mesh>())
    {
        //Get the active object and do the same transform stuff
        auto activeTransform = activeObj.get_ref<Nova::Component::Transform>();
        Nova::Affine3 model = Nova::Affine3::Identity();
        model.translate(activeTransform->position);
        model.rotate(Nova::rotateFromEuler(activeTransform->rotation));
        model.scale(activeTransform->scale);

        //Send to active object shader
        Nova::ShaderProgram program = activeObjShader.getProgram();
        glUseProgram(program);

        //TODO: Refactor
        Nova::UInt modelLoc = glGetUniformLocation(program, "model");
        Nova::UInt viewLoc = glGetUniformLocation(program, "view");
        Nova::UInt projLoc = glGetUniformLocation(program, "proj");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

        //Render
        auto activeMesh = activeObj.get_ref<Nova::Component::Mesh>();
        glBindVertexArray(activeMesh->VAO);
        glDrawElements(GL_TRIANGLES, activeMesh->indices.size(), GL_UNSIGNED_INT, 0);
    }

    //Allow light values to change if the object is active
    if (activeObj.has<Nova::Component::PointLight>())
    {
        lightManager.loadPointLight(activeObj);
    }
}

void Nova::PointLightRenderSystem(flecs::iter & it)
{
    static auto cam = editorCamera.getCameraProperties();

    //Window size must be accounted for as well due to resize
    int windowWidth = 0, windowHeight = 0;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    //This rotation calculation is needed once per render system call 
    //Translation MUST ALSO BE APPLIED to work properly
    //TODO: Fix roll rotation on camera
    //Transform to view space
    Nova::Matrix4 view = editorCamera.viewMatrix();

    //Project into clip space
    Nova::Matrix4 proj = Nova::makePerspective(
        static_cast<Nova::Float>(windowWidth) / static_cast<Nova::Float>(windowHeight),
        cam.fov * Nova::CONST::DEG_TO_RAD, cam.zNear, cam.zFar); //Do not forget about integer division

    //Loop setup
    //Activate program and send matrices to shaders
    Nova::ShaderProgram program = lightSourceShader.getProgram();
    glUseProgram(program);

    Nova::UInt modelLoc = glGetUniformLocation(program, "model");
    Nova::UInt viewLoc = glGetUniformLocation(program, "view");
    Nova::UInt projLoc = glGetUniformLocation(program, "proj");

    Nova::UInt lightColorLoc = glGetUniformLocation(program, "lightColor");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

    while (it.next())
    {
        auto transforms = it.field<const Nova::Component::Transform>(0);
        auto meshes = it.field<const Nova::Component::Mesh>(1);
        auto lights = it.field<const Nova::Component::PointLight>(2);

        //Iterate through each object
        for (auto i : it)
        {
            //Modify transform properties (locally)
            Nova::Component::Transform transform = transforms[i];
            Nova::Component::Mesh mesh = meshes[i];
            Nova::Component::PointLight pointLight = lights[i];

            //Perform world space transformations
            Nova::Affine3 model = Nova::Affine3::Identity();
            model.translate(transform.position);
            model.rotate(Nova::rotateFromEuler(transform.rotation));
            model.scale(transform.scale);

            //Set model matrix and light color
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
            glUniform3fv(lightColorLoc, 1, pointLight.base.diffuse.data());

            //Render the mesh
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        }
    }
}