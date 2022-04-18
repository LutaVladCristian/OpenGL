#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera.h"
#include "lab_m1/tema2/transform3D.h"
#include <stack>
#include <vector>
#include <string>
#include <iostream>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
        typedef struct cell
        {
            cell() : row(0), col(0), value(0), visited(0) {}
            cell(int row, int col, float value, bool visited) : row(row), col(col), value(value), visited(visited) {}
            bool visited;
            bool wall[4];
            float value;
            int col;
            int row;
        }cell;

        typedef struct enemyType
        {
            enemyType() : posX(0), posZ(0), x(0), z(0), xDistance(0), zDistance(0), isHit(0), health(120){}
            enemyType(float posX, float posZ, float x, float z, float xDistance, float zDistance, int isHit, float health) : posX(posX), posZ(posZ), x(x), z(z), xDistance(xDistance), zDistance(zDistance), isHit(isHit), health(120) {}
            float posX;
            float posZ;
            float x;
            float z;
            float xDistance;
            float zDistance;
            int isHit;
            float health;
        }enemyType;

        typedef struct pickUpType
        {
            pickUpType() : posX(0), posZ(0), isHit(0) {}
            pickUpType(float posX, float posZ, int isHit, float health) : posX(posX), posZ(posZ), isHit(isHit) {}
            float posX;
            float posZ;
            int isHit;
        }pickUpType;

    public:
        Tema2();
        ~Tema2();
        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void SetMaze(cell matrix[5][5]);
        bool collisionPlayerWall(glm::vec3 direction);

    protected:
        std::unordered_map<std::string, Texture2D*> mapTextures;
        implemented::Camera2 *camera;
        glm::mat4 projectionMatrix;
        bool renderPlayer;
        float human_scaleX = 0.025f, human_scaleY = 0.1f, human_scaleZ = 0.025f;
        float cameraSpeed = 5.f;
        float angle = 0;
        float score = 0;

        cell matrix[5][5];
        int mazeWidth = 5, mazeHeight = 5;
        int hand = 0, foot = 0, rightHand = 0, rightFoot = 0, head = 0, pick = 0;
        float square_side = 10.0f;
        bool shot = false, shotMode = false, renderBullet = false;
        float bulletX = 0, bulletZ = 0, bulletSpeed = 20;

        enemyType enemy[5];
        pickUpType pickUp[8];
        float Distance;
        float health = 120, time = 120;
        int animation = 0;
    };
}
