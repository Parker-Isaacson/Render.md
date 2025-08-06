#include "render.h"

// Constructors
mdRender::mdRender(std::istream& inStream, htmlElement* root)
    : inStream_(inStream), root_(root) {}

mdRender::mdRender(std::istream& inStream, std::ostream& outStream, htmlElement* root)
    : inStream_(inStream), outStream_(outStream), root_(root) {}

// Rendering methods
void mdRender::render() {
    this->render(root_); // Bypass because void render(htmlElement* parent = root_); is not allowed :(
}

void mdRender::render(htmlElement* parent) {
    // Must handle all attribues that exist at the top of the doc
    // Must init styles_ with requried styles, pretty much 1 for each of the types of things we will use
    // Should go line by line and determine what other function needs to be called
        // Might need to adjust how the logic here works :3
}

// Just returns the text that should be output from the line, up to the caller to handle it
std::string mdRender::renderText(std::string& line) {
    // This is another heavy function
    // Must handle Bold Italics Code, link and Images ( inline )
        // Bold or Italics, just insert a <b> or <i> into the string that will be placed
        // Code, need to change css to have the a new div inline with the text
        // Link and Images, pretty much just change what gets sent to the buffer
    
    return line;
}

void mdRender::renderHeading(htmlElement* parent, std::string& line) {
    // Simple one, just count # and then send the rest to this->renderText(header, remainingLine)
    size_t count = line.find_first_not_of('#'); // I love this <3

    std::string remainingLine = line.substr(count + 1);

    htmlElement* heading = new htmlElement(parent->get_tab_index() + 1, "h" + std::to_string(count), renderText(remainingLine));

    parent->add_child(heading);
}

void mdRender::renderBlockQuote(htmlElement* parent, std::string& line) {
    // Since nesting is not permited outside of lists, just check that the line is started with "> " and send the rest to this->renderText(blockQuote, remainingLine)
}

void mdRender::renderList(htmlElement* parent, std::vector<std::string>& lines) {
    // This is another heavy function
    // Must only handle <ul>?
    // https://cplusplus.com/reference/string/string/find_first_not_of/ -> this might be useful for finding tabs
        // ^index % tabWidth_ == 0 ? "Valid depth" : "invalid depth"
        // tabIndex = ^index / tabWidth_ // So we know if it is one level nested or more
        // If nested is found just call this->renderList with the lines that are nested, should support infinite depth
}

// Output method
void mdRender::output(std::ostream& outStream_) {
    root_->htmlRender(outStream_);
    delete root_;
}