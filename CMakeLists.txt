cmake_minimum_required(VERSION 3.22.1)

project(SortingVisualizer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wformat")

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/lib/imgui")

if(WIN32)
	set(SDL_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2")
	set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
	set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2main.lib")
	string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
endif()

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

file(GLOB SOURCE_FILES "./src/*.cpp")
file(GLOB INCLUDE_FILES "./include/*.h")

file(GLOB IMGUI_FILES "${IMGUI_DIR}/*.cpp")
file(GLOB IMGUI_HEADERS "${IMGUI_DIR}/*.h")

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${IMGUI_FILES} ${SOURCE_FILES} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_HEADERS} ${INCLUDE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR} PUBLIC ${SDL_INCLUDE_DIRS} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()


#Creates filters for Visual Studio
if(WIN32)
	source_group("Source Files" FILES ${SOURCE_FILES})
	source_group("Header Files" FILES ${INCLUDE_FILES})

	source_group("ImGui\\Source Files" FILES ${IMGUI_FILES})
	source_group("ImGui\\Header Files" FILES ${IMGUI_HEADERS})
endif()
