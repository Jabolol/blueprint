#include <cmath>

#include "JSON/primitives/Number.hpp"

Blueprint::JSON::Primitives::Number::Number(const std::string &value)
    : _float(value.find('.') != std::string::npos), _value(std::stod(value))
{
}

double Blueprint::JSON::Primitives::Number::value() const
{
    return _value;
}

void Blueprint::JSON::Primitives::Number::value(double v)
{
    _value = v;
}

std::string Blueprint::JSON::Primitives::Number::toString() const
{
    return _float ? std::to_string(_value)
                  : std::to_string(std::llround(_value));
}
