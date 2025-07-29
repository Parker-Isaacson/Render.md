#include "utils/html.h"

int main() {
    htmlElement* root = new htmlElement(0, "div", "", {{"class", "wrapper"}, {"href", "../IMAGES/photo.svg"}});
    htmlElement* childa = new htmlElement(1, "p", "Hello World!", {{"class", "bold"}});
    htmlElement* childb = new htmlElement(1, "span", "I am a span!");
    htmlElement* childa1 = new htmlElement(2, "p", "I am a single child!");
    
    root->add_child(childa);
    root->add_child(childb);
    childa->add_child(childa1);
    root->htmlRender(std::cout);

    delete root;
    delete childa;
    delete childb;
    delete childa1;

    return 0;
}