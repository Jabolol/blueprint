#include <sstream>

#include "JSON/primitives/Array.hpp"
#include "interfaces/IPrimitive.hpp"

void Blueprint::JSON::Primitives::Array::add(
    const std::shared_ptr<Interfaces::IPrimitive> &value)
{
    _values.push_back(value);
}

const std::vector<std::shared_ptr<Blueprint::Interfaces::IPrimitive>> &
Blueprint::JSON::Primitives::Array::values() const
{
    return _values;
}

const std::shared_ptr<Blueprint::Interfaces::IPrimitive> &
Blueprint::JSON::Primitives::Array::operator[](std::size_t index) const
{
    return _values[index];
}

std::string Blueprint::JSON::Primitives::Array::toString() const
{
    std::stringstream ss;
    ss << '[';

    for (std::size_t i = 0; i < _values.size(); i++) {
        ss << _values[i]->toString();

        if (i < _values.size() - 1) {
            ss << ',';
        }
    }

    ss << ']';

    return ss.str();
}
