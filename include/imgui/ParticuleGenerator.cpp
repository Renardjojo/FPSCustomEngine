#include "Game/ParticuleGenerator.hpp"

#include "Game/LifeDuration.hpp"

#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/LowRenderer/billBoard.hpp"
#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Ressources/ressourcesManager.hpp"

#include <limits>

using namespace Game;
using namespace Engine::Physics;
using namespace Engine::Ressources;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;

ParticuleGenerator::ParticuleGenerator(GameObject &gameObject, const ParticleSystemCreateArg& arg)
    :   Engine::Core::Component::ScriptComponent    {gameObject},
        _modelCreateArg                             {arg.modelCreateArg},
        _physicalObjectCreateArg                    {arg.physicalObjectCreateArg},
        _generationShape                            {arg.generationShape},
        _scale                                      {arg.scale},
        _particleCount                              {0},
        _generationRange                            {arg.generationRange},
        _lifeDuration                               {arg.lifeDuration},
        _velocityEvolutionCoef                      {arg.velocityEvolutionCoef},
        _propulsionLenght                           {arg.propulsionLenght},
        _spawnCountBySec                            {1 / arg.spawnCountBySec},
        _delayCount                                 {std::fmod(arg.spawnCountBySec, arg.spawnCountBySec)},
        _isBillBoard                                {arg.isBillBoard},
        _useScaledTime                              {arg.useScaledTime}
{
    _name = __FUNCTION__;
}

ParticuleGenerator::ParticuleGenerator (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
    :   Engine::Core::Component::ScriptComponent    {refGameObject},
        _modelCreateArg                             {&t_RessourcesManager::getRessourceManagerUse()->get<Shader>(params[0]),
                                                    &t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Material>>(params[1]),
                                                    &t_RessourcesManager::getRessourceManagerUse()->get<Mesh>(params[2]),
                                                    params[0], params[1], params[2], std::stof(params[3]), std::stof(params[4]), std::stof(params[5])},
        _physicalObjectCreateArg                    {   std::stof(params[6]), std::stof(params[7]), std::stof(params[8]),
                                                        std::stof(params[9]), std::stof(params[10]), std::stof(params[11]),
                                                        std::stof(params[12]), std::stof(params[13]), std::stof(params[14]), std::stof(params[15])},
        _generationShape                            {static_cast<EGenerationShape>(std::stoi(params[16]))},
        _scale                                      {{std::stof(params[17]), std::stof(params[18]), std::stof(params[19])}},
        _particleCount                              {std::stof(params[20])},
        _generationRange                            {std::stof(params[21])},
        _lifeDuration                               {std::stof(params[22])},
        _velocityEvolutionCoef                      {std::stof(params[23])},
        _propulsionLenght                           {std::stof(params[24])},
        _spawnCountBySec                            {std::stof(params[25])},
        _delayCount                                 {std::stof(params[26])},
        _isBillBoard                                {std::stof(params[27])},
        _useScaledTime                              {std::stof(params[28])}
{
    _name = __FUNCTION__;
}

void ParticuleGenerator::update()
{   
    _delayCount += _useScaledTime ? Engine::Core::System::TimeSystem::getDeltaTime() : Engine::Core::System::TimeSystem::getUnscaledDeltaTime();

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

    for (auto &&i : _gameObject.children)
    {
        PhysicalObject& physicalObjComp = *(*i).getComponent<PhysicalObject>();
        physicalObjComp.setVelocity(physicalObjComp.getVelocity() * _velocityEvolutionCoef);
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
        returnValue.xy = {Random::peripheralCircularCoordinate(Vec2::zero, _generationRange)};
        break;

        case EGenerationShape::CircleArea :
        returnValue.xy = {Random::circularCoordinate({Vec3::zero.x, Vec3::zero.y}, _generationRange)};
        break;

        case EGenerationShape::Sphere :
        returnValue =  Random::peripheralSphericalCoordinate(Vec3::zero, _generationRange);
        break;        

        case EGenerationShape::SphereArea :
        returnValue =  Random::sphericalCoordinate(Vec3::zero, _generationRange);
        break;    

        case EGenerationShape::Square :
        returnValue.xy = {Random::peripheralSquareCoordinate({Vec3::zero.x, Vec3::zero.y}, _generationRange, _generationRange)};
        break;     

        case EGenerationShape::Cube :
        returnValue = {Random::peripheralCubiqueCoordinate(Vec3::zero, _generationRange, _generationRange, _generationRange)}; 
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
        particleSystemGO.addComponent<BillBoard>(_modelCreateArg);
    }
    else
    {
        particleSystemGO.addComponent<Model>(_modelCreateArg);
    }

    /*Life duration script component*/
    if (_lifeDuration > std::numeric_limits<float>::epsilon())
    {
        particleSystemGO.addComponent<LifeDuration>(_lifeDuration, _useScaledTime);
    }

    /*Physical object script component*/
    PhysicalObject& physicalObject = particleSystemGO.addComponent<PhysicalObject>(_physicalObjectCreateArg);

    switch (_generationShape)
    {
        case EGenerationShape::SphereArea :
        case EGenerationShape::Sphere :
        physicalObject.addForce(Random::unitPeripheralSphericalCoordonate() * _propulsionLenght);
        break;                        

        default:
        physicalObject.addForce((_gameObject.getGlobalPosition() - particleSystemGO.getGlobalPosition()).normalize() * _propulsionLenght);
        break;
    }
}

void ParticuleGenerator::save(xml_document<>& doc, xml_node<>* nodeParent) 
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    /*Model struct*/
    newNode->append_attribute(doc.allocate_attribute("shaderName", doc.allocate_string(_modelCreateArg.shaderName.c_str())));
    newNode->append_attribute(doc.allocate_attribute("materialName", doc.allocate_string(_modelCreateArg.materialName.c_str())));
    newNode->append_attribute(doc.allocate_attribute("meshName", doc.allocate_string(_modelCreateArg.meshName.c_str())));

    newNode->append_attribute(doc.allocate_attribute("loadInGPU", doc.allocate_string(std::to_string(_modelCreateArg.loadInGPU).c_str())));
    newNode->append_attribute(doc.allocate_attribute("enableBackFaceCulling", doc.allocate_string(std::to_string(_modelCreateArg.enableBackFaceCulling).c_str())));
    newNode->append_attribute(doc.allocate_attribute("isOpaque", doc.allocate_string(std::to_string(_modelCreateArg.isOpaque).c_str())));


    /*Physical Object struct*/
    newNode->append_attribute(doc.allocate_attribute("mass", doc.allocate_string(std::to_string(_physicalObjectCreateArg.mass).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeTrX", doc.allocate_string(std::to_string(_physicalObjectCreateArg.freezeTrX).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeTrZ", doc.allocate_string(std::to_string(_physicalObjectCreateArg.freezeTrZ).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeTrY", doc.allocate_string(std::to_string(_physicalObjectCreateArg.freezeTrY).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeRotX", doc.allocate_string(std::to_string(_physicalObjectCreateArg.freezeRotX).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeRotY", doc.allocate_string(std::to_string(_physicalObjectCreateArg.freezeRotY).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeRotZ", doc.allocate_string(std::to_string(_physicalObjectCreateArg.freezeRotZ).c_str())));
    newNode->append_attribute(doc.allocate_attribute("isKinematic", doc.allocate_string(std::to_string(_physicalObjectCreateArg.isKinematic).c_str())));
    newNode->append_attribute(doc.allocate_attribute("useGravity", doc.allocate_string(std::to_string(_physicalObjectCreateArg.useGravity).c_str())));
    newNode->append_attribute(doc.allocate_attribute("sleep", doc.allocate_string(std::to_string(_physicalObjectCreateArg.sleep).c_str())));

    /*Other*/
    newNode->append_attribute(doc.allocate_attribute("generationShape", doc.allocate_string(std::to_string(static_cast<int>(_generationShape)).c_str())));

    newNode->append_attribute(doc.allocate_attribute("scaleX", doc.allocate_string(std::to_string(_scale.e[0]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("scaleY", doc.allocate_string(std::to_string(_scale.e[1]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("scaleZ", doc.allocate_string(std::to_string(_scale.e[2]).c_str())));

    newNode->append_attribute(doc.allocate_attribute("particleCount", doc.allocate_string(std::to_string(_particleCount).c_str())));
    newNode->append_attribute(doc.allocate_attribute("generationRange", doc.allocate_string(std::to_string(_generationRange).c_str())));
    newNode->append_attribute(doc.allocate_attribute("lifeDuration", doc.allocate_string(std::to_string(_lifeDuration).c_str())));
    newNode->append_attribute(doc.allocate_attribute("velocityEvolutionCoef", doc.allocate_string(std::to_string(_velocityEvolutionCoef).c_str())));
    newNode->append_attribute(doc.allocate_attribute("propulsionLenght", doc.allocate_string(std::to_string(_propulsionLenght).c_str())));
    newNode->append_attribute(doc.allocate_attribute("spawnCountBySec", doc.allocate_string(std::to_string(_spawnCountBySec).c_str())));
    newNode->append_attribute(doc.allocate_attribute("delayCount", doc.allocate_string(std::to_string(_delayCount).c_str())));
    newNode->append_attribute(doc.allocate_attribute("isBillBoard", doc.allocate_string(std::to_string(_isBillBoard).c_str())));
    newNode->append_attribute(doc.allocate_attribute("useScaledTime", doc.allocate_string(std::to_string(_useScaledTime).c_str())));

    nodeParent->append_node(newNode);
}