#include <string/application.hpp>

class Sandbox : public String::Application {
public:
    Sandbox(const String::ApplicationSpecification& specification) : String::Application(specification) {}

    ~Sandbox() {}
};

String::Application* String::CreateApplication(String::ApplicationCommandLineArgs args) {
    ApplicationSpecification spec;
    spec.name = "Sandbox";
    spec.working_directory = ".";
    spec.command_line_args = args;

    return new Sandbox(spec);
}