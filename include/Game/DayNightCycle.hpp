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
#include "GE/Ressources/type.hpp"

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
        float    _sunDistance           {100.f};

        /*  Day recap :
            3h -> 5h59 : sunrise		        (40,40,70)		->	(100,100,100)
            6h -> 11h59 : light increase	    (100,100,100)	->	(255,255,255)
            12h -> 17h59 : light deacrease		(255,255,255)	->	(100,100,100)
            18h -> 20h59 : sunset		        (100,100,100)	->	(40, 40, 70 )
            21h -> 2h59 : night					(40,40,70)
        */

        const Engine::Ressources::AmbiantComponent morningSunLightColorAmbiante    {1.f, 1.f, 1.f, 0.3f};        //Sun light color at the evening
        const Engine::Ressources::AmbiantComponent midDaySunLightColorAmbiante     {1.f, 1.f, 1.f, 0.1f};           //Sun light color at the middday
        const Engine::Ressources::AmbiantComponent eveningSunLightColorAmbiante    {1.f, 1.f, 1.f, 0.3f};        //Sun light color at the morning
        const Engine::Ressources::AmbiantComponent nightColorAmbiante              {0.54f, 0.54f, 1.f, 0.4f};     //Sun light color at the night

        const Engine::Ressources::DiffuseComponent morningSunLightColorDiffuse     {1.f, 1.f, 1.f, 0.1f};        //Sun light color at the evening
        const Engine::Ressources::DiffuseComponent midDaySunLightColorDiffuse      {1.f, 1.f, 1.f, .8f};           //Sun light color at the middday
        const Engine::Ressources::DiffuseComponent eveningSunLightColorDiffuse     {1.f, 1.f, 1.f, 0.1f};        //Sun light color at the morning
        const Engine::Ressources::DiffuseComponent nightColorDiffuse               {0.54f, 0.54f, 1.f, 0.f};     //Sun light color at the night

        const Engine::Ressources::SpecularComponent morningSunLightColorSpecular   {1.f, 1.f, 1.f, 0.1f};        //Sun light color at the evening
        const Engine::Ressources::SpecularComponent midDaySunLightColorSpecular    {1.f, 1.f, 1.f, 1.f};           //Sun light color at the middday
        const Engine::Ressources::SpecularComponent eveningSunLightColorSpecular   {1.f, 1.f, 1.f, 0.1f};        //Sun light color at the morning
        const Engine::Ressources::SpecularComponent nightColorSpecular             {0.54f, 0.54f, 1.f, 0.f};     //Sun light color at the night

        Engine::LowRenderer::DirectionnalLight* _pSunDirectionnalLight;

        public:

        DayNightCycle(Engine::Ressources::GameObject &gameObject,   float dayStart, 
                                                                    float dayDuration,
                                                                    float sunRiseDuration,
                                                                    float nightStart, 
                                                                    float nightDuration,
                                                                    float sunSetDuration,
                                                                    float currentTime,
                                                                    float sunDistance)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _dayStart                                   {dayStart},
                _dayDuration                                {dayDuration},
                _sunRiseDuration                            {sunRiseDuration},
                _nightStart                                 {nightStart},
                _nightDuration                              {nightDuration},
                _sunSetDuration                             {sunSetDuration},
                _currentTime                                {std::fmod(currentTime, dayDuration + nightDuration)},
                _midDay                                     {_dayStart + _dayDuration / 2.f},
                _midNight                                   {_dayStart + _nightDuration / 2.f},
                _sunDistance                                {sunDistance}
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

            /*move light and sun*/
            Engine::Core::Maths::Vec3 newDirection;
            
            float rot = (_currentTime - _dayStart) * (M_PI * 2.f) / (_dayDuration + _nightDuration);
            newDirection = {cosf(rot), sinf(rot), 0.f};
            std::cout << "rot : " << rot* 180.f / M_PI << " " << _currentTime << std::endl; 

            _pSunDirectionnalLight->getGameObject().setTranslation(newDirection * _sunDistance);
            _pSunDirectionnalLight->setDirection(newDirection);

            /*Manage luminosity and color temperatur*/
            if (_currentTime < _dayStart - _sunRiseDuration) /*night*/
            {
                _pSunDirectionnalLight->setAmbient(nightColorAmbiante);
                _pSunDirectionnalLight->setDiffuse(nightColorDiffuse);
                _pSunDirectionnalLight->setSpecular(nightColorSpecular);
            }
            else if (_currentTime < _dayStart) /*sunRise*/
            {
                float t = (_currentTime - (_dayStart - _sunRiseDuration)) / _sunRiseDuration;
                _pSunDirectionnalLight->setAmbient(Engine::Core::Maths::Vec4::lerp(nightColorAmbiante.rgbi, morningSunLightColorAmbiante.rgbi, t));
                _pSunDirectionnalLight->setDiffuse(Engine::Core::Maths::Vec4::lerp(nightColorDiffuse.rgbi, morningSunLightColorDiffuse.rgbi, t));
                _pSunDirectionnalLight->setSpecular(Engine::Core::Maths::Vec4::lerp(nightColorSpecular.rgbi, morningSunLightColorSpecular.rgbi, t));
            }
            else if (_currentTime < _midDay) /*light increase*/
            {
                float t = (_currentTime - _dayStart) / (_midDay - _dayStart);
                _pSunDirectionnalLight->setAmbient(Engine::Core::Maths::Vec4::lerp(morningSunLightColorAmbiante.rgbi, midDaySunLightColorAmbiante.rgbi, t));
                _pSunDirectionnalLight->setDiffuse(Engine::Core::Maths::Vec4::lerp(morningSunLightColorDiffuse.rgbi, midDaySunLightColorDiffuse.rgbi, t));
                _pSunDirectionnalLight->setSpecular(Engine::Core::Maths::Vec4::lerp(morningSunLightColorSpecular.rgbi, midDaySunLightColorSpecular.rgbi, t));
            }
            else if (_currentTime < _nightStart) /*light deacrease*/
            {
                float t = (_currentTime - _midDay) / (_nightStart - _midDay);
                _pSunDirectionnalLight->setAmbient(Engine::Core::Maths::Vec4::lerp(midDaySunLightColorAmbiante.rgbi, eveningSunLightColorAmbiante.rgbi, t));
                _pSunDirectionnalLight->setDiffuse(Engine::Core::Maths::Vec4::lerp(midDaySunLightColorDiffuse.rgbi, eveningSunLightColorDiffuse.rgbi, t));
                _pSunDirectionnalLight->setSpecular(Engine::Core::Maths::Vec4::lerp(midDaySunLightColorSpecular.rgbi, eveningSunLightColorSpecular.rgbi, t));
            }
            else if (_currentTime < _nightStart + _sunSetDuration) /*sunset*/
            {
                float t = (_currentTime - _nightStart) / _sunSetDuration;  
                _pSunDirectionnalLight->setAmbient(Engine::Core::Maths::Vec4::lerp(eveningSunLightColorAmbiante.rgbi, nightColorAmbiante.rgbi, t));
                _pSunDirectionnalLight->setDiffuse(Engine::Core::Maths::Vec4::lerp(eveningSunLightColorDiffuse.rgbi, nightColorDiffuse.rgbi, t));
                _pSunDirectionnalLight->setSpecular(Engine::Core::Maths::Vec4::lerp(eveningSunLightColorSpecular.rgbi, nightColorSpecular.rgbi, t));
            }
            else /*night*/
            {
                _pSunDirectionnalLight->setAmbient(nightColorAmbiante);
                _pSunDirectionnalLight->setDiffuse(nightColorDiffuse);
                _pSunDirectionnalLight->setSpecular(nightColorSpecular);
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