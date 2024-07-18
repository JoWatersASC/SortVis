# ImGui Notes
## Display(Application & ImGui Window)
### ImGui Window
- ImGui::GetWindowContentRegionMin() - gets minimum size/relative position of content region(ImVec2)
    - Gets position of content space relative to current window
- ImGui::GetWindowContentRegionMax() - gets maximum size/relative position of content region(ImVec2)
    - Gets position of bounds of conent space relative to current window, how large is content space
- ImGui::GetWindowPos() - gets position of current window relative to application window(ImVec2)
- ImGui indexes windows by name(ImBegin(name)) so if multiple windows have same name, will be treated as one
### Application Window
- ImGui::GetIO().DisplaySize - gets display size of application window in pixels(ImVec2)