#include <glad/glad.h>
#include <Nova/utils.hpp>

#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>

#include <stb_image.h>
#include <json/json.h>

#include <Nova/const.hpp>
#include <Nova/engine.hpp>

Nova::String Nova::readFileToString(Nova::String filename)
{
	FILE* file = fopen(filename.c_str(), "r");
	Nova::String contents;

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

	return Nova::String("Could not read file.");
}

void Nova::processInput(GLFWwindow* window, Nova::Editor::EditorCamera& cam, Nova::Float dt)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cam.updatePosition(Nova::Editor::EditorCameraMovement::FORWARD, dt);
		}

		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cam.updatePosition(Nova::Editor::EditorCameraMovement::LEFT, dt);
		}

		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cam.updatePosition(Nova::Editor::EditorCameraMovement::BACKWARD, dt);
		}

		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cam.updatePosition(Nova::Editor::EditorCameraMovement::RIGHT, dt);
		}
	}
}

Nova::Texture* Nova::loadTexture(Nova::String name, Nova::String filename, Nova::TexType type, Nova::Array<Nova::Texture*>& textureSet)
{
	for(auto texture : textureSet)
	{
		if(texture->name == name)
		{
			std::cerr << "ERROR: Failed to load texture: " << filename <<  ", name is already taken." << std::endl;
			return NULL;
		}
	}

	Nova::Texture* tex = new Nova::Texture();

	//Create the texture object
	GLuint textureObj;
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);

	//TODO: Somehow allow user defined texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load textures
	int width, height, channels;
	Nova::UByte* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		//Choose the right format
		GLint format;
		switch (channels)
		{
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				format = GL_RED;
				break;
		}

		//Give OpenGL the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Clean up and get the texture struct ready
		stbi_image_free(data);

		tex->texture = textureObj;
		tex->type = type;
		tex->name = name;

		textureSet.push_back(tex);
	}
	else
	{
		std::cerr << "ERROR: Failed to load texture: " << filename << std::endl;

		delete tex;
		return nullptr;
	}

	return tex;
}

void Nova::deleteTextures(Nova::Array<Nova::Texture*>& textureSet)
{
	for (auto texture : textureSet)
	{
		delete texture;
	}
}

Nova::Matrix4 Nova::lookAt(const Nova::Vector3& position, const Nova::Vector3& target, const Nova::Vector3& up)
{
	//Calculate orthogonal basis for camera and store in matrix
	Nova::Matrix4 lookAt;
	Nova::Vector3 camDir   = (position - target).normalized();
	Nova::Vector3 camRight = up.cross(camDir).normalized();
	Nova::Vector3 camUp    = camDir.cross(camRight).normalized();

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

Nova::Matrix4 Nova::makePerspective(Nova::Float aspectRatio, Nova::Float fov, Nova::Float near, Nova::Float far)
{
	//Create variables to save on repeated computations
	Nova::Float inverseCotFov = 1.0f / tanf(fov / 2.0f);
	Nova::Float inverseFarNearDiff = 1.0f / (far - near);

	Nova::Matrix4 proj;
	proj << inverseCotFov / aspectRatio, 0.0f, 0.0f, 0.0f,
			0.0f, inverseCotFov, 0.0f, 0.0f,
			0.0f, 0.0f, -(far + near) * inverseFarNearDiff, -(2.0f * far * near) * inverseFarNearDiff,
			0.0f, 0.0f, -1.0f, 0.0f;

	return proj;
}

//x: pitch, y: yaw, z: roll, angles in degrees
Nova::Quaternion Nova::rotateFromEuler(Nova::Vector3 angles, bool isRadians)
{
	Nova::Quaternion rotation;

	//Convert angles to radians
	angles *= (isRadians) ? 1 : Nova::CONST::DEG_TO_RAD;

	Nova::Float cosPitch = cosf(angles.x() * 0.5f);
	Nova::Float sinPitch = sinf(angles.x() * 0.5f);
	Nova::Float cosYaw   = cosf(angles.y() * 0.5f);
	Nova::Float sinYaw   = sinf(angles.y() * 0.5f);
	Nova::Float cosRoll  = cosf(angles.z() * 0.5f);
	Nova::Float sinRoll  = sinf(angles.z() * 0.5f);

	rotation.x() = sinPitch * cosYaw * cosRoll - cosPitch * sinYaw * sinRoll;
	rotation.y() = cosPitch * sinYaw * cosRoll + sinPitch * cosYaw * sinRoll;
	rotation.z() = cosPitch * cosYaw * sinRoll - sinPitch * sinYaw * cosRoll;
	rotation.w() = cosPitch * cosYaw * cosRoll + sinPitch * sinYaw * sinRoll;

	return rotation.normalized();
}

Nova::Int Nova::saveScene(const Nova::String& filepath)
{
	std::cout << "Saving scene to file: " << filepath << std::endl;

	Json::Value root;
	Json::Value data;

	for (const auto& e : Nova::entities)
	{
		Json::Value obj;
		
		if (e.has<Nova::Component::Transform>())
		{
			const auto transform = e.get<Nova::Component::Transform>();
			const auto pos = transform->position.data();
			const auto rot = transform->rotation.data();
			const auto scale = transform->scale.data();

			obj["position"][0] = pos[0];
			obj["position"][1] = pos[1];
			obj["position"][2] = pos[2];

			obj["rotation"][0] = rot[0];
			obj["rotation"][1] = rot[1];
			obj["rotation"][2] = rot[2];

			obj["scale"][0] = scale[0];
			obj["scale"][1] = scale[1];
			obj["scale"][2] = scale[2];
		}

		obj["name"] = e.doc_name();
		root[std::to_string(e.id())] = obj;
	}

	Json::StreamWriterBuilder builder;
	const std::string jsonFile = Json::writeString(builder, root);

	std::ofstream file(filepath);
	file << jsonFile;

	std::cout << "Saved!" << std::endl;

	return Nova::Int();
}
