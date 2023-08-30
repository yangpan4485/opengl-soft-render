#pragma once
#include <memory>
#include "rop.h"
#include "vertex_processor.h"

class GlobalInstance {
public:
    static GlobalInstance& GetInstance();

    std::shared_ptr<Rop> GetRopInstance();
    std::shared_ptr<VertexProcessor> GetVertexProcessorInstance();

private:
    GlobalInstance();
    ~GlobalInstance();
    GlobalInstance(const GlobalInstance&) = delete;
    GlobalInstance& operator=(const GlobalInstance&) = delete;

private:
    std::shared_ptr<Rop> rop_ins_{};
    std::shared_ptr<VertexProcessor> vertex_ins_{};
};