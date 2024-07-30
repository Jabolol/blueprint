#include <optional>

#include "JSON/Lexer.hpp"
#include "JSON/Token.hpp"

void Blueprint::JSON::Lexer::skipWhitespace()
{
    while (_position < _json.length() && isspace(_json[_position])) {
        ++_position;
    }
}

std::optional<Blueprint::JSON::Token> Blueprint::JSON::Lexer::advanceAndReturn(
    Type type, const std::string &value)
{
    ++_position;
    return Token(type, value);
}

std::optional<Blueprint::JSON::Token> Blueprint::JSON::Lexer::parseString()
{
    std::size_t start = ++_position;

    while (_position < _json.length() && _json[_position] != '"') {
        ++_position;
    }

    if (_position > _json.length()) {
        setError("Expected '\"' at JSON::{}", _position);
        return std::nullopt;
    }

    std::string str = _json.substr(start, _position - start);
    ++_position;

    return Token(Type::STRING, str);
}

std::optional<Blueprint::JSON::Token> Blueprint::JSON::Lexer::parseNumber()
{
    size_t start = _position;

    if (_json[_position] == '-') {
        ++_position;
    }

    while (_position < _json.length() && isdigit(_json[_position])) {
        ++_position;
    }

    if (_position < _json.length() && _json[_position] == '.') {
        ++_position;
        while (_position < _json.length() && isdigit(_json[_position])) {
            ++_position;
        }
    }

    if (_position < _json.length()
        && (_json[_position] == 'e' || _json[_position] == 'E')) {
        ++_position;
        if (_position < _json.length()
            && (_json[_position] == '+' || _json[_position] == '-')) {
            ++_position;
        }
        while (_position < _json.length() && isdigit(_json[_position])) {
            ++_position;
        }
    }

    std::string number = _json.substr(start, _position - start);
    return Token(Type::NUMBER, number);
}

std::optional<Blueprint::JSON::Token> Blueprint::JSON::Lexer::parseNull()
{
    if (_json.substr(_position, 4) == "null") {
        _position += 4;
        return Token(Type::NULL_VALUE, "null");
    }

    setError("Expected 'null' at JSON::{}, got '{}'", _position, getWord());

    return std::nullopt;
}

std::optional<Blueprint::JSON::Token> Blueprint::JSON::Lexer::parseBoolean()
{
    if (_json[_position] == 'f' && _json.substr(_position, 5) == "false") {
        _position += 5;
        return Token(Type::BOOLEAN, "false");
    }

    if (_json[_position] == 't' && _json.substr(_position, 4) == "true") {
        _position += 4;
        return Token(Type::BOOLEAN, "true");
    }

    setError("Expected 'true' or 'false' at JSON::{}, got '{}'", _position,
        getWord());

    return std::nullopt;
}

Blueprint::JSON::Lexer::Lexer(const std::string &json) : _json(json)
{
}

std::optional<Blueprint::JSON::Token> Blueprint::JSON::Lexer::nextToken()
{
    skipWhitespace();

    if (_position >= _json.length()) {
        return Token(Type::END_OF_FILE, "");
    }

    char ch = _json[_position];

    switch (ch) {
        case '{': return advanceAndReturn(Type::OBJECT_START, "{");
        case '}': return advanceAndReturn(Type::OBJECT_END, "}");
        case '[': return advanceAndReturn(Type::ARRAY_START, "[");
        case ']': return advanceAndReturn(Type::ARRAY_END, "]");
        case ':': return advanceAndReturn(Type::COLON, ":");
        case ',': return advanceAndReturn(Type::COMMA, ",");
        case '"': return parseString();
        case 't':
        case 'f': return parseBoolean();
        case 'n': return parseNull();
        default:
            if (isdigit(ch) || ch == '-') {
                return parseNumber();
            }
    }

    setError("Unexpected char '{}' at JSON::{}", ch, _position);

    return std::nullopt;
}

const std::string Blueprint::JSON::Lexer::getWord() const
{
    std::size_t position = _json.find(' ', _position);
    std::string word = position == std::string::npos
        ? _json.substr(_position)
        : _json.substr(_position, position - _position);

    return word;
}

const std::string &Blueprint::JSON::Lexer::getError() const
{
    return _error;
}
