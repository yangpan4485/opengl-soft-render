#include "gl_framebuffer.h"

#include <iostream>

#include "global_config.h"

void glGenFramebuffers(GLsizei n, GLuint* framebuffers) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    for (int i = 0; i < n; ++i) {
        GLuint frame_buffer = 0;
        std::unique_lock<std::mutex> lock(context->frame_buffer_mtx);
        if (context->current_frame_buffer_name < context->max_frame_buffer_name) {
            frame_buffer = context->current_frame_buffer_name;
            context->current_frame_buffer_name++;
        } else {
            for(int j = 1; j < context->max_frame_buffer_name; ++j) {
                if (context->frame_buffer_map.find(j) != context->frame_buffer_map.end()) {
                    frame_buffer = j;
                    break;
                }
            }
        }
        FrameBufferObject frame_buffer_object;
        frame_buffer_object.frame_buffer_name = frame_buffer;
        context->frame_buffer_map[frame_buffer] = frame_buffer_object;
        framebuffers[i] = frame_buffer;
    }
}

void glBindFramebuffer(GLenum target, GLuint framebuffer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_FRAMEBUFFER) {
        return;
    }
    context->current_bind_frame_buffer = framebuffer;
    if (context->frame_buffer_map.find(framebuffer) == context->frame_buffer_map.end()) {
        return;
    }
    // std::cout << "current_bind_frame_buffer: " << framebuffer << std::endl;
    // context->current_bind_frame_buffer = framebuffer;
    context->frame_buffer_map[framebuffer].frame_buffer_type = target;
}

void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    for (int i = 0; i < n; ++i) {
        if (context->frame_buffer_map.find(framebuffers[i]) != context->frame_buffer_map.end()) {
            context->frame_buffer_map.erase(framebuffers[i]);
        }
    }
}

void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    for (int i = 0; i < n; ++i) {
        GLuint render_buffer = 0;
        std::unique_lock<std::mutex> lock(context->render_buffer_mtx);
        if (context->current_render_buffer_name < context->max_render_buffer_name) {
            render_buffer = context->current_render_buffer_name;
            context->current_render_buffer_name++;
        } else {
            for(int j = 1; j < context->max_render_buffer_name; ++j) {
                if (context->render_buffer_map.find(j) != context->render_buffer_map.end()) {
                    render_buffer = j;
                    break;
                }
            }
        }
        RenderBufferObject render_buffer_object;
        render_buffer_object.render_buffer_name = render_buffer;
        context->render_buffer_map[render_buffer] = render_buffer_object;
        renderbuffers[i] = render_buffer;
    }
}

void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int render_buffer = context->current_bind_render_buffer;
    context->render_buffer_map[render_buffer].width = width;
    context->render_buffer_map[render_buffer].height = height;
    context->render_buffer_map[render_buffer].format = internalformat;
    context->render_buffer_map[render_buffer].buffers.resize(width * height);
}

void glBindRenderbuffer(GLenum target, GLuint renderbuffer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_RENDERBUFFER) {
        return;
    }
    if (context->render_buffer_map.find(renderbuffer) == context->render_buffer_map.end()) {
        return;
    }
    context->current_bind_render_buffer = renderbuffer;
    context->render_buffer_map[renderbuffer].render_buffer_type = target;
}

void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    for (int i = 0; i < n; ++i) {
        if (context->render_buffer_map.find(renderbuffers[i]) != context->render_buffer_map.end()) {
            context->render_buffer_map.erase(renderbuffers[i]);
        }
    }
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_FRAMEBUFFER) {
        return;
    }
    if (attachment != GL_COLOR_ATTACHMENT0 && attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT) {
        return;
    }
    int frame_buffer = context->current_bind_frame_buffer;
    // context->texture_map[texture]
    context->frame_buffer_map[frame_buffer].frame_texture[attachment] = texture;
    // context->frame_buffer_texture.frame_texture[attachment].push_back(context->texture_map[texture]);
}

void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_FRAMEBUFFER) {
        return;
    }
    if (renderbuffertarget != GL_RENDERBUFFER) {
        return;
    }
    int frame_buffer = context->current_bind_frame_buffer;
    context->frame_buffer_map[frame_buffer].frame_render_buffer[attachment] = context->render_buffer_map[renderbuffer];
}

void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_FRAMEBUFFER) {
        return;
    }
    int frame_buffer = context->current_bind_frame_buffer;
    if (pname == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE) {
        if (GL_DEPTH_ATTACHMENT == attachment) {
            if (context->frame_buffer_map[frame_buffer].frame_render_buffer.find(attachment) != 
                context->frame_buffer_map[frame_buffer].frame_render_buffer.end()) {
                *params = GL_RENDERBUFFER;
            }
        }
    }
}

void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_RENDERBUFFER) {
        return;
    }
    int render_buffer = context->current_bind_render_buffer;
    if (pname == GL_RENDERBUFFER_WIDTH) {
        *params = context->render_buffer_map[render_buffer].width;
    } else if (pname == GL_RENDERBUFFER_HEIGHT) {
        *params = context->render_buffer_map[render_buffer].height;
    }
} 

GLenum glCheckFramebufferStatus(GLenum target) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (target != GL_FRAMEBUFFER) {
        return 0;
    }
    int frame_buffer = context->current_bind_frame_buffer;
    auto frame_render_buffer = context->frame_buffer_map[frame_buffer].frame_render_buffer;
    auto frame_texture = context->frame_buffer_map[frame_buffer].frame_texture;
    if (frame_texture.find(GL_COLOR_ATTACHMENT0) != frame_texture.end() || 
        frame_texture.find(GL_DEPTH_ATTACHMENT) != frame_texture.end() ||
        frame_texture.find(GL_STENCIL_ATTACHMENT) != frame_texture.end() || 
        frame_render_buffer.find(GL_COLOR_ATTACHMENT0) != frame_render_buffer.end() ||
        frame_render_buffer.find(GL_DEPTH_ATTACHMENT) != frame_render_buffer.end() ||
        frame_render_buffer.find(GL_STENCIL_ATTACHMENT) != frame_render_buffer.end()) {
        return GL_FRAMEBUFFER_COMPLETE;
    }
    return 0;
}

GLboolean glIsRenderbuffer(GLuint renderbuffer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->render_buffer_map.find(renderbuffer) != context->render_buffer_map.end()) {
        return true;
    }
    return false;
}

GLboolean glIsFramebuffer(GLuint framebuffer) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    if (context->frame_buffer_map.find(framebuffer) != context->frame_buffer_map.end()) {
        return true;
    }
    return false;
}