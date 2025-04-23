#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "main_menu.h"
#include "../util/scene_importer.h"

#include "../renderers/voxelizer_renderer.h"
#include "../../../rendering/render_window.h"
#include "../scene/scene.h"
#include "../scene/mesh.h"
#include "../scene/material.h"  
#include "../../../core/assets_manager.h"
#include "../renderers/voxelizer_renderer.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <iostream>
#include <filesystem>
#include <fstream>

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
bool UIMainMenu::drawSceneGraph = false;

using namespace ImGui;

static void DrawSceneGraph(std::shared_ptr<Node> node)
{
    if (ImGui::TreeNode(node->name.c_str())) {
        for (auto child : node->nodes) {
            DrawSceneGraph(child);
        }
        ImGui::TreePop();
    }
}

static void PrintSceneGraph(std::shared_ptr<Node> node, uint32_t level = 0)
{
    for (uint32_t i = 0; i < level; ++i)
        std::cout << ' ';

    std::cout << node->name << '\n';

    for (auto child : node->nodes) {
        PrintSceneGraph(child, level + 1);
    }
}


static std::shared_ptr<Node> ImportModel()
{
    std::filesystem::path modelPath = u8"assets\\models\\つみ式ミクさんv4";
    std::filesystem::path modelFile = u8"つみ式ミクさんv4.pmx";

    Scene model((modelPath / modelFile).string());
    SceneImporter::Import(model.GetFilepath(), &model, aiProcessPreset_TargetRealtime_Fast);

    for (auto& mesh : model.meshes)
        mesh->Load();

    for (auto& tex : model.textures)
        tex->Load(oglplus::TextureMinFilter::LinearMipmapLinear,
                  oglplus::TextureMagFilter::Linear,
                  oglplus::TextureWrap::Repeat,
                  oglplus::TextureWrap::Repeat);
    
    for (auto& node : model.rootNode->nodes) {
        node->BuildDrawList();
        node->nodeState = Node::Dynamic;
    }

    Transform::CleanEventMap();

    return model.rootNode;
}

static void LoadModelToScene(std::shared_ptr<Node> model)
{
    auto voxelizer = std::dynamic_pointer_cast<VoxelizerRenderer>(AssetsManager::Instance()->renderers["Voxelizer"]);
    auto& scene = Scene::Active(); 

    scene->rootNode->nodes.push_back(model);
    scene->rootNode->BuildDrawList();

    for (auto& mesh : model->meshes) 
        scene->materials.push_back(mesh->material);

    voxelizer->RevoxelizeScene();
}

void UIMainMenu::Draw()
{
    static bool showAbout = false;
	auto& scene = Scene::Active();

    if (BeginMainMenuBar())
    {
        if (BeginMenu("View"))
        {
            MenuItem("Scene Loader", nullptr, &drawSceneLoader);
            MenuItem("Show Framerate", nullptr, &drawFramerate);
            MenuItem("View Voxels", nullptr, &VoxelizerRenderer::ShowVoxels);
            MenuItem("View Framebuffers", nullptr, &drawFramebuffers);
            ImGui::EndMenu();
        }

        if (BeginMenu("Scene"))
        {
            MenuItem("Cameras", nullptr, &drawSceneCameras);
            MenuItem("Lights", nullptr, &drawSceneLights);
            MenuItem("Materials", nullptr, &drawSceneMaterials);
            MenuItem("Shapes", nullptr, &drawSceneNodes);
            if (MenuItem("Load model", nullptr)) {
                if (scene) {
                    LoadModelToScene(ImportModel());
                } else {
                    std::cerr << "A scene must be loaded to load a model.\n";
                }
            }
            MenuItem("Scene graph", nullptr, &drawSceneGraph);
            ImGui::EndMenu();
        }

        if (BeginMenu("Options"))
        {
            MenuItem("Shadows", nullptr, &drawShadowOptions);
            MenuItem("Voxelization", nullptr, &drawVoxelizationOptions);
            MenuItem("Global Illumination", nullptr, &drawGIOptions);
            MenuItem("About", nullptr, &showAbout);
            ImGui::EndMenu();
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

    if (drawSceneGraph) {
        if (ImGui::Begin("Scene graph", &drawSceneGraph, ImGuiWindowFlags_AlwaysAutoResize)) {
            DrawSceneGraph(scene->rootNode);
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
