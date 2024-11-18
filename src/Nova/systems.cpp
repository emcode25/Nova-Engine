#include <Nova/systems.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Nova/const.hpp>
#include <Nova/engine.hpp>
#include <Nova/utils.hpp>

void Nova::ObjectRenderSystem(flecs::iter& it)
{
    static auto cam = editorCamera.getCameraProperties();

    //Window size must be accounted for as well due to resize
    int windowWidth = 0, windowHeight = 0;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    //This rotation calculation is needed once per render system call 
    //Translation MUST ALSO BE APPLIED to work properly
    //TODO: Fix roll rotation on camera
    //Transform to view space
    Eigen::Matrix4f view = editorCamera.viewMatrix();

    //Project into clip space
    Eigen::Matrix4f proj = Nova::makePerspective(
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
        cam.fov * Nova::CONST::DEG_TO_RAD, cam.zNear, cam.zFar); //Do not forget about integer division

    //Loop setup
    //Activate program and send matrices to shaders
    glUseProgram(activeProgram);

    GLuint modelLoc = glGetUniformLocation(activeProgram, "model");
    GLuint viewLoc = glGetUniformLocation(activeProgram, "view");
    GLuint projLoc = glGetUniformLocation(activeProgram, "proj");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

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
            Eigen::Affine3f model = Eigen::Affine3f::Identity();
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
        Eigen::Affine3f model = Eigen::Affine3f::Identity();
        model.translate(activeTransform->position);
        model.rotate(Nova::rotateFromEuler(activeTransform->rotation));
        model.scale(activeTransform->scale);

        //Send to active object shader
        GLuint program = activeObjShader.getProgram();
        glUseProgram(program);

        //TODO: Refactor
        GLuint modelLoc = glGetUniformLocation(program, "model");
        GLuint viewLoc = glGetUniformLocation(program, "view");
        GLuint projLoc = glGetUniformLocation(program, "proj");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

        //Render
        auto activeMesh = activeObj.get_ref<Nova::Component::Mesh>();
        glBindVertexArray(activeMesh->VAO);
        glDrawElements(GL_TRIANGLES, activeMesh->indices.size(), GL_UNSIGNED_INT, 0);
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
    Eigen::Matrix4f view = editorCamera.viewMatrix();

    //Project into clip space
    Eigen::Matrix4f proj = Nova::makePerspective(
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
        cam.fov * Nova::CONST::DEG_TO_RAD, cam.zNear, cam.zFar); //Do not forget about integer division

    //Loop setup
    //Activate program and send matrices to shaders
    GLuint program = lightSourceShader.getProgram();
    glUseProgram(program);

    GLuint modelLoc = glGetUniformLocation(program, "model");
    GLuint viewLoc = glGetUniformLocation(program, "view");
    GLuint projLoc = glGetUniformLocation(program, "proj");

    GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");

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
            Eigen::Affine3f model = Eigen::Affine3f::Identity();
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