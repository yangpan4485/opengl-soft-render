#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "pipeline_struct.h"

class Uniform {
public:
    static Uniform& GetInstance();
    // void SetLocation(const std::string& key, int location);
    // int GetLocation(const std::string& key);

    void SetValue(int program, int location, const std::vector<float>& value);
    std::vector<float> GetValue(int program, int location);
    std::vector<float> GetValue(const std::string& name);

    void SetCurrentProgram(int program);

private:
    Uniform();
    ~Uniform();
    Uniform(const Uniform&) = delete;
    Uniform operator=(const Uniform) = delete;

private:
    // std::unordered_map<std::string, int> uniform_location_map_;
    // std::unordered_map<int, std::vector<float>> uniform_value_map_;
    std::unordered_map<int, std::unordered_map<int, std::vector<float>>> uniform_value_map_;
    int current_use_program_{};
};