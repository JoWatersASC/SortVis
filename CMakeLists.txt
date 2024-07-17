cmake_minimum_required(VERSION 3.22.1)

project(SortingVisualizer)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/imgui")
set(SDL_DIR "${CMAKE_SOURCE_DIR}/SDL2")
set(SDL_LIB_DIR "${CMAKE_SOURCE_DIR}/SDL2/lib/x64/")
find_package(OpenGL REQUIRED)

file(GLOB SOURCE_FILES "./src/*.cpp")
file(GLOB INCLUDE_FILES "./include/*.h")

file(GLOB IMGUI_FILES "${IMGUI_DIR}/*.cpp")
file(GLOB IMGUI_HEADERS "${IMGUI_DIR}/*.h")

link_directories(${SDL_LIB_DIR})

add_executable(${PROJECT_NAME} ${IMGUI_FILES} ${SOURCE_FILES} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_HEADERS} ${INCLUDE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR} PUBLIC ${SDL_DIR}/include PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2main)
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