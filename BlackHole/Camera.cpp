#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 target, glm::vec3 up, float yaw, float pitch) :
    Target(target), WorldUp(up), Yaw(yaw), Pitch(pitch) {
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    Yaw(yaw), Pitch(pitch) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Target, Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    Pitch += yoffset * PanSpeed;
    if (std::fabs(Pitch) > 360.f) {
        const int abs = static_cast<int>(Pitch / 360.f);
        Pitch -= abs * 360.f;
    }

    bool flip = false;
    if (constrainPitch && std::fabs(Pitch) > 90.f && std::fabs(Pitch) < 270.f) {
        xoffset = -xoffset;
        flip = true;
    }

    Yaw -= xoffset * PanSpeed;
    if (std::fabs(Yaw) > 360.f) {
        const int abs = static_cast<int>(Yaw / 360.f);
        Yaw -= abs * 360.f;
    }

    UpdateCameraVectors(flip);
}

void Camera::ClampDist() {
    if (Dist > MaxDist) { Dist = MaxDist; }
    if (Dist < MinDist) { Dist = MinDist; }
}

void Camera::UpdateCameraVectors(bool flip) {
    glm::vec3 offset;
    offset.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    offset.y = sin(glm::radians(Pitch));
    offset.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    offset = glm::normalize(offset) * Dist;

    Position = Target + offset;
    Front = glm::normalize(Target - Position);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
    if (flip) {
        Up = -Up;
    }
}