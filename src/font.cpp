#include "font.hpp"

#include "parserTool.hpp"
#include "log.hpp"

using namespace Engine::Resources;
using namespace Engine::Core::Debug;

Font::Font(const FontCreateArg& arg)
    : font_ (nullptr)
{
    if (numberFont == 0)
        initSDL_TTFComponent();

    font_ = TTF_OpenFont(arg.fontRoot, arg.fontSize);
    numberFont++;

    name_ = parseNameInPath(arg.fontRoot);

    if (font_ == nullptr)
    {
        Engine::Core::Debug::SLog::logError(std::string("Font initialization error with font name \"") + name_ + "\" : " + TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

Font::~Font()
{
    TTF_CloseFont(font_);

    numberFont--;
    if (numberFont == 0)
    {
        TTF_Quit();
        SLog::log("SDL_TTF release");
    }  
}

void Font::initSDL_TTFComponent()
{
    if(TTF_Init()== -1) 
    {
        SLog::logError(std::string("Initilization of SDL_TFF error : ") + TTF_GetError());
        exit(1);
    }

    SLog::log("SDL_TTF initialize");
}

unsigned int Font::numberFont (0);
