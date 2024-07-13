#include "JSON/Token.hpp"

Blueprint::JSON::Token::Token(Type type, const std::string data)
    : _type(type), _data(data)
{
}

Blueprint::JSON::Type Blueprint::JSON::Token::type() const
{
    return _type;
}

const std::string &Blueprint::JSON::Token::data() const
{
    return _data;
}
