//Project : Engine
//Editing by Six Jonathan
//Date : 09/01/2020 - 16 h 44

#include <string>
#include "GE/Resources/type.hpp"

#ifndef _GE_TEXTURE_H
#define _GE_TEXTURE_H

namespace Engine::Resources
{
    #define PATH_TEXTURE_RESSORUCE "./resources/texture/"

    enum class E_WrapType
    {
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        REPEAT
    };

    enum class E_FilterType
    {
        NEAREST,
        LINEAR
    };

    struct TextureCreateArg
    {
        const char* path;
        E_WrapType        wrapType = E_WrapType::REPEAT;
        E_FilterType      filterType = E_FilterType::LINEAR;
        bool flipTexture = true;
        bool loadInGPU = true;
    };

    class Texture
    {
        public:

        Texture()	                    = default;
        Texture(const Texture &other)   = delete; //TODO:;
        Texture(Texture && other)       = default; //TODO:;
/*
        Texture(unsigned char* pixels, 
                const std::string& name,
                Size            size,
                unsigned int    component,
                E_FilterType    filterType  = E_FilterType::LINEAR,
                E_WrapType      wrapType    = E_WrapType::REPEAT,
                bool            flipTexture = true,
                bool            loadInGPU   = true);*/

        Texture(const char* path, bool flipTexture = true, bool loadInGPU = true);
        Texture(const TextureCreateArg& arg);
        virtual ~Texture();

        unsigned int	        getID		() const 	{return id_;}	
        const unsigned char* 	getPixels	() const 	{return pixels_;} //read only
        unsigned char* 		    getPixels	() 			{return pixels_;} //read/write

        /**
         * @brief Load texture and Mesh from CPU to GPU. This operation can be slow but display element more faster.
         * 
         */
        virtual
        void                    loadInGPU ();
        
        void                    unloadFromGPU ();

        /**
         * @brief return true if texture is load in GPU and ready to use
         * 
         * @return true 
         * @return false 
         */
        bool isLoadInGPU () const noexcept { return isLoadInGPU_; }

        /**
         * @brief bind texture to openGL to be use for the next drawing
         * 
         */
        void use () const noexcept;

        const std::string& getName() const noexcept {return name_;}

        Size getSize() const noexcept {return {w_, h_};}

        //Texture& operator=(Texture const &other); //TODO:
        protected:

        unsigned int	id_;
        bool            isLoadInGPU_;

        std::string     name_;
        unsigned char*	pixels_;
        int             w_, h_; 
        unsigned char   comp_; //RGB = 3 / RGBA = 4
        E_FilterType    filterType_;
        E_WrapType      wrapType_;

        /**
         * @brief inverse pixels of image
         * 
         * @param char 
         */
        void hFlip();
        
    };
}

#endif // _GE_TEXTURE_H