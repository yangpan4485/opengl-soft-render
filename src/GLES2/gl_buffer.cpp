#include "gl_buffer.h"

#include <sstream>
#include <vector>
#include <iostream>

#include "global_config.h"

void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_buffer_name) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    std::string str = name;
    if (str.length() > 3 && str.substr(0,3) == "gl_") {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // std::cout << name << " " << index << std::endl;
    context->program_map[program].custome_attribute_location_map[name] = index;
}

void glGenBuffers(int n, GLuint* buffers) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (n < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    for(int i = 0; i < n; ++i) {
        GLuint buffer;
        std::unique_lock<std::mutex> lock(context->buffer_mtx);
        if (context->current_buffer_name < context->max_buffer_name) {
            buffer = context->current_buffer_name;
            context->current_buffer_name++;
        } else {
            for(int j = 1; j < context->max_buffer_name; ++j) {
                if (context->buffer_map.find(j) != context->buffer_map.end()) {
                    buffer = j;
                    break;
                }
            }
        }
        BufferObject buffer_object;
        buffer_object.buffer_name = buffer;
        context->buffer_map[i] = buffer_object;
        if ( n == 1) {
            *buffers = buffer;
        } else {
            buffers[i] = buffer;
        }
    }
}

void glDeleteBuffers (GLsizei n, const GLuint *buffers) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (n < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    for(int i = 0; i < n; ++i) {
        std::unique_lock<std::mutex> lock(context->buffer_mtx);
        if (buffers[i] == context->current_buffer_name) {
            context->current_buffer_name--;
        }
        context->buffer_map.erase(buffers[i]);
    }
}

void glBindBuffer(int target, GLuint buffer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    context->buffer_map[buffer].buffer_type = target;
    context->current_bind_buffer_map[target] = buffer;

}

void glBufferData(int target, int size, const void* data, int usage) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (usage != GL_STREAM_DRAW && usage != GL_STATIC_DRAW && usage != GL_DYNAMIC_DRAW) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (size < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (context->current_bind_buffer_map[target] == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    int buffer = context->current_bind_buffer_map[target];
    if (target == GL_ARRAY_BUFFER) {
        context->current_buffer = buffer;
    }
    
    context->buffer_map[buffer].usage = usage;
    context->buffer_map[buffer].buffer_data.clear();
    context->buffer_map[buffer].buffer_data.resize(size / 4);
    if (context->buffer_map[buffer].buffer_data.size() != size / 4) {
        context->buffer_map[buffer].buffer_data.clear();
        context->error_code = GL_OUT_OF_MEMORY;
        return;
    }
    if (data == nullptr) {
        return;
    }
    float* f_data = (float*)data;
    for(int i = 0; i < size / 4; ++i) {
        context->buffer_map[buffer].buffer_data[i] = f_data[i];
    }
}

void glVertexAttribPointer(GLuint index, int size, int type, bool normalized, int stride, const void* pointer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int buffer = context->current_buffer;
    context->buffer_map[buffer].vertex_data_map[index].stride = stride;
    context->buffer_map[buffer].vertex_data_map[index].index = index;
    context->buffer_map[buffer].vertex_data_map[index].size = size;
    context->buffer_map[buffer].vertex_data_map[index].pointer = (float*)pointer;

    context->attrib_index_map[index] = context->current_buffer;
}

void glEnableVertexAttribArray(GLuint index) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_buffer_name) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    int buffer = context->current_buffer;
    context->buffer_map[buffer].vertex_data_map[index].enable = true;
}

void glDisableVertexAttribArray(GLuint index) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_buffer_name) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    int buffer = context->current_buffer;
    context->buffer_map[buffer].vertex_data_map[index].enable = false;
}

void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void *data) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (offset < 0 || size < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (context->current_bind_buffer_map[target] == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    int buffer = context->current_bind_buffer_map[target];
    if ((offset + size) / 4 > context->buffer_map[buffer].buffer_data.size()) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    float* f_data = (float*)data;
    int base_index = offset / 4;
    for(int i = 0; i < size / 4; ++i) {
        context->buffer_map[buffer].buffer_data[base_index + i] = f_data[i];
    }
}

void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (pname != GL_BUFFER_SIZE || pname != GL_BUFFER_USAGE) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (context->current_bind_buffer_map[target] == 0) {
        context->error_code = GL_INVALID_OPERATION;
        return;
    }
    int buffer = context->current_bind_buffer_map[target];
    if (pname == GL_BUFFER_SIZE) {
        *params = context->buffer_map[buffer].buffer_data.size() * 4;
    }
    if (pname == GL_BUFFER_USAGE) {
        *params = context->buffer_map[buffer].usage;
    }
}

GLint glGetAttribLocation (GLuint program, const GLchar *name) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    GLint location = context->program_map[program].custome_attribute_location_map[name];
    return location;
}

void glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, 
                        GLint *size, GLenum *type, GLchar *name) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->program_map.find(program) == context->program_map.end()) {
        // GL_INVALID_OPERATION
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
    }
    if (bufSize < 0) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    for (const auto& val : context->program_map[program].custome_attribute_location_map) {
        if (val.second == index) {
            if (bufSize > val.first.size()) {
                name = (GLchar*)val.first.c_str();
                *length = val.first.size();
            } else {
                *length = bufSize - 1;
                for (int i = 0; i < bufSize - 1; ++i) {
                    name[i] = val.first[i];
                }
                name[bufSize - 1] = '\0'; 
            }
        }
    }
}

void glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    int buffer = context->current_buffer;
    // context->buffer_map[buffer].vertex_data_map[index].enable = false;
    if (pname == GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING) {
        *params = 0;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_ENABLED) {
        if (context->buffer_map[buffer].vertex_data_map[index].enable) {
            *params = 1;
        } else {
            *params = 0;
        }
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_SIZE) {
        *params = 4;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_STRIDE) {
        *params = context->buffer_map[buffer].vertex_data_map[index].stride;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_TYPE) {
        *params = GL_FLOAT;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_NORMALIZED) {
        *params = 0;
    } else if (pname == GL_CURRENT_VERTEX_ATTRIB) {
        params[0] = 0;
        params[1] = 0;
        params[2] = 0;
        params[3] = 1;
    } else {
        context->error_code = GL_INVALID_ENUM;
    }
}

void glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    int buffer = context->current_buffer;
    if (pname == GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING) {
        *params = context->current_buffer;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_ENABLED) {
        if (context->buffer_map[buffer].vertex_data_map[index].enable) {
            *params = 1;
        } else {
            *params = 0;
        }
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_SIZE) {
        *params = 4;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_STRIDE) {
        *params = context->buffer_map[buffer].vertex_data_map[index].stride;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_TYPE) {
        *params = GL_FLOAT;
    } else if (pname == GL_VERTEX_ATTRIB_ARRAY_NORMALIZED) {
        *params = 0;
    } else if (pname == GL_CURRENT_VERTEX_ATTRIB) {
        params[0] = 0;
        params[1] = 0;
        params[2] = 0;
        params[3] = 1;
    } else {
        context->error_code = GL_INVALID_ENUM;
    }
}

void glGetVertexAttribPointerv (GLuint index, GLenum pname, void **pointer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (pname != GL_VERTEX_ATTRIB_ARRAY_POINTER) {
        context->error_code = GL_INVALID_ENUM;
        return;
    }
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // *pointer = (void*)context->data_map[index].data.data();
}

GLboolean glIsBuffer (GLuint buffer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->buffer_map.find(buffer) != context->buffer_map.end()) {
        return true;
    }
    return false;
}

void glVertexAttrib1f (GLuint index, GLfloat x) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // context->data_map[index].data.push_back(x);
}

void glVertexAttrib1fv (GLuint index, const GLfloat *v) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // for (int i = 0; i < 1; ++i) {
    //     context->data_map[index].data.push_back(v[i]);
    // }
}

void glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // context->data_map[index].data.push_back(x);
    // context->data_map[index].data.push_back(y);
}

void glVertexAttrib2fv (GLuint index, const GLfloat *v) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // for (int i = 0; i < 2; ++i) {
    //     context->data_map[index].data.push_back(v[i]);
    // }
}

void glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // context->data_map[index].data.push_back(x);
    // context->data_map[index].data.push_back(y);
    // context->data_map[index].data.push_back(z);
}

void glVertexAttrib3fv (GLuint index, const GLfloat *v) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // for (int i = 0; i < 3; ++i) {
    //     context->data_map[index].data.push_back(v[i]);
    // }
}

void glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // context->data_map[index].data.push_back(x);
    // context->data_map[index].data.push_back(y);
    // context->data_map[index].data.push_back(z);
    // context->data_map[index].data.push_back(w);
}

void glVertexAttrib4fv (GLuint index, const GLfloat *v) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (index >= context->max_attribute_num) {
        context->error_code = GL_INVALID_VALUE;
        return;
    }
    // context->data_map[index].data.clear();
    // for (int i = 0; i < 4; ++i) {
    //     context->data_map[index].data.push_back(v[i]);
    // }
}