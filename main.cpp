#include<iostream>

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

#include <random>
#include <ctime>
#include <chrono>
#include <time.h>

#include"sorting_algos.h"

int main(int argc, char** argv) {
	{
		typedef std::chrono::milliseconds MSec;
		typedef std::chrono::high_resolution_clock hrc;
		typedef std::chrono::duration<double, std::milli> milliSecs;
		typedef std::vector<int> intVec;
		typedef std::pair<std::string, std::vector<float>> sv_pair;

		std::map<std::string, std::vector<float>> lists;

		//Populates random, ascending, and descending lists
		{
			srand(static_cast<unsigned int>(time(0)));
			for (int i = 0; i < 5000; i++) {
				lists["Random"].push_back(rand());
				lists["Ascending"].push_back((float)i);
				lists["Descending"].push_back(5000.0f - (float)i);
			}

		}

		for (sv_pair p : lists) {
			std::string list = p.first;
			std::vector<float> v = p.second;
			std::cout << list << " List\n__________________________________________\n";


			for (auto funct : MySrt::functions) {
				std::vector<float> vect(v);
				std::cout << list << " " << funct.first << " on " << 50000 << " elements\n";

				auto start = hrc::now();
				funct.second(vect);
				auto end = hrc::now();

				milliSecs executionDuration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

				printf("\r%d element %s list %s completed in %.3f milliseconds\n", 5000, list.c_str(), funct.first.c_str(), executionDuration.count());

				std::cout << std::endl;
				std::cout << std::endl;
			}
		}
	}



	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);	
	if (window == nullptr)
	{
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool done = false;
	while(!done)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		//DEMO WINDOW_______________________________________________________
		//__________________________________________________________________
		//__________________________________________________________________
		//__________________________________________________________________
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return 0;
}