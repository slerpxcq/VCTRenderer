#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "main_menu.h"

#include <iostream>

#include "../renderers/voxelizer_renderer.h"
#include "../../../rendering/render_window.h"
#include "../scene/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

bool UIMainMenu::drawSceneLoader = true;
bool UIMainMenu::drawFramerate = false;
bool UIMainMenu::drawSceneCameras = false;
bool UIMainMenu::drawSceneLights = false;
bool UIMainMenu::drawFramebuffers = false;
bool UIMainMenu::drawShadowOptions = false;
bool UIMainMenu::drawVoxelizationOptions = false;
bool UIMainMenu::drawGIOptions = false;
bool UIMainMenu::drawSceneMaterials = false;
bool UIMainMenu::drawSceneNodes = false;
bool UIMainMenu::loadModel = false;

using namespace ImGui;

static void PrintSceneGraph(std::shared_ptr<Node> node, uint32_t level = 0)
{
    for (uint32_t i = 0; i < level; ++i)
        std::cout << ' ';

    std::cout << node->name << std::endl;

    for (auto child : node->nodes) {
        PrintSceneGraph(child, level + 1);
    }
}

static void ImportModel(std::unique_ptr<Scene>& scene)
{
    Assimp::Importer importer;
    auto model = importer.ReadFile("assets\\models\\つみ式ミクさんv4\\つみ式ミクさんv4.pmx",
                                   aiProcess_Triangulate);
    assert(model);

    // print all nodes in the 
    model->mRootNode->mChildren;
}

void UIMainMenu::Draw()
{
    static bool showAbout = false;

    if (BeginMainMenuBar())
    {
        if (BeginMenu("View"))
        {
            MenuItem("Scene Loader", nullptr, &drawSceneLoader);
            MenuItem("Show Framerate", nullptr, &drawFramerate);
            MenuItem("View Voxels", nullptr, &VoxelizerRenderer::ShowVoxels);
            MenuItem("View Framebuffers", nullptr, &drawFramebuffers);
            EndMenu();
        }

        if (BeginMenu("Scene"))
        {
            MenuItem("Cameras", nullptr, &drawSceneCameras);
            MenuItem("Lights", nullptr, &drawSceneLights);
            MenuItem("Materials", nullptr, &drawSceneMaterials);
            MenuItem("Shapes", nullptr, &drawSceneNodes);
            if (MenuItem("Load model", nullptr)) {
            // Load model into current scene
                auto& scene = Scene::Active();
                if (scene) {
					// print the scene graph recursively
					PrintSceneGraph(scene->rootNode);
                    ImportModel(scene);
                } else {
                    std::cout << "A scene must be loaded to load a model.\n";
                }
            }
            EndMenu();
        }

        if (BeginMenu("Options"))
        {
            MenuItem("Shadows", nullptr, &drawShadowOptions);
            MenuItem("Voxelization", nullptr, &drawVoxelizationOptions);
            MenuItem("Global Illumination", nullptr, &drawGIOptions);
            MenuItem("About", nullptr, &showAbout);
            EndMenu();
        }

        if(Button("Exit"))
        {
            EngineBase::Instance()->Window().ShouldClose(true);
        };

        EndMainMenuBar();
    }

    if(showAbout)
    {
        if (Begin("About...", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Text("Developer: Jose Villegas.");
            Text("Email: villegasjose.gg@gmail.com");
        }

        End();
    }
}

UIMainMenu::UIMainMenu()
{
}

UIMainMenu::~UIMainMenu()
{
}