//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-26 - 15 h 51

#ifndef __CHECKPOINT_HPP__
#define __CHECKPOINT_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"

namespace Game
{
    class Checkpoint : public Engine::Core::Component::ScriptComponent
    {
        
    private:

        std::vector<Engine::Core::Maths::Vec3> checkpoints;

    public:
        Checkpoint(Engine::Ressources::GameObject &gameObject);
        virtual ~Checkpoint() = default;

        void start() override;
        void update() override;
        void fixedUpdate() override;

        void addCheckpoint(Engine::Core::Maths::Vec3 newPos);

        Engine::Core::Maths::Vec3 getCheckpoint(unsigned int cpNumber);

        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

    class CheckpointManager
    {
    public:
        Checkpoint* _checkpoints                {nullptr};
        Engine::Core::Maths::Vec3 _targetPos    {Engine::Core::Maths::Vec3::zero};
        unsigned int _cpNumber                  {0};

        CheckpointManager()
        {
            _checkpoints = nullptr;
        }

        CheckpointManager(Checkpoint* checkpoints, Engine::Core::Maths::Vec3 targetPos, unsigned int cpNumber)
        {
            _checkpoints = checkpoints;
            _targetPos = targetPos;
            _cpNumber = cpNumber;
        }
        CheckpointManager(Checkpoint* checkpoints)
        {
            _checkpoints = checkpoints;
            nextCheckPoint();
        }
        virtual ~CheckpointManager() = default;


        void nextCheckPoint()
        {
            _targetPos = _checkpoints->getCheckpoint(_cpNumber++);
        }
    };

} // namespace Game

#endif // __CHECKPOINT_HPP__
