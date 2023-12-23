#ifndef EDITOR_CAMERA_HPP
#define EDITOR_CAMERA_HPP

#include <Eigen/Core>

#include <Nova/components.hpp>

namespace Nova
{
    namespace Editor
    {
        enum EditorCameraMovement
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };

        //This class should ONLY be used by the Nova Engine
        //EditorCamera is a special class to handle the unique camera
        //required to run the editor.
        class EditorCamera
        {
        public:
            EditorCamera();
            EditorCamera(const Nova::Component::Transform& t, const Nova::Component::Camera& c, const Eigen::Vector3f& up);

            Nova::Component::Camera getCameraProperties();

            void updatePosition(const Nova::Editor::EditorCameraMovement& direction, float dt);
            void updateDirection(float xoffset, float yoffset);

            Eigen::Matrix4f viewMatrix();

        private:
            Nova::Component::Transform transform;
            Nova::Component::Camera cam;
            Eigen::Vector3f up, worldup, right, front;
            float moveSpeed, sensitivity;

            void updateCameraVectors();
        };
    }
}

#endif