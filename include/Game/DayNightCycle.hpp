//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _DAY_NIGHT_CYCLE_H
#define _DAY_NIGHT_CYCLE_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/LowRenderer/Light/directionnalLight.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Core/Debug/assert.hpp"
#include "GE/Core/Maths/vec.hpp"

namespace Game
{
    class DayNightCycle : public Engine::Core::Component::ScriptComponent
    {
        private:

        float    _dayStart             {6};
        float    _dayDuration          {12.f}; //in second
        float    _sunRiseDuration      {_dayDuration / 4.f};

        float    _nightStart           {_dayDuration + 6};
        float    _nightDuration        {12.f}; //in second
        float    _sunSetDuration       {_nightDuration / 4.f};

        float    _currentTime           {0.f};

        float    _midDay                {_dayStart + _dayDuration / 2.f};
        float    _midNight              {_dayStart + _nightDuration / 2.f};

        /*  Day recap :
            3h -> 5h59 : sunrise		        (40,40,70)		->	(100,100,100)
            6h -> 11h59 : light increase	    (100,100,100)	->	(255,255,255)
            12h -> 17h59 : light deacrease		(255,255,255)	->	(100,100,100)
            18h -> 20h59 : sunset		        (100,100,100)	->	(40, 40, 70 )
            21h -> 2h59 : night					(40,40,70)
        */

        const Engine::Core::Maths::Vec4 morningSunLightColor     {0.4f, 0.4f, 0.4f, 1.f};        //Sun light color at the evening
        const Engine::Core::Maths::Vec4 midDaySunLightColor      {0.7f, 0.7f, 0.7f, 1.f};           //Sun light color at the middday
        const Engine::Core::Maths::Vec4 eveningSunLightColor     {0.4f, 0.4f, 0.4f, 1.f};        //Sun light color at the morning
        const Engine::Core::Maths::Vec4 nightColor               {0.15f, 0.15f, 0.27f, 0.6f};     //Sun light color at the night

        Engine::LowRenderer::DirectionnalLight* _pSunDirectionnalLight;

        public:

        DayNightCycle(Engine::Ressources::GameObject &gameObject,   float dayStart, 
                                                                    float dayDuration,
                                                                    float sunRiseDuration,
                                                                    float nightStart, 
                                                                    float nightDuration,
                                                                    float sunSetDuration,
                                                                    float currentTime)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _dayStart                                   {dayStart},
                _dayDuration                                {dayDuration},
                _sunRiseDuration                            {sunRiseDuration},
                _nightStart                                 {nightStart},
                _nightDuration                              {nightDuration},
                _sunSetDuration                             {sunSetDuration},
                _currentTime                                {std::fmod(currentTime, dayDuration + nightDuration)},
                _midDay                                     {_dayStart + _dayDuration / 2.f},
                _midNight                                   {_dayStart + _nightDuration / 2.f}
        {
            _name = __FUNCTION__;

            //TODO: check value with assert
        }

        DayNightCycle (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject}
        {
            _name = __FUNCTION__;
        }

        virtual ~DayNightCycle() = default;

        void start()override
        {   
            Engine::Ressources::GameObject* pSun = &Engine::Ressources::Scene::getCurrentScene()->getGameObject("Sun");
            GE_assertInfo(pSun != nullptr, "Game object name \"Sun\" does'nt exist on world");
            _pSunDirectionnalLight = pSun->getComponent<Engine::LowRenderer::DirectionnalLight>();
            GE_assertInfo(_pSunDirectionnalLight != nullptr, "Game object name \"Sun\" does'nt have directionnal light component");
        }

        void update() override
        {   
            _currentTime += Engine::Core::System::TimeSystem::getDeltaTime();

            if (_currentTime > _dayDuration + _nightDuration)
            {
                _currentTime -= _dayDuration + _nightDuration;
            }

            if (_currentTime < _dayStart - _sunRiseDuration) /*night*/
            {
                _pSunDirectionnalLight->setGlobalComponent(nightColor);
            }
            else if (_currentTime < _dayStart) /*sunRise*/
            {
                _pSunDirectionnalLight->setGlobalComponent(Engine::Core::Maths::Vec4::lerp(nightColor, morningSunLightColor, (_currentTime - (_dayStart - _sunRiseDuration)) / _sunRiseDuration));
            }
            else if (_currentTime < _midDay) /*light increase*/
            {
                _pSunDirectionnalLight->setGlobalComponent(Engine::Core::Maths::Vec4::lerp(morningSunLightColor, midDaySunLightColor, (_currentTime - _dayStart) / (_midDay - _dayStart)));
            }
            else if (_currentTime < _nightStart) /*light deacrease*/
            {
                _pSunDirectionnalLight->setGlobalComponent(Engine::Core::Maths::Vec4::lerp(midDaySunLightColor, eveningSunLightColor, (_currentTime - _midDay) / (_nightStart - _midDay)));
            }
            else if (_currentTime < _nightStart + _sunSetDuration) /*sunset*/
            {
                _pSunDirectionnalLight->setGlobalComponent(Engine::Core::Maths::Vec4::lerp(eveningSunLightColor, nightColor, (_currentTime - _nightStart) / _sunSetDuration));    
            }
            else /*night*/
            {
                _pSunDirectionnalLight->setGlobalComponent(nightColor);
            }
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_DAY_NIGHT_CYCLE_H