#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "free_camera.h"
#include "../scene/camera.h"
#include "../core/interface.h"
#include "../rendering/render_window.h"
#include "../../../scene/scene.h"

void FreeCamera::Update2()
{
}

void FreeCamera::Update()
{
    static auto enabled = true;
    auto &io = ImGui::GetIO();
    // main render window
    auto &window = EngineBase::Instance()->Window();

    // if(io.KeyShift && ImGui::IsKeyReleased(GLFW_KEY_F))
    // {
    //     enabled = !enabled;
    // }

    // io.MouseDrawCursor = !enabled;
    io.MouseDrawCursor = true;

    // camera movement
    if (Camera::Active() && enabled)
    {
        auto &cam = Camera::Active();
        auto &sceneExtent = Scene::Active()->rootNode->boundaries.Extent();
        auto scaleSpeed = glm::max(sceneExtent.x, glm::max(sceneExtent.y,
                                   sceneExtent.z)) * 0.5f;
        auto cameraSpeed = scaleSpeed * io.DeltaTime;
        auto cameraRight = normalize(cross(cam->Forward(), cam->Up()));
        static auto yaw = 3.14f, pitch = 0.0f;

        if (io.KeysDown[GLFW_KEY_W])
        {
            cam->Position(cam->Position() + cam->Forward() * cameraSpeed);
        }

        if (io.KeysDown[GLFW_KEY_S])
        {
            cam->Position(cam->Position() - cam->Forward() * cameraSpeed);
        }

        if (io.KeysDown[GLFW_KEY_A])
        {
            cam->Position(cam->Position() - cameraRight * cameraSpeed);
        }

        if (io.KeysDown[GLFW_KEY_D])
        {
            cam->Position(cam->Position() + cameraRight * cameraSpeed);
        }

        static constexpr float camRotateSpeed = 0.001f;

        if (io.KeysDown[GLFW_KEY_Q])
        {
            cam->Rotation(glm::angleAxis(-cameraSpeed * camRotateSpeed, cam->Up()) * cam->Rotation());
        }

        if (io.KeysDown[GLFW_KEY_E])
        {
            cam->Rotation(glm::angleAxis(cameraSpeed * camRotateSpeed, cam->Up()) * cam->Rotation());
        }

        if (io.KeysDown[GLFW_KEY_LEFT_SHIFT])
        {
            cam->Position(cam->Position() - cam->Up() * cameraSpeed);
        }

        if (io.KeysDown[GLFW_KEY_SPACE])
        {
            cam->Position(cam->Position() + cam->Up() * cameraSpeed);
        }

        // auto hWidth = window.Info().displayWidth / 2;
        // auto hHeight = window.Info().displayHeight / 2;
        // glfwSetCursorPos(window.Handler(), hWidth, hHeight);
        // Compute new orientation
        // float sensitivity = 0.1f;
        // yaw += sensitivity * io.DeltaTime * float(hWidth - io.MousePos.x);
        // pitch += sensitivity * io.DeltaTime * float(hHeight - io.MousePos.y);
        // cam->Forward(glm::vec3(cos(pitch) * sin(yaw), sin(pitch),
        //                        cos(pitch) * cos(yaw)));
    }
}
