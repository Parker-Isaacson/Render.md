#include "html.h"

// Getters and Setters
int htmlElement::get_tab_index() { return tabIndex_; }
void htmlElement::set_tab_index(const int& val) { tabIndex_ = val; }

std::string htmlElement::get_type() { return type_; }
void htmlElement::set_type(const std::string& val) { type_ = val; }

std::string htmlElement::get_text() { return text_; }
void htmlElement::set_text(const std::string& val) { text_ = val; }

std::map<std::string, std::string> htmlElement::get_attribues() { return attributes_; }
void htmlElement::set_attributes(const std::map<std::string, std::string>& val) { attributes_ = val; }

std::vector<std::unique_ptr<htmlElement>>& htmlElement::get_children() { return children_; }
void htmlElement::set_children(std::vector<std::unique_ptr<htmlElement>>&& val) { children_ = std::move(val); }

// Attribute methods
void htmlElement::set_attribute(const std::string& key, const std::string& val) {
    attributes_[key] = val;
}

void htmlElement::remove_attribute(const std::string& key) {
    attributes_.erase(key);
}

// Child methods
void htmlElement::add_child(std::unique_ptr<htmlElement> child) {
    children_.push_back(std::move(child));
}

void htmlElement::remove_child(size_t index) {
    if (index < children_.size()) {
        children_.erase(children_.begin() + index);
    }
}

void htmlElement::clear_children() {
    children_.clear();
}

// Constructors
htmlElement::htmlElement() {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type)
    : tabIndex_(tabIndex), type_(type) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text)
    : tabIndex_(tabIndex), type_(type), text_(text) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes)
    : tabIndex_(tabIndex), type_(type), text_(text), attributes_(attributes) {}

htmlElement::htmlElement(const int& tabIndex, const std::string& type, const std::string& text, const std::map<std::string, std::string>& attributes, std::vector<std::unique_ptr<htmlElement>>&& children)
    : tabIndex_(tabIndex), type_(type), text_(text), attributes_(attributes), children_(std::move(children)) {}

// Render function
/* Example
htmlElement root;
root.HTML_Render(std::cout); // For output to terminal
std::ofstream file("output.txt"); // For output to file
if ( file.is_open() )
    root.HTML_Render(file);
file.close();
*/
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

std::string htmlElement::repeat(const std::string& str, const size_t& times) {
    std::string result;
    result.reserve(str.size() * times);
    for (size_t i = 0; i < times; ++i)
        result += str;
    return result;
}
