//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_INTERFACE_Model_H
#define _GE_INTERFACE_Model_H

#include "vec.hpp"
#include "entity.hpp"

namespace Engine::LowRenderer
{
    class IModel
        : public Entity
    {
        public:
    
            #pragma region constructor/destructor

            IModel ()                                        = default;    
            IModel ( Engine::Core::Maths::Vec3 position, 
                    Engine::Core::Maths::Vec3 rotation, 
                    Engine::Core::Maths::Vec3 scale, 
                    const char* name)
                    :   Entity          (position, rotation, scale, name),
                        isLoadInGPU_    (false)
                    {}



            IModel (const IModel& other)  = default;
            virtual ~IModel ()           = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            /**
             * @brief Draw element only if it is load in GPU
             * 
             */
            virtual void draw () const noexcept    = 0;

            /**
             * @brief Load texture and Mesh from CPU to GPU. This operation can be slow.
             * 
             */            
            virtual void loadInGPU()      noexcept  = 0;
            virtual void unloadFromGPU()  noexcept  = 0;

            /**
             * @brief return true if Texture is load in GPU and ready to use
             * 
             * @return true 
             * @return false 
             */
            bool isLoadInGPU () const noexcept { return isLoadInGPU_; }
            
            #pragma endregion //!methods

            protected : 

                #pragma region attribut

                bool                               isLoadInGPU_;

                #pragma endregion //!attribut
    };
}

#endif // _GE_INTERFACE_Model_H