#include "html.h"

// Getters and Setters
int htmlElement::get_tab_index() {
    return tabIndex_;
}

void htmlElement::set_tab_index(const int& val) {
    tabIndex_ = val;
} 

std::string htmlElement::get_type() {
    return type_;
}

void htmlElement::set_type(const std::string& val) {
    type_ = val;
}

std::string htmlElement::get_text() {
    return text_;
}

void htmlElement::set_text(const std::string& val) {
    text_ = val;
}

std::map<std::string, std::string> htmlElement::get_attribues() {
    return attributes_;
}

void htmlElement::set_attributes(const std::map<std::string, std::string>& val) {
    attributes_ = val;
}

std::vector<htmlElement*> htmlElement::get_children() {
    return children_;
}

void htmlElement::set_children(const std::vector<htmlElement*>& val) {
    children_ = val;
}

// Getters and Setters that have more specific functionality
void htmlElement::set_attribute(const std::string& key, const std::string& val) {
    attributes_[key] = val;
}

void htmlElement::remove_attribute(const std::string& key) {
    attributes_.erase(key);
}

void htmlElement::add_child(htmlElement* child) {
    children_.push_back(child);
}

void htmlElement::remove_child(const std::vector<htmlElement*>::const_iterator& index) {
    children_.erase(index);
}

void htmlElement::remove_children(const std::vector<htmlElement*>::const_iterator& first, const std::vector<htmlElement*>::const_iterator& last) {
    children_.erase(first, last);
}

// Constructors
htmlElement::htmlElement(const int& tabIndex, const std::string& type)
    : tabIndex_(tabIndex), type_(type) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text)
    : tabIndex_(tabIndex), type_(type), text_(text) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes)
    : tabIndex_(tabIndex), type_(type), text_(text), attributes_(attributes) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes, const std::vector<htmlElement*>& children)
    : tabIndex_(tabIndex), type_(type), text_(text), attributes_(attributes), children_(children) {}

// Computational Methods
/* Example
htmlElement root;
root.HTML_Render(std::cout); // For output to terminal
std::ofstream file("output.txt"); // For output to file
if ( file.is_open() )
    root.HTML_Render(file);
file.close();
*/
bool htmlElement::htmlRender(std::ostream& stream) { // stream must be open if it is a file
    // Build staring tag
    stream << repeat("\t", tabIndex_) << "<" << type_; // Builds the start; like "<div"
    for ( auto& [key, value] : attributes_ ) { // Builds the attributes; like " class="wrapper""
        stream << " " << key << "=\"" << value << "\"";
    }
    stream << ">\n"; // Builds the last part of the starting tag
    if ( text_ != "" ) { // If the internal text is not empty
        stream << repeat("\t", tabIndex_ + 1) << text_ << "\n"; // Place it on the next line, indented by 1 
    }        
    // Call the build for all children
    for ( auto* child : children_) {
        child->htmlRender(stream);
    }        
    // Build the end tag
    stream << repeat("\t", tabIndex_) << "</" << type_ << ">" << "\n"; // Builds the ending tab; like "</div>"
    return true;
}

// I think there is something in std that does this, but I dont know
std::string htmlElement::repeat(const std::string& str, const size_t& times) {
    std::string result;
    result.reserve(str.size() * times);
    for (size_t i = 0; i < times; ++i)
        result += str;
    return result;
}