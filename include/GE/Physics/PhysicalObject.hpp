//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 11 h 38

#ifndef _PHYSICAL_OBJECT_H
#define _PHYSICAL_OBJECT_H

#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/Component.hpp"

namespace Engine::Physics
{
	struct PhysicalObjectCreateArg
	{
		float 						mass				{1};
		bool 						freezeTrX			{false};
		bool 						freezeTrY			{false};
		bool 						freezeTrZ			{false};
		bool 						freezeRotX			{false};
		bool 						freezeRotY			{false};
		bool 						freezeRotZ			{false};
		bool 						isKinematic			{false};
		bool 						useGravity			{true};
		bool 						sleep				{false};
	};

	class PhysicalObject : public Engine::Ressources::Component
	{
	public:
#pragma region constructor / destructor

		PhysicalObject(Engine::Ressources::GameObject &refGameObject);
		PhysicalObject(Engine::Ressources::GameObject &refGameObject, const PhysicalObjectCreateArg& arg);
		PhysicalObject(const PhysicalObject &other);
		PhysicalObject(PhysicalObject &&other);
		virtual ~PhysicalObject();

		PhysicalObject() = delete;
		PhysicalObject &operator=(PhysicalObject const &other) = delete;
		PhysicalObject &operator=(PhysicalObject &&other) = delete;

#pragma endregion //!constructor/destructor

#pragma region methods

		void addForce(const Engine::Core::Maths::Vec3& force) noexcept;
		void addForce(float x, float y, float z) noexcept;
		void addTorque(const Engine::Core::Maths::Vec3& force) noexcept;
		void addTorque(float x, float y, float z) noexcept;

#pragma endregion //!methods

#pragma region accessor

		Engine::Core::Maths::Vec3 getVelocity()
		{
			return _velocity;
		}

		Engine::Core::Maths::Vec3 getAngularVelocity() const noexcept { return _angularVelocity; }
		float getMass() const noexcept { return _mass; }

		bool isKinematic() const noexcept{ return _isKinematic; }
		bool useGravity() const noexcept{ return _useGravity; }
		bool isSleeping() const noexcept { return _sleep; }

#pragma endregion //!accessor

#pragma region mutator

		void setVelocity(const Engine::Core::Maths::Vec3& velocity) noexcept
		{
			_isDirty = true;
			_sleep = false;
			_velocity = velocity;
		}

		void setVelocity(float x, float y, float z) noexcept
		{
			_isDirty = true;
			_sleep = false;
			_velocity.x = x;
			_velocity.y = y;
			_velocity.z = z;
		}

		void setMass			(float mass) noexcept { _mass = mass; }
		void setIsKinematic		(bool state) noexcept { _isKinematic = state; }
		void setUseGravity		(bool state) noexcept { _useGravity = state; }
		void setFreezeTrX		(bool state) noexcept { _freezeTrX = state; }
		void setFreezeTrY		(bool state) noexcept { _freezeTrY = state; }
		void setFreezeTrZ		(bool state) noexcept { _freezeTrZ = state; }
		void setFreezeRotX		(bool state) noexcept { _freezeRotX = state; }
		void setFreezeRotY		(bool state) noexcept { _freezeRotY = state; }
		void setFreezeRotZ		(bool state) noexcept { _freezeRotZ = state; }

		void sleep() noexcept{ _sleep = true; }
		void wakeUp() noexcept{ _sleep = false; }

		void setDirtyFlag(bool state) noexcept { _isDirty = state; }
		bool isDirty() const noexcept { return _isDirty; }

		void save(xml_document<> &doc, xml_node<> *nodeParent) noexcept;

#ifndef DNEDITOR
		virtual void serializeOnEditor() noexcept override;
#endif

#pragma endregion //!mutator

	protected:
#pragma region attribut

		Engine::Core::Maths::Vec3 _velocity{0, 0, 0};
		Engine::Core::Maths::Vec3 _angularVelocity{0, 0, 0};
		float _mass{1};
		bool _freezeTrX{false};
		bool _freezeTrY{false};
		bool _freezeTrZ{false};
		bool _freezeRotX{false};
		bool _freezeRotY{false};
		bool _freezeRotZ{false};
		bool _isKinematic{false};
		bool _useGravity{true};
		bool _sleep{false};
		bool _isDirty {false};

#pragma endregion //!attribut

	private:
	};
} // namespace Engine::Physics

#endif //_PHYSICAL_OBJECT_H