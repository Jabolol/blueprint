#include "JSON/primitives/String.hpp"

Blueprint::JSON::Primitives::String::String(const std::string &value)
    : _value(value)
{
}

const std::string &Blueprint::JSON::Primitives::String::value() const
{
    return _value;
}

void Blueprint::JSON::Primitives::String::value(const std::string &value)
{
    _value = value;
}

std::string Blueprint::JSON::Primitives::String::toString() const
{
    return "\"" + _value + "\"";
}

const std::string &Blueprint::JSON::Primitives::String::getType() const
{
    return _type;
}
