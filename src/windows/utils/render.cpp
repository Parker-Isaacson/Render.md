#include "render.h"

// Utility Functions
size_t countLeadingChar(const std::string& s, char c) {
    size_t count = 0;
    while ( count < s.size() && s[count] == c ) {
        count++;
    }
    return count;
}

size_t countTrailingChar(const std::string& s, char c) {
    size_t count = 0;
    size_t i = s.size();
    while ( i > 0 && s[i - 1] == c ) {
        count++;
        i--;
    }
    return count;
}

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
    std::string line;
    while ( getline(inStream_, line) ) {
        std::cout << line << std::endl;
    }
}

// There is a faster way to write this, put everything in 1 for loop that looks letter by letter.
// Then look for specific things and continue. Should lower the hidden coeffiecnt of the big O.
void mdRender::renderText(std::string& line) {
    bool isItalics = false;
    bool isBold = false;
    bool isBoth = false;

    size_t pos = line.find("***"); // We first look for ***, or both
    while ( pos != std::string::npos ) {
        line.erase(pos, 3);
        if ( isBoth ) {
            line.insert(pos, "</b></i>"); // We need to close at where we found ***
            isBoth = false;
        } else {
            line.insert(pos, "<b><i>"); // We need to open at where we found ***
            isBoth = true;
        }
        pos = line.find("***", pos + 1);
    }

    pos = line.find("**"); // We first look for **, or bold
    while ( pos != std::string::npos ) {
        line.erase(pos, 2);
        if ( isBold ) {
            line.insert(pos, "</b>"); // We need to close at where we found **
            isBold = false;
        } else {
            line.insert(pos, "<b>"); // We need to open at where we found **
            isBold = true;
        }
        pos = line.find("**", pos + 1);
    }

    pos = line.find("*"); // We first look for *, or italics
    while ( pos != std::string::npos ) {
        line.erase(pos, 1);
        if ( isItalics ) {
            line.insert(pos, "</i>"); // We need to close at where we found *
            isItalics = false;
        } else {
            line.insert(pos, "<i>"); // We need to open at where we found *
            isItalics = true;
        }
        pos = line.find("*", pos + 1);
    }

    if ( isBoth ) { // Just in case they are not closed, we close them at the end of the line
        line += "</b></i>";
    } else {
        if ( isBold ) {
            line += "</b>";
        } 
        if ( isItalics ) {
            line += "</i>";
        }
    }

    // Now we need to look for links or images
    pos = line.find("![");
    while ( pos != std::string::npos ) { // We found a image
        size_t closingBracket = line.find(']', pos);
        size_t closingParen = line.find(')', pos);

        if ( closingBracket == std::string::npos || closingParen == std::string::npos ) {
            throw renderError("Bad image, as of now try and avoid using ![.");
        }

        std::string alt = line.substr(pos + 2, closingBracket - pos - 2);
        std::string image = line.substr(closingBracket + 2, closingParen - closingBracket - 2);

        line.erase(pos, closingParen - pos + 1);
        line.insert(pos, "<img src=\"" + image + "\" alt=\"" + alt + "\">");

        pos = line.find("![", pos + 1);
    }

    pos = line.find('[');
    while ( pos != std::string::npos ) { // We found a link
        size_t closingBracket = line.find(']', pos);
        size_t closingParen = line.find(')', pos);

        if ( closingBracket == std::string::npos || closingParen == std::string::npos ) {
            throw renderError("Bad link, as of now try and avoid using [.");
        }

        std::string inside = line.substr(pos + 1, closingBracket - pos - 1);
        std::string link = line.substr(closingBracket + 2, closingParen - closingBracket - 2);

        line.erase(pos, closingParen - pos + 1);
        line.insert(pos, "<a href=\"" + link + "\">" + inside + "</a>");

        pos = line.find('[', pos + 1);
    }
}

void mdRender::renderHeading(htmlElement* parent, std::string& line) {
    size_t count = countLeadingChar(line, '#'); // counts the number of '#'

    if ( count < 1 || count > 6 ) {
        throw renderError("Bad Count for Heading.");
        return;
    }

    std::string remainingLine = line.substr(count + 1);

    renderText(remainingLine);

    htmlElement* heading = new htmlElement(parent->get_tab_index() + 1, "h" + std::to_string(count), remainingLine, styles_["heading"]);

    parent->add_child(heading);
}

void mdRender::renderBlockQuote(htmlElement* parent, std::string& line) {
    if ( line[0] != '>' || line[1] != ' ' ) {
        throw renderError("Bad Block Quote.");
        return;
    }

    std::string remainingLine = line.substr(2);

    renderText(remainingLine);

    htmlElement* quote = new htmlElement(parent->get_tab_index() + 1, "div", styles_["blockquote"]);

    htmlElement* paragraph = new htmlElement(quote->get_tab_index() + 1, "p", remainingLine, styles_["paragraph"]);

    quote->add_child(paragraph);

    parent->add_child(quote);
}

void mdRender::renderList(htmlElement* parent, std::vector<std::string>& lines) {
    if ( lines.empty() ) {
        throw renderError("Empty list.");
        return;
    }

    if ( lines[0][0] != '-' || lines[0][1] != ' ' ) {
        throw renderError("Bad List Format.");
        return;
    }

    htmlElement* list = new htmlElement(parent->get_tab_index() + 1, "ul", styles_["list"]);

    for ( std::string line : lines ) {
        if ( line[0] != '-' || line[1] != ' ' ) {
            throw renderError("Bad line in list.");
            break; // Just leave this loop, will add whatever is in the current list to parent
        }
        std::string remainingLine = line.substr(2);
        renderText(remainingLine);
        htmlElement* item = new htmlElement(list->get_tab_index() + 1, "li", remainingLine, styles_["list"]);
        list->add_child(item);
    }

    parent->add_child(list);
}

// Output method
void mdRender::output(std::ostream& outStream_) {
    root_->htmlRender(outStream_);
    delete root_;
}