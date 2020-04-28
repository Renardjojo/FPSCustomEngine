//Project : Engine
//Editing by Six Jonathan
//Date : 21/01/2020 - 10 h 37

#ifndef _GE_MATERIAL_H
#define _GE_MATERIAL_H

#include "texture.hpp"
#include "type.hpp"
#include "shader_type.hpp"

#include <memory>

namespace Engine::Resources
{
    struct MaterialAndTextureCreateArg
    {
        std::string         name_;
        MaterialComponent   comp_       = { {1.f, 1.f, 1.f, 0.f}, 
                                            {1.f, 1.f, 1.f, 1.f}, 
                                            {1.f, 1.f, 1.f, 1.f}, 1.f};
        const char*         pathDiffuseTexture ;
        E_WrapType          wrapType = E_WrapType::REPEAT;
        E_FilterType        filterType = E_FilterType::LINEAR;
        bool                flipTexture = true;
        bool                loadInGPU   = true;
    };

    struct MaterialCreateArg
    {
        std::string             name;
        MaterialComponent       comp       = { {1.f, 1.f, 1.f, 0.f}, 
                                            {1.f, 1.f, 1.f, 1.f}, 
                                            {1.f, 1.f, 1.f, 1.f}, 1.f};
        std::unique_ptr<Texture> pTexture;
    };
    
    class Material
    {
        public:
    
            #pragma region constructor/destructor
    
            Material ()							= default;
            Material (std::string name_, MaterialComponent comp);

            Material (const MaterialAndTextureCreateArg& arg);
            Material (MaterialCreateArg& arg);

            /**
             * @brief Construct a new Material object after parse mtl files
             * 
             * @param arg 
             */
            Material (const MaterialAttrib& arg);

            Material (const Material& other)	= default;
            Material (Material&& other)			= default;
            virtual ~Material ()				= default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods
            #pragma endregion //!methods
    
            #pragma region static methods
            #pragma endregion //!static methods
    
            #pragma region accessor

            /**
             * @brief Get the Material Component object
             * 
             * @return const MaterialComponent& 
             */
            const MaterialComponent& getMaterialComponent () const noexcept { return comp_; }

            /**
             * @brief Get the pointor to diffuse texture object
             * 
             * @return const Texture* 
             */
            Texture* getPDiffuseTexture () noexcept { return pDiffuseTexturetexture_.get(); }

            /**
             * @brief Get the Name object
             * 
             * @return const std::string& 
             */
            const std::string& getName() const noexcept { return name_;}

            #pragma endregion //!accessor
    
            #pragma region mutator
            #pragma endregion //!mutator
    
            #pragma region operator
            #pragma endregion //!operator
    
            #pragma region convertor
            #pragma endregion //!convertor
    
        protected:
    
            #pragma region attribut

            MaterialComponent           comp_;
            std::unique_ptr<Texture>    pDiffuseTexturetexture_;
            std::string                 name_;

            #pragma endregion //!attribut
    
            #pragma region static attribut
            #pragma endregion //! static attribut
    
            #pragma region methods
            #pragma endregion //!methods
    };
}

#endif // _GE_MATERIAL_H