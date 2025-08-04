#include "utils/render.h"
#include "utils/html.h"
#include <memory>
#include <fstream>

int main() {
    htmlElement* root = new htmlElement(0, "div");
    htmlElement* child = new htmlElement(1, "p", "I am a paragraph");
    root->add_child(child);
    root->htmlRender(std::cout);

    delete root;
    return 0;
}