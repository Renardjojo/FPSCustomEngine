#include "GE/Resources/texture.hpp"
#include "glad/glad.h"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Parsers/parserTool.hpp"
#include "GE/Core/Debug/assert.hpp"
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "GE/Core/Parsers/stb_image.h"

using namespace Engine::Core::Debug;
using namespace Engine::Resources;

/*
Texture::Texture(const Texture &other)
{
	// create new surface. Const cast : SDL do not integer const value for src
	//surface_ = SDL_ConvertSurface(const_cast<SDL_Surface*>(other.getSurface()), other.getSurface()->format, SDL_SWSURFACE);

	//generate new id
    glGenTextures(1, &id_);
}*/
/*
Texture::Texture(unsigned char* pixels, 
                const std::string& name,
                Size            size,
                unsigned int    component,
                E_FilterType    filterType = E_FilterType::LINEAR,
                E_WrapType      wrapType   = E_WrapType::REPEAT,
                bool            flipTexture = true,
                bool            loadInGPU   = true)
    :   pixels_     (pixels_),
        name_       (name),
        w_          (size.width),
        h_          (size.heigth),
        comp_       (component),
        filterType_ (filterType_),
        wrapType_   (wrapType_)

{
    GE_assert(pixels != nullptr);
    GE_assert(component != 0);

    if (flipTexture)
    {
        hFlip();
    }

    if (loadInGPU)
        Texture::loadInGPU();
}
*/

Texture::Texture(const char* path, bool flipTexture, bool loadInGPU)
    :   id_    (0),
        isLoadInGPU_ (false)
{
    filterType_ = E_FilterType::LINEAR;
    wrapType_ = E_WrapType::REPEAT;
    
    stbi_set_flip_vertically_on_load(flipTexture); 
	pixels_ = stbi_load(path, &w_, &h_, (int*)&comp_, 0);

	if (pixels_ == nullptr)
	{
		functError((std::string("STBI canno't load image : ") + path).c_str());
		return;
	}

    parseNameInPath(path);

    if (loadInGPU)
        Texture::loadInGPU();
}

Texture::Texture(const TextureCreateArg& arg)
    :   id_    (0),
        isLoadInGPU_ (false)
{
    filterType_ = arg.filterType;
    wrapType_ = arg.wrapType;

    stbi_set_flip_vertically_on_load(arg.flipTexture); 
	pixels_ = stbi_load(arg.path, &w_, &h_, (int*)&comp_, 0);

	if (pixels_ == nullptr)
	{
		functError((std::string("STBI canno't load image : ") + arg.path).c_str());
		return;
	}

    parseNameInPath(arg.path);

    if (arg.loadInGPU)
        Texture::loadInGPU();
}

Texture::~Texture()
{
	stbi_image_free(pixels_);

    if (isLoadInGPU_)
        unloadFromGPU();
        
}

void Texture::loadInGPU ()
{
    if (isLoadInGPU_)
    {
        functWarning((std::string("Texture name's : ") + name_.c_str() + " already load in GPU").c_str());
        return;
    }

    glGenTextures   (1, &id_);
	glBindTexture	(GL_TEXTURE_2D, id_);
    functGlCheckAndLogError();

	if(comp_ == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w_, h_, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels_);
	else if(comp_ == 4)
    {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_, h_, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_);
    }
    else
    {
        functWarning(std::string("Texture component unsuppported with component : ") + std::to_string(comp_));   
        exit(1);
    }
    

    functGlCheckAndLogError();
    
    GLint wrap, filter;

    switch (wrapType_)
    {
    case E_WrapType::CLAMP_TO_BORDER :
        wrap = GL_CLAMP_TO_BORDER;
        break;

    case E_WrapType::CLAMP_TO_EDGE :
        wrap = GL_CLAMP_TO_EDGE;
        break;

    case E_WrapType::MIRRORED_REPEAT :
        wrap = GL_MIRRORED_REPEAT;
        break;

    case E_WrapType::REPEAT :
        wrap = GL_REPEAT;
        break;

    default:
        wrap = GL_CLAMP_TO_EDGE;
        break;
    }

    switch (filterType_)
    {
    case E_FilterType::LINEAR :
        filter = GL_LINEAR;
        break;

    case E_FilterType::NEAREST :
        filter = GL_NEAREST;
        break;

    default:
        filter = GL_LINEAR;
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    functGlCheckAndLogError();

	glBindTexture(GL_TEXTURE_2D, 0);
    isLoadInGPU_ = true;

    SLog::log((std::string("Texture ") + name_.c_str() + " load in GPU").c_str());
}

void Texture::unloadFromGPU ()
{
    if (!isLoadInGPU_)
    {
        functWarning((std::string("Texture name's : ") + name_.c_str() + " isn't load in GPU").c_str());
        return;
    }

    glDeleteTextures(1, &id_);
    functGlCheckAndLogError();
    isLoadInGPU_ = false;    
}

void Texture::hFlip()
{
    int wComp = w_ * comp_;

    for(int i = 0; i < h_ / 2; i++)
    {
        for(int j = 0; j < wComp; j++)
        {
            std::swap(	pixels_[(wComp * (h_ - 1 - i)) + j], pixels_[(wComp * i) + j]);
        }
    }
}

void Texture::use () const noexcept
{  
    if (!isLoadInGPU()) 
    {
        GE_assert ("Try to use texture not load in GPU");
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    functGlCheckAndLogError();
}
/*
Texture& Texture::operator=(const Texture &other)
{
    if(glIsTexture(id_) == GL_TRUE)
	{
		stbi_image_free(pixels_);
		glDeleteTextures(1, &id_);
	}

	// create new surface. Const cast : SDL do not integer const value for src
	//surface_ = SDL_ConvertSurface(const_cast<SDL_Surface*>(other.getSurface()), other.getSurface()->format, SDL_SWSURFACE);

	//generqte new id
    glGenTextures(1, &id_);

    return *this;
}*/