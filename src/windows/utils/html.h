#ifndef HTML_H
#define HTML_H

/* Test case to demonstrate functionality.
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
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <memory> // for std::unique_ptr

class htmlElement {
private:
    int tabIndex_ = 0;
    std::string type_ = "div";
    std::string text_ = "";
    std::map<std::string, std::string> attributes_;
    std::vector<std::unique_ptr<htmlElement>> children_;

    std::string repeat(const std::string& str, const size_t& times);

public:
    // Getters and Setters
    int get_tab_index();
    void set_tab_index(const int& val);
    std::string get_type();
    void set_type(const std::string& val);
    std::string get_text();
    void set_text(const std::string& val);
    std::map<std::string, std::string> get_attribues();
    void set_attributes(const std::map<std::string, std::string>& val);

    // Children access
    std::vector<std::unique_ptr<htmlElement>>& get_children();
    void set_children(std::vector<std::unique_ptr<htmlElement>>&& val); // move-based

    // Attribute methods
    void set_attribute(const std::string& key, const std::string& val);
    void remove_attribute(const std::string& key);

    // Child management
    void add_child(std::unique_ptr<htmlElement> child);
    void remove_child(size_t index);
    void clear_children();

    // Constructors
    htmlElement();
    htmlElement(const int& tabIndex, const std::string& type);
    htmlElement(const int& tabIndex, const std::string& type, const std::string& text);
    htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes);
    htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes, std::vector<std::unique_ptr<htmlElement>>&& children);
    htmlElement(const int& tabIndex, const std::string& type, const std::map<std::string, std::string>& attributes);
    htmlElement(const int& tabIndex, const std::string& type, const std::map<std::string, std::string>& attributes, std::vector<std::unique_ptr<htmlElement>>&& children);

    // Render
    bool htmlRender(std::ostream& stream);
};

#endif // HTML_H
