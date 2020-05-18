//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_INTERFACE_Model_H
#define _GE_INTERFACE_Model_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Component/RenderingObject.hpp"

namespace Engine::LowRenderer
{
    class IModel
        : public Engine::Core::Component::RenderingObject
    {
        public:
    
            #pragma region constructor/destructor

            IModel(Engine::Ressources::GameObject &refGameObject)
            :   Engine::Core::Component::RenderingObject  (refGameObject), 
                isLoadInGPU_                              {false}
            {}

            IModel(const IModel &other)                             = default;  
            IModel(IModel &&other)                                  = default;  
            virtual ~IModel()                                       = default;

            IModel()                                                = delete;
            IModel &operator=(IModel const &other)                  = delete;
            IModel &operator=(IModel &&other)                       = delete;
        
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            /**
             * @brief Draw element only if it is load in GPU
             * 
             */
            virtual void draw () const noexcept = 0;

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

                bool isLoadInGPU_ = false;

                #pragma endregion //!attribut
    };
}

#endif // _GE_INTERFACE_Model_H