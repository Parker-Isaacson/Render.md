#include "utils/render.h"
#include "utils/html.h"
#include <fstream>
#include <iostream>

// Main entry point to the program
int main(int argc, char** argv) {
    std::string inputFile = "input.md";
    std::string outputFile = "markdown.html";
    for ( int i = 1; i < argc; i++ ) { // Parse through the arguements provided
        std::string param = std::string(argv[i]);
        if ( param == "--help" ) {
            std::cout << "See https://github.com/Parker-Isaacson/Render.md for help!" << std::endl;
            return 0;
        } else if ( param == "--input-file" ) {
            std::cout << "Setting Input File To: " << argv[++i] << "\n";
            inputFile = argv[i];
        } else if ( param == "--output-file" ) {
            std::cout << "Setting Output File To: " << argv[++i] << "\n";
            outputFile = argv[i];
        } else {
            std::cout << "Bad Input Param: " << param << ". Skipping." << "\n";
        }
    }
    htmlElement* root = new htmlElement(); // Created out here so it can be deleted just in case
    try {
        std::ifstream inStream(inputFile);
        std::ofstream outStream(outputFile);
        mdRender renderObj(inStream, outStream, root); 
        renderObj.render();
        renderObj.output();
    } catch (const renderError& e) {
        std::cout << "Rendering Error: " << e.what() << "\n";
        delete root;
        return 1;
    } catch (const htmlError& e) {
        std::cout << "HTML Error: " << e.what() << "\n";
        delete root;
        return 2;
    }
    std::cout << "Markdon Generation Complete!\nAdd: \"<iframe src=\"" + outputFile + "\" style=\"border:none; width:750px; height: 750px;\"></iframe>\" into the body of your html file where you want the markdown to go! Make sure to check the src, width and height!" << std::endl;
    return 0;
}