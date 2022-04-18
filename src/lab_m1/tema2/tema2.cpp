#include "lab_m1/tema2/tema2.h"


using namespace std;
using namespace m1;


bool collisionEnemyTema2(float enemyX, float enemyZ, float cx, float cy)
{
    float distance = sqrt(pow(cx - enemyX, 2) + pow(cy - enemyZ, 2));
    if (distance < 0.75)
        return true;
    else
        return false;
}


bool collisionBulletTema2(float cx, float cz, float enemyX, float enemyZ, float bulletX, float bulletZ)
{
    float x, z;
    x = max(cx + bulletX, min(enemyX, cx + bulletX));
    z = max(cz + bulletZ, min(enemyZ, cz + bulletZ));
    float dist = sqrt(pow(cx + bulletX - enemyX, 2) + pow(cz + bulletZ - enemyZ, 2));
    if ((dist < 0.75f))
        return true;
    else
        return false;
}


bool Tema2::collisionPlayerWall(glm::vec3 direction)
{
    glm::vec3 player_position = camera->GetTargetPosition();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (matrix[i][j].wall[0])//perete dreapta
            {
                if (player_position.x >= j * square_side + square_side - camera->distanceToTarget
                    && player_position.x <= j * square_side + square_side + 0.1 * camera->distanceToTarget
                    && player_position.z >= i * square_side
                    && player_position.z <= square_side + i * square_side
                    && direction.x >= 0)
                {
                    return true;
                }
            }

            if (matrix[i][j].wall[1])//perete stanga
            {
                if (player_position.x >= j * square_side - 0.1 * camera->distanceToTarget
                    && player_position.x <= j * square_side + camera->distanceToTarget
                    && player_position.z >= i * square_side
                    && player_position.z <= square_side + i * square_side
                    && direction.x <= 0)
                {
                    return true;
                }
            }

            if (matrix[i][j].wall[2])//perete jos
            {
                if (player_position.x >= j * square_side
                    && player_position.x <= j * square_side + square_side
                    && player_position.z >= i * square_side + square_side - camera->distanceToTarget
                    && player_position.z <= i * square_side + square_side + 0.1 * camera->distanceToTarget
                    && direction.z >= 0)
                {
                    return true;
                }
            }

            if (matrix[i][j].wall[3])//perete sus
            {
                if (player_position.x >= j * square_side 
                    && player_position.x <= j * square_side + square_side
                    && player_position.z >= i * square_side - 0.1 * camera->distanceToTarget
                    && player_position.z <= i * square_side + camera->distanceToTarget
                    && direction.z <= 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}


void Tema2::SetMaze(cell matrix[5][5])
{
    stack <cell> stack;
    int random = -1;
    int start_row = 2;
    int start_col = 2;
    cell current(start_row, start_col, matrix[start_row][start_col].value, matrix[start_row][start_col].visited);
    int row = start_row, col = start_col;
    vector <int> posibilities;
    current.visited = true;
    current.value = 0;
    matrix[current.row][current.col].visited = true;
    matrix[current.row][current.col].value = 0;
    stack.push(current);
    int stack_elem = 1;
    int iter = 0;

    while (!stack.empty())
    {
        if (current.col + 1 < 5 && !matrix[current.row][current.col + 1].visited)
            posibilities.push_back(0);

        if (current.col - 1 >= 0 && !matrix[current.row][current.col - 1].visited)
            posibilities.push_back(1);

        if (current.row + 1 < 5 && !matrix[current.row + 1][current.col].visited)
            posibilities.push_back(2);

        if (current.row - 1 >= 0 && !matrix[current.row - 1][current.col].visited)
            posibilities.push_back(3);

        for (int i = 0; i < posibilities.size(); i++)
            if (posibilities.size())
                random = posibilities[rand() % (posibilities.size())];

        if ((current.col == 4 || current.col == 0 || current.row == 4 || current.row == 0) && iter < 2)
        {
            switch (current.col)
            {
            case 4:
                iter++;
                matrix[current.row][current.col].wall[0] = false;

            case 0:
                iter++;
                matrix[current.row][current.col].wall[1] = false;
            }

            switch (current.row)
            {
            case 4:
                iter++;
                matrix[current.row][current.col].wall[3] = false;
            case 0:
                iter++;
                matrix[current.row][current.col].wall[2] = false;
            }
        }

        if (stack_elem == 25)
        {
            break;
        }

        else if (posibilities.size() == 0)
        {
            current.row = stack.top().row;
            current.col = stack.top().col;
            stack.pop();
        }

        else if (random == 0)
        {
            current.col++;
            matrix[current.row][current.col].visited = true;
            matrix[current.row][current.col].value = 0;
            matrix[current.row][current.col - 1].wall[0] = false;
            matrix[current.row][current.col].wall[1] = false;
            stack.push(current);
            stack_elem++;
        }

        else if (random == 1)
        {
            current.col--;
            matrix[current.row][current.col].visited = true;
            matrix[current.row][current.col].value = 0;
            matrix[current.row][current.col + 1].wall[1] = false;
            matrix[current.row][current.col].wall[0] = false;
            stack.push(current);
            stack_elem++;
        }

        else if (random == 2)
        {
            current.row++;
            matrix[current.row][current.col].visited = true;
            matrix[current.row][current.col].value = 0;
            matrix[current.row - 1][current.col].wall[2] = false;
            matrix[current.row][current.col].wall[3] = false;
            stack.push(current);
            stack_elem++;
        }

        else if (random == 3)
        {
            current.row--;
            matrix[current.row][current.col].visited = true;
            matrix[current.row][current.col].value = 0;
            matrix[current.row + 1][current.col].wall[3] = false;
            matrix[current.row][current.col].wall[2] = false;
            stack.push(current);
            stack_elem++;
        }

        posibilities.erase(posibilities.begin(), posibilities.end());
    }
}


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderPlayer = true;

    camera = new implemented::Camera2();
    camera->Set(glm::vec3(5.f, 1.05f, 5.f), glm::vec3(5.64f, 0.854f, 5.75f), glm::vec3(0.06f, 1.f, 0.06f));

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("podea");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load textures
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures");
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "floor.jpg").c_str(), GL_REPEAT);
        mapTextures["ground"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wall.jpg").c_str(), GL_REPEAT);
        mapTextures["walls"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "tower.jpg").c_str(), GL_REPEAT);
        mapTextures["tower"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "sun.jpg").c_str(), GL_REPEAT);
        mapTextures["sun"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "moon.jpg").c_str(), GL_REPEAT);
        mapTextures["moon"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "bullet.jpg").c_str(), GL_REPEAT);
        mapTextures["bullet"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "military.png").c_str(), GL_REPEAT);
        mapTextures["military"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "head.png").c_str(), GL_REPEAT);
        mapTextures["head"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "enemy.jpg").c_str(), GL_REPEAT);
        mapTextures["enemy"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "enemy2.jpg").c_str(), GL_REPEAT);
        mapTextures["enemy2"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "enemy3.jpg").c_str(), GL_REPEAT);
        mapTextures["enemy3"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "health.png").c_str(), GL_REPEAT);
        mapTextures["health"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "time.png").c_str(), GL_REPEAT);
        mapTextures["time"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "heal.jpg").c_str(), GL_REPEAT);
        mapTextures["heal"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "time3.jpg").c_str(), GL_REPEAT);
        mapTextures["timePickUp"] = texture;
    }

    // Create a shader program
    {
        Shader* shader = new Shader("MazeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderMaze.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderMaze.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("PlayerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderPlayer.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderPlayer.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("EnemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderEnemy.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderEnemy.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 300.0f);

    SetMaze(matrix);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.1f, 0.f, 0.4f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    glm::mat4 modelMatrix = glm::mat4(1);

    //moon
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(150.0f, 35.f, 50.f);
        modelMatrix *= transform3D::Scale(30.f, 25.f, 30.f);
        RenderSimpleMesh(meshes["sphere"], shaders["MazeShader"], modelMatrix, mapTextures["moon"]);
    }

    //sun
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(50.0f, 35.f, 150.f);
        modelMatrix *= transform3D::Scale(30.f, 25.f, 30.f);
        RenderSimpleMesh(meshes["sphere"], shaders["MazeShader"], modelMatrix, mapTextures["sun"]);
    }

    //maze
    {
        for (int i = 0; i < mazeHeight; i++)
            for (int j = 0; j < mazeWidth; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    if (matrix[i][j].wall[k])
                    {
                        if (k == 0)//dreapta
                        {
                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            modelMatrix = glm::mat4(1);
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side + square_side, 1.f, square_side / 2 + i * square_side));
                            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 1.f, 6.f));
                            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["walls"]);
                        }

                        if (k == 1)//stanga
                        {
                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            modelMatrix = glm::mat4(1);
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.f, square_side / 2 + i * square_side));
                            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 1.f, 6.f));
                            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["walls"]);
                        }

                        if (k == 2)//jos
                        {
                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            modelMatrix = glm::mat4(1);
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(square_side / 2 + j * square_side, 1.f, i * square_side + square_side));
                            modelMatrix = glm::scale(modelMatrix, glm::vec3(6.f, 1.f, 1.f));
                            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["walls"]);
                        }

                        if (k == 3)//sus
                        {
                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            {
                                modelMatrix = glm::mat4(1);
                                modelMatrix = glm::translate(modelMatrix, glm::vec3(j * square_side, 1.5f, i * square_side));
                                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
                            }

                            modelMatrix = glm::mat4(1);
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(square_side / 2 + j * square_side, 1.f, i * square_side));
                            modelMatrix = glm::scale(modelMatrix, glm::vec3(6.f, 1.f, 1.f));
                            RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["walls"]);
                        }
                    }
                }
            }

        for (int i = 0; i <= mazeHeight; i++)
        {
            {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(mazeHeight * square_side, 1.5f, i * square_side));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
            }

            {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i * square_side, 1.5f, mazeHeight * square_side));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.5f, 1.3f));
                RenderSimpleMesh(meshes["box"], shaders["MazeShader"], modelMatrix, mapTextures["tower"]);
            }
        }

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(30.f, 0.f, 30.f);
        modelMatrix *= transform3D::Scale(10.f, 0.f, 10.f);
        RenderSimpleMesh(meshes["podea"], shaders["MazeShader"], modelMatrix, mapTextures["ground"]);
    }

    //player
    if (renderPlayer)
    {
        //picior stanga
        foot = 1;
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.f, -0.485f, 0.f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::Translate(0.05f, 0.f, 0.f);
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Translate(-0.03f, 0.f, 0.f);
        modelMatrix *= transform3D::Scale(human_scaleX, human_scaleY, human_scaleZ);
        RenderSimpleMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, mapTextures["military"]);

        //picior dreapta
        rightFoot = 1;
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.1f, -0.485f, 0.f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::Translate(-0.05f, 0.f, 0.f);
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Translate(0.03f, 0.f, 0.f);
        modelMatrix *= transform3D::Scale(human_scaleX, human_scaleY, human_scaleZ);
        RenderSimpleMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, mapTextures["military"]);
        foot = 0;
        rightFoot = 0;

        //corp
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.05f, -0.275f, 0.f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Scale(human_scaleX + 0.025f, human_scaleY, human_scaleZ + 0.025f);
        RenderSimpleMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, mapTextures["military"]);

        //mana stanga
        hand = 1;
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(-0.08f, -0.265f, 0.f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::Translate(0.05f + 0.08f, 0.f, 0.f);
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Translate(-0.005f - 0.08f, 0.f, 0.f);
        modelMatrix *= transform3D::Scale(human_scaleX, human_scaleY, human_scaleZ);
        RenderSimpleMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, mapTextures["military"]);

        //mana dreapta
        rightHand = 1;
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.18f, -0.265f, 0.f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::Translate(-0.05f - 0.08f, 0.f, 0.f);
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Translate(0.005f + 0.08f, 0.f, 0.f);
        modelMatrix *= transform3D::Scale(human_scaleX, human_scaleY, human_scaleZ);
        RenderSimpleMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, mapTextures["military"]);
        hand = 0;
        rightHand = 0;

        //cap
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.05f, -0.1f, 0.f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Scale(0.1f, 0.1f, 0.1f);
        RenderSimpleMesh(meshes["sphere"], shaders["PlayerShader"], modelMatrix, mapTextures["head"]);

        //health_bar
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.f, 0.95f, 0.0f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Scale(0.002f * health, 0.05f, 0.001f);
        if (health >= 0)
            RenderSimpleMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, mapTextures["health"]);

        //time_bar
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.f, 0.85f, 0.0f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= transform3D::RotateOY(angle);
        modelMatrix *= transform3D::Scale(0.002f * time, 0.05f, 0.001f);
        if (time >= 0)
            RenderSimpleMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, mapTextures["time"]);
        time -=  2.5 * deltaTimeSeconds;
    }

    if (time <= 0)
    {
        cout << endl;
        cout << "Din pacate nu ai reusit sa termini la timp!" << endl;
        cout << "Revino cand devii mai puternic!!!" << endl;
        cout << "Scorul tau: " << score  << "! :3333" << endl << endl << endl;
        Exit();
    }

    if (health <= 0)
    {
        cout << endl;
        cout << "Din pacate inamicii te-au coplesit!" << endl;
        cout << "Revino cand devii mai puternic!!!" << endl;
        cout << "Scorul tau: " << score << "! :3333" << endl << endl << endl;
        Exit();
    }

    if (camera->GetTargetPosition().x >= 52.5f || camera->GetTargetPosition().z >= 52.5f || camera->GetTargetPosition().x < -0.5f || camera->GetTargetPosition().z < -0.5f)
    {
        cout << endl;
        cout << "Felicitari!" << endl;
        cout << "Ai reusit sa duci provocarea Labirintului Cosmic pana la sfarsit cu succes!!!" << endl;
        cout << "Scorul tau: " << score << "! :3333" << endl << endl << endl;
        Exit();
    }

    //enemy
    {
        //enemy movement
        {
            enemy[0].posX = square_side * 4;  enemy[0].posZ = square_side * 0;
            enemy[1].posX = square_side * 2;  enemy[1].posZ = square_side * 1;
            enemy[2].posX = square_side * 0;  enemy[2].posZ = square_side * 2;
            enemy[3].posX = square_side * 1;  enemy[3].posZ = square_side * 3;
            enemy[4].posX = square_side * 3;  enemy[4].posZ = square_side * 4;
        }

        for (int i = 0; i < 5; i++)
            {
                enemy[i].xDistance = camera->GetTargetPosition().x - (enemy[i].posX + 5.f) - enemy[i].x;
                enemy[i].zDistance = camera->GetTargetPosition().z - (enemy[i].posZ + 5.f) - enemy[i].z;
                Distance = sqrt((enemy[i].xDistance * enemy[i].xDistance) + (enemy[i].zDistance * enemy[i].zDistance));
                if (Distance < 5)
                {
                    enemy[i].x += deltaTimeSeconds * 2 * (enemy[i].xDistance / Distance);
                    enemy[i].z += deltaTimeSeconds * 2 * (enemy[i].zDistance / Distance);
                }
            }

        for (int i = 0; i < 5; i++)
        {
                if (collisionBulletTema2(camera->GetTargetPosition().x, camera->GetTargetPosition().z, enemy[i].x + enemy[i].posX + 5.f, enemy[i].z + enemy[i].posZ + 5.f, bulletX, bulletZ) == true)
                {
                    score += 100;
                    enemy[i].isHit = 1;
                    enemy[i].health -= 10;
                }

                if (collisionBulletTema2(camera->GetTargetPosition().x, camera->GetTargetPosition().z, enemy[i].x + enemy[i].posX + 5.f, enemy[i].z + enemy[i].posZ + 5.f, 0, 0) == true)
                {
                    score += 100;
                    if (enemy[i].health >= 0)
                        health -= 5;
                }

                head = 1;
                modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(enemy[i].posX + 5.f, 0.5f, enemy[i].posZ + 5.f);
                if (enemy[i].health >= 0)
                    modelMatrix *= transform3D::Translate(enemy[i].x, 0, enemy[i].z);
                modelMatrix *= transform3D::RotateOY(angle);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
                if(enemy[i].health >= 60)
                    RenderSimpleMesh(meshes["sphere"], shaders["EnemyShader"], modelMatrix, mapTextures["enemy"]);
                if ((enemy[i].health >= 30) && (enemy[i].health < 60))
                    RenderSimpleMesh(meshes["sphere"], shaders["EnemyShader"], modelMatrix, mapTextures["enemy2"]);
                if ((enemy[i].health >= 0) && (enemy[i].health < 30))
                    RenderSimpleMesh(meshes["sphere"], shaders["EnemyShader"], modelMatrix, mapTextures["enemy3"]);
                head = 0;

                modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(enemy[i].posX + 5.f, 1.f, enemy[i].posZ + 5.f);
                if (enemy[i].health >= 0)
                    modelMatrix *= transform3D::Translate(enemy[i].x, 0, enemy[i].z);
                modelMatrix *= transform3D::RotateOY(angle);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.10f, 0.15f));
                if (enemy[i].health >= 60)
                    RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, mapTextures["enemy"]);
                if ((enemy[i].health >= 30) && (enemy[i].health < 60))
                    RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, mapTextures["enemy2"]);
                if ((enemy[i].health >= 0) && (enemy[i].health < 30))
                    RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, mapTextures["enemy3"]);
                enemy[i].isHit = 0;
        }
    }

    //bullet
    if (renderBullet)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.05f, -0.2f, 0.f);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        if (shot == true)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, camera->forward.y, camera->forward.z));
            bulletX += dir.x * deltaTimeSeconds * bulletSpeed;
            bulletZ += dir.z * deltaTimeSeconds * bulletSpeed;
            modelMatrix *= transform3D::Translate(bulletX, 0, bulletZ);
            if ((bulletX > 5) || (bulletZ > 5) || (bulletX < -5) || (bulletZ < -5))
            {
                bulletX = 0;
                bulletZ = 0;
                shot = false;
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            }
        }
        modelMatrix *= transform3D::Scale(0.075f, 0.075f, 0.075f);
        head = 1;
        RenderSimpleMesh(meshes["sphere"], shaders["EnemyShader"], modelMatrix, mapTextures["bullet"]);
        head = 0;
    }

    //pick_up
    {
        {
            //HEAL
            pickUp[0].posX = square_side * 1;   pickUp[0].posZ = square_side * 1;
            pickUp[1].posX = square_side * 2;   pickUp[1].posZ = square_side * 2;
            pickUp[2].posX = square_side * 0;   pickUp[2].posZ = square_side * 3;
            pickUp[3].posX = square_side * 4;   pickUp[3].posZ = square_side * 4;

            //TIME
            pickUp[4].posX = square_side * 2;   pickUp[4].posZ = square_side * 0;
            pickUp[5].posX = square_side * 4;   pickUp[5].posZ = square_side * 1;
            pickUp[6].posX = square_side * 3;   pickUp[6].posZ = square_side * 3;
            pickUp[7].posX = square_side * 1;   pickUp[7].posZ = square_side * 4;
        }

        for (int i = 0; i <= 3; i++)
        {
            if (collisionBulletTema2(camera->GetTargetPosition().x, camera->GetTargetPosition().z, pickUp[i].posX + 5.f, pickUp[i].posZ + 5.f, 0, 0) == true)
            {
                if (health <= 120)
                    health += 10;
                pickUp[i].isHit = 1;
            }
            pick = 1;
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(pickUp[i].posX + 5.f, 0.5f, pickUp[i].posZ + 5.f);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
            if (pickUp[i].isHit != 1)
                RenderSimpleMesh(meshes["sphere"], shaders["PlayerShader"], modelMatrix, mapTextures["heal"]);
            pick = 0;
        }

        for (int i = 4; i <= 7; i++)
        {
            if (collisionBulletTema2(camera->GetTargetPosition().x, camera->GetTargetPosition().z, pickUp[i].posX + 5.f, pickUp[i].posZ + 5.f, 0, 0) == true)
            {
                if (time <= 120)
                    time += 10;
                pickUp[i].isHit = 1;
            }
            pick = 1;
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(pickUp[i].posX + 5.f, 0.5f, pickUp[i].posZ + 5.f);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
            if (pickUp[i].isHit != 1)
                RenderSimpleMesh(meshes["sphere"], shaders["PlayerShader"], modelMatrix, mapTextures["timePickUp"]);
            pick = 0;
        }
    }

    //cout << " bulletX: " << camera->GetTargetPosition().x << " bulletZ: " << camera->GetTargetPosition().z << endl;
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);
    
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "Time");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    int isHand = glGetUniformLocation(shader->GetProgramID(), "hand");
    glUniform1i(isHand, hand);

    int isRightHand = glGetUniformLocation(shader->GetProgramID(), "rightHand");
    glUniform1i(isRightHand, rightHand);

    int isFoot = glGetUniformLocation(shader->GetProgramID(), "foot");
    glUniform1i(isFoot, foot);

    int isAnimation = glGetUniformLocation(shader->GetProgramID(), "animation");
    glUniform1i(isAnimation, animation);

    int isRightFoot = glGetUniformLocation(shader->GetProgramID(), "rightFoot");
    glUniform1i(isRightFoot, rightFoot);

    int isHead = glGetUniformLocation(shader->GetProgramID(), "head");
    glUniform1i(isHead, head);

    int isHit = glGetUniformLocation(shader->GetProgramID(), "isHit[0]");
    glUniform1i(isHit, enemy[0].isHit);

    int isHit1 = glGetUniformLocation(shader->GetProgramID(), "isHit[1]");
    glUniform1i(isHit1, enemy[1].isHit);

    int isHit2 = glGetUniformLocation(shader->GetProgramID(), "isHit[2]");
    glUniform1i(isHit2, enemy[2].isHit);

    int isHit3 = glGetUniformLocation(shader->GetProgramID(), "isHit[3]");
    glUniform1i(isHit3, enemy[3].isHit);

    int isHit4 = glGetUniformLocation(shader->GetProgramID(), "isHit[4]");
    glUniform1i(isHit4, enemy[4].isHit);

    int isPickUp = glGetUniformLocation(shader->GetProgramID(), "pickUp");
    glUniform1i(isPickUp, pick);

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if ((window->KeyHold(GLFW_KEY_W)) && (!collisionPlayerWall(camera->forward))) {
        camera->MoveForward(cameraSpeed * deltaTime);
        animation = 1;
    }
    else
        animation = 0;

    if ((window->KeyHold(GLFW_KEY_A)) && (!collisionPlayerWall(camera->forward))) {
        camera->TranslateRight(-(cameraSpeed * deltaTime));
        animation = 1;
    }

    if ((window->KeyHold(GLFW_KEY_S)) && (!collisionPlayerWall(camera->forward))) {
        camera->MoveForward(-(cameraSpeed * deltaTime));
        animation = 1;
    }

    if ((window->KeyHold(GLFW_KEY_D)) && (!collisionPlayerWall(camera->forward))) {
        camera->TranslateRight(cameraSpeed * deltaTime);
        animation = 1;
    }

    if (window->KeyHold(GLFW_KEY_Q) && (camera->GetTargetPosition().y > 0.75f)) {
        camera->TranslateUpward(-(cameraSpeed * deltaTime));
        animation = 1;
    }

    if (window->KeyHold(GLFW_KEY_E) && (camera->GetTargetPosition().y < 10.0f)) {
        camera->TranslateUpward(cameraSpeed * deltaTime);
        animation = 1;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_R)
        switch (renderPlayer)
        {
            case true:
                renderPlayer = false;
                projectionMatrix = glm::perspective(RADIANS(30), window->props.aspectRatio, 0.01f, 300.0f);
                shotMode = true;
                renderBullet = true;
                break;
            case false:
                renderPlayer = true;
                projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 300.0f);
                shotMode = false;
                renderBullet = false;
                break;
        }

    if ((key == GLFW_KEY_SPACE) && (shotMode == true))
        switch (shot)
        {
        case false:
            shot = true;
            break;
        }
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        angle =  mouseX * sensivityOX - 0.75f;

        if (renderPlayer == false) {
            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
        }

        if (renderPlayer == true) {
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
