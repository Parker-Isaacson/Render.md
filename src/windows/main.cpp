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
    renderObj.render();
    
    std::string line = "I am a line with **bold** and *italic* and ***both***. I also have a link [Click me!](https://github.parkerisaacson.com) and an image ![background](../../media/KolibriBackground.png).";
    std::vector<std::string> lines = {"- Item 1", "- Item 2", "- Item 3"};

    renderObj.renderText(line);
    
    std::cout << line << std::endl;

    renderObj.output(std::cout);
    
    return 0;
}