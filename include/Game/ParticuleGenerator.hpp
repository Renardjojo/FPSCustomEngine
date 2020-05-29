//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _PARTICULE_GENERATOR_H
#define _PARTICULE_GENERATOR_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/PhysicalObject.hpp"


namespace Game
{
    class ParticuleGenerator : public Engine::Core::Component::ScriptComponent
    {
        public : 

        enum class EGenerationShape
        {
            Circle,
            CircleArea,
            Sphere,
            SphereArea,
            Square,
            Cube,
            Cylinder, 
            Cone, 
            Count
        };

        struct ParticleSystemCreateArg
        {
            Engine::LowRenderer::ModelCreateArg     modelCreateArg              {};
            Engine::Physics::PhysicalObjectCreateArg physicalObjectCreateArg    {};
            EGenerationShape                        generationShape             {EGenerationShape::Sphere};
            Engine::Core::Maths::Vec3               scale                       {Engine::Core::Maths::Vec3::one};
            float                                   generationRange             {1.f};
            float                                   lifeDuration                {3.f};          //in second
            float                                   velocityEvolutionCoef       {1.f};          //if this number is under 1, the velocity will reduce each second. Else it will increase
            float                                   propulsionLenght            {10.f};         //Indicate the force of the propulsion at the creation of the particle
            float                                   spawnCountBySec             {10.f};         //in second
            float                                   spawnDelayOffSet            {0.f};          //in second
            bool                                    isBillBoard                 {false};
            bool                                    useScaledTime               {true};
            bool                                    instanteParticuleGeneration {false};         //Do not considere the time. Spawn "spawnCountBySec" directely
            bool                                    stopParticuleGeneration     {false};
        };

        private:

        Engine::LowRenderer::ModelCreateArg     _modelCreateArg             {};
        Engine::Physics::PhysicalObjectCreateArg _physicalObjectCreateArg   {};
        EGenerationShape                        _generationShape            {EGenerationShape::Sphere};
        Engine::Core::Maths::Vec3               _scale                      {Engine::Core::Maths::Vec3::one};
        size_t                                  _particleCount              {0};
        float                                   _generationRange            {1.f};
        float                                   _lifeDuration               {3.f};          //in second
        float                                   _velocityEvolutionCoef      {1.f};          //if this number is under 1, the velocity will reduce each second. Else it will increase
        float                                   _propulsionLenght           {10.f};         //Indicate the force of the propulsion at the creation of the particle
        float                                   _spawnCountBySec            {10.f};         //in second
        float                                   _delayCount                 {0.f};          //in second      
        bool                                    _isBillBoard                {false};
        bool                                    _useScaledTime              {true};
        bool                                    _instanteParticuleGeneration{false};         //Do not considere the time. Spawn "spawnCountBySec" directely
        bool                                    _stopParticuleGeneration    {false};
        
        protected :

        Engine::Core::Maths::Vec3 generatePosition();
        void addComponents(Engine::Ressources::GameObject& particleSystemGO);

        public:

        ParticuleGenerator(Engine::Ressources::GameObject &gameObject, const ParticleSystemCreateArg& arg);

        ParticuleGenerator (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~ParticuleGenerator() = default;

        void update() override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

} //namespace Game

#endif //_PARTICULE_GENERATOR_H