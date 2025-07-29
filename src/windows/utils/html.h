#ifndef HTML_H
#define HTML_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>

class htmlElement {
    private:
        // Private variables
        int tabIndex_ = 0; // By default exist at root
        std::string type_ = "div"; // By defualt be a div
        std::string text_ = ""; // By default have no internal text
        std::map<std::string, std::string> attributes_; // By defualt have no attribues
        std::vector<htmlElement*> children_; // By defualt have no children

        // Private methods
        std::string repeat(const std::string& str, const size_t& times);
    public:
        // Getters and Setters for all private variables
        int get_tab_index();
        void set_tab_index(const int& val);
        std::string get_type();
        void set_type(const std::string& val);
        std::string get_text();
        void set_text(const std::string& val);
        std::map<std::string, std::string> get_attribues();
        void set_attributes(const std::map<std::string, std::string>& val);
        std::vector<htmlElement*> get_children();
        void set_children(const std::vector<htmlElement*>& val);
        
        // Special Getters and Setters
        void set_attribute(const std::string& key, const std::string& val);
        void remove_attribute(const std::string& key);
        void add_child(htmlElement* child); // Cant be const??
        // No support for finding a child, loop through the result of get_children()
        void remove_child(const std::vector<htmlElement*>::const_iterator& index);
        void remove_children(const std::vector<htmlElement*>::const_iterator& first, const std::vector<htmlElement*>::const_iterator& last);

        // Constructors, might add support for others if it becomes needed, through can be added externally
        htmlElement();
        htmlElement(const int& tabIndex, const std::string& type);
        htmlElement(const int& tabIndex, const std::string& type, const std::string& text);
        htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes);
        htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes, const std::vector<htmlElement*>& children);

        // Computation Methods
        bool htmlRender(std::ostream& stream);
};

#endif // HTML_H