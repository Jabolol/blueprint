#include "JSON/primitives/Null.hpp"

std::string Blueprint::JSON::Primitives::Null::toString() const
{
    return "null";
}

const std::string &Blueprint::JSON::Primitives::Null::getType() const
{
    return _type;
}
