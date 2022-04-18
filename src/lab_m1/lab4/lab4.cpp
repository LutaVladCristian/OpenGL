#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;
    Mesh* mesh2 = new Mesh("planeta");
    mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh2->GetMeshID()] = mesh2;
    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 0.5f;
    scaleY = 0.5f;
    scaleZ = 0.5f;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    angularStepOYplaneta = 0;
    v1 = 5.0;
    v2 = 10.0;
    d1 = 1.5;
    d2 = 0.75;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    angularStepOYplaneta += deltaTimeSeconds;
    v1 += deltaTimeSeconds * 0.5;
    v2 += deltaTimeSeconds * 0.75;

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    //planeta1
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-7.0f, 1.5f, 7.0f);
    modelMatrix *= transform3D::Scale(1, 1, 1);
    modelMatrix *= transform3D::RotateOY(angularStepOYplaneta);
    RenderMesh(meshes["planeta"], shaders["VertexNormal"], modelMatrix);

    //planeta2
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-7.0f, 1.5f, 7.0f);
    modelMatrix *= transform3D::RotateOY(v1);
    modelMatrix *= transform3D::Translate(7.0f, -1.5f, -7.0f);
    modelMatrix *= transform3D::Translate(-7.0f + d1, 1.5f, 7.0f - d1);
    modelMatrix *= transform3D::RotateOY(angularStepOYplaneta);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["planeta"], shaders["VertexNormal"], modelMatrix);

    //planeta3
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-7.0f, 1.5f, 7.0f);
    modelMatrix *= transform3D::RotateOY(v1);
    modelMatrix *= transform3D::Translate(7.0f, -1.5f, -7.0f);
    modelMatrix *= transform3D::Translate(-7.0f + d1, 1.5f, 7.0f - d1);
    modelMatrix *= transform3D::RotateOY(v2);
    modelMatrix *= transform3D::Translate(7.0f - d1, -1.5f, -7.0f + d1);
    modelMatrix *= transform3D::Translate(-7.0f + d1 + d2, 1.5f, 7.0f - d1 - d2);
    modelMatrix *= transform3D::RotateOY(angularStepOYplaneta);
    modelMatrix *= transform3D::Scale(0.25, 0.25, 0.25);
    RenderMesh(meshes["planeta"], shaders["VertexNormal"], modelMatrix);
}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if (window->KeyHold(GLFW_KEY_W))
        translateX += 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_S))
        translateX -= 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_A))
        translateZ -= 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_D))
        translateZ += 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_R))
        translateY += 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_F))
        translateY -= 10 * deltaTime;

    if (window->KeyHold(GLFW_KEY_1))
    {
        scaleX += 10 * deltaTime;
        scaleY += 10 * deltaTime;
        scaleZ += 10 * deltaTime;
        if (modeP == true)
            v1 += 2 * deltaTime;
        if (modeP == false)
            v2 += 2 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_2))
    {
        scaleX -= 10 * deltaTime;
        scaleY -= 10 * deltaTime;
        scaleZ -= 10 * deltaTime;
        if (modeP == true)
            v1 -= 2 * deltaTime;
        if (modeP == false)
            v2 -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3))
    {
        angularStepOX -= 10 * deltaTime;
        if (modeP == true)
            d1 += 2 * deltaTime;
        if (modeP == false)
            d2 += 2 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_4))
    {
        angularStepOX += 10 * deltaTime;
        if (modeP == true)
            d1 -= 2 * deltaTime;
        if (modeP == false)
            d2 -= 2 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_5))
        angularStepOY += 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_6))
        angularStepOY -= 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_7))
        angularStepOZ += 10 * deltaTime;
    if (window->KeyHold(GLFW_KEY_8))
        angularStepOZ -= 10 * deltaTime;
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_0)
    {
        switch (modeP)
        {
        case true:
            modeP = false;
            break;
        case false:
            modeP = true;
            break;
        default:
            modeP = true;
            break;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
