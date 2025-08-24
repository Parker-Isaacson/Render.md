#ifndef RENDER_H
#define RENDER_H

#include <exception>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "html.h"
#include "colors.h"

class renderError : public std::exception {
private:
    std::string msg_; // No default error

public:
    renderError(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
};

class mdRender {
private:
    std::istream& inStream_;
    std::ostream& outStream_ = std::cout;
    htmlElement* root_;

    // Attributes that originate from Adjustmnets, handled at the start of the render
    // No getters or setters as these should be writen into the markdown or passed as parameters
    int tabWidth_ = 2;
    int padding_ = 0;
    int margin_ = 0;
    std::string textColor_ = "#000000";
    std::string backgroundColor_ = "#ffffff";

    // Defualt styles for fast lookup
    // Should contain, heading, paragraph, blockquote, code, list, list-item, defualt
    std::map<std::string, std::map<std::string, std::string>> styles_ = std::map<std::string, std::map<std::string, std::string>>{
        { "default", { { "class", "default" } } },
        { "heading",  { { "class", "heading" } } },
        { "paragraph",  { { "class", "paragraph" } } },
        { "blockquote",  { { "class", "blockquote" } } },
        { "list",  { { "class", "list" } } },
        { "item",  { { "class", "default" } } },
        { "code",  { { "class", "code" } } },
        { "taskList",  { { "class", "taskList"} } },
        { "task", { { "class", "task" }, { "type", "checkbox" } } },
        { "checked", { { "class", "checked" }, { "type", "checkbox" }, { "checked", "checked" } } },
        { "dd",  { { "class", "dd" } } },
        { "table", { { "class", "table" } } },
        { "cell", { { "class", "cell" } } }
    };;

    // Utility Methods
    size_t countLeadingChar(const std::string& s, char c);
    size_t countTrailingChar(const std::string& s, char c);
    void trim(std::string& s);
    void warn(const std::string& warning);
    std::string genFootnote(std::string footnote, bool isRef);

    // Rendering Methods
    void renderText(std::string& line);
    void renderHeading(htmlElement* parent, std::string& line);
    void renderBlockQuote(htmlElement* parent, std::string& line);
    void renderList(htmlElement* parent, std::vector<std::string>& lines);
    void renderTaskList(htmlElement* parent, std::vector<std::string>& lines);
    void renderFencedCode(htmlElement* parent, std::vector<std::string>& lines);
    void renderDefinitionList(htmlElement* parent, std::vector<std::string>& lines);
    void renderOrderedList(htmlElement* parent, std::vector<std::string> lines);
    void rendertable(htmlElement* parent, std::vector<std::string> lines);
public:
    // Constructors
    mdRender(std::istream& inStream, htmlElement* root);
    mdRender(std::istream& inStream, std::ostream& outStream, htmlElement* root);

    // Method for outputing
    void output();
    void output(std::ostream& outStream);

    // Rendering Methods
    void render(); 
    void render(htmlElement* parent);
};

#endif // RENDER_H
