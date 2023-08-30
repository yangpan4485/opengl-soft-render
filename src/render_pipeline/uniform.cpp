#include "uniform.h"

#include "global_config.h"

Uniform& Uniform::GetInstance() {
    static Uniform instance;
    return instance;
}

// void Uniform::SetLocation(const std::string& key, int location) {
//     std::cout << key << " " << location << std::endl;
//     uniform_location_map_[key] = location;
// }

// int Uniform::GetLocation(const std::string& key) {
//     if (uniform_location_map_.find(key) == uniform_location_map_.end()) {
//         return -1;
//     }
//     return uniform_location_map_[key];
// }

void Uniform::SetValue(int program, int location, const std::vector<float>& value) {
    // std::cout << "SetValue: " << program << " " << location << " " << value.size() << std::endl;
    uniform_value_map_[program][location] = value;
    // uniform_value_map_[location] = value;
}

std::vector<float> Uniform::GetValue(int program, int location) {
    // std::cout << "program: " << program << std::endl;
    // std::cout << "location: " << location << std::endl;
    return uniform_value_map_[program][location];
}

std::vector<float> Uniform::GetValue(const std::string& name) {
    // std::cout << "name: " << name << std::endl;
    std::shared_ptr<Context> context = GlobalConfig::GetInstance().GetContext();
    int uniform_location = context->program_map[current_use_program_].uniform_location_map[name];
    // std::cout << "uniform_location: " << uniform_location << std::endl;
    return GetValue(current_use_program_, uniform_location);
}

void Uniform::SetCurrentProgram(int program) {
    // std::cout << "SetCurrentProgram: " << program << std::endl;
    current_use_program_ = program;
}


Uniform::Uniform() {}

Uniform::~Uniform() {}