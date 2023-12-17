# CMakeList.txt : CMake project for Nova, include source and define
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
project (Nova VERSION 0.1)

# Add source to this project's executable.
#file(GLOB_RECURSE SRC_FILES *.cpp CONFIGURE_DEPENDS)
add_executable(Nova "src/main.cpp" "src/flecs.c" "src/glad.c" "src/Nova/shader.cpp" "src/Nova/utils.cpp" "src/Nova/callbacks.cpp" "src/Nova/objects.cpp" "src/imgui/imgui.cpp" "src/imgui/imgui_impl_glfw.cpp" "src/imgui/imgui_impl_opengl3.cpp" "src/imgui/imgui_tables.cpp" "src/imgui/imgui_widgets.cpp" "src/imgui/imgui_draw.cpp" "src/imgui/imgui_demo.cpp" "src/Nova/ui.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()


# Determine the correct library locations
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message("Linux detected.")
  set(LIB_GLOBAL_DIR )

  find_library(LIB_GLFW3 glfw)
  if(NOT LIB_GLFW3)
    message(FATAL_ERROR "glfw3 not found.")
  endif()

  find_library(LIB_GL GL)
  if(NOT LIB_GL)
    message(FATAL_ERROR "GL not found.")
  endif()

  find_library(LIB_X11 X11)
  if(NOT LIB_X11)
    message(FATAL_ERROR "X11 not found.")
  endif()

  find_library(LIB_PTHREAD pthread)
  if(NOT LIB_PTHREAD)
    message(FATAL_ERROR "pthread not found.")
  endif()

  find_library(LIB_XRANDR Xrandr)
  if(NOT LIB_XRANDR)
    message(FATAL_ERROR "Xrandr not found.")
  endif()

  find_library(LIB_XI Xi)
  if(NOT LIB_XI)
    message(FATAL_ERROR "Xi not found.")
  endif()

  find_library(LIB_DL dl)
  if(NOT LIB_DL)
    message(FATAL_ERROR "dl not found.")
  endif()

  set(LIBS glfw GL X11 pthread Xrandr Xi dl)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  find_library(GLFW3 glfw3 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
  set(LIBS ${GLFW3} opengl32)
endif()

# Find directories and libraries
target_include_directories(Nova PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)
target_link_libraries(Nova PUBLIC ${LIBS})