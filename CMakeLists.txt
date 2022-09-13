cmake_minimum_required(VERSION 3.10)
project(Rendering-Application)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#####################################################
# SETUP RENDERING-APPLICATION-CORE
#####################################################
file(GLOB RENDERING_APPLICATION_CORE_SOURCE_FILES ./Rendering-Application-Core/*.cpp)
file(GLOB RENDERING_APPLICATION_CORE_HEADER_FILES ./Rendering-Application-Core/*.hpp)
source_group("Source Files" FILES ${RENDERING_APPLICATION_CORE_SOURCE_FILES})
source_group("Header Files" FILES ${RENDERING_APPLICATION_CORE_HEADER_FILES})
add_compile_definitions(NOMINMAX)
add_compile_definitions(UNICODE)
find_package(OpenGL REQUIRED)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_library(Rendering-Application-Core STATIC 
	${RENDERING_APPLICATION_CORE_SOURCE_FILES}
	${RENDERING_APPLICATION_CORE_HEADER_FILES}
	${GLFW_SOURCE_FILES})
add_subdirectory(./Dependencies/glfw)
add_subdirectory(./Dependencies/glew-cmake)
target_link_libraries(Rendering-Application-Core PUBLIC
	${OPENGL_LIBRARIES}
	glfw
	libglew_static)
target_include_directories(Rendering-Application-Core PUBLIC
	${OPENGL_INCLUDE_DIRS}
	./Dependencies/glfw/include
	./Dependencies/glew-cmake/include)

#####################################################
# SETUP RENDERING-APPLICATION-EXAMPLE
#####################################################
file(GLOB RENDERING_APPLICATION_EXAMPLE_SOURCE_FILES ./Rendering-Application-Example/*.cpp)
file(GLOB RENDERING_APPLICATION_EXAMPLE_HEADER_FILES ./Rendering-Application-Example/*.hpp)
source_group("Source Files" FILES ${RENDERING_APPLICATION_EXAMPLE_SOURCE_FILES})
source_group("Header Files" FILES ${RENDERING_APPLICATION_EXAMPLE_HEADER_FILES})
add_executable(Rendering-Application-Example
	${RENDERING_APPLICATION_EXAMPLE_SOURCE_FILES}
	${RENDERING_APPLICATION_EXAMPLE_HEADER_FILES})
target_link_libraries(Rendering-Application-Example PUBLIC
	Rendering-Application-Core)
target_include_directories(Rendering-Application-Example PUBLIC
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/Rendering-Application-Core)

#####################################################
# FINAL
#####################################################
if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'Rendering-Application-Example' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Rendering-Application-Example)
    endif()
endif()