#include"UIElement.h"

UIElement::UIElement(unsigned int textureID) 
    : m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f), textureID(textureID) {}

void UIElement::set_size(float width, float height)
{
    m_height = height;
    m_width = width;
}

void UIElement::set_position(float x, float y)
{
    m_x = x;
    m_y = y;
}

bool UIElement::is_selected()
{
    return isSelected;
}

void UIElement::toggle_select()
{
    isSelected = !isSelected;
}

void UIElement::draw(Shader* shader, glm::mat4 projection) {
    shader->use();
    shader->setMat4("projection", projection);

    if (isSelected)
    {
        shader->setBool("selected", true);
    }
    else {
        shader->setBool("selected", false);
    }

    float vertices[] = {
        m_x, m_y, 0.0f, 0.0f, 1.0f,
        m_x + m_width, m_y, 1.0f, 0.0f, 1.0f,
        m_x + m_width, m_y + m_height, 1.0f, 1.0f, 1.0f,
        m_x, m_y + m_height, 0.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
