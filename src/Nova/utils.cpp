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
#include <Nova/objects.hpp>

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

Nova::TextureInfo* Nova::findTexture(const Nova::String& name, const Nova::Array<Nova::TextureInfo*>& textures)
{
	for (auto& texture : textures)
	{
		if (texture->name == name)
		{
			return texture;
		}
	}

	std::cerr << "WARNING: Could not find texture name: " << name << std::endl;
	return NULL;
}

Nova::MeshInfo Nova::findMesh(const Nova::String& name, const Nova::Array<MeshInfo>& meshes)
{
	for (auto& mesh : meshes)
	{
		if (mesh.name == name)
		{
			return mesh;
		}
	}

	std::cerr << "WARNING: Could not find mesh name: " << name << std::endl;
	MeshInfo invalidMesh;
	invalidMesh.indexCount = -1;
	invalidMesh.name = "INVALID";
	invalidMesh.VAO = -1;

	return invalidMesh;
}

void Nova::deleteMeshes(Nova::Array<Nova::MeshInfo>& meshes)
{
	for (auto mesh : meshes)
	{
		glDeleteVertexArrays(1, &mesh.VAO);
		glDeleteBuffers(1, &mesh.VBO);
		glDeleteBuffers(1, &mesh.VBO);
	}

	meshes.clear();
	meshes.shrink_to_fit();
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

Nova::TextureInfo* Nova::loadTexture(Nova::String name, Nova::String filename, Nova::TexType type, Nova::Array<Nova::TextureInfo*>& textureSet)
{
	for(auto texture : textureSet)
	{
		if(texture->name == name)
		{
			std::cerr << "ERROR: Failed to load texture: " << filename <<  ", name is already taken." << std::endl;
			return NULL;
		}
	}

	Nova::TextureInfo* tex = new Nova::TextureInfo();

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
		tex->path = filename;

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

void Nova::deleteTextures(Nova::Array<Nova::TextureInfo*>& textureSet)
{
	for (auto texture : textureSet)
	{
		glDeleteTextures(1,	&texture->texture);

		delete texture;
	}

	textureSet.clear();
	textureSet.shrink_to_fit();
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
	Json::Value entities;

	//Save all the meshes used
	Json::Value meshes;
	for (int i = 0; i < Nova::globalMeshes.size(); ++i)
	{
		const auto& mesh = Nova::globalMeshes[i];
		Json::Value m;

		m["name"] = mesh.name;
		m["path"] = mesh.filepath;

		meshes.append(m);
	}
	root["meshes"] = meshes;

	//Save all the textures used
	Json::Value textures;
	for (int i = 0; i < Nova::globalTextures.size(); ++i)
	{
		const auto tex = Nova::globalTextures[i];
		Json::Value t;

		t["name"] = tex->name;
		t["path"] = tex->path;
		t["type"] = tex->type;

		textures.append(t);
	}
	root["textures"] = textures;

	for (const auto& e : Nova::entities)
	{
		Json::Value obj;
		obj["name"] = e.doc_name();
		
		//Save transform info
		if (e.has<Nova::Component::Transform>())
		{
			Json::Value t;

			const auto transform = e.get<Nova::Component::Transform>();
			const auto pos = transform->position.data();
			const auto rot = transform->rotation.data();
			const auto scale = transform->scale.data();

			t["position"][0] = pos[0];
			t["position"][1] = pos[1];
			t["position"][2] = pos[2];

			t["rotation"][0] = rot[0];
			t["rotation"][1] = rot[1];
			t["rotation"][2] = rot[2];

			t["scale"][0] = scale[0];
			t["scale"][1] = scale[1];
			t["scale"][2] = scale[2];

			obj["transform"] = t;
		}

		//Save mesh info
		if (e.has<Nova::Component::Mesh>())
		{
			Json::Value m;
			Json::Value ts;

			const auto mesh = e.get<Nova::Component::Mesh>();
			const auto& meshInfo = mesh->meshInfo;
			const auto& textures = mesh->textures;
			
			//Only the name is needed for lookup later for both mesh and textures
			m["name"] = meshInfo.name;

			for (int i = 0; i < textures.size(); ++i)
			{
				Json::Value tex;

				tex["name"] = textures[i]->name;

				ts.append(tex);
			}

			m["textures"] = ts;
			obj["mesh"] = m;
		}

		//Save point light info
		if (e.has<Nova::Component::PointLight>())
		{
			Json::Value pl;
			Json::Value b;

			const auto pointLight = e.get<Nova::Component::PointLight>();
			const auto& base = pointLight->base;
			const auto quadratic = pointLight->quadratic;
			const auto linear = pointLight->linear;
			const auto constant = pointLight->constant;

			//Only the name is needed for lookup later for both mesh and textures
			b["ambient"][0] = base.ambient[0];
			b["ambient"][1] = base.ambient[1];
			b["ambient"][2] = base.ambient[2];

			b["diffuse"][0] = base.diffuse[0];
			b["diffuse"][1] = base.diffuse[1];
			b["diffuse"][2] = base.diffuse[2];

			b["specular"][0] = base.specular[0];
			b["specular"][1] = base.specular[1];
			b["specular"][2] = base.specular[2];

			pl["base"] = b;

			pl["quadratic"] = quadratic;
			pl["linear"] = linear;
			pl["constant"] = constant;

			obj["point_light"] = pl;
		}

		//Use the unique ID to ensure that nothing is overwritten
		entities.append(obj);
	}
	root["entities"].append(entities);
	
	Json::StreamWriterBuilder builder;
	const std::string jsonFile = Json::writeString(builder, root);

	std::ofstream file(filepath);
	file << jsonFile;

	std::cout << "Saved!" << std::endl;

	return 0;
}

Nova::Int Nova::loadScene(const Nova::String& filepath)
{
	Nova::String jsonData = readFileToString(filepath);
	JSONCPP_STRING err;
	Json::Value root;
	
	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse(jsonData.c_str(), jsonData.c_str() + jsonData.length(), &root, &err)) {
		std::cout << "ERROR: " << err << std::endl;
		return -1;
	}

	clearScene();

	//Properly load everything back into the scene
	std::cout << root["meshes"][0]["name"] << std::endl;
	loadCubeMesh();

	return 0;
}

void Nova::clearScene()
{
	deleteTextures(Nova::globalTextures);
	deleteMeshes(Nova::globalMeshes);

	Nova::lightManager.deleteLights();

	for (auto entity : Nova::entities)
	{
		entity.destruct();
	}

	Nova::entities.clear();
	Nova::entities.shrink_to_fit();
}
