#include "vertex_compiler_cube_map.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "uniform.h"

VertexCompilerCubeMap::VertexCompilerCubeMap() {
}

VertexCompilerCubeMap::~VertexCompilerCubeMap() {
}

Point VertexCompilerCubeMap::Decode(const std::vector<std::vector<float>>& attributes) {
    Point point_attri;
    glm::mat4 view = glm::mat4(1.0f);
    std::vector<float> vec = Uniform::GetInstance().GetValue("view");
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            view[i][j] = vec[i * 4 + j];
        }
    }
    glm::mat4 projection = glm::mat4(1.0f);
    vec = Uniform::GetInstance().GetValue("projection");
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            projection[i][j] = vec[i * 4 + j];
        }
    }
    glm::vec4 pos{attributes[0][0], attributes[0][1], attributes[0][2], 1.0f};
    pos = projection * view * pos;
    point_attri.point_position.x = pos[0];
    point_attri.point_position.y = pos[1];
    point_attri.point_position.z = pos[3];
    point_attri.point_position.w = pos[3];
    // point size default 1.0
    point_attri.point_size = 1.0;
    PointAttribute attri;
    for (int i = 0; i < 3; ++i) {
        attri.attri[i] = attributes[0][i];
    }
    point_attri.point_attributes.push_back(attri);
    return point_attri;
}

uint32_t VertexCompilerCubeMap::AttributeCount() {
    return 3;
}