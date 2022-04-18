#include "objectTema2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquaree(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateCirclee(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    int numberOfVertices = 10000;
    float Pi = M_PI;
    float alpha = (float)(2 * Pi) / (numberOfVertices - 2);

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color, glm::vec3(0.0f, 0.0f, 0.0f)));
    for (int i = 0; i <= numberOfVertices; i++)
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(i * alpha), radius * sin(i * alpha), 0), color, glm::vec3(0.0f, 0.0f, 0.0f)));

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 0; i < numberOfVertices; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}