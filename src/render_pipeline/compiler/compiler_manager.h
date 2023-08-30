#pragma once
#include <memory>

#include "pixel_compiler.h"
#include "vertex_compiler.h"

class CompilerManager {
public:
    static CompilerManager& GetInstance();
    
    void SetPixelCompiler(std::shared_ptr<PixelCompiler> pixel_compiler);
    std::shared_ptr<PixelCompiler> GetPixelCompiler();
    void SetVertexCompiler(std::shared_ptr<VertexCompiler> vertex_compiler);
    std::shared_ptr<VertexCompiler> GetVertexCompiler();

private:
    CompilerManager();
    ~CompilerManager();
    CompilerManager(const CompilerManager&) = delete;
    CompilerManager operator=(const CompilerManager&) = delete;

private:
    std::shared_ptr<PixelCompiler> pixel_compiler_{};
    std::shared_ptr<VertexCompiler> vertex_compiler_{};
};