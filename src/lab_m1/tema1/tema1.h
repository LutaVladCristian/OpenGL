#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };
    public:
        Tema1();
        ~Tema1();
        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void DrawScene(glm::mat3 visMatrix);
        void DrawScene2(glm::mat3 visMatrix);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        float cx = 0, cy = 0;
        float mapX = 1280, mapY = 720;
        float bulletX = 0, bulletY = 0;
        glm::mat3 modelMatrix, visMatrix, visMatrixMap;
        float scaleHuman = 0.25f;
        float angularStep = 0, angularStep2 = 0;
        float health = 100;
        float enemyX = 0, enemyY = 0;
        float pozX, pozY;
        float healX, healY;
        int score = 0;
        int positioning = 0, healPos = 0;
        int pickUp = 0;
        int speed = 500;
        int bulletScale = 8, bulletScaleMap = 5;
        float pickUpX, pickUpY;
        float centerX, centerY;
        bool shot = false;
        float cursorX, cursorY;
        ViewportSpace viewSpace;
        LogicSpace logicSpace, logicSpaceMap;
    };
}   // namespace m1
