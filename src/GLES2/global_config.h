#pragma once
#include <memory>
#include <unordered_map>
#include <mutex>
#include <vector>

#include "gl_common.h"
#include "context/gl_context.h"

class GlobalConfig {
public:
    static GlobalConfig& GetInstance();

    std::shared_ptr<Context> GetContext();

private:
    GlobalConfig();
    ~GlobalConfig();
    GlobalConfig(const GlobalConfig&) = delete;
    GlobalConfig operator=(const GlobalConfig&) = delete;

private:
    std::shared_ptr<Context> context_;
};