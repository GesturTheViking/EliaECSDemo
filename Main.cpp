extern "C"
{
#include "raylib/raylib.h"
#include "raylib/raymath.h"
}

#include <stdio.h>
#include "Game/Game.h"

namespace Config
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;
    constexpr char* title = "Elia ECS";
    constexpr int targetFPS = 30;

    constexpr Vector3 cameraPos = { 30.f, 30.f, 30.f };
    constexpr float cameraFOV = 45.f;
    constexpr int cameraProjection = CAMERA_PERSPECTIVE;
}

int main()
{
    InitWindow(Config::screenWidth, Config::screenHeight, Config::title);
    SetTargetFPS(Config::targetFPS);

    /* Init Camera */
    Camera camera{0};
    {
        camera.position = Config::cameraPos;
        camera.target = { 0.0f, 25.0f, 0.0f };
        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = Config::cameraFOV;
        camera.projection = Config::cameraProjection;

        SetCameraMode(camera, CAMERA_FREE);
    }

    /* Define HUD */
    constexpr int fontSize = 35;
    Rectangle buttons[] = {
        { Config::screenWidth - 75, 25, 50, 50 },
        { Config::screenWidth - 75, 85, 50, 50 },
        { Config::screenWidth - 75, 145, 50, 50 },
        { Config::screenWidth - 75, 205, 50, 50 },
        { Config::screenWidth - 75, 265, 50, 50 },
        { Config::screenWidth - 75, 325, 50, 50 },
    };
    constexpr int buttonCount = sizeof(buttons) / sizeof(buttons[0]);
    const char* const labels[buttonCount] = {
        "+1", "+10", "+100", "-1", "-10", "-100"
    };

    Game::Init();

    /* Main Loop */
    while (!WindowShouldClose())
    {
        UpdateCamera(&camera);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

            Game::Update();

            /* Bounds */
            DrawLine3D({ 25.f, 0.f, 25.f }, { 25.f, 50.f, 25.f }, RED);
            DrawLine3D({ -25.f, 0.f, 25.f }, { -25.f, 50.f, 25.f }, RED);
            DrawLine3D({ -25.f, 0.f, -25.f }, { -25.f, 50.f, -25.f }, RED);
            DrawLine3D({ 25.f, 0.f, -25.f }, { 25.f, 50.f, -25.f }, RED);

            DrawLine3D({ 25.f, 50.f, 25.f }, { -25.f, 50.f, 25.f }, RED);
            DrawLine3D({ -25.f, 50.f, 25.f },  { -25.f, 50.f, -25.f }, RED);
            DrawLine3D({ -25.f, 50.f, -25.f },  { 25.f, 50.f, -25.f }, RED);
            DrawLine3D({ 25.f, 50.f, -25.f },  { 25.f, 50.f, 25.f }, RED);

            DrawGrid(50, 1.0f);

            EndMode3D();
            
            /* HUD */
            DrawFPS(10, 10);

            for (int btn = 0; btn < buttonCount; ++btn)
            {
                DrawRectangleRec(buttons[btn], BLACK);
                DrawText(labels[btn], buttons[btn].x + buttons[btn].width/2 - MeasureText(labels[btn], fontSize)/2,
                    buttons[btn].y + buttons[btn].height/2 - fontSize/2, fontSize, WHITE);

                if (CheckCollisionPointRec(GetMousePosition(), buttons[btn])
                    && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    switch (btn)
                    {
                        case 0: Game::AddEntities(1); break;
                        case 1: Game::AddEntities(10); break;
                        case 2: Game::AddEntities(100); break;
                        case 3: Game::RemoveEntities(1); break;
                        case 4: Game::RemoveEntities(10); break;
                        case 5: Game::RemoveEntities(100); break;
                    }
                }
            }

            if (Game::IsMaxEntitiesReached())
            {
                constexpr char* value = "Max entities reached.";
                DrawText(value, Config::screenWidth / 2 - MeasureText(value, fontSize) / 2, 10, fontSize, RED);
            }

            char entityCountBuf[32];
            sprintf(entityCountBuf, "%u", Game::GetEntityCount());
            DrawText(entityCountBuf, Config::screenWidth - 20 - MeasureText(entityCountBuf, fontSize), Config::screenHeight - 40, fontSize, RED);
        }
        EndDrawing();
    }

    /* Shutdown */
    {
        Game::Terminate();
        CloseWindow();
    }

    return 0;
}