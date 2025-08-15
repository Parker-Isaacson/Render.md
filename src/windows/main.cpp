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

    renderObj.renderBlockQuote(root, line);

    root->htmlRender(std::cout);

    delete root;

    return 0;
}