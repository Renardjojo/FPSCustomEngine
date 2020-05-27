#include "GE/Ressources/text.hpp"

#include <SDL2/SDL_pixels.h>
#include "GE/Core/Debug/log.hpp"

using namespace Engine::Ressources;
using namespace Engine::Core::Debug;

Text::Text(const TextCreateArg& arg)
    :	Texture		(),
        str_		(arg.text),
        pFont_		(arg.pFont),
        color_		(arg.color),
        isSmooth_	(arg.createSmoothText)
{
    pixels_     = nullptr;
    wrapType_	= arg.wrapType;
    filterType_	= arg.filterType;
    isLoadInGPU_   = false;
    
    if (pFont_ == nullptr)
    {
        functError("pFont == nullptr. Invalid argument or invalid path");
        exit (1);
    }
    
    initStr();

    if (arg.flipTexture)
    {
        hFlip();
    }

    if (arg.loadInGPU)
    {
        loadInGPU();
    }
}

Text::~Text()
{
    if (surface_ != nullptr)
    {
        SDL_FreeSurface(surface_);
    }

    pixels_ = nullptr;
}

void Text::setColorAndText(const std::string& str, const ColorRGBA& color)
{
    str_ = str; 
    color_ = color;

    if(isLoadInGPU_)
    {
        unloadFromGPU();
        initStr();
        loadInGPU();
    }
    else
    {
        initStr();
    }
}

void Text::setColor(const ColorRGBA& color)
{
    color_ = color;

    if(isLoadInGPU_)
    {
        unloadFromGPU();
        initStr();
        loadInGPU();
    }
    else
    {
        initStr();
    }	
}

void Text::setText(const std::string& str)
{
    str_ = str;

    if(isLoadInGPU_)
    {
        unloadFromGPU();
        initStr();
        loadInGPU();
    }
    else
    {
        initStr();
    }
}

void Text::initStr	()
{
    if (surface_ != nullptr)
    {
        SDL_FreeSurface(surface_);
    }

    if (isSmooth_)
    {
        surface_ = TTF_RenderText_Blended(pFont_->getpFont(), str_.c_str(), SDL_Color{Uint8(color_.r * 255), Uint8(color_.g * 255), Uint8(color_.b * 255), Uint8(color_.a * 255)});
    }
    else
    {
        surface_ = TTF_RenderText_Solid(pFont_->getpFont(), str_.c_str(), SDL_Color{Uint8(color_.r * 255), Uint8(color_.g * 255), Uint8(color_.b * 255), Uint8(color_.a * 255)});
    }

    w_ 		= surface_->w;
    h_ 		= surface_->h;
    comp_	= surface_->format->BytesPerPixel;
    pixels_	= static_cast<unsigned char*>(surface_->pixels);

    if (pixels_ == nullptr)
    {
        SLog::logError(std::string("Text initialization error : \"") + str_ + "\" : " + TTF_GetError());
        exit(1);
    }
}