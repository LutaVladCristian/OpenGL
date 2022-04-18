#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/objectTema2D.h"

using namespace std;
using namespace m1;


struct Position
{
    float x, y, width, height;
    Position(float x, float y, float width, float height)
        :x(x), y(y), width(width), height(height)
    {
    }
};

std::vector<Position> posVector;
float error_factor = 25;
float viewX = 300, viewY = 300;
float radius1 = 75, radius2 = 25, radius3 = 80, radius4 = 15;

bool isCollision(float bulletX, float bulletY, float cx, float cy, Position box)
{
    if ((cx + viewX + bulletX >= box.x) && (cx + viewX + bulletX <= box.x + box.width) && (cy + viewY + bulletY > box.y) && (cy + viewY + bulletY <= box.y + box.height))
        return true;
    else
        return false;
}

bool collisionUp(float cx, float cy, Position box)
{
    float x, y;
    x = max(box.x, min(cx + viewX, box.x + box.width));
    y = max(box.y, min(cy + viewY, box.y + box.height));
    float dist = sqrt(pow(x - cx - viewX, 2) + pow(y - cy - viewY, 2));
    if ((dist < radius2) && (cy + viewY <= box.y))
        return false;
    else
        return true;

}

bool collisionDown(float cx, float cy, Position box)
{
    float x, y;
    x = max(box.x, min(cx + viewX, box.x + box.width));
    y = max(box.y, min(cy + viewY, box.y + box.height));
    float dist = sqrt(pow(x - cx - viewX, 2) + pow(y - cy - viewY, 2));
    if ((dist < radius2) && (cy + viewY >= box.y + box.height))
        return false;
    else
        return true;
}

bool collisionRight(float cx, float cy, Position box)
{
    float x, y;
    x = max(box.x, min(cx + viewX, box.x + box.width));
    y = max(box.y, min(cy + viewY, box.y + box.height));
    float dist = sqrt(pow(x - cx - viewX, 2) + pow(y - cy - viewY, 2));
    if ((dist < radius2) && (cx + viewX <= box.x + box.width))
        return false;
    else
        return true;
}

bool collisionLeft(float cx, float cy, Position box)
{
    float x, y;
    x = max(box.x, min(cx + viewX, box.x + box.width));
    y = max(box.y, min(cy + viewY, box.y + box.height));
    float dist = sqrt(pow(x - cx - viewX, 2) + pow(y - cy - viewY, 2));
    if ((dist < radius2) && (cx + viewX >= box.x))
        return false;
    else
        return true;
}

bool collisionEnemy(float enemyX, float enemyY, float cx, float cy)
{
    float distance = sqrt(pow(cx + viewX - enemyX, 2) + pow(cy + viewY - enemyY, 2));
    if (distance < radius2)
        return true;
    else
        return false;
}

bool collisionBullet(float cx, float cy, float enemyX, float enemyY, float bulletX, float bulletY)
{
    float x, y;
    x = max(cx + bulletX, min(enemyX, cx + bulletX));
    y = max(cy + bulletY, min(enemyY, cy + bulletY));
    float dist = sqrt(pow(x - enemyX, 2) + pow(y - enemyY, 2));
    if ((dist < radius2))// && (cy + viewY >= box.y + box.height))
        return true;
    else
        return false;
}


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    posVector.reserve(6);

    Mesh* circle1 = object2D::CreateCirclee("circleBody", corner, radius1, glm::vec3(0.81f, 0.71f, 0.23f));
    AddMeshToList(circle1);

    Mesh* circle2 = object2D::CreateCirclee("circleHand", corner, radius2, glm::vec3(1.0f, 1.1f, 0.0f));
    AddMeshToList(circle2);

    Mesh* circle3 = object2D::CreateCirclee("circleBehind", corner, radius3, glm::vec3(0.0f, 0.0f, 0.0f));
    AddMeshToList(circle3);

    Mesh* square1 = object2D::CreateSquaree("map", corner, 1, glm::vec3(0.7f, 0.5f, 0.5f), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquaree("behindWall", corner, 1, glm::vec3(0.0f, 0.0f, 0.0f), true);
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquaree("mapBorder", corner, 1, glm::vec3(0.0f, 0.0f, 0.0f), true);
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquaree("bullet", corner, 1, glm::vec3(0.0f, 1.0f, 1.0f), true);
    AddMeshToList(square4);

    Mesh* square5 = object2D::CreateSquaree("wall", corner, 1, glm::vec3(1.0f, 0.6f, 0.1f), true);
    AddMeshToList(square5);

    Mesh* square6 = object2D::CreateSquaree("healthBar", corner, 1, glm::vec3(0.6f, 0.0f, 0.0f), true);
    AddMeshToList(square6);

    Mesh* square7 = object2D::CreateSquaree("behindHealthBar", corner, 1, glm::vec3(0.0f, 0.0f, 0.0f), true);
    AddMeshToList(square7);

    Mesh* circle4 = object2D::CreateCirclee("enemyPart", corner, radius1, glm::vec3(0.6f, 0.0f, 0.0f));
    AddMeshToList(circle4);

    Mesh* circle5 = object2D::CreateCirclee("enemyPartBehind", corner, radius3, glm::vec3(0.0f, 0.0f, 0.0f));
    AddMeshToList(circle5);

    Mesh* circle6 = object2D::CreateCirclee("enemyHand", corner, radius2, glm::vec3(0.6f, 0.0f, 0.0f));
    AddMeshToList(circle6);

    Mesh* circle7 = object2D::CreateCirclee("heal", corner, 1, glm::vec3(0.0f, 0.9f, 0.3f));
    AddMeshToList(circle7);

    Mesh* square8 = object2D::CreateSquaree("cross", corner, 1, glm::vec3(0.8f, 1.0f, 1.0f), true);
    AddMeshToList(square8);

    Mesh* square9 = object2D::CreateSquaree("wallMap", corner, 1, glm::vec3(0.7f, 0.0f, 0.5f), true);
    AddMeshToList(square9);

    Mesh* square10 = object2D::CreateSquaree("mapColorMap", corner, 1, glm::vec3(0.6f, 0.5f, 0.1f), true);
    AddMeshToList(square10);

    Mesh* circle8 = object2D::CreateCirclee("pickUp", corner, 1, glm::vec3(0.5f, 0.0f, 0.4f));
    AddMeshToList(circle8);

    Mesh* circle9 = object2D::CreateCirclee("circleBehindPickUp", corner, 1, glm::vec3(0.0f, 0.0f, 0.0f));
    AddMeshToList(circle9);
}


void Tema1::DrawScene(glm::mat3 visMatrix)
{
    //player
    if (health > 0)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX, viewY);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleBody"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX, viewY);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["circleBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["circleBehind"], shaders["VertexColor"], modelMatrix);

        //enemy
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX, pozY);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyPart"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX, pozY);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyPartBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["enemyPartBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["enemyPartBehind"], shaders["VertexColor"], modelMatrix);
    }

    //heal
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX + 5, healY - 10);
    modelMatrix *= transform2D::Scale(5, 15);
    RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX, healY);
    modelMatrix *= transform2D::Rotate(RADIANS(90));
    modelMatrix *= transform2D::Scale(5, 15);
    RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX + 7.5, healY - 5);
    modelMatrix *= transform2D::Scale(15, 15);
    RenderMesh2D(meshes["heal"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX + 7.5, healY - 5);
    modelMatrix *= transform2D::Scale(17, 17);
    RenderMesh2D(meshes["circleBehindPickUp"], shaders["VertexColor"], modelMatrix);

    //health bar
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(720, 550);
    modelMatrix *= transform2D::Translate(cx, cy);
    if (health > 0)
        modelMatrix *= transform2D::Scale(health, 20);
    RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(715, 545);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(110, 30);
    RenderMesh2D(meshes["behindHealthBar"], shaders["VertexColor"], modelMatrix);

    //pickUp
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(pickUpX - 2.5, pickUpY - 2.5);
    modelMatrix *= transform2D::Scale(bulletScaleMap, bulletScaleMap);
    RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(pickUpX, pickUpY);
    modelMatrix *= transform2D::Scale(20, 20);
    RenderMesh2D(meshes["pickUp"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(pickUpX, pickUpY);
    modelMatrix *= transform2D::Scale(22, 22);
    RenderMesh2D(meshes["circleBehindPickUp"], shaders["VertexColor"], modelMatrix);

    //wall
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(110, 80);
    modelMatrix *= transform2D::Scale(40, 190);
    RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(110, 45);
    modelMatrix *= transform2D::Scale(200, 40);
    RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(855, 605);
    modelMatrix *= transform2D::Scale(200, 40);
    RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(1055, 405);
    modelMatrix *= transform2D::Scale(40, 240);
    RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(140, 505);
    modelMatrix *= transform2D::Scale(140, 140);
    RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(905, 125);
    modelMatrix *= transform2D::Scale(90, 90);
    RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);

    //behind wall
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(105, 40);
    modelMatrix *= transform2D::Scale(50, 235);
    posVector.push_back(Position(105, 40, 50, 235));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(115, 40);
    modelMatrix *= transform2D::Scale(200, 50);
    posVector.push_back(Position(115, 40, 200, 50));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(850, 600);
    modelMatrix *= transform2D::Scale(200, 50);
    posVector.push_back(Position(850, 600, 200, 50));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(1050, 400);
    modelMatrix *= transform2D::Scale(50, 250);
    posVector.push_back(Position(1050, 400, 50, 250));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(135, 500);
    modelMatrix *= transform2D::Scale(150, 150);
    posVector.push_back(Position(135, 500, 150, 150));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(900, 120);
    modelMatrix *= transform2D::Scale(100, 100);
    posVector.push_back(Position(900, 120, 100, 100));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    //map
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(5, 5);
    modelMatrix *= transform2D::Scale(1270, 710);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Scale(1280, 720);
    RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawScene2(glm::mat3 visMatrix)
{
    //player
    if (health > 0)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX, viewY);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleBody"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX, viewY);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["circleBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["circleHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(viewX + radius4, viewY + radius4);
        modelMatrix *= transform2D::Translate(cx, cy);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["circleBehind"], shaders["VertexColor"], modelMatrix);
    }

    //wall
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(110, 80);
    modelMatrix *= transform2D::Scale(40, 190);
    RenderMesh2D(meshes["wallMap"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(110, 45);
    modelMatrix *= transform2D::Scale(200, 40);
    RenderMesh2D(meshes["wallMap"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(855, 605);
    modelMatrix *= transform2D::Scale(200, 40);
    RenderMesh2D(meshes["wallMap"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(1055, 405);
    modelMatrix *= transform2D::Scale(40, 240);
    RenderMesh2D(meshes["wallMap"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(140, 505);
    modelMatrix *= transform2D::Scale(140, 140);
    RenderMesh2D(meshes["wallMap"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(905, 125);
    modelMatrix *= transform2D::Scale(90, 90);
    RenderMesh2D(meshes["wallMap"], shaders["VertexColor"], modelMatrix);

    //enemy(minimap)
    if(health > 0)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX, pozY);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyPart"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX, pozY);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyPartBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);;
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Translate(-radius4, -radius4);
        modelMatrix *= transform2D::Rotate(90);
        modelMatrix *= transform2D::Translate(radius4, radius4);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["enemyPartBehind"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman, scaleHuman);
        RenderMesh2D(meshes["enemyHand"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(pozX + radius4, pozY + radius4);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        modelMatrix *= transform2D::Scale(scaleHuman / 2.5, scaleHuman / 2.5);
        RenderMesh2D(meshes["enemyPartBehind"], shaders["VertexColor"], modelMatrix);
    }

    //heal
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX + 5, healY - 10);
    modelMatrix *= transform2D::Scale(5, 15);
    RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX, healY);
    modelMatrix *= transform2D::Rotate(RADIANS(90));
    modelMatrix *= transform2D::Scale(5, 15);
    RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX + 7.5, healY - 5);
    modelMatrix *= transform2D::Scale(15, 15);
    RenderMesh2D(meshes["heal"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(healX + 7.5, healY - 5);
    modelMatrix *= transform2D::Scale(17, 17);
    RenderMesh2D(meshes["circleBehindPickUp"], shaders["VertexColor"], modelMatrix);

    //pickUp
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(pickUpX - 2.5, pickUpY - 2.5);
    modelMatrix *= transform2D::Scale(bulletScaleMap, bulletScaleMap);
    RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(pickUpX, pickUpY);
    modelMatrix *= transform2D::Scale(20, 20);
    RenderMesh2D(meshes["pickUp"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(pickUpX, pickUpY);
    modelMatrix *= transform2D::Scale(22, 22);
    RenderMesh2D(meshes["circleBehindPickUp"], shaders["VertexColor"], modelMatrix);

    //behind wall
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(105, 40);
    modelMatrix *= transform2D::Scale(50, 235);
    posVector.push_back(Position(105, 40, 50, 235));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(115, 40);
    modelMatrix *= transform2D::Scale(200, 50);
    posVector.push_back(Position(115, 40, 200, 50));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(850, 600);
    modelMatrix *= transform2D::Scale(200, 50);
    posVector.push_back(Position(850, 600, 200, 50));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(1050, 400);
    modelMatrix *= transform2D::Scale(50, 250);
    posVector.push_back(Position(1050, 400, 50, 250));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(135, 500);
    modelMatrix *= transform2D::Scale(150, 150);
    posVector.push_back(Position(135, 500, 150, 150));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(900, 120);
    modelMatrix *= transform2D::Scale(100, 100);
    posVector.push_back(Position(900, 120, 100, 100));
    RenderMesh2D(meshes["behindWall"], shaders["VertexColor"], modelMatrix);

    //map
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(5, 5);
    modelMatrix *= transform2D::Scale(1270, 710);
    RenderMesh2D(meshes["mapColorMap"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Scale(1280, 720);
    RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], modelMatrix);
}


void Tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    logicSpace.x = cx;
    logicSpace.y = cy;
    logicSpace.width = 600;
    logicSpace.height = 600;

    logicSpaceMap.x = -5800;
    logicSpaceMap.y = -200;
    logicSpaceMap.width = 5000;
    logicSpaceMap.height = 5000;

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    visMatrixMap = glm::mat3(1);
    visMatrixMap *= VisualizationTransf2DUnif(logicSpaceMap, viewSpace);

    cursorX = window->GetCursorPosition().x;
    cursorY = logicSpace.height - window->GetCursorPosition().y;

    //bullet
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(viewX, viewY);
    modelMatrix *= transform2D::Translate(cx, cy);
    switch (shot)
    {
        case true:
            if ((DEGREES(angularStep2) > 0) && (DEGREES(angularStep2) <= 70))
            {
                bulletX += deltaTimeSeconds * speed;
                bulletY -= deltaTimeSeconds * speed;
            }

            if ((DEGREES(angularStep2) > 70) && (DEGREES(angularStep2) <= 120))
                bulletY -= deltaTimeSeconds * speed;

            if ((DEGREES(angularStep2)  > 120) && (DEGREES(angularStep2) <= 180))
            {
                bulletX -= deltaTimeSeconds * speed;
                bulletY -= deltaTimeSeconds * speed;
            }

            if ((DEGREES(angularStep2) > 180) && (DEGREES(angularStep2) <= 200))
                bulletX -= deltaTimeSeconds * speed;

            if ((DEGREES(angularStep2) > 200) && (DEGREES(angularStep2) <= 250))
            {
                bulletX -= deltaTimeSeconds * speed;
                bulletY += deltaTimeSeconds * speed;
            }

            if ((DEGREES(angularStep2) > 250) && (DEGREES(angularStep2) <= 270))
                bulletY += deltaTimeSeconds * speed;

            if ((DEGREES(angularStep2) <= -30) && (DEGREES(angularStep2) > -85))
            {
                bulletX += deltaTimeSeconds * speed;
                bulletY += deltaTimeSeconds * speed;
            }

            if ((DEGREES(angularStep2) > -30) && (DEGREES(angularStep2) <= 0))
                bulletX += deltaTimeSeconds * speed;

            modelMatrix *= transform2D::Translate(bulletX, bulletY);
            if ((((bulletX > 150) && (bulletY > 150))|| ((bulletX < -150) && (bulletY > 150)) || ((bulletX > 150) && (bulletY < -150)) || ((bulletX < -150) && (bulletY < -150)))
            || (isCollision(bulletX, bulletY, cx, cy, posVector[0]) == true)
            || (isCollision(bulletX, bulletY, cx, cy, posVector[1]) == true)
            || (isCollision(bulletX, bulletY, cx, cy, posVector[2]) == true)
            || (isCollision(bulletX, bulletY, cx, cy, posVector[3]) == true)
            || (isCollision(bulletX, bulletY, cx, cy, posVector[4]) == true)
            || (isCollision(bulletX, bulletY, cx, cy, posVector[5]) == true)
            || ((cx + viewX + bulletX <= 0) || (cy + viewY + bulletY <= 0) || (cx + viewX + bulletX >= mapX) || (cy + viewY + bulletY >= mapY)))
            {
                shot = false;
                modelMatrix *= transform2D::Translate( -bulletX, -bulletY);
                bulletX = 0;
                bulletY = 0;
            }
            break;
    }
    modelMatrix *= transform2D::Scale(bulletScale, bulletScale);
    if (health > 0)
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

    float xDistance = cx + viewX - pozX - enemyX;
    float yDistance = cy + viewY - pozY - enemyY;
    float Distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));
    if (Distance < 800)
    {
        enemyY += deltaTimeSeconds * 100 * (yDistance / Distance);
        enemyX += deltaTimeSeconds * 100 * (xDistance / Distance);
    }

    DrawScene2(visMatrixMap);
    DrawScene(visMatrix);

    if (collisionEnemy(enemyX + pozX, enemyY + pozY, cx, cy) == true)
        if (health > 0)
            health -= 25;

    if (health <= 0)
    {
        cout << endl << endl << "Congrats! " << endl << "Your score: " << score << "! UwU" << endl << "Te pwp dulke! :333 " << endl << endl;
        Exit();
    }

    //cout << viewX + cx << " " << cy + viewY << " " << endl;
    //cout << "body_pos: " << cx + viewX << " " << cy + viewY << " " << endl;
    //cout << "bullet_pos: " << bulletX + cx + viewX + 5 << " " << bulletY + cy + viewX + 5 << endl << endl;
    //cout << DEGREES(angularStep2) << endl;
    //cout << enemyX << " " << enemyY << endl;
    //cout << health << endl;
}


void Tema1::FrameEnd()
{
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if ((window->KeyHold(GLFW_KEY_W)) && (cy + viewY <= mapY - error_factor))
        if (collisionUp(cx, cy, posVector[0]) == true)
            if (collisionUp(cx, cy, posVector[1]) == true)
                if (collisionUp(cx, cy, posVector[2]) == true)
                    if (collisionUp(cx, cy, posVector[3]) == true)
                        if (collisionUp(cx, cy, posVector[4]) == true)
                            if (collisionUp(cx, cy, posVector[5]) == true)
                                cy += 250 * deltaTime;
    if ((window->KeyHold(GLFW_KEY_S)) && (cy + viewY >= error_factor))
        if (collisionDown(cx, cy, posVector[0]) == true)
            if (collisionDown(cx, cy, posVector[1]) == true)
                if (collisionDown(cx, cy, posVector[2]) == true)
                    if (collisionDown(cx, cy, posVector[3]) == true)
                        if (collisionDown(cx, cy, posVector[4]) == true)
                            if (collisionDown(cx, cy, posVector[5]) == true)
                                cy -= 250 * deltaTime;
    if ((window->KeyHold(GLFW_KEY_D)) && (cx + viewX <= mapX - error_factor))
        if (collisionRight(cx, cy, posVector[0]) == true)
            if (collisionRight(cx, cy, posVector[1]) == true)
                if (collisionRight(cx, cy, posVector[2]) == true)
                    if (collisionRight(cx, cy, posVector[3]) == true)
                        if (collisionRight(cx, cy, posVector[4]) == true)
                            if (collisionRight(cx, cy, posVector[5]) == true)
                                cx += 250 * deltaTime;
    if ((window->KeyHold(GLFW_KEY_A)) && (cx + viewX >= error_factor))
        if (collisionLeft(cx, cy, posVector[0]) == true)
            if (collisionLeft(cx, cy, posVector[1]) == true)
                if (collisionLeft(cx, cy, posVector[2]) == true)
                    if (collisionLeft(cx, cy, posVector[3]) == true)
                        if (collisionLeft(cx, cy, posVector[4]) == true)
                            if (collisionLeft(cx, cy, posVector[5]) == true)
                                cx -= 250 * deltaTime;

    switch (positioning)
    {
        case 0:
            pozX = 180;
            pozY = 230;
            if ((collisionEnemy(enemyX + pozX, enemyY + pozY, cx, cy) == true) || (collisionBullet(cx + viewY, cy + viewY, enemyX + pozX, enemyY + pozY, bulletX, bulletY) == true))
            {
                positioning++;
                score += 100;
                enemyX = 0;
                enemyY = 0;
            }
            break;
        case 1:
            pozX = 580;
            pozY = 610;
            if ((collisionEnemy(enemyX + pozX, enemyY + pozY, cx, cy) == true) || (collisionBullet(cx + viewY, cy + viewY, enemyX + pozX, enemyY + pozY, bulletX, bulletY) == true))
            {
                positioning++;
                score += 100;
                enemyX = 0;
                enemyY = 0;
            }
            break;
        case 2:
            pozX = 1000;
            pozY = 300;
            if ((collisionEnemy(enemyX + pozX, enemyY + pozY, cx, cy) == true) || (collisionBullet(cx + viewY, cy + viewY, enemyX + pozX, enemyY + pozY, bulletX, bulletY) == true))
            {
                positioning++;
                score += 100;
                enemyX = 0;
                enemyY = 0;
            }
            break;
        case 3:
            pozX = 550;
            pozY = 100;
            if ((collisionEnemy(enemyX + pozX, enemyY + pozY, cx, cy) == true) || (collisionBullet(cx + viewY, cy + viewY, enemyX + pozX, enemyY + pozY, bulletX, bulletY) == true))
            {
                positioning = 0;
                score += 100;
                enemyX = 0;
                enemyY = 0;
            }
            break;
    }

    switch (healPos)
    {
        case 0:
            healX = 1200;
            healY = 545;
            if (collisionEnemy(healX, healY, cx, cy) == true)
            {
                healPos = 1;
                if (health < 100)
                    health += 25;
            }
            break;
        case 1:
            healX = 230;
            healY = 140;
            if (collisionEnemy(healX, healY, cx, cy) == true)
            {
                healPos = 0;
                if (health < 100)
                    health += 25;
            }
            break;
    }

    switch (pickUp)
    {
    case 0:
        pickUpX = 55;
        pickUpY = 670;
        bulletScaleMap = 5;
        if (collisionEnemy(pickUpX, pickUpY, cx, cy) == true)
        {
            speed = 1000;
            bulletScale = 5;
            pickUp = 1;
        }
        break;
    case 1:
        pickUpX = 640;
        pickUpY = 50;
        bulletScaleMap = 8;
        if (collisionEnemy(pickUpX, pickUpY, cx, cy) == true)
        {
            speed = 500;
            bulletScale = 8;
            pickUp = 0;
        }
        break;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float Y = window->GetResolution().y - mouseY;
    float X = mouseX;
    centerX = cx + viewX;
    centerY = cy + viewY;
    if (X - centerX >= 0)
        angularStep = -atan((Y - centerY) / (X - centerX));
    else
        angularStep = 3.14f - atan((Y - centerY) / (X - centerX));
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1)
    {
        cursorX = mouseX;
        cursorY = window->GetResolution().y - mouseY;
        centerX = cx + viewX;
        centerY = cy + viewY;
        if (cursorX - centerX >= 0)
            angularStep2 = -atan((cursorY - centerY) / (cursorX - centerX));
        else
            angularStep2 = 3.14f - atan((cursorY - centerY) / (cursorX - centerX));
        shot = true;
    }
    if (button != 1)
        shot = false;
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
