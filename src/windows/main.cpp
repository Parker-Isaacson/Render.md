#include "utils/render.h"
#include "utils/html.h"
//#include <memory>
#include <fstream>
#include <iostream>

// Currently used for testing purposes, will be changed to be a real entry point later.
int main(int argc, char** argv) {
    std::ifstream inStream("../../examples/markdown/example1.md");
    htmlElement* root = new htmlElement();

    mdRender renderObj(inStream, root); // Above to satisfity the compiler
    renderObj.render(root);
    
    std::string line = "> I am a test heading";
    std::vector<std::string> lines = {"- Item 1", "- Item 2", "- Item 3"};

    renderObj.renderList(root, lines);

    renderObj.output(std::cout);
    
    return 0;
}