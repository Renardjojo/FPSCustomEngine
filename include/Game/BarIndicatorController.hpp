//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-08 - 17 h 39

#ifndef _BAR_INDICATOR_CONTROLLER_H
#define _BAR_INDICATOR_CONTROLLER_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"

namespace Game
{
    template<class T = float>
    class BarIndicatorController : public Engine::Core::Component::ScriptComponent
    {
        private:
        
        T&      _refValue;
        T       _maxValue;

        float   _maxScaleX;

        public:


        BarIndicatorController(Engine::Ressources::GameObject &gameObject, T& value, const T& maxValue)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _refValue                                   {value},
                _maxValue                                   {maxValue},
                _maxScaleX                                  {gameObject.getScale().x}
        {}

        virtual ~BarIndicatorController() = default;

        void update() override
        {
            Engine::Core::Maths::Vec3 currentScale = _gameObject.getScale();
            currentScale.x = (_maxScaleX * static_cast<float>(_refValue)) / static_cast<float>(_maxValue);

            if (currentScale.x >= 0.f)
                _gameObject.setScale(currentScale);
        }


    };

} //namespace Game

#endif //_BAR_INDICATOR_CONTROLLER_H