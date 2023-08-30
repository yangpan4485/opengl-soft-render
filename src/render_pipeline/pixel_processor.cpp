#include "pixel_processor.h"
#include "compiler_manager.h"
#include "global_instance.h"

#include <iostream>

PixelProcessor::PixelProcessor() {
    
}

PixelProcessor::~PixelProcessor() {}

void PixelProcessor::PushData(const Point& point_data) {
    if (rop_ins_ == nullptr) {
        rop_ins_ = GlobalInstance::GetInstance().GetRopInstance();
    }
    // std::cout << point_data.point_position.x << " " << point_data.point_position.y << " "
    //           << point_data.point_attributes[0].attri[0] << " "
    //           << point_data.point_attributes[0].attri[1] << " "
    //           << point_data.point_attributes[0].attri[2] << std::endl;
    pixel_compiler_ = CompilerManager::GetInstance().GetPixelCompiler();
    PixelAttri pixel = pixel_compiler_->Decode(point_data);
    rop_ins_->PushData(pixel);
}