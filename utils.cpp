#include <PGE/utils.hpp>

#include <string>
#include <iostream>
#include <stdio.h>

#include <PGE/const.hpp>

std::string PGE::readFileToString(const char* filename)
{
	FILE* file = fopen(filename, "r");
	std::string contents;

	if (file != NULL)
	{
		fseek(file, 0, SEEK_END);
		long len = ftell(file);
		rewind(file);

		contents.resize(len);
		fread(&contents[0], sizeof(char), len, file);
		fclose(file);

		return contents;
	}

	std::cerr << "File could not be opened." << std::endl;

	return std::string("Could not read file.");
}

void PGE::processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

Eigen::Matrix4f PGE::lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
{
	//Calculate orthogonal basis for camera and store in matrix
	Eigen::Matrix4f lookAt;
	Eigen::Vector3f camDir   = (position - target).normalized();
	Eigen::Vector3f camRight = up.cross(camDir).normalized();
	Eigen::Vector3f camUp    = camDir.cross(camRight).normalized();

	lookAt << camRight[0], camRight[1], camRight[2], 0.0f,
			  camUp[0],    camUp[1],    camUp[2],    0.0f,
			  camDir[0],   camDir[1],   camDir[2],   0.0f,
			  0.0f,		   0.0f,		0.0f,	     1.0f;

	//Translate by -position
	lookAt(0, 3) = camRight.dot(-position);
	lookAt(1, 3) = camUp.dot(-position);
	lookAt(2, 3) = camDir.dot(-position);

	return lookAt;
}

Eigen::Matrix4f PGE::makePerspective(float aspectRatio, float fov, float near, float far)
{
	//Create variables to save on repeated computations
	float inverseCotFov = 1.0f / tanf(fov / 2.0f);
	float inverseFarNearDiff = 1.0f / (far - near);

	Eigen::Matrix4f proj;
	proj << inverseCotFov / aspectRatio, 0.0f, 0.0f, 0.0f,
			0.0f, inverseCotFov, 0.0f, 0.0f,
			0.0f, 0.0f, -(far + near) * inverseFarNearDiff, -(2.0f * far * near) * inverseFarNearDiff,
			0.0f, 0.0f, -1.0f, 0.0f;

	return proj;
}

//x: pitch, y: yaw, z: roll, angles in degrees
Eigen::Quaternionf PGE::rotateFromEuler(Eigen::Vector3f angles, bool isRadians)
{
	Eigen::Quaternionf rotation;

	//Convert angles to radians
	angles *= (isRadians) ? 1 : PGE::CONST::DEG_TO_RAD;

	float cosPitch = cosf(angles.x() * 0.5f);
	float sinPitch = sinf(angles.x() * 0.5f);
	float cosYaw   = cosf(angles.y() * 0.5f);
	float sinYaw   = sinf(angles.y() * 0.5f);
	float cosRoll  = cosf(angles.z() * 0.5f);
	float sinRoll  = sinf(angles.z() * 0.5f);

	rotation.x() = sinPitch * cosYaw * cosRoll - cosPitch * sinYaw * sinRoll;
	rotation.y() = cosPitch * sinYaw * cosRoll + sinPitch * cosYaw * sinRoll;
	rotation.z() = cosPitch * cosYaw * sinRoll - sinPitch * sinYaw * cosRoll;
	rotation.w() = cosPitch * cosYaw * cosRoll + sinPitch * sinYaw * sinRoll;

	return rotation;
}