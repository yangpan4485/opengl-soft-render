#include "gl_draw.h"

#include <iostream>

#include "render_pipeline/global_instance.h"
#include "global_config.h"
#include "texture/texture_data.h"

void glDrawArrays(int mode, int first, int count) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    // rop
    std::shared_ptr<Rop> rop_ins = GlobalInstance::GetInstance().GetRopInstance();
    rop_ins->SetColorMask(context->color_mask);
    rop_ins->SetBlendConfig(context->blend_config);
    rop_ins->SetDepthConfig(context->depth_config);
    rop_ins->SetScissorConfig(context->scissor_config);
    rop_ins->SetStencilConfig(context->stencil_config);

    // TextureData::GetInstance().InitTextureMap();

    int program = context->current_use_program;
    int attribute_size = context->program_map[program].attribute_location_map.size();
    PointData point_data;
    point_data.first = first;
    point_data.count = count;
    point_data.mode = mode;
    // context->attrib_index_map[index] = context->current_buffer;
    int array_buffer = context->current_bind_buffer_map[GL_ARRAY_BUFFER];
    // std::vector<float> buffer_data = context->buffer_map[array_buffer].buffer_data;
    float* base = (float*)0;
    for(int i = 0; i < attribute_size; ++i) {
        std::vector<float> attri_data;
        std::string name = context->program_map[program].attribute_location_map[i];
        // std::cout << "name: " << name << std::endl;
        // std::cout << "size: " << context->program_map[program].custome_attribute_location_map.size() << std::endl;
        // for (const auto& val : context->program_map[program].custome_attribute_location_map) {
        //     std::cout << val.first << " " << val.second << std::endl;
        // }
        int custome_location = context->program_map[program].custome_attribute_location_map[name];
        // std::cout << "custome_location: " << custome_location << std::endl;
        int data_buffer = context->attrib_index_map[custome_location];
        std::vector<float> buffer_data = context->buffer_map[data_buffer].buffer_data;
        auto vertex_data = context->buffer_map[array_buffer].vertex_data_map[custome_location];
        if (vertex_data.enable) {
            int start = (vertex_data.pointer - base);
            int offset = vertex_data.stride / 4;
            for (int j = start; j < buffer_data.size(); ) {
                for (int k = 0; k < vertex_data.size; ++k) {
                    attri_data.push_back(buffer_data[j + k]);
                }
                j = j + offset;
            }
        }
        point_data.stride.push_back(vertex_data.size);
        point_data.data.push_back(attri_data);
    }
    std::shared_ptr<VertexProcessor> vertex_ins = GlobalInstance::GetInstance().GetVertexProcessorInstance();
    vertex_ins->SetCullfaceConfig(context->cullface_config);
    vertex_ins->SetViewportConfig(context->viewport_config);
    vertex_ins->SetRasterConfig(context->raster_config);
    vertex_ins->PushData(point_data);
}

void glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices) {
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    // rop
    std::shared_ptr<Rop> rop_ins = GlobalInstance::GetInstance().GetRopInstance();
    rop_ins->SetColorMask(context->color_mask);
    rop_ins->SetBlendConfig(context->blend_config);
    rop_ins->SetDepthConfig(context->depth_config);
    rop_ins->SetScissorConfig(context->scissor_config);
    rop_ins->SetStencilConfig(context->stencil_config);

    int element_buffer = context->current_bind_buffer_map[GL_ELEMENT_ARRAY_BUFFER];
    int array_buffer = context->current_bind_buffer_map[GL_ARRAY_BUFFER];
    int program = context->current_use_program;
    int attribute_size = context->program_map[program].attribute_location_map.size();
    PointData point_data;
    // point_data.first = first;
    point_data.count = count;
    point_data.mode = mode;
    // std::vector<float> buffer_data = context->buffer_map[array_buffer].buffer_data;
    float* base = (float*)0;
    for(int i = 0; i < attribute_size; ++i) {
        std::vector<float> attri_data;
        std::string name = context->program_map[program].attribute_location_map[i];
        int custome_location = context->program_map[program].custome_attribute_location_map[name];
        int data_buffer = context->attrib_index_map[custome_location];
        std::vector<float> buffer_data = context->buffer_map[data_buffer].buffer_data;
        auto vertex_data = context->buffer_map[array_buffer].vertex_data_map[custome_location];
        if (vertex_data.enable) {
            int start = (vertex_data.pointer - base);
            int offset = vertex_data.stride / 4;
            for (int j = start; j < buffer_data.size(); ) {
                for (int k = 0; k < vertex_data.size; ++k) {
                    attri_data.push_back(buffer_data[j + k]);
                }
                j = j + offset;
            }
        }
        point_data.stride.push_back(vertex_data.size);
        point_data.data.push_back(attri_data);
    }
    std::vector<int> point_indices(count);
    for (int i = 0; i < context->buffer_map[element_buffer].buffer_data.size(); ++i) {
        point_indices[i] = (int)context->buffer_map[element_buffer].buffer_data[i];
    }

    std::shared_ptr<VertexProcessor> vertex_ins = GlobalInstance::GetInstance().GetVertexProcessorInstance();
    vertex_ins->SetCullfaceConfig(context->cullface_config);
    vertex_ins->SetViewportConfig(context->viewport_config);
    vertex_ins->SetRasterConfig(context->raster_config);
    vertex_ins->PushData(point_data, point_indices);
}