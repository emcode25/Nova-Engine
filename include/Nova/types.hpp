#ifndef TYPES_HPP
#define TYPES_HPP

#include <glad/glad.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <string>
#include <flecs.h>

namespace Nova
{
	typedef int Int;
	typedef unsigned int UInt;
	typedef float Float;
	typedef char Byte;
	typedef unsigned char UByte;

	typedef UInt ShaderProgram;

	typedef std::string String;

	typedef Eigen::Vector2f Vector2;
	typedef Eigen::Vector3f Vector3;
	typedef Eigen::Matrix4f Matrix4;
	typedef Eigen::Quaternionf Quaternion;
	typedef Eigen::Affine3f Affine3;
	
	template<typename T>
	using Array = std::vector<T>;

	typedef flecs::entity Entity;
	typedef flecs::world World;
}

#endif