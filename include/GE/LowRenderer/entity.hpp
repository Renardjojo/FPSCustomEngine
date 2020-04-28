//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_ENTITY_H
#define _GE_ENTITY_H

#include "vec.hpp"
#include "transform.hpp"
#include "log.hpp"
#include <string>

namespace Engine::LowRenderer
{
    class Entity
        : public Engine::Physics::Transform
    {
        public:
    
            #pragma region constructor/destructor

            Entity ()                                        = default;    

            Entity (Engine::Core::Maths::Vec3 position, 
                    Engine::Core::Maths::Vec3 rotation, 
                    Engine::Core::Maths::Vec3 scale,
                    const char* name)
                    :   Transform   {position, rotation, scale},
                        name_       (name)
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
            const char* getName() const noexcept { return name_.c_str(); }

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
                Engine::Core::Debug::SLog::log((std::string("Entity rename from \"") + name_ + "\" to " + newName).c_str());
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