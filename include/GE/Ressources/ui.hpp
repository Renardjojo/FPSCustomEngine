#ifndef __UI_HPP__
#define __UI_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <SDL2/SDL_ttf.h>

#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/shader.hpp"
#include "GE/Ressources/texture.hpp"
#include "GE/Ressources/text.hpp"
#include "GE/Physics/transform.hpp"
#include "GE/Ressources/font.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "Game/define.h"


#include <functional>
#include <vector>
#include <memory>

namespace Engine::Ressources
{
    class Button
    {
    private:
        Font *font;
        Engine::Ressources::Shader* shader;
        SDL_Color color;
        SDL_Color currentColor;
        GLuint VAO;
        int buttonState = 0;

        float x;
        float y;
        float w;
        float h;

    public:        
        GLuint texture;
        std::string value;
        std::unique_ptr<Engine::Physics::Transform> transform;
        E_GAME_STATE whenIsActive;
        bool isActive;

        std::function<void()> function = [](){};

        Button(Font*, Engine::Ressources::Shader*, float, float, float, float, SDL_Color, const std::string&,  E_GAME_STATE);
        // Button(const Button &other) = delete;
        // Button(Button &&other) = delete;
        virtual ~Button();

        Button() = delete;
        Button &operator=(Button const &other) = delete;
        Button &operator=(Button &&other) = delete;

        void draw();
        void updateTexture();
        bool isButtonPressed(float x, float y, int state);

        Engine::Core::Maths::Vec2 getPos() { return Engine::Core::Maths::Vec2{x, y}; }
    };

    class TextField
    {
    private:
        Font *font;
        Engine::Ressources::Shader *shader;
        SDL_Color color;
        GLuint texture;
        GLuint VAO;

        float x;
        float y;
        float w;
        float h;

    public:
        std::string value;
        std::function<void()> function = []() {};
        bool active = false;
        bool keyPressed = false;
        bool wasActive = false;

        TextField(Font *font, Shader *_shader, float, float, float, float, const std::string&);
        // TextField(const TextField &other);
        // TextField(TextField &&other);
        virtual ~TextField();

        TextField() = delete;
        TextField &operator=(TextField const &other) = delete;
        TextField &operator=(TextField &&other) = delete;

        void draw();
        void updateTexture();
        void updateString(char);
        bool isTextFieldactive(float x, float y);
        void desactivateTextField();
        void stringDel();
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

}; // namespace Engine::Ressources

#endif