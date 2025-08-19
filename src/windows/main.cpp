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

    std::ofstream outStream("../../test/markdown.html");
    renderObj.output(outStream);
    
    return 0;
}