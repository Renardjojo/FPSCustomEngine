//Project : Engine
//Editing by Six Jonathan
//Date : 15/01/2020 - 13 h 00

#ifndef _GE_SHADER_PARSER_H
#define _GE_SHADER_PARSER_H

#include <string>

namespace Engine::Core::Parsers
{
    //singletone
    class ShaderParser
    {
        public:

            #pragma region constructor/destructor

            ShaderParser ()							    = delete;
            ShaderParser (const ShaderParser& other)	= delete;
            ShaderParser (ShaderParser&& other)			= delete;
            ~ShaderParser ()				            = delete;

            #pragma endregion //!constructor/destructor

            #pragma region methods
            #pragma endregion //!methods

            #pragma region static methods

            /**
             * @brief call and use all parse function
             * 
             * @param sourceCode 
             */
            static inline 
            void parse(std::string& sourceCode)
            {
                parseIncludesShader (sourceCode);
            }     

            /**
             * @brief Search and include shader source code if the shader contain #include "path"
             * 
             * @param sourceCode 
             */
            static
            void parseIncludesShader(std::string& sourceCode);

            #pragma endregion //!static methods

            #pragma region accessor
            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut
            #pragma endregion //!attribut

            #pragma region static attribut
            #pragma endregion //! static attribut

            #pragma region methods
            #pragma endregion //!methods

        private:

    };
} //namespace Engine::Core::Parsers

#endif // _GE_SHADER_PARSER_H