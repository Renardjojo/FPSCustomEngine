//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-19 - 12 h 18

#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "GE/LowRenderer/EditorTools/SceneGraphWindow.hpp"

namespace Engine::LowRenderer::EditorTools
{
    class Editor
    {

    private:

    protected:

    static bool _enable;

    static void updateInput();

    public:

#pragma region constructor / destructor

        Editor()                                = delete;
        Editor(const Editor &other)             = delete;
        Editor(Editor &&other)                  = delete;
        virtual ~Editor()                       = delete;
        Editor &operator=(Editor const &other)  = delete;
        Editor &operator=(Editor &&other)       = delete;

#pragma endregion //!constructor/destructor

#pragma region methods

    static void update(Engine::Core::DataStructure::Graph& graph);

#pragma endregion //!methods

#pragma region accessor
#pragma endregion //!accessor

#pragma region mutator
#pragma endregion //!mutator

#pragma region operator
#pragma endregion //!operator

#pragma region convertor
#pragma endregion //!convertor
    };

} /*namespace Engine::LowRenderer::Editor*/

#endif // __EDITOR_HPP__
