//
// Created by Ainon on 25.11.2023.
//

#include <json_common.h>

#include <iostream>
#include <sstream>

int main() {
    std::cout << "Version is: " << JSON::json_common_version() << std::endl;
    std::stringstream input;
    input << R"({"others": [ ], "obj": { "a": "a" }, "name": "asdf", "statuses": [ "fill", 4, 0.5, false ],  "flags": null })";
    auto root = JSON::JsonParser::Read(input);
    std::stringstream out;
    JSON::JsonParser::Write(root, out, true);
    std::cout << out.str();
    return 0;
}

