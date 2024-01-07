//
// Created by Ainon on 25.11.2023.
//

#include <json_common.h>
#include <sstream>


/* Json tokens scheme:
 * | or
 * {N} count, {*}
 * ? - optional
 * whitespace: \s
 * number: [main][fraction]?[exponent]?
 *    main: -?(0|[1-9][\d]*)
 *    fraction: .[0-9]+
 *    exponent: [eE][+-]?[0-9]*
 * string: "([any]|[escape_symbols]|[unicode]){*}"
 *    any: any symbol except '\' and '"'
 *    escape_symbols: \" \\ \/ \b \f \n \r \t
 *    unicode: u[0-9]{4}
 * value: [whitespace][string|number|object|array|true|false|null][whitespace]
 * array: '['[whitespace|value,value..]']'
 * object: '{'[whitespace|(whitespace string whitespace : value, ...]'}'
 */

namespace JSON {
    std::string json_common_version()
    {
        return JSON_COMMON_VER;
    }

    /* JsonNode */
    JsonNode::JsonNode(std::nullptr_t)   : _value(nullptr) {}
    JsonNode::JsonNode(const bool val)   : _value(val) {}
    JsonNode::JsonNode(const int val)    : _value(val) {}
    JsonNode::JsonNode(const long val)   : _value(val) {}
    JsonNode::JsonNode(const double val) : _value(val) {}

    JsonNode::JsonNode(const String& val) : _value(val) {}
    JsonNode::JsonNode(String&& val)      : _value(std::move(val)) {}
    JsonNode::JsonNode(const Array& val)  : _value(val) {}
    JsonNode::JsonNode(Array&& val)       : _value(std::move(val)) {}
    JsonNode::JsonNode(const Object& val) : _value(val) {}
    JsonNode::JsonNode(Object&& val)      : _value(std::move(val)) {}

    bool JsonNode::IsNull()       const noexcept { return GetTypeIndx() == TYPE::EMPTY;  }
    bool JsonNode::IsBool()       const noexcept { return GetTypeIndx() == TYPE::BOOL;   }
    bool JsonNode::IsInt()        const noexcept { return GetTypeIndx() == TYPE::INT;    }
    bool JsonNode::IsLong()       const noexcept { return IsPureLong() || IsInt();       }
    bool JsonNode::IsPureLong()   const noexcept { return GetTypeIndx() == TYPE::LONG;   }
    bool JsonNode::IsDouble()     const noexcept { return IsPureDouble() || IsLong();    }
    bool JsonNode::IsPureDouble() const noexcept { return GetTypeIndx() == TYPE::DOUBLE; }
    bool JsonNode::IsString()     const noexcept { return GetTypeIndx() == TYPE::STRING; }
    bool JsonNode::IsArray()      const noexcept { return GetTypeIndx() == TYPE::ARRAY;  }
    bool JsonNode::IsObject()     const noexcept { return GetTypeIndx() == TYPE::OBJECT; }

    bool JsonNode::AsBool() const {
        const bool result = AsBool(this->_err);
        if (!_err) return result;
        throw std::runtime_error("Node is not hold <bool>");
    }

    int JsonNode::AsInt() const {
        const int result = AsInt(_err);
        if (!_err) return result;
        throw std::runtime_error("Node is not hold <int>");
    }

    long JsonNode::AsLong() const {
        const long result = AsLong(_err);
        if (!_err) return result;
        throw std::runtime_error("Node is not hold <long> or <int>");
    }

    double JsonNode::AsDouble() const {
        const double result = AsDouble(_err);
        if (!_err) return result;
        throw std::runtime_error("Node is not hold <double> or <long> of <int>");
    }

    const JsonNode::String& JsonNode::AsString() const {
        const String& result = AsString(_err);
        if (!_err) return result;
        throw std::runtime_error("Node is not hold <string>");
    }

    const JsonNode::Array& JsonNode::AsArray() const {
        const Array& result = AsArray(_err);
        if (!_err) return result;
        throw std::runtime_error("Node is not hold <Array>");
    }

    const JsonNode::Object& JsonNode::AsObject() const {
        const Object& result = AsObject(_err);
        if (!_err) return result;
        throw std::runtime_error("Node is not hold <Object>");
    }

    bool JsonNode::AsBool(bool& err) const noexcept {
        err = false;
        switch (GetTypeIndx()) {
            case TYPE::BOOL: return std::get<bool>(_value);
            default: err = true;
        }
        return false;
    }

    int JsonNode::AsInt(bool& err) const noexcept {
        err = false;
        switch (GetTypeIndx()) {
            case TYPE::INT: return std::get<int>(_value);
            default: err = true;
        }
        return 0;
    }

    long JsonNode::AsLong(bool& err) const noexcept {
        err = false;
        switch (GetTypeIndx()) {
            case TYPE::INT : return std::get<int>(_value);
            case TYPE::LONG : return std::get<long>(_value);
            default: err = true;
        }
        return 0;
    }

    double JsonNode::AsDouble(bool &err) const noexcept {
        err = false;
        switch (GetTypeIndx()) {
            case TYPE::INT: return std::get<TYPE::INT>(_value);
            case TYPE::LONG: return static_cast<double>(std::get<long>(_value));
            case TYPE::DOUBLE: return std::get<double>(_value);
            default: err = true;
        }
        return 0;
    }

    const JsonNode::String& JsonNode::AsString(bool& err) const noexcept {
        err = false;
        switch (GetTypeIndx()) {
            case TYPE::STRING: return std::get<TYPE::STRING>(_value);
            default: err = true;
        }
        return _empty_string;
    }

    const JsonNode::Array &JsonNode::AsArray(bool &err) const noexcept {
        err = false;
        switch (GetTypeIndx()) {
            case TYPE::ARRAY: return std::get<TYPE::ARRAY>(_value);
            default: err = true;
        }
        return _empty_array;
    }

    const JsonNode::Object& JsonNode::AsObject(bool &err) const noexcept {
        err = false;
        switch (GetTypeIndx()) {
            case TYPE::OBJECT: return std::get<TYPE::OBJECT>(_value);
            default: err = true;
        }
        return _empty_object;
    }

    JsonNode::operator int() const {
        return AsInt();
    }

    JsonNode::operator long() const {
        return AsLong();
    }

    JsonNode::operator double() const {
        return AsDouble();
    }

    JsonNode::operator const JsonNode::String&() const {
        return AsString();
    }

    JsonNode::operator const JsonNode::Array&() const {
        return AsArray();
    }

    JsonNode::operator const JsonNode::Object&() const {
        return AsObject();
    }

    const JsonNode& JsonNode::operator[](const size_t idx) const {
        switch (GetTypeIndx()) {
            case TYPE::ARRAY:
                return AsArray()[idx];
            default:
                throw std::runtime_error("JsonNode is not Array type");
        }
    }

    const JsonNode &JsonNode::operator[](const String &name) const {
        switch (GetTypeIndx()) {
            case TYPE::OBJECT:
                return AsObject().at(name);
            default:
                throw std::runtime_error("JsonNode is not Object type");
        }
    }

    const JsonNode::NodeValue &JsonNode::GetValue() const {
        return _value;
    }


    JsonNode::TYPE JsonNode::GetTypeIndx() const {
        return static_cast<TYPE>(_value.index());
    }

    const JsonNode::String JsonNode::_empty_string;
    const JsonNode::Array JsonNode::_empty_array;
    const JsonNode::Object JsonNode::_empty_object;

    // JsonParser

    JsonNode JsonParser::Read(std::istream &in) {
        SkipWhiteSpaces(in);
        const char c = StreamPeek(in);
        if (c == '{') {
            return ReadObject(in);
        } else if (c == '[') {
            return ReadArray(in);
        } else if (c == '\"') {
            return ReadString(in);
        } else if (std::isdigit(c) || c == '-') {
            return ReadNumber(in);
        } else {
            return ReadOther(in);
        }
    }

    void JsonParser::Write(const JsonNode &root, std::ostream &out, bool pretty) {
        if (pretty) {
            std::visit(PrettyWriter{out, 0, true}, root.GetValue());
        } else {
            std::visit(OneLineWriter{out}, root.GetValue());
        }
    }

    void JsonParser::SkipWhiteSpaces(std::istream &in) {
        while (std::isspace(StreamPeek(in))) {
            StreamGet(in);
        }
    }

    bool JsonParser::SkipSymbol(std::istream &in, const char ch) {
        bool skiped = false;
        SkipWhiteSpaces(in);
        if (StreamPeek(in) == ch) {
            StreamGet(in);
            skiped = true;
        }
        SkipWhiteSpaces(in);
        return skiped;
    }

    bool JsonParser::SkipSymbolUntil(std::istream &in, const char ch, const char until) {
        bool skiped = SkipSymbol(in, ch);
        if (!skiped && StreamPeek(in) == until) skiped = true;
        return skiped;
    }


    char JsonParser::StreamPeek(std::istream &in) {
        const char c = static_cast<char>(in.peek());
        if (in.fail()) {
            in.clear();
            throw std::runtime_error("istream bad after peek()");
        }
        if (in.eof()) in.clear();
        return c;
    }

    char JsonParser::StreamGet(std::istream& in) {
        const char c = static_cast<char>(in.get());
        if (in.fail()) {
            in.clear();
            throw std::runtime_error("istream bad after get()");
        }
        return c;
    }

    JsonNode JsonParser::ReadObject(std::istream &in) {
        if (StreamGet(in) != '{') throw std::runtime_error("Expected { ar start of object");
        JsonNode::Object object;
        do {
            SkipWhiteSpaces(in);
            if (StreamPeek(in) == '}') break;
            JsonNode key = ReadString(in);
            if (!SkipSymbol(in, ':')) {
                throw JsonParserException(key, "expected :");
            }
            JsonNode value = Read(in);
            object.try_emplace(key.AsString(), std::move(value));
            if (!SkipSymbolUntil(in, ',', '}')) {
                throw JsonParserException(object.at(key.AsString()), "expected ,");
            }
        } while (StreamPeek(in) != '}');
        SkipSymbol(in, '}');
        return JsonNode(std::move(object));
    }

    JsonNode JsonParser::ReadArray(std::istream &in) {
        if (StreamGet(in) != '[') throw std::runtime_error("Expected [ at start of array");
        JsonNode::Array array;
        do {
            SkipWhiteSpaces(in);
            if (StreamPeek(in) == ']') break;
            array.push_back(std::move(Read(in)));
            if (!SkipSymbolUntil(in, ',', ']')) {
                throw JsonParserException(array.back(), "expected ','");
            }
        } while (StreamPeek(in) != ']');
        SkipSymbol(in, ']');
        return JsonNode(std::move(array));
    }

    JsonNode JsonParser::ReadString(std::istream &in) {
        std::stringstream str;
        if (StreamGet(in) != '\"') throw std::runtime_error("Expected \" at start of string");
        while (StreamPeek(in) != '\"') {
            str << StreamGet(in);
        }
        if (StreamGet(in) != '\"') throw std::runtime_error("Expected \" at end of string");
        return JsonNode(str.str());
    }

    JsonNode JsonParser::ReadNumber(std::istream &in) {
        std::stringstream number_str;

        /* Get sign if exists */
        if (StreamPeek(in) == '-') {
            number_str << StreamGet(in);
        }

        /* Must be or 0 or [1-9][0-9]* */
        char c = StreamPeek(in);
        if (c == '0') {
            number_str << StreamGet(in);\
        } else if (std::isdigit(c)) {
            while (std::isdigit(StreamPeek(in))) {
                number_str << StreamGet(in);
            }
        } else {
            throw std::runtime_error("Read from stream "
                + std::string(1, StreamPeek(in))
                + "when expected digit");
        }

        /* Get fraction if exists */
        bool is_double = false;
        if (StreamPeek(in) == '.') {
            is_double = true;
            number_str << StreamGet(in);
            while (std::isdigit(StreamPeek(in))) {
                number_str << StreamGet(in);
            }
        }

        /* Get exponent if exists */
        c = StreamPeek(in);
        if (c == 'E' || c == 'e') {
            is_double = true;
            number_str << StreamGet(in);
            c = StreamPeek(in);
            if (c == '+' || c == '-') {
                number_str << StreamGet(in);
            }
            while (std::isdigit(StreamPeek(in))) {
                number_str << StreamGet(in);
            }
        }

        try {
            if (!is_double) {
                /* Try parse as int */
                try {
                    const int d = std::stoi(number_str.str());
                    return JsonNode(d);
                } catch (...) { /* when get exception - try parse like long */ }
                try {
                    const long d = std::stol(number_str.str());
                    return JsonNode(d);
                } catch (...) { /* when get exception - try parse as double */ }
            }
            const double d = std::stod(number_str.str());
            return JsonNode(d);

        } catch (...) {
            throw std::runtime_error("Can't convert "
                + number_str.str() + "to c++ type");
        }
    }

    JsonNode JsonParser::ReadOther(std::istream &in) {
        std::string s;
        in >> s;
        if (s == "true") return JsonNode(true);
        if (s == "false") return JsonNode(false);
        if (s == "null") return JsonNode(nullptr);
        throw std::runtime_error("Expected true|false|null");
    }

    // OneLineWriter
    void OneLineWriter::operator()(std::nullptr_t) const {
        out << "null";
    }

    void OneLineWriter::operator()(const bool arg) const {
        out << (arg ? "true" : "false");
    }

    void OneLineWriter::operator()(const int arg) const {
        out << arg;
    }

    void OneLineWriter::operator()(const long arg) const {
        out << arg;
    }

    void OneLineWriter::operator()(const double arg) const {
        out << arg;
    }

    void OneLineWriter::operator()(const JsonNode::String &arg) const {
        out << '"' << arg << '"';
    }

    void OneLineWriter::operator()(const JsonNode::Array &arg) const {
        out << "[ ";
        bool first = true;
        for (const auto& node : arg) {
            if (!first) {
                out << ", ";
            }
            std::visit(OneLineWriter{out}, node.GetValue());
            first = false;
        }
        out << " ]";
    }

    void OneLineWriter::operator()(const JsonNode::Object &arg) const {
        out << "{ ";
        bool first = true;
        for (const auto& [key, node] : arg) {
            if (!first) {
                out << ", ";
            }
            OneLineWriter{out}.operator()(key);
            out << ": ";
            std::visit(OneLineWriter{out}, node.GetValue());
            first = false;
        }
        out << " }";
    }

    // PrettyWriter
    void PrettyWriter::operator()(std::nullptr_t) const {
        PrintIdent();
        out << "null";
    }

    void PrettyWriter::operator()(const bool arg) const {
        PrintIdent();
        out << (arg ? "true" : "false");
    }

    void PrettyWriter::operator()(const int arg) const {
        PrintIdent();
        out << arg;
    }

    void PrettyWriter::operator()(const long arg) const {
        PrintIdent();
        out << arg;
    }

    void PrettyWriter::operator()(const double arg) const {
        PrintIdent();
        out << arg;
    }

    void PrettyWriter::operator()(const JsonNode::String &arg) const {
        PrintIdent();
        out << '"' << arg << '"';
    }

    void PrettyWriter::operator()(const JsonNode::Array &arg) const {
        PrintIdent();
        out << "[ ";
        bool first = true;
        for (const auto& node : arg) {
            if (!first) {
                out << ",\n";
            } else {
                out << '\n';
            }
            std::visit(PrettyWriter{out, ident + 4, true}, node.GetValue());
            first = false;
        }
        if (!first) {
            out << '\n';
        }
        need_ident = !arg.empty();
        PrintIdent();
        out << "]";
    }

    void PrettyWriter::operator()(const JsonNode::Object &arg) const {
        PrintIdent();
        out << "{ ";
        bool first = true;
        for (const auto& [key, node] : arg) {
            if (!first) {
                out << ", \n";
            } else {
                out << '\n';
            }
            PrettyWriter{out, ident + 4, true}.operator()(key);
            out << ": ";
            std::visit(PrettyWriter{out, ident + 4, false}, node.GetValue());
            first = false;
        }
        if (!first) {
            out << '\n';
        }
        need_ident = !arg.empty();
        PrintIdent();
        out << "}";
    }

    void PrettyWriter::PrintIdent() const {
        if (need_ident) {
            int count = ident;
            while (count--) {
                out << ' ';
            }
        }
    }


    // JsonParserException


    JsonParserException::JsonParserException(const JsonNode &last_node, const std::string &what)
        : std::runtime_error("Parser exception")
    {
        std::stringstream what_ss;
        what_ss << what << " after node: ";
        std::visit(OneLineWriter{what_ss}, last_node.GetValue());
        _what = what_ss.str();
    }

    const char *JsonParserException::what() const noexcept {
        return _what.c_str();
    }

}






