#ifndef APPLICATION_H
#define APPLICATION_H

#include<random>
#include"sorting_window.h"

namespace MySrt{
    void Start();
    void Update(SDL_Window* window);
    void Draw(SDL_Window* window, ImGuiIO& io);
    void Run();
    void End();

    void RenderMainMenuBar();
    void RenderMainMenuOptions();
    void RenderMainMenuAbout();

    void RenderInputs();
    void RenderWindows();
    void Sort();

    void Reset(bool);
}

#endif