#include "global_instance.h"

GlobalInstance& GlobalInstance::GetInstance() {
    static GlobalInstance instance;
    return instance;
}

std::shared_ptr<Rop> GlobalInstance::GetRopInstance() {
    return rop_ins_;
}

std::shared_ptr<VertexProcessor> GlobalInstance::GetVertexProcessorInstance() {
    return vertex_ins_;
}


GlobalInstance::GlobalInstance() {
    rop_ins_.reset(new Rop());
    vertex_ins_.reset(new VertexProcessor());
}

GlobalInstance::~GlobalInstance() = default;