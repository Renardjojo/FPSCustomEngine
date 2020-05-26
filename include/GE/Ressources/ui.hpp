#ifndef __UI_HPP__
#define __UI_HPP__

#include <glad/glad.h>

#include <SDL2/SDL_ttf.h>

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
    class Title
    {
    private:
        Font *font;
        Engine::Ressources::Shader *shader;
        SDL_Color color;
        GLuint VAO;

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

        Title(Font *, Engine::Ressources::Shader *, float, float, float, float, SDL_Color, const std::string &, E_GAME_STATE);
        // Title(const Title &other) = delete;
        // Title(Title &&other) = delete;
        virtual ~Title();

        Title() = delete;
        Title &operator=(Title const &other) = delete;
        Title &operator=(Title &&other) = delete;

        void draw();
        void updateTexture();

        Engine::Core::Maths::Vec2 getPos() { return Engine::Core::Maths::Vec2{x, y}; }
    };

    class ReferencedTitle
    {
    private:
        Font *font;
        Engine::Ressources::Shader *shader;
        SDL_Color color;
        GLuint VAO;

        float x;
        float y;
        float w;
        float h;

    public:
        GLuint texture;
        std::string value;
        int* valuePtr;
        std::unique_ptr<Engine::Physics::Transform> transform;
        bool isActive;
        E_GAME_STATE whenIsActive;

        ReferencedTitle(Font *, Engine::Ressources::Shader *, float, float, float, float, SDL_Color, int*, E_GAME_STATE);
        // ReferencedTitle(const ReferencedTitle &other) = delete;
        // ReferencedTitle(ReferencedTitle &&other) = delete;
        virtual ~ReferencedTitle();

        ReferencedTitle() = delete;
        ReferencedTitle &operator=(ReferencedTitle const &other) = delete;
        ReferencedTitle &operator=(ReferencedTitle &&other) = delete;

        void draw() ;
        void update();
        void updateTexture();

    };

    class Button
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

        SDL_Color currentColor;
        int buttonState = 0;

    public:
        std::string value;
        std::unique_ptr<Engine::Physics::Transform> transform;
        E_GAME_STATE whenIsActive;
        bool isActive;

        std::function<void()> function = []() {};

        Button(Font *, Engine::Ressources::Shader *, float, float, float, float, SDL_Color, const std::string &, E_GAME_STATE);
        // Button(const Button &other) = delete;
        // Button(Button &&other) = delete;
        virtual ~Button();

        Button() = delete;
        Button &operator=(Button const &other) = delete;
        Button &operator=(Button &&other) = delete;

        void draw();
        void updateTexture();

        int isButtonPressed(float x, float y, int state);
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

        TextField(Font *font, Shader *_shader, float, float, float, float, const std::string &);
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

    class Image
    {
    private:
        GLuint _texture;
        Engine::Ressources::Shader *_shader;
        float _x, _y, _w, _h;
        GLuint _VAO;

    public:
        bool isActive;
        E_GAME_STATE whenIsActive;

        std::unique_ptr<Engine::Physics::Transform> transform;

        virtual ~Image();
        Image(unsigned int textureId, Engine::Ressources::Shader *shader, float x, float y, float width, float height, E_GAME_STATE state);

        Image() = delete;
        Image &operator=(Image const &other) = delete;
        Image &operator=(Image &&other) = delete;

        void draw();

        Engine::Core::Maths::Vec2 getPos();
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