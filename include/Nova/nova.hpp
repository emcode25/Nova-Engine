#ifndef NOVA_HPP
#define NOVA_HPP

#ifndef NOVA_HPP_NO_HEADERS

#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>

#include <imgui/imgui.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <flecs.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "types.hpp"
#include "const.hpp"
#include "utils.hpp"
#include "callbacks.hpp"
#include "components.hpp"
#include "objects.hpp"
#include "shader.hpp"
#include "ui.hpp"
#include "editor_camera.hpp"
#include "systems.hpp"

#endif

#include "engine.hpp"

#endif