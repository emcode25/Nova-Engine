#ifndef EDITOR_CAMERA_HPP
#define EDITOR_CAMERA_HPP

#include <Eigen/Core>

#include <Nova/types.hpp>
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
            EditorCamera(const Nova::Component::Transform& t, const Nova::Component::Camera& c, const Nova::Vector3& up);

            Nova::Component::Camera getCameraProperties();

            void updatePosition(const Nova::Editor::EditorCameraMovement& direction, Nova::Float dt);
            void updateDirection(Nova::Float xoffset, Nova::Float yoffset);

            Nova::Matrix4 viewMatrix();

            Nova::Vector3 getPosition();

        private:
            Nova::Component::Transform transform;
            Nova::Component::Camera cam;
            Nova::Vector3 up, worldup, right, front;
            Nova::Float moveSpeed, sensitivity;

            void updateCameraVectors();
        };
    }
}

#endif