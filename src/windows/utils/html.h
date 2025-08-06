#ifndef HTML_H
#define HTML_H

/* Test case to demonstrate functionality.
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
*/

#include <exception>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <memory> // for std::unique_ptr

class htmlError : public std::exception {
private:
    std::string msg_; // No default error

public:
    htmlError(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
};

class htmlElement {
private:
    int tabIndex_ = 0;
    std::string type_ = "div";
    std::string text_ = "";
    std::map<std::string, std::string> attributes_;
    std::vector<htmlElement*> children_; // std::unique_ptr would be better, but can be changed later

    std::string repeat(const std::string& str, const size_t& times);

public:
    // Getters and Setters
    int get_tab_index();
    void set_tab_index(const int& tabIndex);
    std::string get_type();
    void set_type(const std::string& type);
    std::string get_text();
    void set_text(const std::string& text);
    std::map<std::string, std::string> get_attribues();
    void set_attributes(const std::map<std::string, std::string>& attributes);

    // Children access
    std::vector<htmlElement*>& get_children();
    void set_children(std::vector<htmlElement*>& children);

    // Attribute methods
    void set_attribute(const std::string& key, const std::string& value);
    void remove_attribute(const std::string& key);

    // Child management
    void add_child(htmlElement* child);
    void remove_child(size_t index);
    void clear_children();

    // Constructors
    htmlElement();
    htmlElement(const int& tabIndex, const std::string& type);
    htmlElement(const int& tabIndex, const std::string& type, const std::string& text);
    htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes);
    htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes, std::vector<htmlElement*>& children);
    htmlElement(const int& tabIndex, const std::string& type, const std::map<std::string, std::string>& attributes);
    htmlElement(const int& tabIndex, const std::string& type, const std::map<std::string, std::string>& attributes, std::vector<htmlElement*>& children);
    htmlElement(const int& tabIndex, const std::string& type, std::vector<htmlElement*>& children);

    // Destructor
    ~htmlElement();

    // Render
    bool htmlRender(std::ostream& stream);
};

#endif // HTML_H
