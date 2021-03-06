#include "GE/Core/Parsers/shaderParser.hpp"
#include "GE/Core/Debug/log.hpp"

#include <sstream> 
#include <iostream>
#include <fstream>

using namespace Engine::Core::Parsers;
using namespace Engine::Core::Debug;

void ShaderParser::parseIncludesShader(std::string& sourceCode)
{
    std::stringstream input (sourceCode);
    std::stringstream output;
    std::string line;
    size_t curs = 0;

    while(std::getline(input, line))
    {
        std::size_t cursorWord = line.find("#include");

        if (cursorWord != std::string::npos)
        {
            std::size_t cursorStart = line.find("\"", cursorWord) + 1;
            std::size_t cursorEnd = line.find("\"", cursorStart);
            std::string path = line.substr(cursorStart, cursorEnd - cursorStart);

            //Read file
            std::ifstream file;
            std::stringstream fileStream;
            std::string codeToInclude;

            file.open(path);
            if(!file)
            {
                functError((std::string("Error to found or read file : ") + path).c_str());
                exit(1);
            }

            // read file's buffer contents into streams
            fileStream << file.rdbuf();

            // close file handlers
            file.close();

            // convert stream into string
            codeToInclude = fileStream.str().c_str();

            //replace #include "[...]" by the code

            sourceCode.replace(curs + cursorWord, cursorEnd - cursorWord + 1, codeToInclude);
        }

        curs = input.tellg();
    }
}