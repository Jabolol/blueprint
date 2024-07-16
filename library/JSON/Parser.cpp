#include <cassert>
#include <format>
#include <iostream>
#include <memory>
#include <optional>

#include "JSON/Parser.hpp"
#include "JSON/Token.hpp"
#include "JSON/primitives/Array.hpp"
#include "JSON/primitives/Boolean.hpp"
#include "JSON/primitives/Null.hpp"
#include "JSON/primitives/Number.hpp"
#include "JSON/primitives/Object.hpp"
#include "JSON/primitives/String.hpp"
#include "interfaces/IPrimitive.hpp"

Blueprint::JSON::Parser::Parser()
{
    _callbacks = {
        {
            Type::ARRAY_START,
            std::bind(&Parser::parseArray, this, std::placeholders::_1),
        },
        {
            Type::OBJECT_START,
            std::bind(&Parser::parseObject, this, std::placeholders::_1),
        },
        {
            Type::STRING,
            std::bind(&Parser::parseString, this, std::placeholders::_1),
        },
        {
            Type::NUMBER,
            std::bind(&Parser::parseNumber, this, std::placeholders::_1),
        },
        {
            Type::BOOLEAN,
            std::bind(&Parser::parseBoolean, this, std::placeholders::_1),
        },
        {
            Type::NULL_VALUE,
            std::bind(&Parser::parseNull, this, std::placeholders::_1),
        },
    };
}

std::optional<std::shared_ptr<Blueprint::Interfaces::IPrimitive>>
Blueprint::JSON::Parser::parse(const std::string &json)
{
    _lexer = Lexer(json);

    std::optional<Token> token = _lexer.nextToken();
    if (!token.has_value()) {
        _error = "Unexpected end of file";
        return std::nullopt;
    }

    auto it = _callbacks.find(token->type());
    if (it == _callbacks.end()) {
        _error = std::format("Unexpected token '{}'", token->data());
        return std::nullopt;
    }

    std::shared_ptr<Blueprint::Interfaces::IPrimitive> ptr =
        it->second(token.value());
    if (ptr == nullptr) {
        std::cerr << _error << std::endl;
        return std::nullopt;
    }

    return ptr;
}

std::shared_ptr<Blueprint::Interfaces::IPrimitive>
Blueprint::JSON::Parser::parseObject(Token &token)
{
    if (token.type() != Type::OBJECT_START) {
        _error = std::format("Expected object start, got '{}'", token.data());
        return nullptr;
    }

    std::optional<Token> current;
    std::shared_ptr<Primitives::Object> object =
        std::make_shared<Primitives::Object>();

    while (current->type() != Type::OBJECT_END) {
        current = _lexer.nextToken();

        if (!current.has_value()) {
            _error = "Unexpected end of file";
            return nullptr;
        }

        if (current->type() == Type::OBJECT_END) {
            break;
        }

        if (current->type() != Type::STRING) {
            _error = std::format("Expected string, got '{}'", current->data());
            return nullptr;
        }

        std::string key = current->data();
        current = _lexer.nextToken();

        if (!current.has_value()) {
            _error = "Unexpected end of file after key";
            return nullptr;
        }

        if (current->type() != Type::COLON) {
            _error = std::format("Expected colon, got '{}'", current->data());
            return nullptr;
        }
        current = _lexer.nextToken();

        if (!current.has_value()) {
            _error = "Unexpected end of file";
            return nullptr;
        }

        auto it = _callbacks.find(current->type());
        if (it == _callbacks.end()) {
            _error = std::format("Unexpected token '{}'", current->data());
            return nullptr;
        }

        std::shared_ptr<Blueprint::Interfaces::IPrimitive> value =
            it->second(current.value());
        if (value == nullptr) {
            _error =
                std::format("Failed to parse value '{}'", current->data());
            return nullptr;
        }

        object->add(key, value);
        current = _lexer.nextToken();
    }

    return object;
}

std::shared_ptr<Blueprint::Interfaces::IPrimitive>
Blueprint::JSON::Parser::parseArray(Token &token)
{
    if (token.type() != Type::ARRAY_START) {
        _error = std::format("Expected array start, got '{}'", token.data());
        return nullptr;
    }

    std::optional<Token> current;
    std::shared_ptr<Primitives::Array> array =
        std::make_shared<Primitives::Array>();

    while (current->type() != Type::ARRAY_END) {
        current = _lexer.nextToken();

        if (!current.has_value()) {
            _error = "Unexpected end of file after array start";
            return nullptr;
        }

        if (current->type() == Type::ARRAY_END) {
            break;
        }

        auto it = _callbacks.find(current->type());
        if (it == _callbacks.end()) {
            _error = std::format("Unexpected token '{}'", current->data());
            return nullptr;
        }

        std::shared_ptr<Blueprint::Interfaces::IPrimitive> value =
            it->second(current.value());

        if (value == nullptr) {
            _error =
                std::format("Failed to parse value '{}'", current->data());
            return nullptr;
        }

        array->add(value);
        current = _lexer.nextToken();
    }

    return array;
}

std::shared_ptr<Blueprint::Interfaces::IPrimitive>
Blueprint::JSON::Parser::parseString(Token &token)
{
    if (token.type() != Type::STRING) {
        _error = std::format("Expected string, got '{}'", token.data());
        return nullptr;
    }

    return std::make_shared<Primitives::String>(token.data());
}

std::shared_ptr<Blueprint::Interfaces::IPrimitive>
Blueprint::JSON::Parser::parseNumber(Token &token)
{
    if (token.type() != Type::NUMBER) {
        _error = std::format("Expected number, got '{}'", token.data());
        return nullptr;
    }

    return std::make_shared<Primitives::Number>(token.data());
}

std::shared_ptr<Blueprint::Interfaces::IPrimitive>
Blueprint::JSON::Parser::parseBoolean(Token &token)
{
    if (token.type() != Type::BOOLEAN) {
        _error = std::format("Expected boolean, got '{}'", token.data());
        return nullptr;
    }

    return std::make_shared<Primitives::Boolean>(token.data());
}

std::shared_ptr<Blueprint::Interfaces::IPrimitive>
Blueprint::JSON::Parser::parseNull(Token &token)
{
    if (token.type() != Type::NULL_VALUE) {
        _error = std::format("Expected null, got '{}'", token.data());
        return nullptr;
    }

    return std::make_shared<Primitives::Null>();
}

const std::string &Blueprint::JSON::Parser::getError() const
{
    return _error;
}
