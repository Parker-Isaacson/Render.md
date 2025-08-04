#ifndef RENDER_H
#define RENDER_H

#include <exception>
#include <vector>
#include <string>
#include <fstream>

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
    htmlElement root_;

    // Attributes that originate from Adjustmnets, handled at the start of the render
    // No getters or setters as these should be writen into the markdown or passed as parameters
    int tabWidth_ = 2;
    int padding_ = 0;
    int margin_ = 0;
    std::string textColor_ = "#000000";
    std::string backgroundColor_ = "#ffffff";

    // Defualt styles for fast lookup
    // Should contain, heading, paragraph, blockquote, code, list, list-item, defualt
    std::map<std::string, std::map<std::string, std::string>> styles_;

public:
    // Constructors
    mdRender(std::istream& inStream, const htmlElement& root);
    mdRender(std::istream& inStream, std::ostream& outStream, const htmlElement& root);

    // Rendering Methods
    void render(); // Bypass because void render(htmlElement& parent = root_); is not allowed :(
    void render(htmlElement& parent);
    void renderText(htmlElement& parent, std::string& line); // Checks a give line for Bold, Italics, Code, Link, or Image
    void renderHeading(htmlElement& parent, std::string& line);
    void renderBlockQuote(htmlElement& parent, std::string& line);
    void renderList(htmlElement& parent, std::vector<std::string>& lines);

    // Method for outputing
    void output(std::ostream& outStream_ = std::cout);
};

#endif // RENDER_H
