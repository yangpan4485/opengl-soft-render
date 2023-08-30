#include "gl_uniform.h"

#include <vector>

#include "global_config.h"
#include "uniform.h"

int glGetUniformLocation (GLuint program, const char* name) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return -1;
    }
    // for (const auto& val : context->program_map[program].uniform_location_map) {
    //     std::cout << val.first << " " << val.second << std::endl;
    // }
    if (context->program_map[program].uniform_location_map.find(name) != context->program_map[program].uniform_location_map.end()) {
        return context->program_map[program].uniform_location_map[name];
    }
    return -1;
}

void glUniform1f (GLint location, GLfloat v0) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{v0};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform1fv (GLint location, GLsizei count, const GLfloat *value) {
    // only support count = 1
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{value[0]};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform1i (GLint location, GLint v0) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{v0 * 1.0f};
    // std::cout << location << " " << vec[0] << std::endl;
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform1iv (GLint location, GLsizei count, const GLint *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{value[0] * 1.0f};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform2f (GLint location, GLfloat v0, GLfloat v1) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{v0, v1};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform2fv (GLint location, GLsizei count, const GLfloat *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{value[0], value[1]};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform2i (GLint location, GLint v0, GLint v1) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(2);
    vec[0] = v0 * 1.0f;
    vec[1] = v1 * 1.0f;
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform2iv (GLint location, GLsizei count, const GLint *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(2);
    vec[0] = value[0] * 1.0f;
    vec[1] = value[1] * 1.0f;
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{v0, v1, v2};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform3fv (GLint location, GLsizei count, const GLfloat *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{value[0], value[1], value[2]};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform3i (GLint location, GLint v0, GLint v1, GLint v2) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(3);
    vec[0] = v0 * 1.0f;
    vec[1] = v1 * 1.0f;
    vec[2] = v2 * 1.0f;
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform3iv (GLint location, GLsizei count, const GLint *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(3);
    vec[0] = value[0] * 1.0f;
    vec[1] = value[1] * 1.0f;
    vec[2] = value[2] * 1.0f;
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{v0, v1, v2, v3};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform4fv (GLint location, GLsizei count, const GLfloat *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec{value[0], value[1], value[2], value[3]};
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(4);
    vec[0] = v0 * 1.0f;
    vec[1] = v1 * 1.0f;
    vec[2] = v2 * 1.0f;
    vec[3] = v3 * 1.0f;
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniform4iv (GLint location, GLsizei count, const GLint *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(4);
    vec[0] = value[0] * 1.0f;
    vec[1] = value[1] * 1.0f;
    vec[2] = value[2] * 1.0f;
    vec[3] = value[3] * 1.0f;
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(4);
    for (int i = 0; i < 4; ++i) {
        vec[i] = value[i];
    }
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(9);
    for (int i = 0; i < 9; ++i) {
        vec[i] = value[i];
    }
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->current_use_program == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    std::vector<float> vec;
    vec.resize(16);
    for (int i = 0; i < 16; ++i) {
        vec[i] = value[i];
        // std::cout << vec[i] << " ";
        // if ((i + 1) % 4 == 0) {
        //     std::cout << std::endl;
        // }
    }
    Uniform::GetInstance().SetValue(context->current_use_program, location, vec);
}

void glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    int count = context->program_map[program].uniform_location_map.size();
    if (index >= count) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::string uniform_name = "";
    for (const auto& val: context->program_map[program].uniform_location_map) {
        if (val.second == index) {
            std::string uniform_name = val.first;
            break;
        }
    }
    int output_size = (bufSize - 1) > uniform_name.size() ? uniform_name.size() : bufSize - 1;
    *length = output_size;
    for (int i = 0; i < output_size; ++i) {
        name[i] = uniform_name[i];
    }
    name[output_size] = '\0';
}

void glGetUniformfv (GLuint program, GLint location, GLfloat *params) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::string name = "";
    for (const auto& val: context->program_map[program].uniform_location_map) {
        if (val.second == location) {
            std::string uniform_name = val.first;
            break;
        }
    }
    if (name.empty()) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    int shader_location = context->program_map[program].uniform_location_map[name];
    std::vector<float> result = Uniform::GetInstance().GetValue(program, location);
    for (int i = 0; i < result.size(); ++i) {
        params[i] = result[i];
    }
}

void glGetUniformiv (GLuint program, GLint location, GLint *params) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::string name = "";
    for (const auto& val: context->program_map[program].uniform_location_map) {
        if (val.second == location) {
            std::string uniform_name = val.first;
            break;
        }
    }
    if (name.empty()) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    int shader_location = context->program_map[program].uniform_location_map[name];
    std::vector<float> result = Uniform::GetInstance().GetValue(program, location);
    for (int i = 0; i < result.size(); ++i) {
        params[i] = (GLint)result[i];
    }
}