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
    class BarIndicatorController : public Engine::Core::Component::ScriptComponent
    {
        private:
        
        int*      _refValue   {nullptr};
        int       _maxValue   {1};
        float     _maxScaleX  {0.f};

        public:

        BarIndicatorController(Engine::Ressources::GameObject &gameObject)
            :   Engine::Core::Component::ScriptComponent    {gameObject}
        {
            _name = __FUNCTION__;
        }

        void init (int& value, const int& maxValue)
        {
            _refValue = &value;
            _maxValue = maxValue;
        }

        virtual ~BarIndicatorController() = default;

        void start() override
        {
            _maxScaleX = _gameObject.getScale().x;
            std::cout << _maxScaleX << "    " << _gameObject.getScale().x << "  " << *_refValue << "    " << _maxValue << std::endl;
        }

        void update() override
        {
            Engine::Core::Maths::Vec3 currentScale = _gameObject.getScale();
            currentScale.x = (_maxScaleX * (*_refValue)) / static_cast<float>(_maxValue);



            if (currentScale.x >= 0.f)
                _gameObject.setScale(currentScale);

        }

        void save(xml_document<>& doc, xml_node<>* nodeParent)
        { 
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_BAR_INDICATOR_CONTROLLER_H