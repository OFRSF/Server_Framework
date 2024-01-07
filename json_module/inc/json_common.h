//
// Created by Ainon on 25.11.2023.
//

#ifndef SERVER_FRAMEWORK_JSON_COMMON_H
#define SERVER_FRAMEWORK_JSON_COMMON_H

#include <string>
#include <iostream>
#include <sstream>
#include <variant>
#include <vector>
#include <map>
#include <unordered_map>
#include <any>
#include <memory>

/* Features for implementing:
 * supprot escape symbols for String type
 * support unicode for String type */


#define JSON_COMMON_VER "0.1.2"


namespace JSON
{
    /**
     * \brief  Version of lib
     * \return printable version
     */
    std::string json_common_version();

    /**
     * \brief Content one token
     */
    class JsonNode {
    public:
        /* nullptr, bool, int, long, double */
        using String = std::string;
        using Array = std::vector<JsonNode>;
        using Object = std::map<String, JsonNode>;
        using NodeValue = std::variant<
            std::nullptr_t, bool, int, long, double,
            String,
            Array,
            Object
        >;

        /** Constructors */
        JsonNode() = default;
        explicit JsonNode(std::nullptr_t);
        explicit JsonNode(bool);
        explicit JsonNode(int);
        explicit JsonNode(long);
        explicit JsonNode(double);
        explicit JsonNode(const String&);
        explicit JsonNode(String&&);
        explicit JsonNode(const Array&);
        explicit JsonNode(Array&&);
        explicit JsonNode(const Object&);
        explicit JsonNode(Object&&);

        /** Functions for check content type */
        [[nodiscard]] bool IsNull()       const noexcept;
        [[nodiscard]] bool IsBool()       const noexcept;
        [[nodiscard]] bool IsInt()        const noexcept;
        [[nodiscard]] bool IsLong()       const noexcept;
        [[nodiscard]] bool IsPureLong()   const noexcept;
        [[nodiscard]] bool IsDouble()     const noexcept;
        [[nodiscard]] bool IsPureDouble() const noexcept;
        [[nodiscard]] bool IsString()     const noexcept;
        [[nodiscard]] bool IsArray()      const noexcept;
        [[nodiscard]] bool IsObject()     const noexcept;

        /** Functions for get value
         *  If content type mismatch - throw exception */
        [[nodiscard]] bool          AsBool()   const;
        [[nodiscard]] int           AsInt()    const;
        [[nodiscard]] long          AsLong()   const;
        [[nodiscard]] double        AsDouble() const;
        [[nodiscard]] const String& AsString() const;
        [[nodiscard]] const Array&  AsArray()  const;
        [[nodiscard]] const Object& AsObject() const;

        /** Functions for get value
         *  If content type mismath - set err to true
         *  and return default value for particular type */
        [[nodiscard]] bool          AsBool(bool& err)   const noexcept;
        [[nodiscard]] int           AsInt(bool& err)    const noexcept;
        [[nodiscard]] long          AsLong(bool& err)   const noexcept;
        [[nodiscard]] double        AsDouble(bool& err) const noexcept;
        [[nodiscard]] const String& AsString(bool& err) const noexcept;
        [[nodiscard]] const Array&  AsArray(bool &err)  const noexcept;
        [[nodiscard]] const Object& AsObject(bool &err) const noexcept;

        /** Functions for implicit cast
         *  If content type mismatch - throw exception */
        operator int() const;
        operator long() const;
        operator double() const;
        operator const String&() const;
        operator const Array&() const;
        operator const Object&() const;

        /** Fucntions for access conteiner elements
         *  If content type Array - operator[] with size_t
         *  can be called
         *  If content type Object - operator[] with String
         *  can be called
         *  Otherwise - throw exception */
        const JsonNode& operator[](size_t idx) const;
        const JsonNode& operator[](const String& name) const;

        /**
         * \brief  Function for print json
         * \return value as std::variant<>
         */
        const NodeValue& GetValue() const;
    private:
        NodeValue _value;

        /* Must be equal of declaration NodeValue */
        enum TYPE {
            EMPTY, BOOL, INT, LONG, DOUBLE,
            STRING, ARRAY, OBJECT
        };
        mutable bool _err = false;
        [[nodiscard]] TYPE GetTypeIndx() const;

        static const String _empty_string;
        static const Array _empty_array;
        static const Object _empty_object;

    };

    class JsonParser {
    public:
        /**
         * \brief    Read json from stream
         * \param in Input stream
         * \return   Root node of json
         */
        static JsonNode Read(std::istream& in);

        /**
         * \brief        Whrite json to stream
         * \param root   started from this node
         * \param out    output stream
         * \param pretty if true - whrite in pretty format
         *               if false - whrite in one line
         */
        static void Write(const JsonNode& root, std::ostream& out, bool pretty);
    private:
        static void SkipWhiteSpaces(std::istream& in);
        static bool SkipSymbol(std::istream& in, const char ch);
        static bool SkipSymbolUntil(std::istream& in, const char ch, const char until);
        static char StreamPeek(std::istream& in);
        static char StreamGet(std::istream& in);
        static JsonNode ReadObject(std::istream& in);
        static JsonNode ReadArray(std::istream& in);
        static JsonNode ReadString(std::istream& in);
        static JsonNode ReadNumber(std::istream& in);
        static JsonNode ReadOther(std::istream& in);
    };

    struct OneLineWriter {
        std::ostream& out;
        void operator()(std::nullptr_t) const;
        void operator()(bool arg) const;
        void operator()(int arg) const;
        void operator()(long arg) const;
        void operator()(double arg) const;
        void operator()(const JsonNode::String& arg) const;
        void operator()(const JsonNode::Array& arg) const;
        void operator()(const JsonNode::Object& arg) const;
    };

    struct PrettyWriter {
        std::ostream& out;
        mutable int ident;
        mutable bool need_ident;
        void operator()(std::nullptr_t) const;
        void operator()(bool arg) const;
        void operator()(int arg) const;
        void operator()(long arg) const;
        void operator()(double arg) const;
        void operator()(const JsonNode::String& arg) const;
        void operator()(const JsonNode::Array& arg) const;
        void operator()(const JsonNode::Object& arg) const;
        void PrintIdent() const;
    };

    class JsonParserException final : public std::runtime_error {
    public:
        JsonParserException(const JsonNode& last_node, const std::string& what);
        const char* what() const noexcept override  ;
    private:
        std::string _what;
    };







}  /* namespace JSON */

#endif //SERVER_FRAMEWORK_JSON_COMMON_H