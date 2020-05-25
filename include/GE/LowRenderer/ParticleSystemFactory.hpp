//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 10 h 15

#ifndef __PARTICLE_SYSTEM_FACTORY_HPP__
#define __PARTICLE_SYSTEM_FACTORY_HPP__

#include "GE/Ressources/GameObject.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Core/Maths/vec.hpp"

#include <string>

namespace Engine::LowRenderer
{
    class ParticleSystemFactory
    {
        private:

        protected:

        #pragma region attribut
        #pragma endregion //!attribut

        #pragma region static attribut
        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        public:

        #pragma region constructor/destructor

        ParticleSystemFactory ()					                            = delete;
        ParticleSystemFactory (const ParticleSystemFactory& other)			    = delete;
        ParticleSystemFactory (ParticleSystemFactory&& other)				    = delete;
        virtual ~ParticleSystemFactory ()				                        = delete;
        ParticleSystemFactory& operator=(ParticleSystemFactory const& other)	= delete;
        ParticleSystemFactory& operator=(ParticleSystemFactory && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        /**
         * @brief Create a Particle System object
         * 
         * @param parent 
         * @param arg 
         * @return Engine::Ressources::GameObject& of the decale. Do not return the parent but the decale gamobject create inside.
         */
        static Engine::Ressources::GameObject& createDecale(Engine::Ressources::GameObject& parent, const Engine::Ressources::GameObjectCreateArg& gameObjectPrefabs, const ModelCreateArg& modelPrefabs, const Engine::Core::Maths::Vec3& normal)
        {
            Engine::Ressources::GameObject& decaleGO = parent.addChild<Engine::Ressources::GameObject>(gameObjectPrefabs);
            decaleGO.addComponent<Model>(modelPrefabs);
            decaleGO.setVecUp(normal);
            return decaleGO;
        }


        #pragma endregion //!static methods
    };

} /*namespace Engine::LowRenderer*/

#endif // __PARTICLE_SYSTEM_FACTORY_HPP__
