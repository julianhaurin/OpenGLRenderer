// OpenGL Renderer Camera class header file - Julian Haurin - 2/3/23

#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// possible camera movements
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// initial camera values
static const float s_YAW = -90.0f;
static const float s_PITCH = 2.5f;
static const float s_SPEED = 5.0f;
static const float s_SENSITIVITY = 0.1f;
static const float s_ZOOM = 45.0f;

class Camera
{

public:

	// euler agnles
	float yaw;
	float pitch;

	// camera options
	float cameraSpeed;
	float mouseSensitivity;
	float cameraZoom;

	// camera attributes
	glm::vec3 camPosition;
	glm::vec3 camUp;
	glm::vec3 camFront;
	glm::vec3 camRight;

	glm::vec3 worldUp;


	// constructor with vectors - default yaw and pitch
	Camera(glm::vec3 in_camPosition, glm::vec3 in_worldUp)
		// member initializer list
		: yaw(s_YAW), pitch(s_PITCH), cameraSpeed(s_SPEED), mouseSensitivity(s_SENSITIVITY), cameraZoom(s_ZOOM),
		camPosition(in_camPosition), worldUp(in_worldUp)
	{
		// calculates remaining camera vectors based on worldUp vector and current euler angles
		calculateCameraVectors();

	}

	// Gram-Schmidt process

	// calculates proper view matrix using glm LookAt matrix
	glm::mat4 calculateViewMatrix();

	// handles keyboard camera movements from GLFW callback functions
	void ProcessKeyboardMovement(Camera_Movement in_direction, float in_deltaTime);

	// handles mouse camera movements using GLFW callback functions
	void ProcessMouseMovement(float in_xOffset, float in_yOffset);

	// handles mouse scrolling using GLFW callback functions
	void ProcessMouseScroll(float in_yOffset);

private:

	// recalculates front, right, and up camera vectors based on current euler angles
	void calculateCameraVectors();

};
