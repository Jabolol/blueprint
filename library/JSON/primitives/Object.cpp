#include <sstream>

#include "JSON/primitives/Object.hpp"

void Blueprint::JSON::Primitives::Object::add(
    std::string &key, std::shared_ptr<Interfaces::IPrimitive> &value)
{
    if (_values.find(key) != _values.end()) {
        throw std::runtime_error("Key already exists in object");
    }

    _values[key] = value;
}

const std::unordered_map<std::string,
    std::shared_ptr<Blueprint::Interfaces::IPrimitive>> &
Blueprint::JSON::Primitives::Object::values() const
{
    return _values;
}

const std::shared_ptr<Blueprint::Interfaces::IPrimitive> &
Blueprint::JSON::Primitives::Object::operator[](std::string key) const
{
    return _values.at(key);
}

std::string Blueprint::JSON::Primitives::Object::toString() const
{
    std::stringstream result;
    result << "{";

    for (auto it = _values.begin(); it != _values.end(); ++it) {
        result << "\"" << it->first << "\":" << it->second->toString();
        if (std::next(it) != _values.end()) {
            result << ",";
        }
    }

    result << "}";

    return result.str();
}
