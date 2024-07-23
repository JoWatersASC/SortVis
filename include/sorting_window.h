#ifndef SORTING_WINDOW_H
#define SORTING_WINDOW_H

#include <chrono>
#include <future>

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
	typedef size_t elemType;
	inline std::map<std::string, bool> windows_open;


	class SortingWindow {
	private:
		std::vector<int> list;
		const char* sortFunc;
		std::string sortFuncString;
		ImVec2 pos;
		ImVec2 dim;

		HRC timer;
		bool *open;

		bool RenderMenuBar();
		bool RenderWinList();

	public:
		SortingWindow(ImVec2);
		SortingWindow(ImVec2, char*);
		SortingWindow(ImVec2, const char*, bool*);

		void Render(const ImVec2);
		bool Render(const ImGuiWindowFlags, bool&);

		void setList();
		void setList(const std::vector<int>&);
		void sortList();
		void printList();

		ImVec2& position();
		ImVec2& dimension();
		
		std::string getName();
		bool isOpen();

		friend bool colliding(const SortingWindow&, const SortingWindow&);
		friend void sortList(SortingWindow&);

		static std::vector<int> winStartList;
		static std::map<std::string, SortingWindow*>* SortingWindowList;
	};
}

//experimental imvec2 algebra operators
ImVec2 operator+(const ImVec2& a, const ImVec2& b);
ImVec2 operator-(const ImVec2& a, const ImVec2& b);
ImVec2 operator*(const ImVec2& vec, const ImVec2& factor);
ImVec2 operator*(const ImVec2& vec, float factor);
ImVec2 operator/(const ImVec2& vec, const ImVec2& divisor);
ImVec2 operator/(const ImVec2& vec, float divisor);

#endif