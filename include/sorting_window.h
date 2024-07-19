#ifndef SORTING_WINDOW_H
#define SORTING_WINDOW_H

#include <chrono>
#include <cstdlib>

#include"imgui.h"
#include"imgui_impl_opengl3.h"
#include"imgui_impl_sdl2.h"
#include"imgui_impl_sdlrenderer2.h"

#include<SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include"sorting_algos.h"

namespace MySrt
{
	typedef std::chrono::high_resolution_clock HRC;
	typedef int elemType;

	class SortingWindow {
	private:
		std::vector<int> list;
		const char* sortFunc;
		std::string sortFuncString;
		ImVec2 pos;
		HRC timer;
		bool *open;

		bool RenderMenuBar();
		bool RenderWinList();

	public:
		SortingWindow(ImVec2);
		SortingWindow(ImVec2, char*);
		SortingWindow(ImVec2, const char*, bool*);

		void Render(const ImVec2);
		bool Render(const ImVec2, const ImGuiWindowFlags);

		void setList();
		void setList(const std::vector<int>&);
		void sortList();
		void printList();

		ImVec2& position();
		bool isOpen();
		friend bool colliding(const SortingWindow&, const SortingWindow&);

		static std::vector<int> winStartList;
		static std::map<std::string, SortingWindow*>* SortingWindowList;
	};
}

namespace MySrt {
	void open_url(const std::string& url);
}


//experimental imvec2 algebra operators
ImVec2 operator+(const ImVec2& a, const ImVec2& b);
ImVec2 operator-(const ImVec2& a, const ImVec2& b);
ImVec2 operator*(const ImVec2& vec, const ImVec2& factor);
ImVec2 operator*(const ImVec2& vec, int a);

#endif