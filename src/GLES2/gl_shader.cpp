#include "gl_shader.h"

#include <sstream>
#include <vector>

#include "global_config.h"
#include "texture/texture_data.h"
#include "uniform.h"

GLuint glCreateShader(GLenum type) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    GLuint shader = 0;
    {
        std::unique_lock<std::mutex> lock(context->shader_mtx);
        if (context->current_shader_name < context->max_shader_name) {
            shader = context->current_shader_name;
            context->current_shader_name++;
        } else {
            for(int i = 1; i < context->max_shader_name; ++i) {
                if (context->shader_map.find(i) != context->shader_map.end()) {
                    shader = i;
                    break;
                }
            }
        }
    }
    ShaderObject shader_object;
    shader_object.shader_type = type;
    shader_object.shader_name = shader;
    context->shader_map[shader] = shader_object;
    return shader;
}

void glDeleteShader (GLuint shader) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->shader_map.find(shader) == context->shader_map.end()) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::unique_lock<std::mutex> lock(context->shader_mtx);
    context->shader_map.erase(shader);
}

void glShaderSource(GLuint shader, int count, const char* const* string, const int* length) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->shader_map.find(shader) == context->shader_map.end()) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (count < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::string source = "";
    for(int i = 0; i < count; ++i) {
        source = source + string[i];
    }
    context->shader_map[shader].shader_source = source;
}

void glCompileShader(GLuint shader) {
    // TODO:compiler
}

void glReleaseShaderCompiler() {
    // TODO:compiler
}

GLuint glCreateProgram(){
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    GLuint program = 0;
    {
        std::unique_lock<std::mutex> lock(context->program_mtx);
        if (context->current_program_name < context->max_program_name) {
            program = context->current_program_name;
            context->current_program_name++;
        } else {
            for(int i = 1; i < context->max_program_name; ++i) {
            if (context->program_map.find(i) != context->program_map.end()) {
                program = i;
                break;
            }
        }
        }
    }
    ProgramObject program_object;
    context->program_map[program] = program_object;
    return program;
}

void glDeleteProgram (GLuint program) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::unique_lock<std::mutex> lock(context->program_mtx);
    context->program_map.erase(program);
}

void glAttachShader(GLuint program, GLuint shader) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (context->shader_map.find(shader) == context->shader_map.end()) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }

    if (context->shader_map[shader].shader_type == GL_FRAGMENT_SHADER) {
        if (context->program_map[program].fragment_shader.shader_name != 0 ||
            context->program_map[program].fragment_shader.shader_name == shader) {
            context->error_code = GL_INVALID_OPERATION;
            return;
        }
        context->program_map[program].fragment_shader = context->shader_map[shader];
    } else {
        if (context->program_map[program].vertex_shader.shader_name != 0 || 
            context->program_map[program].vertex_shader.shader_name == shader) {
            context->error_code = GL_INVALID_OPERATION;
            return;
        }
        context->program_map[program].vertex_shader = context->shader_map[shader];
    }
}

void glDetachShader (GLuint program, GLuint shader) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (context->shader_map.find(shader) == context->shader_map.end()) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    if (context->program_map[program].fragment_shader.shader_name == shader) {
        context->program_map[program].fragment_shader = ShaderObject();
    } else if (context->program_map[program].vertex_shader.shader_name == shader) {
        context->program_map[program].vertex_shader = ShaderObject();
    } else {
        context->error_code = GL_INVALID_OPERATION;
    }
}

void glLinkProgram(GLuint program) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::vector<std::string> shader_sources(2);
    shader_sources[0] = context->program_map[program].vertex_shader.shader_source;
    shader_sources[1] = context->program_map[program].fragment_shader.shader_source;
    std::vector<std::string> words;
    std::string word;
    for (int i = 0; i < shader_sources.size(); ++i) {
        std::istringstream ss(shader_sources[i]);
        while(ss >> word) {
	        words.push_back(word);
        }
    }
    int uniform_location = 0;
    int texture_val = 0;
    int cube_map_val = 0;
    int attribute_location = 0;
    for(size_t i = 0; i < words.size(); ++i) {
        // std::cout << "words[i]: " << words[i] << std::endl;
        if (words[i] == "uniform") {
            std::string name = words[i + 2].substr(0, words[i + 2].length() - 1);
            // std::cout << "name: " << name << std::endl;
            context->program_map[program].uniform_location_map[name] = uniform_location;
            if (words[i + 1] == "sampler2D") {
                std::vector<float> vec{(float)texture_val};
                texture_val++;
                Uniform::GetInstance().SetValue(program, uniform_location, vec);
            } else if (words[i + 1] == "samplerCube") {
                std::vector<float> vec{(float)cube_map_val};
                cube_map_val++;
                Uniform::GetInstance().SetValue(program, uniform_location, vec);
            }
            // if (words[i + 1] == "sampler2D") {
            //     context->shader_map[shader].texture_location_map[name] = texture_location;
            //     context->shader_map[shader].custome_texture_location_map[texture_location] = name;
            //     texture_location++;
            // } else {
            //     context->shader_map[shader].uniform_location_map[name] = uniform_location;
            //     context->shader_map[shader].custome_uniform_location_map[uniform_location] = name;
            //     uniform_location++;
            // }
            uniform_location++;
            i = i + 2;
        } else if (words[i] == "attribute") {
            std::string name = words[i + 2].substr(0, words[i + 2].length() - 1);
            context->program_map[program].attribute_location_map[attribute_location] = name;
            if (context->program_map[program].custome_attribute_location_map.find(name) == 
                context->program_map[program].custome_attribute_location_map.end()) {
                context->program_map[program].custome_attribute_location_map[name] = attribute_location;
                attribute_location++;
            }
            // std::cout << "===: " << name << " " << attribute_location << std::endl;
            // context->program_map[program].custome_attribute_location_map[name] = attribute_location;
            // attribute_location++;
            i = i + 2;
        }
    }

#if 0
    int uniform_index = 0;
    for (const auto& val: context->program_map[program].vertex_shader.uniform_location_map) {
        context->program_map[program].uniform_location_map[val.first] = uniform_index;
        TextureData::GetInstance().SetTextureUniformMap(val.first, uniform_index);
        uniform_index++;
    }
    for (const auto& val: context->program_map[program].fragment_shader.uniform_location_map) {
        context->program_map[program].uniform_location_map[val.first] = uniform_index;
        TextureData::GetInstance().SetTextureUniformMap(val.first, uniform_index);
        uniform_index++;
    }
#endif
}

void glUseProgram(GLuint program) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    TextureData::GetInstance().SetCurrentUseProgram(program);
    Uniform::GetInstance().SetCurrentProgram(program);
    context->current_use_program = program;
}

void glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (bufSize < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (context->shader_map.find(shader) == context->shader_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    int len = context->shader_map[shader].shader_source.length();
    int max_len = (bufSize - 1) > len ? len : bufSize - 1;
    for (int i = 0; i < max_len; ++i) {
        source[i] = context->shader_map[shader].shader_source[i];
    }
    source[max_len] = '\0';
}

void glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) {
    // TODO:compiler
}

void glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (maxCount < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (maxCount >= 1) {
        shaders[0] = context->program_map[program].vertex_shader.shader_name;
    }
    if (maxCount >= 1) {
        shaders[1] = context->program_map[program].fragment_shader.shader_name;
    }
}

GLboolean glIsProgram (GLuint program) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) != context->program_map.end()) {
        return true;
    }
    return false;
}

GLboolean glIsShader (GLuint shader) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->shader_map.find(shader) != context->shader_map.end()) {
        return true;
    }
    return false;
}

void glShaderBinary (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length) {
    // TODO:compiler
}

void glValidateProgram (GLuint program) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
    }
}