cmake_minimum_required(VERSION 3.14)

SET(PROJ_NAME "Game")
project(${PROJ_NAME})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

file(GLOB GAME Game/*.cpp Game/*.h)
file(GLOB ENGINE Engine3D/*.cpp Engine3D/*.c  Engine3D/*.h)
file(GLOB DISPLAY DisplayGLFW/*.cpp DisplayGLFW/*.h)

include_directories(${PROJECT_SOURCE_DIR}/res/includes ${PROJECT_SOURCE_DIR}/Engine3D
 ${PROJECT_SOURCE_DIR}/DisplayGLFW)
add_subdirectory(${PROJECT_SOURCE_DIR}/res/includes/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/res/includes/glad)


add_library(Display ${DISPLAY})
add_library(Engine ${ENGINE})


add_executable(${PROJ_NAME} ${GAME} Game/ray.h Game/hittable.h Game/sphere.h Game/sphere.cpp Game/hittable_list.cpp Game/hittable_list.h Game/ray.cpp Game/eye.h Game/plane.cpp Game/plane.h Game/light.h Game/directional_light.cpp Game/directional_light.h Game/light.cpp Game/light_list.cpp Game/light_list.h Game/spotlight.cpp Game/spotlight.h)

target_link_libraries(${PROJ_NAME} Engine Display glfw glad)

