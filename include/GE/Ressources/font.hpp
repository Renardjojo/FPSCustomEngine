//Project : Engine
//Editing by Six Jonathan
//Date : 25/01/2020 - 17 h 59

#ifndef _GE_FONT_H
#define _GE_FONT_H

#include <SDL2/SDL_ttf.h>
#include <string>

namespace Engine::Ressources
{
    struct FontCreateArg
    {
        const char* fontRoot;
        unsigned int fontSize;
        
    };

	class Font
	{
		public:

        Font()	                  = delete;
        Font(const Font &other)   = delete;
        Font(Font && other)       = default;
        Font(const FontCreateArg& arg);
        ~Font();

        const std::string&  getName()   const noexcept { return name_; }
        TTF_Font*           getpFont()  const noexcept { return font_; }

		protected:

		void initSDL_TTFComponent();

        std::string 	name_;
        TTF_Font* 		font_;

		#pragma region static attribut

		static unsigned int numberFont;

		#pragma endregion //! static attribut

		private:
	};	

}//namespace Engine::Ressources

#endif //_GE_FONT_H