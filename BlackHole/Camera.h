#pragma once

// THIS CLASS IS BASED ON: https://learnopengl.com/Getting-started/Camera

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
    public:
    // camera Attributes
    glm::vec3 Target;
    glm::vec3 Position;
    glm::vec3 Front, Right, Up, WorldUp;
    // euler Angles
    float Yaw, Pitch;
    // camera options
    float PanSpeed = 0.1f;
    float Dist = 40.f;
    float MinDist = 1e-2f;
    float MaxDist = 1e2f;

    // constructor with vectors
    Camera(glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f);
    
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const;

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // clamp Dist between MinDist and MaxDist
    void ClampDist();

    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors(bool flip = false);
};
#endif