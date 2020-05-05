#include "GE/Core/Debug/log.hpp"
#include "GE/Ressources/ui.hpp"
#include "GE/Ressources/text.hpp"
#include "GE/Core/System/UISystem.hpp"
#include "GE/Physics/transform.hpp"
#include "Game/define.h"


using namespace Engine::Physics;
using namespace Engine::Ressources;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Systems;


Button::Button(Font * _font, Engine::Ressources::Shader* _shader,
                float _x, float _y, float _w, float _h, SDL_Color _color, const std::string& _string, E_GAME_STATE _whenIsActive)
    :   font(_font),
        shader(_shader)
{   
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    value = _string;

    color = currentColor = _color;

    isActive = true;
    whenIsActive = _whenIsActive;

    float wid = WIDTH;
    float hei = HEIGHT;

    float tempx1 = ((x) / wid * 2) - 1;
    float tempy1 = 1 - ((y) / hei * 2);
    float tempx2 = ((x + w) / wid * 2) - 1;
    float tempy2 = 1 - ((y + h) / hei * 2);

    GLfloat vertices[] = {tempx1, tempy1, 0.0f,
                          tempx2, tempy1, 0.0f,
                          tempx2, tempy2, 0.0f,
                          tempx1, tempy2, 0.0f};

    GLfloat uvs[]      = {0.0f, 0.0f, 0.0f,
                          1.0f, 0.0f, 0.0f,
                          1.0f, 1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    GLuint VBO, VBO2, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);  

    updateTexture();

    transform = std::make_unique<Transform>();

    UISystem::addButton(this);  
}

void Button::draw()
{
    glDisable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, texture);
    

    shader->use();
    GLuint matrixID = glGetUniformLocation(shader->getIdProgramm(), "matrix");

    glUniformMatrix4fv(matrixID, 1, GL_TRUE, &transform->getModelMatrix()[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);    
    glEnable(GL_DEPTH_TEST);
}


void Button::updateTexture()
{
    int bpp;
    Uint32 Rmask, Gmask, Bmask, Amask;
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp, &Rmask, &Gmask, &Bmask, &Amask);
    SDL_Surface *img_rgba8888 = SDL_CreateRGBSurface(0, w, h, bpp, Rmask, Gmask, Bmask, Amask);
    SDL_Surface* surface = TTF_RenderText_Solid(font->getpFont(), value.c_str(), currentColor);
    SDL_BlitSurface(surface, NULL, img_rgba8888, NULL);
    SDL_FreeSurface(surface);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_rgba8888->pixels);
    SDL_FreeSurface(img_rgba8888);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texture);
}

bool Button::isButtonPressed(float x, float y, int state)
{
    if (x >= this->x && x <= this->x + this->w
    &&  y >= this->y && y <= this->y + this->h)
    {
        if (state == 1)
        {
            this->function();
            return true;
        }
        else if (state == 2)
        {
            currentColor.r = color.r / 2.0f;
            currentColor.g = color.g / 2.0f;
            currentColor.b = color.b / 2.0f;
            if (buttonState != 2)
            {
                buttonState = 2;
                updateTexture();
            }
            return false;
        }
        else
        {
            currentColor.r = color.r / 1.5f;
            currentColor.g = color.g / 1.5f;
            currentColor.b = color.b / 1.5f;
            if (buttonState != 3)
            {
                buttonState = 3;
                updateTexture();
            }
            return false;
        }
    }

    currentColor = color;
    if (buttonState != 4)
    {
        buttonState = 4;
        updateTexture();
    }
    return false;
}

Button::~Button()
{
    glDeleteVertexArrays(1, &VAO);
    UISystem::removeButton(this);
}




TextField::TextField(Font* _font, Shader* _shader, float _x, float _y, float _w, float _h, const std::string& _string)
    :   font(_font),
        shader(_shader)
{
    color.r = 255;
    color.g = 255;
    color.b = 255;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    value = _string;

    updateTexture();

    UISystem::addTextField(this);
}

TextField::~TextField()
{   
    glDeleteVertexArrays(1, &VAO);
    UISystem::removeTextField(this);
}

void TextField::updateTexture()
{
    glDeleteBuffers(1, &VAO);
/////////////////////////

    int bpp;
    Uint32 Rmask, Gmask, Bmask, Amask;
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp, &Rmask, &Gmask, &Bmask, &Amask);
    SDL_Surface *img_rgba8888 = SDL_CreateRGBSurface(0, w, h, bpp, Rmask, Gmask, Bmask, Amask);
    SDL_Surface* surface = TTF_RenderText_Solid(font->getpFont(), value.c_str(), color);
    SDL_BlitSurface(surface, NULL, img_rgba8888, NULL);
    SDL_FreeSurface(surface);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_rgba8888->pixels);
    SDL_FreeSurface(img_rgba8888);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texture);

/////////////////////////////

    float wid = WIDTH;
    float hei = HEIGHT;

    float tempx1 = ((x) / wid * 2) - 1;
    float tempy1 = 1 - ((y) / hei * 2);
    float tempx2 = ((x + w) / wid * 2) - 1;
    float tempy2 = 1 - ((y + h) / hei * 2);

    GLfloat vertices[] = {tempx1, tempy1, 0.0f,
                          tempx2, tempy1, 0.0f,
                          tempx2, tempy2, 0.0f,
                          tempx1, tempy2, 0.0f};

    GLfloat uvs[]      = {0.0f, 0.0f, 0.0f,
                          1.0f, 0.0f, 0.0f,
                          1.0f, 1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    GLuint VBO, VBO2, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);  
}

void TextField::draw()
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    Mat4 matrix;
    GLuint matrixID = glGetUniformLocation(shader->getIdProgramm(), "matrix");
    shader->use();

    glUniformMatrix4fv(matrixID, 1, GL_TRUE, &matrix[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);    
    glEnable(GL_DEPTH_TEST);
}

bool TextField::isTextFieldactive(float x, float y)
{
    if (x >= this->x && x <= this->x + this->w
    &&  y >= this->y && y <= this->y + this->h
    && !active)
    {
        active = true;
        wasActive = true;
        color.r = 255;
        color.g = 0;
        color.b = 0;
        
        updateTexture();
        return true;
    }
    else
        desactivateTextField();
    
    return false;
}

void TextField::desactivateTextField()
{   
    color.r = 255;
    color.g = 255;
    color.b = 255;
    active = false;
    if (wasActive)
    {
        updateTexture();
        wasActive = false;
    }
}

void TextField::updateString(char toAdd)
{
    if (!active)
        return;

    color.r = 255;
    color.g = 0;
    color.b = 0;

    value += toAdd;
    function();
    updateTexture();
}

void TextField::stringDel()
{
    if (value.size() == 0)
        return;
    value.resize(value.size() - 1);
    if (value.size() >= 1)
        function();
    updateTexture();
}

UIBox::UIBox()
{
    
}

UIBox::~UIBox()
{

}
