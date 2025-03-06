#include "EngineFileManager.hpp"

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace TulparEngine {
    EngineFileManager::EngineFileManager() {}

    EngineFileManager::~EngineFileManager() { cleanup(); }

    void EngineFileManager::Init() {

    }

    void EngineFileManager::run() {
        
    }

    void EngineFileManager::cleanup() {
        
    }

    std::vector<char> EngineFileManager::readFile(const std::string& filePath) {
        std::string realPath = ENGINE_DIR + filePath;
        std::ifstream file(realPath, std::ios::ate | std::ios::binary);
        
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }
        
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        
        file.close();
        return buffer;
    }
}