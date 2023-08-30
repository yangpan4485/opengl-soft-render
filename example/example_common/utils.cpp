#include "utils.h"

#include <iostream>
#include <fstream>

std::string GetFileContent(const std::string& filename) {
    std::ifstream fin(filename, std::ios::binary | std::ios::in);
    if(!fin || !fin.is_open()) {
        std::cout << "open file failed, filename: " << filename << std::endl;
        return "";
    }
    std::string result;
    std::string line;
    while(getline(fin, line)) {
        result += line + "\n";
    }
    fin.close();
    return result;
}

uint64_t GetTimeStamp() {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	return ms.count();
}