#include "global_config.h"

GlobalConfig& GlobalConfig::GetInstance() {
    static GlobalConfig instance;
    return instance;
}

std::shared_ptr<Context> GlobalConfig::GetContext() {
    return context_;
}

GlobalConfig::GlobalConfig() {
    context_.reset(new Context());
}

GlobalConfig::~GlobalConfig() {}