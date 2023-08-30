#include "compiler_manager.h"

#include "compiler_header.h"

CompilerManager& CompilerManager::GetInstance() {
    static CompilerManager instance;
    return instance;
}
    
void CompilerManager::SetPixelCompiler(std::shared_ptr<PixelCompiler> pixel_compiler) {
    pixel_compiler_ = pixel_compiler;
}

std::shared_ptr<PixelCompiler> CompilerManager::GetPixelCompiler() {
    return pixel_compiler_;
}

void CompilerManager::SetVertexCompiler(std::shared_ptr<VertexCompiler> vertex_compiler) {
    vertex_compiler_ = vertex_compiler;
}

std::shared_ptr<VertexCompiler> CompilerManager::GetVertexCompiler() {
    return vertex_compiler_;
}

CompilerManager::CompilerManager() = default;

CompilerManager::~CompilerManager() = default;