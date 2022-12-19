#pragma once

#include <string/application.hpp>
#include <string/core/debug.hpp>
#include <string/core/platform_detection.hpp>

#ifdef STRING_PLATFORM_LINUX
extern String::Application* String::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) { 
    String::Logger::initialize();

    auto app = String::CreateApplication({argc, argv});
    app->run();
    delete app;

    return EXIT_SUCCESS;
}
#endif