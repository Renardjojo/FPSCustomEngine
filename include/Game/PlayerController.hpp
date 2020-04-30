#ifndef __PLAYERCONTROLLER_HPP__
#define __PLAYERCONTROLLER_HPP__

namespace Engine::Core::InputSystem{class Input;}

namespace Game
{

class PlayerController
{
private:
    const Engine::Core::InputSystem::Input& _input;
    void move();
public:
    PlayerController(const Engine::Core::InputSystem::Input& input);
    ~PlayerController();

    void update();
    void draw();
};
}
#endif // __PLAYERCONTROLLER_HPP__
