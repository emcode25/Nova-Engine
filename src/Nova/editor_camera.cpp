#include <Nova/editor_camera.hpp>

#include <Nova/const.hpp>
#include <Nova/utils.hpp>

Nova::Editor::EditorCamera::EditorCamera()
{
    transform.position = {0.0f, 0.0f, 3.0f};
    transform.rotation = {-90.0f, 0.0f, 0.0f};
    transform.scale = {1.0f, 1.0f, 1.0f};

    cam.fov = 45.0f;
    cam.zNear = 0.1f;
    cam.zFar = 100.0f;

    moveSpeed = 2.5f;
    sensitivity = 0.1f;

    front = {0.0f, 0.0f, -1.0f};
    worldup = {0.0f, 1.0f, 0.0f};
    updateCameraVectors();
}

Nova::Editor::EditorCamera::EditorCamera(const Nova::Component::Transform& t, const Nova::Component::Camera& c, const Nova::Vector3& up)
{
    transform = t;
    cam = c;

    moveSpeed = 2.5f;
    sensitivity = 0.1f;

    front = {0.0f, 0.0f, -1.0f};
    worldup = up;
    updateCameraVectors();
}

Nova::Component::Camera Nova::Editor::EditorCamera::getCameraProperties()
{
    return cam;
}

void Nova::Editor::EditorCamera::updatePosition(const Nova::Editor::EditorCameraMovement& direction, Nova::Float dt)
{
    Nova::Float vel = moveSpeed * dt;

    switch(direction)
    {
        case FORWARD:
            transform.position += front * vel;
            break;
        case BACKWARD:
            transform.position -= front * vel;
            break;
        case LEFT:
            transform.position -= right * vel;
            break;
        case RIGHT:
            transform.position += right * vel;
            break;
        default:
            break;
    }
}

Nova::Matrix4 Nova::Editor::EditorCamera::viewMatrix()
{
    return Nova::lookAt(transform.position, transform.position + front, up);
}

Nova::Vector3 Nova::Editor::EditorCamera::getPosition()
{
    return transform.position;
}

void Nova::Editor::EditorCamera::updateCameraVectors()
{
    Nova::Float yaw = Nova::CONST::DEG_TO_RAD * transform.rotation(0);
    Nova::Float pitch = Nova::CONST::DEG_TO_RAD * transform.rotation(1);

    Nova::Vector3 newFront;
    newFront(0) = std::cos(yaw) * std::cos(pitch);
    newFront(1) = std::sin(pitch);
    newFront(2) = std::sin(yaw) * std::cos(pitch);

    front = newFront.normalized();
    right = front.cross(worldup).normalized();
    up = right.cross(front).normalized();
}

void Nova::Editor::EditorCamera::updateDirection(Nova::Float xoffset, Nova::Float yoffset)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    transform.rotation(0) += xoffset;
    transform.rotation(1) += yoffset;

    if(transform.rotation(1) > 89.0f)
    {
        transform.rotation(1) = 89.0f;
    }

    if(transform.rotation(1) < -89.0f)
    {
        transform.rotation(1) = -89.0f;
    }

    updateCameraVectors();
}