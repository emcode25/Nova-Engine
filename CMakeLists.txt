# CMakeList.txt : CMake project for PGE, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# C++ Standard Stuff
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Begin projecting config
project (PGE VERSION 0.1)

# Add source to this project's executable.
#file(GLOB_RECURSE SRC_FILES *.cpp CONFIGURE_DEPENDS)
add_executable(PGE "src/main.cpp" "src/flecs.c" "src/glad.c" "src/PGE/shader.cpp" "src/PGE/utils.cpp" "src/PGE/callbacks.cpp" "src/PGE/objects.cpp" "src/imgui/imgui.cpp" "src/imgui/imgui_impl_glfw.cpp" "src/imgui/imgui_impl_opengl3.cpp" "src/imgui/imgui_tables.cpp" "src/imgui/imgui_widgets.cpp" "src/imgui/imgui_draw.cpp" "src/imgui/imgui_demo.cpp" "src/PGE/ui.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_include_directories(PGE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)

find_library(GLFW3 glfw3 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(LIBS ${GLFW3} opengl32)
target_link_libraries(PGE PUBLIC ${LIBS})
