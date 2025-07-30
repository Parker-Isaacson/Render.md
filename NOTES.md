Taken from old render.cpp, consider adding back into it?

```c++
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
```