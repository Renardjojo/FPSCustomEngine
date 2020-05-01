#ifndef __UI_HPP__
#define __UI_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <SDL2/SDL_ttf.h>


#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/shader.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"

#include <functional>
#include <vector>


namespace Engine::Ressources
{

class Button
{
public:
    // void(*function)();
    std::function<void()> function = [](){};

    GLuint VAO;
    Engine::Ressources::Shader shader;

    Engine::Core::Maths::Vec3 color;

    float x;
    float y;
    float w;
    float h;
    
public:

    Button(float, float, float, float, Engine::Core::Maths::Vec3);
    // Button(const Button &other);
    // Button(Button &&other);
    virtual ~Button();

    Button()                                = delete;
    Button &operator=(Button const &other)  = delete;
    Button &operator=(Button &&other)       = delete;

    void draw();
    bool isButtonPressed(float x, float y, int state);
};



class TextField
{
private:
    TTF_Font* font;
    SDL_Color color;
    GLuint texture;
    GLuint VAO;
    Engine::Ressources::Shader shader;

    std::string value;

    float x;
    float y;
    float w;
    float h; 


public:
    std::function<void()> function = [](){};
    bool active = false;
    bool keyPressed = false;
    bool wasActive = false;

    TextField(const char* TTFPath, float, float, float, float, const std::string&);
    // TextField(const TextField &other);
    // TextField(TextField &&other);
    virtual ~TextField();

    TextField()                                   = delete;
    TextField &operator=(TextField const &other)  = delete;
    TextField &operator=(TextField &&other)       = delete;

    void draw();
    void updateTexture();
    void updateString(char);
    bool isTextFieldactive(float x, float y);
    void desactivateTextField();
    void stringDel();
   
    std::string getValue() const
    {
        return value;
    }
    std::string& setValue()
    {
        return value;
    }
};

class UIBox
{
private:
    std::vector<Button> buttons;
    std::vector<TextField> textfields;

    
public:
    UIBox();
    ~UIBox();
};

};

#endif