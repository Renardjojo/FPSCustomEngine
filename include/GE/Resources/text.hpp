//Project : Engine
//Editing by Six Jonathan
//Date : 25/01/2020 - 17 h 44

#ifndef _GE_TEXT_H
#define _GE_TEXT_H

#include <string>

#include "texture.hpp"
#include "font.hpp"
#include "type.hpp"

namespace Engine::Resources
{
    struct TextCreateArg
    {
        const char* 	text;
		Font*			pFont;
		ColorRGBA		color 				= {1.f, 1.f, 1.f, 1.f};
        E_WrapType      wrapType 			= E_WrapType::REPEAT;
        E_FilterType    filterType 			= E_FilterType::LINEAR;
		bool			createSmoothText 	= true;
        bool 			flipTexture 		= false;
        bool 			loadInGPU 			= true;
    };

	class Text
		: public Texture
	{
		public:

        Text()	                  = delete;
        Text(const Text &other)   = delete;
        Text(Text && other)       = default;
        Text(const TextCreateArg& arg);

        virtual ~Text() = default;

		void setColorAndText(const std::string& str, const ColorRGBA& color);
		void setColor(const ColorRGBA& color);
		void setText(const std::string& str);

        const std::string&  getStr()   	const noexcept 	{ return str_; }
        const ColorRGBA&  	getColor()  const noexcept { return color_; }
        const Font&     	getFont()  	const noexcept { return *pFont_; }

		protected:

		#pragma region attribut

		std::string 	str_;
		Font* 			pFont_;
		ColorRGBA		color_;
		bool 			isSmooth_;

		#pragma endregion //!attribut

		#pragma region methods
		
		void initStr	();

		#pragma endregion //!methods

		private:
	};	
}//namespace Engine::Resources

#endif //_GE_TEXT_H