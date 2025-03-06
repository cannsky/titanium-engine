#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "Engine.hpp"

using namespace TulparEngine;

int main() {
    try {
        Engine::GetInstance().Init();
        Engine::GetInstance().run();
        Engine::GetInstance().cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}