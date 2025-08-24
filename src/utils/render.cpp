#include "render.h"

// Utility Functions
size_t mdRender::countLeadingChar(const std::string& s, char c) {
    size_t count = 0;
    while ( count < s.size() && s[count] == c ) {
        count++;
    }
    return count;
}

size_t mdRender::countTrailingChar(const std::string& s, char c) {
    size_t count = 0;
    size_t i = s.size();
    while ( i > 0 && s[i - 1] == c ) {
        count++;
        i--;
    }
    return count;
}

void mdRender::trim(std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    s = s.substr(start, end - start);
}

// Method to quickly print out a warning
void mdRender::warn(const std::string& warning) {
    std::cout << "\033[38;5;208mWARNING:\033[0m " << warning << "\n";
}

// Method to create text for a footnote
// isRef should be set true if being called during renderText
std::string mdRender::genFootnote(std::string footnote, bool isRef) {
    if ( footnote[0] != '[' || footnote[1] != '^' || footnote[footnote.length() - 1] != ']' ) {
        throw renderError("Bad Footnote.");
    }

    std::string fnValue = footnote.substr(2, footnote.length() - 3 );

    /*
    <sup id="fnref:1"><a href="#fn:1">1</a></sup> returned when isRef == true
    <sup id="fn:1"><a href="#fnref:1">1</a></sup> returned when isRef == false
    */

    std::string fn = "<sup id=\"";
    if ( isRef ) // handle the id
        fn += "#fnref:" + fnValue;
    else
        fn += "#fn:" + fnValue;
    fn += "\"><a href=\"";
    if ( isRef ) // handle the anchor
        fn += "#fn:" + fnValue;
    else
        fn += "#fnref:" + fnValue;
    fn += "\">" + fnValue + "</a></sup>";
    return fn;
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
    std::string styles = 
    ".default{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";}"
    ".heading{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";}"
    ".paragraph{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";}"
    ".blockquote{background-color:" + darkenColor(backgroundColor_,0.15) + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";border-left:4px solid " + darkenColor(backgroundColor_,0.3) + ";border-radius:4px;padding-left:2px;width:fit-content;}"
    ".list{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + "," + std::to_string(padding_) + "," + std::to_string(padding_) + "," + std::to_string(padding_+10) + ";margin:" + std::to_string(margin_) + ";}"
    ".item{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";}"
    ".code{background-color:" + darkenColor(backgroundColor_,0.3) + ";color:" + textColor_ + ";padding:" + std::to_string(padding_+2) + ";margin:" + std::to_string(margin_) + ";border-radius:4px;font-family:monospace;}"
    ".taskList{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + "," + std::to_string(padding_) + "," + std::to_string(padding_) + "," + std::to_string(padding_+10) + ";margin:" + std::to_string(margin_) + ";list-style:none;}"
    ".task{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";}"
    ".checked{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";}"
    ".dd{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";}"
    ".table{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";border-collapse:collapse;width:100%;text-align:left;}"
    ".cell{background-color:" + backgroundColor_ + ";color:" + textColor_ + ";padding:" + std::to_string(padding_) + ";margin:" + std::to_string(margin_) + ";border:1px solid " + darkenColor(backgroundColor_,0.3) + ";}";
    htmlElement* head = new htmlElement(parent->get_tab_index(), "head");
    htmlElement* style = new htmlElement(head->get_tab_index(), "style", styles);
    head->add_child(style);
    parent->add_child(head);
    // Now we check line by line for what it is
    htmlElement* body = new htmlElement(parent->get_tab_index(), "body");
    for (; i < lines.size(); i++ ) {
        if ( lines[i] == "---" ) { // hr
            body->add_child(new htmlElement(body->get_tab_index() + 1, "hr"));
        } else if ( lines[i][0] == '#' ) { // Heading
            renderHeading(body, lines[i]);
        } else if ( lines[i][0] == '>' ) { // Blockquote
            renderBlockQuote(body, lines[i]);
        } else if ( lines[i].compare(0, 3, "- [") == 0 ) {
            std::vector<std::string> listLines;
            for (; i < lines.size() && lines[i].compare(0, 3, "- [") == 0; i++) {
                listLines.push_back(lines[i]);
            }
            renderTaskList(body, listLines);
        } else if ( lines[i][0] == '-' ) { // List
            std::vector<std::string> listLines;
            for (; i < lines.size() && lines[i][0] == '-'; i++) {
                listLines.push_back(lines[i]);
            }
            renderList(body, listLines);
        } else if (std::isdigit(lines[i][0])) { // Ordered list
            std::vector<std::string> listLines;
            for (; i < lines.size(); i++) {
                const std::string& line = lines[i];
                size_t pos = 0;
                while (pos < line.size() && std::isdigit(line[pos])) pos++;
                if (pos == 0 || pos >= line.size() || line[pos] != '.') break;
                listLines.push_back(line);
            }
            renderOrderedList(body, listLines);
        } else if ( lines[i] == "```" ) { // Fenced Code Block
            std::vector<std::string> codeLines;
            i += 1; // Jump top the actuall code
            for (; i < lines.size() && lines[i] != "```"; i++ ) {
                codeLines.push_back(lines[i]);
            }
            i += 1; // Jump off the code
            renderFencedCode(body, codeLines);
        } else if ( lines[i][0] == '|' ) { // Table
            std::vector<std::string> tableLines;
            for (; i < lines.size() && lines[i][0] == '|'; i++) {
                tableLines.push_back(lines[i]);
            }
            rendertable(body, tableLines);
        } else if ( i + 1 < lines.size() && !lines[i + 1].empty() && lines[i + 1][0] == ':' ) { // Definition list
            size_t j = i + 1;
            std::vector<std::string> definitionList = { lines[i] };
            while (j < lines.size() && !lines[j].empty() && lines[j][0] == ':') {
                definitionList.push_back(lines[j]);
                j++;
            }
            i = j - 1;
            renderDefinitionList(body, definitionList);
        } else if ( lines[i].compare(0, 2, "[^") == 0 ) { // Footnote
            line = lines[i];
            size_t pos = line.find(']', 2);
            std::string footnote = line.substr(0, pos + 1);
            std::string inside = line.substr(pos + 1);
            renderText(inside);
            htmlElement* paragraph = new htmlElement(body->get_tab_index() + 1, "p", genFootnote(footnote, false) + inside, styles_["paragraph"]);
            body->add_child(paragraph);
        } else if ( lines[i] == "" ) { // Blank Line
            continue;
        } else {
            line = lines[i];
            renderText(line);
            htmlElement* paragraph = new htmlElement(body->get_tab_index() + 1, "p", line, styles_["paragraph"]);
            body->add_child(paragraph);
        }
    }
    parent->add_child(body);
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
            if ( i > 0 && line[i - 1] == '\\' ) {
                result += line[i];
                i++;
                continue;
            }
            result += (isItalics ? "</i>" : "<i>");
            isItalics = !isItalics;
            i++;
        } else if ( line.compare(i, 2, "![") == 0 ) { // image
            size_t closingBracket = line.find(']', i + 2);
            size_t closingParen = line.find(')', closingBracket + 1);
            if (closingBracket == std::string::npos || closingParen == std::string::npos) {
                warn("Image not successfully closed!");
                result.push_back(line[i]);
                result.push_back(line[i + 1]);
                i += 2;
                continue;
            }
            std::string alt = line.substr(i + 2, closingBracket - (i + 2));
            std::string image = line.substr(closingBracket + 2, closingParen - (closingBracket + 2));
            result += "<img src=\"" + image + "\" alt=\"" + alt + "\">";
            i = closingParen + 1;
        } else if ( line.compare(i, 2, "[^") == 0 ) { // Footnote
            size_t pos = line.find(']', i + 2);
            if ( pos == std::string::npos ) {
                warn("Footnote not closed.");
                result.push_back(line[i]);
                result.push_back(line[i + 1]);
                i += 2;
                continue;
            }
            std::string footnote = genFootnote(line.substr(i, pos - i + 1 ), true);
            result += footnote;
            i = pos + 1;
        } else if ( line[i] == '[' ) { // link
            size_t closingBracket = line.find(']', i + 1);
            size_t closingParen = line.find(')', closingBracket + 1);
            if (closingBracket == std::string::npos || closingParen == std::string::npos) {
                warn("Link not successfully closed!");
                result.push_back(line[i]);
                i++;
                continue;
            }
            std::string inside = line.substr(i + 1, closingBracket - (i + 1));
            std::string link = line.substr(closingBracket + 2, closingParen - (closingBracket + 2));
            result += "<a href=\"" + link + "\">" + inside + "</a>";
            i = closingParen + 1;
        } else if ( line.compare(i, 2, "~~") == 0 ) { // strikethrough
            size_t closer = line.find("~~", i + 1);
            if ( closer == std::string::npos ) {
                warn("Strikethrough not successfully closed!");
                result.push_back(line[i]);
                i += 2;
                continue;
            }
            std::string inside = line.substr(i + 2, closer - (i + 2));
            result += "<s>" + inside + "</s>";
            i = closer + 2;
        } else if ( line[i] == '^' ) { // superscript
            size_t closer = line.find('^', i + 1);
            if ( closer == std::string::npos ) {
                warn("Superscript not successfully closed!");
                result.push_back(line[i]);
                i++;
                continue;
            }
            std::string inside = line.substr(i + 1, closer - ( i + 1 ) );
            result += "<sup>" + inside + "</sup>";
            i = closer + 1;
        } else if ( line[i] == '~' ) { // subscript
            size_t closer = line.find('~', i + 1);
            if ( closer == std::string::npos ) {
                warn("Subscript not successfully closed!");
                result.push_back(line[i]);
                i++;
                continue;
            }
            std::string inside = line.substr(i + 1, closer - ( i + 1 ) );
            result += "<sub>" + inside + "</sub>";
            i = closer + 1;
        } else if ( line.compare(i, 2, "==") == 0 ) { // highlight
            size_t closer = line.find("==", i + 1);
            if ( closer == std::string::npos ) {
                warn("Highlight not successfully closed!");
                result.push_back(line[i]);
                i++;
                continue;
            }
            std::string inside = line.substr(i + 2, closer - (i + 2));
            result += "<mark>" + inside + "</mark>";
            i = closer + 2;
        } else if ( line[i] == '`' ) { // code
            size_t closer = line.find('`', i + 1);
            if ( closer == std::string::npos ) {
                warn("Code not successfully closed!");
                result.push_back(line[i]);
                i++;
                continue;
            }
            std::string inside = line.substr(i + 1, closer - ( i + 1 ) );
            result += "<span";
            for ( const auto& [key, value] : styles_["code"] ) {
                result += " " + key + "=\"" + value + "\"";
            }
            result += ">" + inside + "</span>";
            i = closer + 1;
        } else {
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
    
    std::map<std::string, std::string> idStyle = styles_["heading"];
    size_t idStart = line.find('{', count + 1);
    
    if ( idStart == std::string::npos ) { // No id
        warn("No ID For Heading!");
        idStart = 0;
    } else { // id found
        size_t idEnd = line.find('}', idStart + 1);
        std::string id = line.substr(idStart + 2, idEnd - ( idStart + 2) );
        idStyle["id"] = id;
    }

    std::string remainingLine = line.substr(count + 1, idStart - ( count + 1 ) );

    renderText(remainingLine);

    htmlElement* heading = new htmlElement(parent->get_tab_index() + 1, "h" + std::to_string(count), remainingLine, idStyle);

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

void mdRender::renderTaskList(htmlElement* parent, std::vector<std::string>& lines) {
    if ( lines.empty() ) {
        throw renderError("Empty list.");
        return;
    }

    if ( lines[0].compare(0, 3, "- [") != 0 ) {
        throw renderError("Bad Task List Format.");
        return;
    }

    htmlElement* list = new htmlElement(parent->get_tab_index() + 1, "ul", styles_["taskList"]);

    for ( std::string line : lines ) {
        if ( line.compare(0, 3, "- [") != 0 ){
            throw renderError("Bad line in list.");
            break; // Just leave this loop, will add whatever is in the current list to parent
        }
        std::string remainingLine = line.substr(5);
        renderText(remainingLine);
        htmlElement* item = new htmlElement(list->get_tab_index() + 1, "li", styles_["item"]);
        htmlElement* checkbox = new htmlElement(item->get_tab_index() + 1, "input",  remainingLine, ( line[3] == 'x' ) ? styles_["checked"] : styles_["task"] );
        item->add_child(checkbox);
        list->add_child(item);
    }

    parent->add_child(list);
}


void mdRender::renderFencedCode(htmlElement* parent, std::vector<std::string>& lines) {
    std::string codeText;
    for ( int i = 0; i < lines.size(); i++ ) {
        if ( i + 1 < lines.size() ) {
            codeText += lines[i] + "<br>";
        } else {
            codeText += lines[i];
        }
    }
    htmlElement* paragraph = new htmlElement(parent->get_tab_index() + 1, "p", codeText, styles_["code"]);
    parent->add_child(paragraph);
}

void mdRender::renderDefinitionList(htmlElement* parent, std::vector<std::string>& lines) { // TODO: Write me :3
    if ( lines.empty() ) {
        throw renderError("Empty List.");
        return;
    }

    htmlElement* list = new htmlElement(parent->get_tab_index() + 1, "dl", styles_["list"]);

    renderText(lines[0]);

    htmlElement* dt = new htmlElement(list->get_tab_index() + 1, "dt", lines[0], styles_["item"]);

    list->add_child(dt);

    for ( int i = 1; i < lines.size(); i++ ) {
        std::string remainingLine = lines[i].substr(2);

        htmlElement* dd = new htmlElement(list->get_tab_index() + 1, "dd", remainingLine, styles_["dd"]);

        list->add_child(dd);
    }

    parent->add_child(list);
}

void mdRender::renderOrderedList(htmlElement* parent, std::vector<std::string> lines) {
    if (lines.empty()) {
        throw renderError("Empty ordered list.");
        return;
    }

    const std::string& first = lines[0];
    size_t pos = 0;
    while (pos < first.size() && std::isdigit(first[pos])) pos++;
    if (pos == 0 || pos >= first.size() || first[pos] != '.' || pos + 1 >= first.size() || first[pos + 1] != ' ') {
        throw renderError("Bad Ordered List Format.");
        return;
    }

    htmlElement* list = new htmlElement(parent->get_tab_index() + 1, "ol", styles_["list"]);

    for (const std::string& line : lines) {
        // Validate each line
        size_t pos = 0;
        while (pos < line.size() && std::isdigit(line[pos])) pos++;
        if (pos == 0 || pos >= line.size() || line[pos] != '.' || pos + 1 >= line.size() || line[pos + 1] != ' ') {
            throw renderError("Bad line in ordered list.");
            break; 
        }

        // Strip "1. " (or "23. ") off the front
        std::string remainingLine = line.substr(pos + 2);

        renderText(remainingLine); 
        htmlElement* item = new htmlElement(list->get_tab_index() + 1, "li", remainingLine, styles_["list"]);
        list->add_child(item);
    }

    parent->add_child(list);
}

void mdRender::rendertable(htmlElement* parent, std::vector<std::string> lines) {
    if ( lines.empty() ) {
        throw renderError("Empty Table.");
        return;
    }

    if ( lines[0][0] != '|' ) {
        throw renderError("Bad Table Format.");
        return;
    }

    htmlElement* table = new htmlElement(parent->get_tab_index() + 1, "table", styles_["table"]);

    bool th = true;

    for ( std::string line : lines ) {
        if ( line[0] != '|' ) {
            throw renderError("Bad line in Table.");
            return;
        }
        htmlElement* tr = new htmlElement(table->get_tab_index() + 1, "tr", styles_["default"]);
        size_t start = 0;
        size_t end = line.find('|', 1);
        while ( end != std::string::npos ) {
            std::string inside = line.substr(start + 2, end - start - 2);
            trim(inside);
            renderText(inside);
            htmlElement* cell = new htmlElement(tr->get_tab_index() + 1, ( th ) ? "th" : "td", inside, styles_["cell"]);
            tr->add_child(cell);
            start = end;
            end = line.find('|', start + 1);
        }
        table->add_child(tr);
        th = false; // We are no longer on the first line
    }

    parent->add_child(table);
}

// Output method
void mdRender::output() {
    outStream_ << "<!DOCTYPE html>\n";
    root_->htmlRender(outStream_);
    delete root_;
}

void mdRender::output(std::ostream& outStream) {
    outStream << "<!DOCTYPE html>\n";
    root_->htmlRender(outStream);
    delete root_;
}