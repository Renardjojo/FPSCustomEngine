#include "Game/ParticuleGenerator.hpp"

#include "Game/LifeDuration.hpp"

#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/LowRenderer/billBoard.hpp"
#include "GE/Physics/ColliderShape/SphereCollider.hpp"

#include <limits>

using namespace Game;
using namespace Engine::Physics;
using namespace Engine::Ressources;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;

ParticuleGenerator::ParticuleGenerator(GameObject &gameObject, const ParticleSystemCreateArg& arg)
    :   Engine::Core::Component::ScriptComponent    {gameObject},
        _optionnalModelCreateArg                    {arg.optionnalModelCreateArg},
        _generationShape                            {arg.generationShape},
        _scale                                      {arg.scale},
        _particleCount                              {0},
        _generationRange                            {arg.generationRange},
        _lifeDuration                               {arg.lifeDuration},
        _velocityEvolutionCoef                      {arg.velocityEvolutionCoef},
        _propulsionLenght                           {arg.propulsionLenght},
        _spawnCountBySec                            {1 / arg.spawnCountBySec},
        _delayCount                                 {std::fmod(arg.spawnCountBySec, arg.spawnCountBySec)},
        _mass                                       {arg.mass},
        _isBillBoard                                {arg.isBillBoard},
        _useGravity                                 {arg.useGravity},
        _useScaledTime                              {arg.useScaledTime}
{}

void ParticuleGenerator::update()
{   
    _delayCount += _useScaledTime ? Engine::Core::System::TimeSystem::getDeltaTime() : Engine::Core::System::TimeSystem::getUnscaledDetlaTime();

    while(_delayCount >= _spawnCountBySec)
    {
        _delayCount -= _spawnCountBySec;

        GameObjectCreateArg particleSystemGOArg {std::string("ParticleSystem") + std::to_string(_particleCount)};
        _particleCount++;
        if (_particleCount == std::numeric_limits<size_t>::max())
        {
            _particleCount = 0;
        }

        particleSystemGOArg.transformArg.scale = _scale;
        particleSystemGOArg.transformArg.position = generatePosition();
        GameObject& particleSystemGO = _gameObject.addChild<GameObject>(particleSystemGOArg);
        addComponents(particleSystemGO);
    }
}

Vec3 ParticuleGenerator::generatePosition()
{
    Vec3 returnValue = Vec3::zero;
    switch (_generationShape)
    {
        case EGenerationShape::Cone :
        case EGenerationShape::Cylinder :
        case EGenerationShape::Circle :
        returnValue.xy = {Random::peripheralCircularCoordinate({_gameObject.getPosition().x, _gameObject.getPosition().y}, _generationRange)};
        break;

        case EGenerationShape::CircleArea :
        returnValue.xy = {Random::circularCoordinate({_gameObject.getPosition().x, _gameObject.getPosition().y}, _generationRange)};
        break;

        case EGenerationShape::Sphere :
        returnValue =  Random::peripheralSphericalCoordinate(_gameObject.getPosition(), _generationRange);
        break;        

        case EGenerationShape::SphereArea :
        returnValue =  Random::sphericalCoordinate(_gameObject.getPosition(), _generationRange);
        break;    

        case EGenerationShape::Square :
        returnValue.xy = {Random::peripheralSquareCoordinate({_gameObject.getPosition().x, _gameObject.getPosition().y}, _generationRange, _generationRange)};
        break;     

        case EGenerationShape::Cube :
        returnValue = {Random::peripheralCubiqueCoordinate(_gameObject.getPosition(), _generationRange, _generationRange, _generationRange)}; 
        break;                  

        default:
        break;
    }

    return returnValue;
}

void ParticuleGenerator::addComponents(GameObject& particleSystemGO)
{
    /*Model or billboard component*/
    if (_isBillBoard)
    {
        particleSystemGO.addComponent<BillBoard>(_optionnalModelCreateArg);
    }
    else
    {
        particleSystemGO.addComponent<Model>(_optionnalModelCreateArg);
    }

    /*Life duration script component*/
    if (_lifeDuration > std::numeric_limits<float>::epsilon())
    {
        particleSystemGO.addComponent<LifeDuration>(_lifeDuration, _useScaledTime);
    }

    /*Physical object script component*/
    if (_useGravity || _propulsionLenght > std::numeric_limits<float>::epsilon())
    {
        PhysicalObject& physicalObject = particleSystemGO.addComponent<PhysicalObject>();
        physicalObject.SetMass(_mass);
        physicalObject.SetUseGravity(_useGravity);
        physicalObject.SetUseGravity(_useGravity);

        switch (_generationShape)
        {
            case EGenerationShape::SphereArea :
            case EGenerationShape::Sphere :
            physicalObject.AddForce(Random::unitPeripheralSphericalCoordonate() * _propulsionLenght);
            break;                        

            default:
            physicalObject.AddForce((_gameObject.getGlobalPosition() - particleSystemGO.getGlobalPosition()).normalize() * _propulsionLenght);
            break;
        }
    }
}