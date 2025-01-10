#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Shader.h"
#include "UIElement.h"
#include "AssetManager.h"

class UIManager {
public:
    UIManager(Shader* shader, int screenWidth, int screenHeight);
    ~UIManager();

    void load_elements(unsigned int* cards, unsigned int card);
    void draw();
private:
    Shader* shader;
    int screenWidth, screenHeight;
    glm::mat4 projection;
    std::vector<UIElement*> uiElements;
    
    void load_defaults();
    void set_screen_res(int width, int height);
    void center(std::vector<UIElement*> elements);
};
#endif // !UI_MANAGER_H