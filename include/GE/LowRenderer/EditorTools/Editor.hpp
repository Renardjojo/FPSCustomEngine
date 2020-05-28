//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-19 - 12 h 18

#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "GE/GE.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/LowRenderer/EditorTools/SceneGraphWindow.hpp"

namespace Engine::LowRenderer::EditorTools
{
    class Editor
    {

    private:

    protected:


    static void quickSaveSceneOption(Engine::Ressources::Scene& scene, Engine::GE& engine);
    static void saveSceneAsOption(Engine::Ressources::Scene& scene, Engine::GE& engine);
    static void loadSceneOption(Engine::GE& engine);
    static void updateInput();

    public:

        static bool _enable;
#pragma region constructor / destructor

        Editor()                                = delete;
        Editor(const Editor &other)             = delete;
        Editor(Editor &&other)                  = delete;
        virtual ~Editor()                       = delete;
        Editor &operator=(Editor const &other)  = delete;
        Editor &operator=(Editor &&other)       = delete;

#pragma endregion //!constructor/destructor

#pragma region methods

    static void update(Engine::Ressources::Scene& scene, Engine::GE& engine);

#pragma endregion //!methods

    };

} /*namespace Engine::LowRenderer::Editor*/

#endif // __EDITOR_HPP__
