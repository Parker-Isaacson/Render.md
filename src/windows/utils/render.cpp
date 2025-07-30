#include "render.h"

// Constructors
mdRender::mdRender(std::istream& inStream, const htmlElement& root) 
    : inStream_(inStream), root_(root) {}

mdRender::mdRender(std::istream& inStream, std::ostream& outStream, const htmlElement& root)
    : inStream_(inStream), outStream_(outStream), root_(root) {}

// Rendering Methods
void mdRender::render() {
    htmlElement parent = root_;
    render(parent);
}

void mdRender::render(htmlElement& parent) {
    // First: Make attributes
    std::string line;
    std::string key;
    std::string value;
    while ( std::getline(inStream_, line) ) {
        if ( line[0] != '?' )
            break;
        size_t doubleEqual = line.find("==");
        if ( doubleEqual == std::string::npos )
            throw renderError("False Adjustment. Check  the line \"" + line + "\".");
        key = line.substr(1, doubleEqual - 1);
        value = line.substr(doubleEqual + 2);
        if ( key == "tab-width" ) // Cant use switch because string is not a valid input
            tabWidth_ = std::stoi(value);
        else if ( key == "background-color" )
            backgroundColor_ = value;
        else if ( key == "text-color" )
            textColor_ = value;
        else if ( key == "margin" )
            margin_ = std::stoi(value);
        else if ( key == "padding" )
            padding_ = std::stoi(value);
        else
            throw renderError("Bad Adjustment. Check  the line \"" + line + "\"."); 
    }
    // Second: Make styles_, will be litterals because loops are hard :<
    std::map<std::string, std::string> defualt = {
        {"background-color", backgroundColor_},
        {"color", textColor_},
        {"padding", std::to_string(padding_)},
        {"margin", std::to_string(margin_)},
        {"width", "fit-content"}
    };
    styles_["defualt"] = defualt;
    styles_["heading"] = defualt;
    styles_["paragraph"] = defualt;
    styles_["list"] = defualt;
    styles_["list-item"] = defualt;
    styles_["blockquote"] = {
        {"background-color", darkenColor(backgroundColor_, 0.15)},
        {"color", textColor_},
        {"padding", std::to_string(padding_)},
        {"margin", std::to_string(margin_)},
        {"width", "fit-content"},
        {"border-left", "4px solid " + darkenColor(backgroundColor_, 0.3)},
        {"border-radius", "4px"},
        {"padding-left", "2px"}
    };
    styles_["code"] = {
        {"background-color", backgroundColor_},
        {"color", textColor_},
        {"padding", std::to_string(std::max(2, padding_)) + "px"},
        {"margin", std::to_string(margin_)},
        {"width", "fit-content"}
    };
    // Third: Render :3
}

void mdRender::renderText(htmlElement& parent, std::string& line) {

}

void mdRender::renderHeading(htmlElement& parent, std::string& line) {
    int count = 0;
    while ( line[count] == '#' && count < 6 ) // Finds the amount of '#' elements - 1, up to 6
        count++;
    if ( count == 0 ) // This should never happen
        throw renderError("False Heading. Check the line starting with \"" + line.substr(0, 10) + "\".");
    auto heading = std::make_unique<htmlElement>(parent.get_tab_index() + 1, "h" + std::to_string(++count), "", styles_["heading"]);
    std::string remaningLine = line.substr(count);
    renderText(*heading, remaningLine);
    parent.add_child(std::move(heading));
    return;
}

void mdRender::renderBlockQuote(htmlElement& parent, std::string& line) {
    if ( !(line[0] == '>' && line[1] == ' ') ) // This should never happen
        throw renderError("False Heading. Check the line starting with \"" + line.substr(0, 10) + "\".");
    auto blockquote = std::make_unique<htmlElement>(parent.get_tab_index() + 1, "div", styles_["blockquote"]);
    std::string remainingLine = line.substr(2);
    renderText(*blockquote, remainingLine);
    parent.add_child(std::move(blockquote));
    return;
}

void mdRender::renderList(htmlElement& parent, std::vector<std::string>& lines) {
    if ( lines[0][0] == '1' )  { // Must start at 1 for list, even if nested
        auto list = std::make_unique<htmlElement>(parent.get_tab_index() + 1, "ol", styles_["list"]);
    }
    else if ( lines[0][0] == '-' ) {
        auto list = std::make_unique<htmlElement>(parent.get_tab_index() + 1, "ul", styles_["list"]);
        for ( auto line : lines ) {
            if ( line[0] == '-' ) { // Is <li>
                auto li = std::make_unique<htmlElement>(list->get_tab_index() + 1, "li", styles_["list-item"]);
                std::string remainingLine = line.substr(2);
                renderText(*li, remainingLine);
                list->add_child(std::move(li));
            }
            else { // Is nested list

            }
        } 
        parent.add_child(std::move(list))
    } 
}
