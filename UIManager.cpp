#include "UIManager.h"
 
UIManager::UIManager(Shader* shader, int screenWidth, int screenHeight)
    : shader(shader), screenWidth(screenWidth), screenHeight(screenHeight) 
{
    uiElements.clear();
    set_screen_res(screenWidth, screenHeight);
    load_defaults();
}

UIManager::~UIManager() 
{
    for (auto& element : uiElements) {
        delete element;
    }
    uiElements.clear();
}

void UIManager::load_defaults() 
{
    std::cout << "Loading default assets..." << std::endl;
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c01.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c02.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c03.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c04.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c05.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c06.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c07.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c08.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c09.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c10.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c11.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c12.png")));
    //uiElements.push_back(new UIElement(AssetManager::load_texture("C:/Dev/assets/cards/c13.png")));

    center(uiElements);
}

void UIManager::set_screen_res(int width, int height) 
{
    screenWidth = width;
    screenHeight = height;
    projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
}

//will probably need to be decoupled.
void UIManager::load_elements(unsigned int* cards, unsigned int selectedCard)
{
    int size = 0;
    uiElements.clear();
    if (selectedCard == 6)
    {
        size = 1;
    }
    else {
        size = 5;
    }
    for (int i = 0; i < size; i++)
    {
        uiElements.push_back(new UIElement(AssetManager::get_card(cards[i] - 1)));
        if (i == selectedCard)
        {
            uiElements.back()->toggle_select();
        }
    }
    
    center(uiElements);
}

void UIManager::draw() 
{
    for (UIElement* element : uiElements)
    {
        element->draw(shader, projection);
    }
}

void UIManager::center(std::vector<UIElement*> elements)
{
    float normalizedWidth = 142.0f / (float)screenWidth;
    float normalizedHeight = 272.0f / (float)screenHeight;
    float buffer = (screenWidth - elements.size() * 142.0f ) / 2;

    for (int i = 0; i < elements.size(); i++)
    {
        float normalizedX = i * normalizedWidth;
        // float normalizedY = 0.0f;
        //fix lol
        /*if(elements[i]->is_selected())
        {
            elements[i]->set_size(normalizedWidth * (float)screenWidth * 2.0f, normalizedHeight * (float)screenHeight * 2.0f);
            elements[i]->set_position(normalizedX * 2.0f * (float)screenWidth + buffer, (float)screenHeight - (272.0f * 2.0f )- 50.0f);
        } else {*/
            elements[i]->set_size(normalizedWidth * (float)screenWidth, normalizedHeight * (float)screenHeight);
            elements[i]->set_position(normalizedX * (float)screenWidth + buffer, (float)screenHeight - 272.0f - 50.0f);
        //}
        
    }
}