#include "JSON/primitives/Boolean.hpp"

Blueprint::JSON::Primitives::Boolean::Boolean(const std::string &value)
    : _value(value == "true")
{
}

bool Blueprint::JSON::Primitives::Boolean::value() const
{
    return _value;
}

void Blueprint::JSON::Primitives::Boolean::value(bool value)
{
    _value = value;
}

std::string Blueprint::JSON::Primitives::Boolean::toString() const
{
    return _value ? "true" : "false";
}
