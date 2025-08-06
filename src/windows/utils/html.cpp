#include "html.h"

// Constructors
htmlElement::htmlElement() {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type)
    : tabIndex_(tabIndex), type_(type) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text)
    : tabIndex_(tabIndex), type_(type), text_(text) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes)
    : tabIndex_(tabIndex), type_(type), text_(text), attributes_(attributes) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes, std::vector<htmlElement*>& children)
    : tabIndex_(tabIndex), type_(type), text_(text), attributes_(attributes), children_(children) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::map<std::string, std::string>& attributes)
    : tabIndex_(tabIndex), type_(type), attributes_(attributes) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::map<std::string, std::string>& attributes, std::vector<htmlElement*>& children)
    : tabIndex_(tabIndex), type_(type), attributes_(attributes), children_(children) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, std::vector<htmlElement*>& children)
    : tabIndex_(tabIndex), type_(type), children_(children) {}

// Destructor, will also delete all children
htmlElement::~htmlElement() {
    for ( auto child : children_ ) {
        delete child;
    }
    
    // Debug line
    //std::cout << "Deleting: " << type_ << " at tab index: " << tabIndex_ << std::endl;
    return;
}

// Render
bool htmlElement::htmlRender(std::ostream& stream) {
    stream << repeat("\t", tabIndex_) << "<" << type_;
    for (const auto& [key, value] : attributes_) {
        stream << " " << key << "=\"" << value << "\"";
    }
    stream << ">\n";

    if (!text_.empty()) {
        stream << repeat("\t", tabIndex_ + 1) << text_ << "\n";
    }

    for (const auto& child : children_) {
        child->htmlRender(stream);
    }

    stream << repeat("\t", tabIndex_) << "</" << type_ << ">\n";
    return true;
}


// Getters and Setters
int htmlElement::get_tab_index() {
    return tabIndex_;
}

void htmlElement::set_tab_index(const int& tabIndex) {
    tabIndex_ = tabIndex;
}

std::string htmlElement::get_type() {
    return type_;
}

void htmlElement::set_type(const std::string& type) {
    type_ = type;
}

std::string htmlElement::get_text() {
    return text_;
}

void htmlElement::set_text(const std::string& text) {
    text_ = text;
}

std::map<std::string, std::string> htmlElement::get_attribues() {
    return attributes_;
}

void htmlElement::set_attributes(const std::map<std::string, std::string>& attributes) {
    attributes_ = attributes;
}

// Children access
std::vector<htmlElement*>& htmlElement::get_children() {
    return children_;
}

void htmlElement::set_children(std::vector<htmlElement*>& children) {
    children_ = children;
}

// Attribute methods
void htmlElement::set_attribute(const std::string& key, const std::string& value) {
    attributes_[key] = value;
}

void htmlElement::remove_attribute(const std::string& key) {
    if ( attributes_.find(key) != attributes_.end() ) { 
        attributes_.erase(key);
    }
    else {
        throw htmlError("No attribute of type "  + key + ".");
    }
}

// Child management
void htmlElement::add_child(htmlElement* child) {
    children_.push_back(child);
}

void htmlElement::remove_child(size_t index) {
    if ( children_.size() < index ) {
        children_.erase(children_.begin() + index);
    }
    else {
        throw std::out_of_range("Index does not exist for children_");
    }
}

void htmlElement::clear_children() {
    children_.clear();
}

std::string htmlElement::repeat(const std::string& str, const size_t& times) {
    std::string result;
    result.reserve(str.size() * times);
    for (size_t i = 0; i < times; ++i)
        result += str;
    return result;
}