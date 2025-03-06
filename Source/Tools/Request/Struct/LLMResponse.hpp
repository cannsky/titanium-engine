#pragma once

#include <string>

struct LLMResponse {
    std::string model;
    std::string response;
    std::string created_at;
    bool done;
};