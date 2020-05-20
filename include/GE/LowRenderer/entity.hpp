//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_ENTITY_H
#define _GE_ENTITY_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/transform.hpp"
#include "GE/Core/Debug/log.hpp"
#include <string>

namespace Engine::LowRenderer
{
    typedef struct S_EntityCreateArg
    {
        std::string         name            = "";
        Engine::Physics::TransformCreateArg  transformArg    = {Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::one};
        
    } EntityCreateArg;

    class Entity
        : public Engine::Physics::Transform
    {
        public:
    
            #pragma region constructor/destructor

            Entity ()
                :   Transform   {Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::one},
                    name_       ("")
            {}

            Entity (const char* name)
                :   Transform   {Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::one},
                    name_       {name}
            {}

            Entity (const Engine::Core::Maths::Vec3& position, 
                    const Engine::Core::Maths::Vec3& rotation, 
                    const Engine::Core::Maths::Vec3& scale,
                    const char* name)
                    :   Transform   {position, rotation, scale},
                        name_       (name)
                    {}
 
            Entity (const EntityCreateArg& arg)
                    :   Transform   {arg.transformArg},
                        name_       (arg.name)
                    {}

            Entity (const Entity& other) = default;
            virtual ~Entity ()            = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            #pragma endregion //!methods

            #pragma region accessor

            /**
             * @brief Get the Name object
             * 
             * @return const char* 
             */
            inline
            std::string getName() const noexcept { return name_; }

            inline
            const char* getCName() const noexcept { return name_.c_str(); }

            #pragma endregion //!accessor
    
            #pragma region mutator

            /**
             * @brief Set the Name object
             * 
             * @param newName 
             */
            inline
            void setName(const char* newName) noexcept 
            { 
                name_ = newName; 
            }

            #pragma endregion //!mutator

            protected : 

                #pragma region attribut

                std::string                 name_;

                #pragma endregion //!attribut
    };
} //namespace Engine::LowRenderer

#endif // _GE_ENTITY_H