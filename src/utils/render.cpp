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
    // Load the file into memory
    std::vector<std::string> lines;
    std::string line;
    while ( getline(inStream_, line) ) { // Loads file into memory
        lines.push_back(line);
    }
    if ( lines.size() < 1) {
        throw renderError("No Lines in File!");
        return;
    }
    int i = 0;
    // Read in the Adjustments and set them here
    for (; lines[i][0] == '?' && i < lines.size(); i++ ) {
        size_t pos = lines[i].find("==");
        std::string adjustmentName = lines[i].substr(1, pos - 1);
        std::string adjustmentValue = lines[i].substr(pos + 2);
        // I cant switch a std::string ;-;
        if ( adjustmentName == "tab-width" ) {
            tabWidth_ = std::stoi(adjustmentValue);
        } else if ( adjustmentName == "text-color" ) {
            textColor_ = adjustmentValue;
        } else if ( adjustmentName == "background-color" ) {
            backgroundColor_ = adjustmentValue;
        } else if ( adjustmentName == "margin" ) {
            margin_ = std::stoi(adjustmentValue);
        } else if ( adjustmentName == "padding" ) {
            padding_ = std::stoi(adjustmentValue);
        } else {
            throw renderError("Bad adjustment name!");
            return;
        }
    }
    // Use the Adjustments to create the styles_
    std::string default_ = "background-color: " + backgroundColor_ + "; color: " + textColor_ + "; padding: " + std::to_string(padding_) + "; margin: " + std::to_string(margin_) + ";";
    std::string blockquote_ = "background-color: " + darkenColor(backgroundColor_, 0.15) + "; color: " + textColor_ + "; padding: " + std::to_string(padding_) + "; margin: " + std::to_string(margin_) + "; border-left: 4px solid " + darkenColor(backgroundColor_, 0.3) + "; border-radius: 4px; padding-left: 2px;";
    std::string code_ = "background-color: " + backgroundColor_ + "; color: " + textColor_ + "; padding: max(2px, " + std::to_string(padding_) + "); margin: " + std::to_string(margin_) + "; width: fit-content;";
    std::string list_ = "background-color: " + backgroundColor_ + "; color: " + textColor_ + "; padding: " + std::to_string(padding_) + "," + std::to_string(padding_) + "," + std::to_string(padding_) + "," + std::to_string(padding_ + 10) + "; margin: " + std::to_string(margin_) + ";";
    styles_ = std::map<std::string, std::map<std::string, std::string>>{
        { "default", { { "style", default_ } } },
        { "heading",  { { "style", default_ } } },
        { "paragraph",  { { "style", default_ } } },
        { "blockquote",  { { "style", blockquote_ + " width: fit-content;" } } },
        { "list",  { { "style", list_ } } },
        { "item",  { { "style", default_ } } },
        { "code",  { { "style", code_ } } },
        { "code_content", { { "style", default_ + " width: fit-content;" } } }
    };
    // Now we check line by line for what it is
    for (; i < lines.size(); i++ ) {
        if ( lines[i] == "---" ) { // hr
            parent->add_child(new htmlElement(parent->get_tab_index() + 1, "hr"));
        } else if ( lines[i][0] == '#' ) { // Heading
            renderHeading(parent, lines[i]);
        } else if ( lines[i][0] == '>' ) { // Blockquote
            renderBlockQuote(parent, lines[i]);
        } else if ( lines[i][0] == '-' ) { // List
            std::vector<std::string> listLines;
            for (; i < lines.size() && lines[i][0] == '-'; i++) {
                listLines.push_back(lines[i]);
            }
            renderList(parent, listLines);
        } else if ( lines[i] == "" ) { // Blank Line
            continue;
        } else {
            line = lines[i];
            renderText(line);
            htmlElement* paragraph = new htmlElement(parent->get_tab_index() + 1, "p", line, styles_["paragraph"]);
            parent->add_child(paragraph);
        }
    }
}

// This is the faster re-write
void mdRender::renderText(std::string& line) {
    std::string result;
    result.reserve(line.size() * 2); // reserve extra for tags

    bool isItalics = false;
    bool isBold = false;
    bool isBoth = false;

    for (size_t i = 0; i < line.size(); ) {
        if ( line.compare(i, 3, "***") == 0 ) { // both
            result += (isBoth ? "</b></i>" : "<b><i>");
            isBoth = !isBoth;
            i += 3;
        } else if ( line.compare(i, 2, "**") == 0 ) { // bold
            result += (isBold ? "</b>" : "<b>");
            isBold = !isBold;
            i += 2;
        } else if ( line[i] == '*' ) { // italics
            result += (isItalics ? "</i>" : "<i>");
            isItalics = !isItalics;
            i++;
        } else if ( line.compare(i, 2, "![") == 0 ) { // image
            size_t closingBracket = line.find(']', i + 2);
            size_t closingParen = line.find(')', closingBracket + 1);
            if (closingBracket == std::string::npos || closingParen == std::string::npos)
                throw renderError("Bad image, as of now try and avoid using ![.");
            std::string alt = line.substr(i + 2, closingBracket - (i + 2));
            std::string image = line.substr(closingBracket + 2, closingParen - (closingBracket + 2));
            result += "<img src=\"" + image + "\" alt=\"" + alt + "\">";
            i = closingParen + 1;
        } else if ( line[i] == '[' ) { // link
            size_t closingBracket = line.find(']', i + 1);
            size_t closingParen = line.find(')', closingBracket + 1);
            if (closingBracket == std::string::npos || closingParen == std::string::npos)
                throw renderError("Bad link, as of now try and avoid using [.");
            std::string inside = line.substr(i + 1, closingBracket - (i + 1));
            std::string link = line.substr(closingBracket + 2, closingParen - (closingBracket + 2));
            result += "<a href=\"" + link + "\">" + inside + "</a>";
            i = closingParen + 1;
        } else if ( line.compare(i, 2, "~~") == 0 ) { // strikethrough
            size_t closer = line.find("~~", i + 1);
            if ( closer == std::string::npos )
                throw renderError("Bad strikethrough, as of now try and avoid using ~~.");
            std::string inside = line.substr(i + 2, closer - (i + 2));
            result += "<s>" + inside + "</s>";
            i = closer + 2;
        } else if ( line[i] == '^' ) {
            size_t closer = line.find('^', i + 1);
            if ( closer == std::string::npos )
                throw renderError("Bad superscript, as of now try and avoid using ^.");
            std::string inside = line.substr(i + 1, closer - ( i + 1 ) );
            result += "<sup>" + inside + "</sup>";
            i = closer + 1;
        } else if ( line[i] == '~' ) {
            size_t closer = line.find('~', i + 1);
            if ( closer == std::string::npos )
                throw renderError("Bad superscript, as of now try and avoid using ~.");
            std::string inside = line.substr(i + 1, closer - ( i + 1 ) );
            result += "<sub>" + inside + "</sub>";
            i = closer + 1;
        }else {
            result.push_back(line[i]);
            i++;
        }
    }

    // Close any unclosed tags
    if (isBoth) result += "</b></i>";
    if (isBold) result += "</b>";
    if (isItalics) result += "</i>";

    line = std::move(result); // overwrite input string
}

// There is a faster way to write this, put everything in 1 for loop that looks letter by letter.
// Then look for specific things and continue. Should lower the hidden coeffiecnt of the big O.
void renderTextOld(std::string& line) {
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

    htmlElement* quote = new htmlElement(parent->get_tab_index() + 1, "div", styles_["default"]);

    htmlElement* paragraph = new htmlElement(quote->get_tab_index() + 1, "p", remainingLine, styles_["blockquote"]);

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
void mdRender::output() {
    root_->htmlRender(outStream_);
    delete root_;
}

void mdRender::output(std::ostream& outStream) {
    root_->htmlRender(outStream);
    delete root_;
}