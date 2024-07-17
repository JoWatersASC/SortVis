cmake_minimum_required(VERSION 3.22.1)

project(SortingVisualizer)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/imgui")
set(SDL_DIR "${CMAKE_SOURCE_DIR}/SDL2")
set(SDL_LIB_DIR "${CMAKE_SOURCE_DIR}/SDL2/lib/x64/")
set(OPENGL_LIB "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64")

file(GLOB SOURCE_FILES "./src/*.cpp")
file(GLOB INCLUDE_FILES "./include/*.h")

file(GLOB IMGUI_FILES "${IMGUI_DIR}/*.cpp")
file(GLOB IMGUI_HEADERS "${IMGUI_DIR}/*.h")

link_directories(${SDL_LIB_DIR} ${OPENGL_LIB})

add_executable(${PROJECT_NAME} ${IMGUI_FILES} ${SOURCE_FILES} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_HEADERS} ${INCLUDE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR} PUBLIC ${SDL_DIR}/include PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2main OpenGL32)

#Creates filters for Visual Studio
if(WIN32)
	source_group("Source Files" FILES ${SOURCE_FILES})
	source_group("Header Files" FILES ${INCLUDE_FILES})

	source_group("ImGui\\Source Files" FILES ${IMGUI_FILES})
	source_group("ImGui\\Header Files" FILES ${IMGUI_HEADERS})
endif()