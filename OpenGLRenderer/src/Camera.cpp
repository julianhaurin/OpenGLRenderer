// OpenGL Renderer Camera class - Julian Haurin - 2/3/23

#include "Camera.h"

// calculates proper view matrix using glm LookAt matrix
glm::mat4 Camera::calculateViewMatrix() {

	return glm::lookAt(camPosition, camPosition + camFront, camUp);

}

// handles keyboard camera movements using GLFW callback functions
void Camera::ProcessKeyboardMovement(Camera_Movement in_direction, float in_deltaTime) {

	float velocity = cameraSpeed * in_deltaTime;

	// WASD movements
	if (in_direction == FORWARD)
		camPosition += camFront * velocity;
	if (in_direction == BACKWARD)
		camPosition -= camFront * velocity;
	if (in_direction == LEFT)
		camPosition -= glm::normalize(glm::cross(camFront, camUp)) * velocity;
	if (in_direction == RIGHT)
		camPosition += glm::normalize(glm::cross(camFront, camUp)) * velocity;

	// UP DOWN movements
	if (in_direction == UP)
		camPosition += camUp * velocity;
	if (in_direction == DOWN)
		camPosition -= camUp * velocity;

}

// handles mouse camera movements using GLFW callback functions
void Camera::ProcessMouseMovement(float in_xOffset, float in_yOffset) {

	// update yaw and pitch values
	yaw += in_xOffset * mouseSensitivity;
	pitch += in_yOffset * mouseSensitivity;

	// controls pitch range
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// calculates camera direction value
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(direction);

}

// handles mouse scrolling using GLFW callback functions
void Camera::ProcessMouseScroll(float in_yOffset) {

	cameraZoom -= in_yOffset;

	if (cameraZoom < 1.0f)
		cameraZoom = 1.0f;
	if (cameraZoom > 45.0f)
		cameraZoom = 45.0f;

}

// recalculates front, right, and up camera vectors based on current euler angles
void Camera::calculateCameraVectors() {
	
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	camFront = glm::normalize(front);
	camRight = glm::normalize(glm::cross(camFront, worldUp));
	camUp = glm::normalize(glm::cross(camRight, camFront));

}
