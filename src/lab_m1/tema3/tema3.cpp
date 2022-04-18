#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    //declarare mesh-uri
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        mapTextures["random"] = CreateRandomTexture(16, 16);
    }

    //textura
    {
        Mesh* circle1 = CreateCircle("circle", glm::vec3(0, 0, 0), 1, glm::vec3(1, 1, 1), false);
        AddMeshToList(circle1);
    }

    //declarare shadere
    {
        {
            Shader* shader = new Shader("FloorShader");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderFloor.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderFloor.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders[shader->GetName()] = shader;
        }

        {
            Shader* shader = new Shader("DancerShader");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderDancer.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderDancer.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders[shader->GetName()] = shader;
        }

        {
            Shader* shader = new Shader("GlobeShader");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderGlobe.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderGlobe.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders[shader->GetName()] = shader;
        }

        {
            Shader* shader = new Shader("WallsShader");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderWalls.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderWalls.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders[shader->GetName()] = shader;
        }

        {
            Shader* shader = new Shader("RoofShader");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderRoof.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderRoof.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders[shader->GetName()] = shader;
        }

        {
            Shader* shader = new Shader("SpotlightShader");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderSpotlight.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderSpotlight.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders[shader->GetName()] = shader;
        }
    }

    //valori light shader
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                light_pos[i][j] = glm::vec3(i, 0, j);
        lightPosition = glm::vec3(0, -1, 1);
        lightPosition1 = glm::vec3(1, -0, 0);
        lightDirection = glm::vec3(0, 1, 0);
        lightDirection_spot = lightDirection;
        materialShininess = 50;
        materialKd = 1.5;
        materialKs = 3;
    }

    //generare pozitii si culori lumina
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                colors[i][j] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                colors_wall_E[i][j] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                colors_wall_W[i][j] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                colors_wall_N[i][j] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                colors_wall_S[i][j] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                colors_roof[i][j] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

        for (int i = 0; i < 8; i++)
        {
            floor_light_pos[i] = glm::vec3(-0.1f, 0.2f, i);
            floor_color_lights[i] = colors[0][i];
        }
        for (int i = 0; i < 8; i++)
        {
            floor_light_pos[8 + i] = glm::vec3(i, 0.2f, 7 - 0.1f);
            floor_color_lights[8 + i] = colors[i][7];
        }
        for (int i = 0; i < 8; i++)
        {
            floor_light_pos[16 + i] = glm::vec3(7 - 0.1f, 0.2f, i);
            floor_color_lights[16 + i] = colors[7][i];
        }
        for (int i = 0; i < 8; i++)
        {
            floor_light_pos[24 + i] = glm::vec3(i, 0.2f, -0.1f);
            floor_color_lights[24 + i] = colors[i][0];
        }

        for (int i = 0; i < 4; i++)
        {
            dancer x;
            dancers.push_back(x);
        }

        spot_colors[0] = glm::vec3(0, 0.7, 0.5);
        spot_colors[1] = glm::vec3(0.6, 0, 0);
        spot_colors[2] = glm::vec3(0.9, 0.5, 0.3);
        spot_colors[3] = glm::vec3(0, 0, 1);

        spot_pos[0] = glm::vec3(2, 6, 2);
        spot_pos[1] = glm::vec3(5, 6, 2);
        spot_pos[2] = glm::vec3(5, 6, 5);
        spot_pos[3] = glm::vec3(2, 6, 5);

        dancers[0].translate = glm::vec3(3, 1, 3);
        dancers[1].translate = glm::vec3(5, 1, 2);
        dancers[2].translate = glm::vec3(6, 1, 5);
        dancers[3].translate = glm::vec3(3, 1, 5);

        disco_ball_pos = glm::vec3(3.5, 5.5, 3.5);
        mode = 0;
    }
}


void Tema3::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    Time = glfwGetTime();

    //podea
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 0, j));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            RenderSimpleMesh(meshes["box"], shaders["FloorShader"], modelMatrix, mapTextures["random"], colors[i][j]);
        }

    //pereti
    {
        //est
        /*for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, i, j));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            RenderSimpleMesh(meshes["box"], shaders["WallsShader"], modelMatrix, mapTextures["random"], colors_wall_E[i][j], -1, -1, 0);
        }*/

        //vest
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(8, i, j));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
                RenderSimpleMesh(meshes["box"], shaders["WallsShader"], modelMatrix, NULL, colors_wall_W[i][j], -1, -1, 16);
            }

        //nord
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i, j, -1));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
                RenderSimpleMesh(meshes["box"], shaders["WallsShader"], modelMatrix, NULL, colors_wall_N[i][j], -1, -1, 24);
            }

        //sud
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i, j, 8));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
                RenderSimpleMesh(meshes["box"], shaders["WallsShader"], modelMatrix, NULL, colors_wall_S[i][j], -1, -1, 8);
            }

        //acoperis
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 8, j));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
                RenderSimpleMesh(meshes["box"], shaders["RoofShader"], modelMatrix, NULL, colors_roof[i][j]);
            }
    }

    //dansatori
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 1, 0.5));
        row = (int)floor(dancers[0].position.x + 0.25f);
        col = (int)floor(dancers[0].position.z + 0.25f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["box"], shaders["DancerShader"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);

        if (Time < 5)
            translate = dancers[0].translate + glm::vec3(0.5 * sin(0.7*Time) * sin(0.7*1.5 * Time), 0.0f, 1 * cos(0.7*1.5 * Time));

        else if (Time >= 5 && Time < 10)
            translate = dancers[0].translate + glm::vec3(cos(Time - 5)*0.5*sin(0.7 * Time) * sin(0.7 * 1.5 * Time) + sin(Time - 5), 0.0f, cos(Time - 5) * 1 * cos(0.7 * 1.5 * Time));
    
        else
            translate = dancers[0].translate + glm::vec3(cos(2 * Time - 20) * cos(Time - 5) * 0.5 * sin(0.7 * Time) * sin(0.7 * 1.5 * Time) + sin(Time - 5) + 0.5f* sin(Time - 10), 0.0f, cos(Time - 5) * 1 * cos(0.7 * 1.5f * Time) * cos(1.5 * Time - 15) + 0.5f*sin(Time - 10));
    
        dancers[0].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 1, 0.5f));
        row = (int)floor(dancers[1].position.x + 0.25f);
        col = (int)floor(dancers[1].position.z + 0.25f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["box"], shaders["DancerShader"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);
    
        if (Time < 3)
            translate = dancers[1].translate + glm::vec3(2 * cos(Time) * sin(1.5 * Time), 0.0f, 1 * cos(1.5 * Time) + sin(Time));
    
        else if (Time >= 3 && Time < 8)
            translate = dancers[1].translate + glm::vec3(cos(Time - 3) * 2 * cos(Time) * sin(1.5 * Time) + sin(Time - 3), 0.0f, cos(1.5 * Time - 4.5f) * (1 * cos(1.5 * Time) + sin(Time)));
    
        else
            translate = dancers[1].translate + glm::vec3(cos(2 * Time - 16) * (cos(Time - 3) * 2 * cos(Time) * sin(1.5 * Time) + sin(Time - 3)) + 0.6*sin(Time - 8), 0.0f, (cos(1.5 * Time - 4.5f) * (1 * cos(1.5 * Time) + sin(Time))) * cos(1.5 * Time - 1.5 * 8) + 2*sin(Time - 8));
    
        dancers[1].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 1, 0.5f));
        row = (int)floor(dancers[2].position.x + 0.25f);
        col = (int)floor(dancers[2].position.z + 0.25f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["box"], shaders["DancerShader"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);
    
        if (Time < 6)
            translate = dancers[2].translate + glm::vec3(cos(Time) * sin(2 * Time), 0.0f, 1.5 * sin(1.5 * Time) + cos(Time) * sin(1.5f * Time));
    
        else if (Time >= 6 && Time < 13)
            translate = dancers[2].translate + glm::vec3(cos(1.5 * Time - 9) * cos(Time) * sin(2 * Time) + sin(Time - 6), 0.0f, cos(1.2 * Time - 1.2 * 6) * (1.5 * sin(1.5 * Time) + cos(Time) * sin(1.5f * Time)) + 1.4 * sin(Time - 6));
    
        else
            translate = dancers[2].translate + glm::vec3(cos(1.1 * Time - 13 * 1.1) * (cos(1.5 * Time - 9) * cos(Time) * sin(2 * Time) + sin(Time - 6)) + 0.1 * sin(Time - 13), 0.0f, cos(1.5 * Time - 1.5 * 13) * (cos(1.2 * Time - 1.2 * 6) * (1.5 * sin(1.5 * Time) + cos(Time) * sin(1.5f * Time)) + 1.4 * sin(Time - 6)) + 0.85 * sin(2 * Time - 26));
    
        dancers[2].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 1, 0.5f));
        row = (int)floor(dancers[3].position.x + 0.25f);
        col = (int)floor(dancers[3].position.z + 0.25f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["box"], shaders["DancerShader"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);
        
        if (Time < 7)
            translate = dancers[3].translate + glm::vec3(cos(1.5 * Time) * sin(Time), 0.0f, 1 * sin(1.5 * Time) + sin(2 * Time) * sin(1.5f * Time));
        
        else if (Time >= 7 && Time < 15)
            translate = dancers[3].translate + glm::vec3(cos(1.6 * Time - 1.6 * 7) * cos(1.5 * Time) * sin(Time) + 2.7 * sin(1.3 * Time - 1.3 * 7), 0.0f, cos(1.45 * Time - 1.45 * 7) * (1 * sin(1.5 * Time) + sin(2 * Time) * sin(1.5f * Time)) + 1.5 * sin(1.3 * Time - 1.3 * 7));
        
        else
            translate = dancers[3].translate + glm::vec3(cos(1.3 * Time - 1.3 * 15) * (cos(1.6 * Time - 1.6 * 7) * cos(1.5 * Time) * sin(Time) + 2.7 * sin(1.3 * Time - 1.3 * 7)) + 0.8 * sin(1.2 * Time - 1.2 * 15), 0.0f, cos(1.22 * Time - 1.22 * 15) * (cos(1.45 * Time - 1.45 * 7) * (1 * sin(1.5 * Time) + sin(2 * Time) * sin(1.5f * Time)) + 1.5 * sin(1.3 * Time - 1.3 * 7)) + 1.8 * sin(1.22 * Time - 1.22 * 15));
        
        dancers[3].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);
    }

    //glob
    if (mode % 3 == 2 || all % 2 != 0)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, disco_ball_pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
        RenderSimpleMesh(meshes["sphere"], shaders["GlobeShader"], modelMatrix, mapTextures["random"]);
    }

    //proprietati lumina
    {
        angleOZ = 0.3f * sin(2.f * Time);
        angle_OZ_aux = 0.6 * sin(2 * Time);
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, angleOY, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, angleOZ, glm::vec3(0, 0, 1));
        lightDirection_spot = lightDirection;
        lightDirection_spot = glm::vec3(modelMatrix * glm::vec4(lightDirection, 1));
    }

    //reflectoare
    if (mode % 3 == 1 || all % 2 != 0)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[0]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        RenderSimpleMesh(meshes["circle"], shaders["SpotlightShader"], modelMatrix, NULL, spot_colors[0]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[1]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        RenderSimpleMesh(meshes["circle"], shaders["SpotlightShader"], modelMatrix, NULL, spot_colors[1]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[2]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        RenderSimpleMesh(meshes["circle"], shaders["SpotlightShader"], modelMatrix, NULL, spot_colors[2]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[3]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        RenderSimpleMesh(meshes["circle"], shaders["SpotlightShader"], modelMatrix, NULL, spot_colors[3]);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
    }
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    for (int i = 0; i < size; i++)
        data[i] = rand() % 256;
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


void Tema3::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, const glm::vec3& color, int row, int col, int index)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    if (row != -1 && col != -1)
    {
        light_pos_dancer[0] = glm::vec3(row, 0, col);
        color_pos_dancer[0] = colors[row][col];

        if (row > 1)
        {
            light_pos_dancer[1] = glm::vec3(row - 1, 0, col);
            color_pos_dancer[1] = colors[row - 1][col];
        }
        else
        {
            light_pos_dancer[1] = glm::vec3(0);
            color_pos_dancer[1] = glm::vec3(0);
        }

        if (row < 7)
        {
            light_pos_dancer[2] = glm::vec3(row + 1, 0, col);
            color_pos_dancer[2] = colors[row + 1][col];
        }
        else
        {
            light_pos_dancer[2] = glm::vec3(0);
            color_pos_dancer[2] = glm::vec3(0);
        }

        if (col > 1)
        {
            light_pos_dancer[3] = glm::vec3(row, 0, col - 1);
            color_pos_dancer[3] = colors[row][col - 1];
        }
        else
        {
            light_pos_dancer[3] = glm::vec3(0);
            color_pos_dancer[3] = glm::vec3(0);
        }

        if (col < 7)
        {
            light_pos_dancer[4] = glm::vec3(row, 0, col + 1);
            color_pos_dancer[4] = colors[row][col + 1];
        }
        else
        {
            light_pos_dancer[4] = glm::vec3(0);
            color_pos_dancer[4] = glm::vec3(0);
        }

        if (row > 1 && col > 1)
        {
            light_pos_dancer[5] = glm::vec3(row - 1, 0, col - 1);
            color_pos_dancer[5] = colors[row - 1][col - 1];
        }
        else
        {
            light_pos_dancer[5] = glm::vec3(0);
            color_pos_dancer[5] = glm::vec3(0);
        }

        if (row > 1 && col < 7)
        {
            light_pos_dancer[6] = glm::vec3(row - 1, 0, col + 1);
            color_pos_dancer[6] = colors[row - 1][col + 1];
        }
        else
        {
            light_pos_dancer[6] = glm::vec3(0);
            color_pos_dancer[6] = glm::vec3(0);
        }

        if (row < 7 && col < 7)
        {
            light_pos_dancer[7] = glm::vec3(row + 1, 0, col + 1);
            color_pos_dancer[7] = colors[row + 1][col + 1];
        }
        else
        {
            light_pos_dancer[7] = glm::vec3(0);
            color_pos_dancer[7] = glm::vec3(0);
        }

        if (row < 7 && col > 1)
        {
            light_pos_dancer[8] = glm::vec3(row + 1, 0, col - 1);
            color_pos_dancer[8] = colors[row + 1][col - 1];
        }
        else
        {
            light_pos_dancer[8] = glm::vec3(0);
            color_pos_dancer[8] = glm::vec3(0);
        }

        glUniform3fv(glGetUniformLocation(shader->program, "light_pos"), 9, glm::value_ptr(light_pos_dancer[0]));

        glUniform3fv(glGetUniformLocation(shader->program, "color_box"), 9, glm::value_ptr(color_pos_dancer[0]));

        glUniform3fv(glGetUniformLocation(shader->program, "light_pos_1"), 9, glm::value_ptr(light_pos_dancer[0]));

        glUniform3fv(glGetUniformLocation(shader->program, "color_box_1"), 9, glm::value_ptr(color_pos_dancer[0]));
    }
    else if (index != -1)
    {
        glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 8,
            glm::value_ptr(floor_light_pos[index]));

        glUniform3fv(glGetUniformLocation(shader->program, "color_box"), 8,
            glm::value_ptr(floor_color_lights[index]));

        int light_pos = glGetUniformLocation(shader->program, "disco_light_pos");
        glUniform3f(light_pos, disco_ball_pos.x, disco_ball_pos.y, disco_ball_pos.z);
    }

    int light_pos = glGetUniformLocation(shader->program, "disco_light_pos");
    glUniform3f(light_pos, disco_ball_pos.x, disco_ball_pos.y, disco_ball_pos.z);

    glUniform3fv(glGetUniformLocation(shader->program, "spot_position"), 4, glm::value_ptr(spot_pos[0]));

    glUniform3fv(glGetUniformLocation(shader->program, "color_spot"), 4, glm::value_ptr(spot_colors[0]));

    glUniform3fv(glGetUniformLocation(shader->program, "spot_position_1"), 4, glm::value_ptr(spot_pos[0]));

    glUniform3fv(glGetUniformLocation(shader->program, "color_spot_1"), 4, glm::value_ptr(spot_colors[0]));

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    int light_direction_spot = glGetUniformLocation(shader->program, "light_direction_spot");
    glUniform3f(light_direction_spot, lightDirection_spot.x, lightDirection_spot.y, lightDirection_spot.z);

    int mode_loc = glGetUniformLocation(shader->program, "mode");
    glUniform1i(mode_loc, mode);

    int light_position = glGetUniformLocation(shader->program, "light_position_roof");
    glUniform3f(light_position, disco_ball_pos.x, disco_ball_pos.y, disco_ball_pos.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    GLfloat angle = glGetUniformLocation(shader->program, "cuttOffAngle");
    glUniform1f(angle, cutt_off_angle);

    GLint isAll = glGetUniformLocation(shader->program, "is_all");
    glUniform1i(isAll, all);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    float Time = Engine::GetElapsedTime();
    GLint loc_Time_matrix = glGetUniformLocation(shader->program, "Time");
    glUniform1f(loc_Time_matrix, Time);
}


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

    if (true)
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        if (window->KeyHold(GLFW_KEY_W))
        {
            lightPosition -= forward * deltaTime * speed;
            lightPosition1 -= forward * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_A))
        {
            lightPosition -= right * deltaTime * speed;
            lightPosition1 -= right * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_S))
        {
            lightPosition += forward * deltaTime * speed;
            lightPosition1 += forward * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            lightPosition += right * deltaTime * speed;
            lightPosition1 += right * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_E))
        {
            lightPosition += up * deltaTime * speed;
            lightPosition1 += up * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_Q))
        {
            lightPosition -= up * deltaTime * speed;
            lightPosition1 -= up * deltaTime * speed;
        }

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, angleOY, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, angleOZ, glm::vec3(0, 0, 1));

        lightDirection = glm::vec3(0, -1, 0);
        lightDirection = glm::vec3(modelMatrix * glm::vec4(lightDirection, 1));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
        mode++;

    if (key == GLFW_KEY_F)
        all++;
}


void Tema3::OnKeyRelease(int key, int mods)
{
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
