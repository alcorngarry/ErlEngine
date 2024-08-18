#pragma once
#ifndef CAMERA_H

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
	public:
		Camera();
	
		glm::vec3 getCameraUp();
		void setCameraUp(glm::vec3 cameraUp);
		glm::vec3 getCameraFront();
		void setCameraFront(glm::vec3 cameraFront);
		glm::vec3 getCameraPos();
		void setCameraPos(glm::vec3 cameraPos);
	private:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
};
#endif // !CAMERA_H