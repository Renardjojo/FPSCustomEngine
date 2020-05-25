//Project : Engine
//Editing by Six Jonathan
//Date : 13/01/2020 - 09 h 07

#ifndef _GE_Ressources_MMANAGER_H
#define _GE_Ressources_MMANAGER_H

#include <unordered_map>
#include <string>
#include <utility>
#include "GE/Core/Debug/log.hpp"

#include "GE/Ressources/ui.hpp"
#include "GE/Ressources/music.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/shader.hpp"
#include "GE/Ressources/material.hpp"
#include "GE/Ressources/texture.hpp"
#include "GE/Ressources/text.hpp"
#include "GE/Ressources/sample.hpp"
#include "GE/Ressources/font.hpp"


namespace Engine::Ressources
{


    //Technical inspired about understanding example found from stackOverFlow : https://stackoverflow.com/questions/27941661/generating-one-class-member-per-variadic-template-argument/53112843#53112843?newreg=f97a957ca4ca467bab0d0ce1cc6ea7b2

    //Variadic template class
    template<class LType, class... RType> class RessourcesManager;

    //Ressources of one type. Will be created for each differente type.
    template<class LType>
    class RessourcesManager<LType>
    {
        public:

        RessourcesManager ()                                = default;
        RessourcesManager (const RessourcesManager& other)  = delete;
        RessourcesManager (RessourcesManager&& other)       = default;
        ~RessourcesManager ()                               = default;

        protected:

        /**
         * @brief return element of index
         * 
         * @param key 
         * @return LType& 
         */
        LType& get(const std::string& key)
        {    
            //iterator of LType
            auto it = ressource_.find(key);
            if (it == ressource_.end())
            {
                Engine::Core::Debug::SLog::logError(std::string("ressource not found with key : ") + key);
                exit(1);    
            }

            return it->second;
        }

        /**
         * @brief add element with argument of constructor in parameter
         * 
         * @tparam Args 
         * @param args 
         * @return unsigned int 
         */
        template<typename ...Args>
        LType& add(std::string key, Args&&... args)
        {
            //auto for pair of iterator of LType and bool
            auto rst = ressource_.emplace(  std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(args...));

            if (rst.second == false)
            {
                Engine::Core::Debug::SLog::logError(std::string("ressource insert with same key as an element existing : ") + key);
                exit(1);
            }

            return rst.first->second;
        }

        private:

        std::unordered_map <std::string, LType> ressource_;

    };

    //Global ressource manager.
    //create a class hierarchy where each base class is a specialization for one of the types
    //For example : ressourceManager<int, float, std::string> wille create :
    //ressourceManager<int>, ressourceManager<float> and ressourceManager<std::string>.
    //ressourceManager<int, float, std::string> inherited about these 3 class and can access to each element
    
    template<class LType, class... RType>
    class RessourcesManager
        : public RessourcesManager<LType>, public RessourcesManager<RType...>
    {
        public:

            #pragma region constructor/destructor

            RessourcesManager ()                                = default;
            RessourcesManager (const RessourcesManager& other)  = delete;
            RessourcesManager (RessourcesManager&& other)       = default;
            ~RessourcesManager ()                               = default;

            #pragma endregion //!constructor/destructor

            #pragma region methods
            #pragma endregion //!methods

            #pragma region accessor

            /**
             * @brief Return ressource. Example : get<Texture>(0)
             * 
             * @tparam T 
             * @param key 
             * @return T& 
             */
            template<class T>
            T& get(const std::string& key)
            {
                return RessourcesManager<T>::get(key);
            }

            #pragma endregion //!accessor

            #pragma region mutator

            /**
             * @brief Add ressource. Example : add<Texture>(Texture())
             * 
             * @tparam T 
             * @tparam Args 
             * @param args 
             * @return unsigned int 
             */
            template<class T, typename... Args>
            T& add(const std::string& key, Args&&... args)
            {
                return RessourcesManager<T>::add(key, std::forward<Args>(args)...);
            }

            #pragma endregion //!mutator

        protected:

        private:

    };

    template<class LType, class... RType>
    class RessourcesManagerWithGlobalUsage 
        : public RessourcesManager<LType, RType...>
    {
        public:

            #pragma region constructor/destructor

            RessourcesManagerWithGlobalUsage ()                                = default;
            RessourcesManagerWithGlobalUsage (const RessourcesManagerWithGlobalUsage& other)  = delete;
            RessourcesManagerWithGlobalUsage (RessourcesManagerWithGlobalUsage&& other)       = default;
            ~RessourcesManagerWithGlobalUsage ()                               = default;

            #pragma endregion //!constructor/destructor

            #pragma region methods
            #pragma endregion //!methods

            #pragma region accessor

            void use () noexcept;

            static RessourcesManagerWithGlobalUsage<LType, RType...>* getRessourceManagerUse() noexcept;

        protected:

        static RessourcesManagerWithGlobalUsage<LType, RType...>* ressourceManagerToUse; //pointor to be in nullptr by default

        private:

    };

    template<class LType, class... RType>
    RessourcesManagerWithGlobalUsage<LType, RType...>* RessourcesManagerWithGlobalUsage<LType, RType...>::ressourceManagerToUse {nullptr};

    template<class LType, class... RType>
    RessourcesManagerWithGlobalUsage<LType, RType...>* RessourcesManagerWithGlobalUsage<LType, RType...>::getRessourceManagerUse() noexcept
    { 
        return ressourceManagerToUse; 
    }

    template<class LType, class... RType>
    void RessourcesManagerWithGlobalUsage<LType, RType...>::use () noexcept
    {
        ressourceManagerToUse = this;
    }

    
    typedef RessourcesManagerWithGlobalUsage<  Engine::Ressources::Mesh,
                                Engine::Ressources::Shader,
                                Engine::Ressources::Material,
                                Engine::Ressources::Text,
                                Engine::Ressources::Texture,
                                Engine::Ressources::Sample,
                                Engine::Ressources::Music,
                                Engine::Ressources::Font,
                                Engine::Ressources::Title,
                                Engine::Ressources::Button,
                                Engine::Ressources::Image,
                                Engine::Ressources::TextField,
                                Engine::Ressources::ReferencedTitle> t_RessourcesManager;


}// namespaceEngine::Ressources

#endif // _GE_Ressources_MMANAGER_H