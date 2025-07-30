#include "utils/render.h"
#include "utils/html.h"
#include <memory>
#include <fstream>

int main() {
    // Test for html.h
    auto root = std::make_unique<htmlElement>(0, "div");
    auto child = std::make_unique<htmlElement>(1, "p", "I am a paragraph");
    root->add_child(std::move(child));
    root->htmlRender(std::cout);
    return 0;
}