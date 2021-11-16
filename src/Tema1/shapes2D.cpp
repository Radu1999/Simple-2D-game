#include "shapes2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* shapes2D::CreateSquare(
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
        VertexFormat(corner + glm::vec3(length, 0, 1), color),
        VertexFormat(corner + glm::vec3(length, length, 1), color),
        VertexFormat(corner + glm::vec3(0, length, 1), color)
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

Mesh* shapes2D::CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(center, color));

    for (int angle = 0; angle < 360; angle++) {
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
    }

    Mesh* circle = new Mesh(name);
    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
        for (int i = 1; i < 360; i++) {
            indices.push_back(i);
        }
    }
    else {
        for (int i = 1; i < 360; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }
    

   
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* shapes2D::CreateTriangle(const std::string& name, glm::vec3 corner, float length, glm::vec3 color, bool fill) {
    Mesh* triangle = new Mesh(name);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 1), color),
        VertexFormat(corner + glm::vec3(length, length, 1), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2};
    triangle->InitFromData(vertices, indices);
    return triangle;


}
