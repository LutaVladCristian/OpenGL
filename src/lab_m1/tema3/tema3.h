#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {

    public:
        struct dancer
        {
            dancer() : position(0), translate(0) {}
            dancer(glm::vec3 position, glm::vec3 translate) : position(position), translate(translate) {}
            glm::vec3 position;
            glm::vec3 translate;
        };

    public:
        Tema3();
        ~Tema3();
        void Init() override;
        Mesh* CreateCircle( const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill)
        {
            glLineWidth(3);
            glPointSize(5);
            Mesh* circle = new Mesh(name);
            std::vector <VertexFormat> vertices_c;
            std::vector <unsigned int> indices_c;
            vertices_c.push_back(VertexFormat(center, glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)));
            indices_c.push_back(0);
            float angle = 2 * PI / 20;
            for (int i = 0; i <= 20; i++)
            {
                vertices_c.push_back(VertexFormat(glm::vec3(radius * cos(i * angle) + center[0], -6, radius * sin(i * angle) + center[2]), color, glm::vec3(1, 0, 0)));
                indices_c.push_back(i + 1);
            }
            circle->SetDrawMode(GL_TRIANGLE_FAN);
            circle->InitFromData(vertices_c, indices_c);
            return circle;
        }

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, const glm::vec3& color = glm::vec3(1), int row = -1, int col = -1, int index = -1);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);


        std::unordered_map<std::string, Texture2D*> mapTextures;
        std::vector <dancer> dancers;
        glm::vec3 colors[8][8];
        glm::vec3 colors_wall_E[8][8];
        glm::vec3 colors_wall_W[8][8];
        glm::vec3 colors_wall_N[8][8];
        glm::vec3 colors_wall_S[8][8];
        glm::vec3 colors_roof[8][8];
        glm::vec3 translate;
        glm::vec3 light_pos[8][8];
        glm::vec3 lightPosition, lightPosition1;
        glm::vec3 floor_light_pos[32];
        glm::vec3 floor_color_lights[32];
        glm::vec3 spot_colors[4];
        glm::vec3 spot_pos[4];
        glm::vec3 light_pos_dancer[9];
        glm::vec3 color_pos_dancer[9];
        glm::vec3 lightDirection;
        glm::vec3 lightDirection_spot;
        glm::vec3 disco_ball_pos;

        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        int row, col;
        float PI = 3.1415;
        float cutt_off_angle = 10;
        float angleOZ = 0.f, angleOY = 0.f;
        float angle_OZ_aux;
        float Time;
        int mode;
        int all = 0;
    };
}
