#include "utils/html.h"
#include <memory>

int main() {
    auto root = std::make_unique<htmlElement>(0, "div", "", std::map<std::string, std::string>{{"class", "wrapper"}});
    auto child1 = std::make_unique<htmlElement>(1, "p", "Hello World!", std::map<std::string, std::string>{{"class", "bold"}});
    auto child2 = std::make_unique<htmlElement>(1, "span", "I am a span!");
    auto grandchild = std::make_unique<htmlElement>(2, "p", "I am a single child!");

    child1->add_child(std::move(grandchild));
    root->add_child(std::move(child1));
    root->add_child(std::move(child2));

    root->htmlRender(std::cout);
    return 0;
}
