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
    // Read in the Adjustments and set them here

    // Use the Adjustments to create the styles_
    std::string default_ = "background-color: " + backgroundColor_ + "; color: " + textColor_ + "; padding: " + std::to_string(padding_) + "; margin: " + std::to_string(margin_) + ";";
    std::string blockquote_ = "background-color: " + darkenColor(backgroundColor_, 0.15) + "; color: " + textColor_ + "; padding: " + std::to_string(padding_) + "; margin: " + std::to_string(margin_) + "; border-left: 4px solid " + darkenColor(backgroundColor_, 0.3) + "; border-radius: 4px; padding-left: 2px;";
    std::string code_ = "background-color: " + backgroundColor_ + "; color: " + textColor_ + "; padding: max(2px, " + std::to_string(padding_) + "); margin: " + std::to_string(margin_) + "; width: fit-content;";
    styles_ = std::map<std::string, std::map<std::string, std::string>>{
        { "default", { { "style", default_ } } },
        { "heading",  { { "style", default_ } } },
        { "paragraph",  { { "style", default_ } } },
        { "blockquote",  { { "style", blockquote_ } } },
        { "list",  { { "style", default_ } } },
        { "item",  { { "style", default_ } } },
        { "code",  { { "style", code_ } } },
        { "code_content", { { "style", default_ + " width: fit-content;" } } }
    };
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

    if ( count < 1 || count > 6 ) {
        throw renderError("Bad Count for Heading.");
        return;
    }

    std::string remainingLine = line.substr(count + 1);

    htmlElement* heading = new htmlElement(parent->get_tab_index() + 1, "h" + std::to_string(count), renderText(remainingLine), styles_["heading"]);

    parent->add_child(heading);
}

void mdRender::renderBlockQuote(htmlElement* parent, std::string& line) {
    // Since nesting is not permited outside of lists, just check that the line is started with "> " and send the rest to this->renderText(blockQuote, remainingLine)
    if ( line[0] != '>' && line[1] != ' ' ) {
        throw renderError("Bad Block Quote.");
        return;
    }

    std::string remainingLine = line.substr(2);

    htmlElement* quote = new htmlElement(parent->get_tab_index() + 1, "div", styles_["blockquote"]);

    htmlElement* paragraph = new htmlElement(quote->get_tab_index() + 1, "p", renderText(remainingLine), styles_["paragraph"]);

    quote->add_child(paragraph);

    parent->add_child(quote);
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