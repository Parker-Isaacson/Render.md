#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>

class HTML_Element {
    private:
        // Private variables
        int tab_index = 0; // By default exist at root
        std::string type = "div"; // By defualt be a div
        std::string text = ""; // By default have no internal text
        std::map<std::string, std::string> attributes; // By defualt have no attribues
        std::vector<HTML_Element*> children; // By defualt have no children

        // Private methods
        std::string repeat(const std::string& str, const size_t& times) {
            std::string result;
            result.reserve(str.size() * times);
            for (size_t i = 0; i < times; ++i)
                result += str;
            return result;
        }
    public:
        // Getters and Setters for all private variables
        int get_tab_index() { return tab_index; }
        void set_tab_index(const int& val) { tab_index = val; } 
        std::string get_type() { return type; }
        void set_type(const std::string& val) { type = val; }
        std::string get_text() { return text; }
        void set_text(const std::string& val) { text = val; }
        std::map<std::string, std::string> get_attribues() { return attributes; }
        void set_attributes(const std::map<std::string, std::string>& val) { attributes = val; }
        std::vector<HTML_Element*> get_children() { return children; }
        void set_children(const std::vector<HTML_Element*>& val) { children = val; }
        
        // Special Getters and Setters
        void set_attribute(const std::string& key, const std::string& val) { attributes[key] = val; }
        void remove_attribute(const std::string& key) { attributes.erase(key); }
        void add_child(HTML_Element* child) { children.push_back(child); } // Cant be const??
        // No support for finding a child, loop through the result of get_children()
        void remove_child(const std::vector<HTML_Element*>::const_iterator& index) { children.erase(index); }
        void remove_children(const std::vector<HTML_Element*>::const_iterator& first, const std::vector<HTML_Element*>::const_iterator& last) { children.erase(first, last); }

        // Constructors, might add support for others if it becomes needed, through can be added externally
        HTML_Element() {}
        HTML_Element(const int& tabIndex, const std::string& elementType) : tab_index(tabIndex), type(elementType) {}
        HTML_Element(const int& tabIndex, const std::string& elementType, const std::string& innerText) : tab_index(tabIndex), type(elementType), text(innerText) {}
        HTML_Element(const int& tabIndex, const std::string& elementType, const std::string& innerText, const std::map<std::string, std::string>& attrs) : tab_index(tabIndex), type(elementType), text(innerText), attributes(attrs) {}
        HTML_Element(const int& tabIndex, const std::string& elementType, const std::string& innerText, const std::map<std::string, std::string>& attrs, const std::vector<HTML_Element*>& childElements) : tab_index(tabIndex), type(elementType), text(innerText), attributes(attrs), children(childElements) {}

        // Computation Methods
        /* Example on how to call
        HTML_Element root;
        root.HTML_Render(std::cout); // For output to terminal
        std::ofstream file("output.txt"); // For output to file
        if ( file.is_open() )
            root.HTML_Render(file);
        file.close();
        */
        bool HTML_Render(std::ostream& stream) { // stream must be open if it is a file
            // Build staring tag
            stream << repeat("\t", tab_index) << "<" << type; // Builds the start; like "<div"
            for ( auto& [key, value] : attributes ) { // Builds the attributes; like " class="wrapper""
                stream << " " << key << "=\"" << value << "\"";
            }
            stream << ">\n"; // Builds the last part of the starting tag
            if ( text != "" ) { // If the internal text is not empty
                stream << repeat("\t", tab_index + 1) << text << "\n"; // Place it on the next line, indented by 1 
            }
            
            // Call the build for all children
            for ( auto* child : children) {
                child->HTML_Render(stream);
            }
            
            // Build the end tag
            stream << repeat("\t", tab_index) << "</" << type << ">" << "\n"; // Builds the ending tab; like "</div>"
            return true;
        } 
};

int main() {
    HTML_Element* root = new HTML_Element(0, "div", "", {{"class", "wrapper"}, {"href", "../IMAGES/photo.svg"}});
    HTML_Element* childa = new HTML_Element(1, "p", "Hello World!", {{"class", "bold"}});
    HTML_Element* childb = new HTML_Element(1, "span", "I am a span!");
    HTML_Element* childa1 = new HTML_Element(2, "p", "I am a single child!");
    
    root->add_child(childa);
    root->add_child(childb);
    childa->add_child(childa1);
    root->HTML_Render(std::cout);

    delete root;
    delete childa;
    delete childb;
    delete childa1;

    return 0;
}