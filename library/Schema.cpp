#include <memory>
#include <string>

#include "JSON/primitives/Array.hpp"
#include "JSON/primitives/Number.hpp"
#include "JSON/primitives/Object.hpp"
#include "JSON/primitives/String.hpp"
#include "Schema.hpp"
#include "interfaces/IPrimitive.hpp"

Blueprint::Schema::Schema()
{
    _callbacks = {
        {
            "MIN_VALUE",
            std::bind(&Blueprint::Schema::minValue, this,
                std::placeholders::_1, std::placeholders::_2),
        },
        {
            "MAX_VALUE",
            std::bind(&Blueprint::Schema::maxValue, this,
                std::placeholders::_1, std::placeholders::_2),
        },
        {
            "MIN_LENGTH",
            std::bind(&Blueprint::Schema::minLength, this,
                std::placeholders::_1, std::placeholders::_2),
        },
        {
            "MAX_LENGTH",
            std::bind(&Blueprint::Schema::maxLength, this,
                std::placeholders::_1, std::placeholders::_2),
        },
        {
            "ENUM",
            std::bind(&Blueprint::Schema::enumValue, this,
                std::placeholders::_1, std::placeholders::_2),
        },
        {
            "REQUIRED",
            std::bind(&Blueprint::Schema::required, this,
                std::placeholders::_1, std::placeholders::_2),
        },
        {
            "VALUES",
            std::bind(&Blueprint::Schema::exactValue, this,
                std::placeholders::_1, std::placeholders::_2),
        },
    };
}

bool Blueprint::Schema::verify(std::string schema, std::string data)
{
    std::shared_ptr schemaObject =
        Schema::as<JSON::Primitives::Object>(_parser.parse(schema));
    if (schemaObject == nullptr) {
        setError("Invalid schema. Expected object, got '{}'", schema);
        return false;
    }

    std::shared_ptr dataObject = _parser.parse(data);
    if (dataObject == nullptr) {
        setError("Invalid data. Expected object, got '{}'", data);
        return false;
    }

    try {
        return handle(schemaObject, dataObject);
    } catch (const std::out_of_range &error) {
        setError("Out of bounds access: {}", error.what());
        return false;
    }
}

// TODO(jabolo): Improve error messages to include the invalid value path
bool Blueprint::Schema::handle(
    std::shared_ptr<JSON::Primitives::Object> schema,
    std::shared_ptr<Interfaces::IPrimitive> data)
{
    std::string type = data->getType();

    if (type == "array") {
        std::shared_ptr primitive = Schema::as<JSON::Primitives::Array>(data);
        if (primitive == nullptr) {
            setError("Invalid array '{}'", data->toString());
            return false;
        }

        std::shared_ptr constraints =
            Schema::as<JSON::Primitives::Array>((*schema)["constraints"]);
        if (constraints == nullptr) {
            setError("Invalid constraints '{}'", schema->toString());
            return false;
        }

        std::shared_ptr innerSchema =
            Schema::as<JSON::Primitives::Object>((*schema)["data"]);
        if (innerSchema == nullptr) {
            setError("Invalid innerSchema '{}'", schema->toString());
            return false;
        }

        if (!check(constraints, data)) {
            return false;
        }

        for (const auto &value : primitive->values()) {
            if (!handle(innerSchema, value)) {
                return false;
            }
        }

        return true;
    }

    if (type == "object") {
        std::shared_ptr primitive = Schema::as<JSON::Primitives::Object>(data);
        if (primitive == nullptr) {
            setError("Invalid object '{}'", data->toString());
            return false;
        }

        std::shared_ptr constraints =
            Schema::as<JSON::Primitives::Array>((*schema)["constraints"]);
        if (constraints == nullptr) {
            setError("Invalid constraints '{}'", schema->toString());
            return false;
        }

        std::shared_ptr innerSchema =
            Schema::as<JSON::Primitives::Object>((*schema)["data"]);
        if (innerSchema == nullptr) {
            setError("Invalid innerSchema '{}'", schema->toString());
            return false;
        }

        if (!check(constraints, data)) {
            return false;
        }

        for (const auto &key : JSON::Primitives::Object::keys(primitive)) {
            std::shared_ptr objectSchema =
                Schema::as<JSON::Primitives::Object>((*innerSchema)[key]);
            if (!handle(objectSchema, (*primitive)[key])) {
                return false;
            }
        }

        return true;
    }

    std::shared_ptr constraints =
        Schema::as<JSON::Primitives::Array>((*schema)["constraints"]);
    if (constraints == nullptr) {
        setError("Invalid constraints '{}'", schema->toString());
        return false;
    }

    return check(constraints, data);
}

bool Blueprint::Schema::minValue(std::shared_ptr<Interfaces::IPrimitive> data,
    std::shared_ptr<JSON::Primitives::Object> schema)
{
    int primitive = std::stoi(data->toString());
    int constraint = std::stoi((*schema)["MIN_VALUE"]->toString());

    if (primitive < constraint) {
        setError("Value '{}' is less than '{}'", primitive, constraint);
        return false;
    }

    return true;
}

bool Blueprint::Schema::maxValue(std::shared_ptr<Interfaces::IPrimitive> data,
    std::shared_ptr<JSON::Primitives::Object> schema)
{
    int primitive = std::stoi(data->toString());
    int constraint = std::stoi((*schema)["MAX_VALUE"]->toString());

    if (primitive > constraint) {
        setError("Value '{}' is greater than '{}'", primitive, constraint);
        return false;
    }

    return true;
}

bool Blueprint::Schema::minLength(std::shared_ptr<Interfaces::IPrimitive> data,
    std::shared_ptr<JSON::Primitives::Object> schema)
{
    std::string type = data->getType();

    if (type == "array") {
        std::shared_ptr primitive = Schema::as<JSON::Primitives::Array>(data);
        if (primitive == nullptr) {
            setError("Invalid array '{}'", data->toString());
            return false;
        }

        std::size_t length = std::stoll((*schema)["MIN_LENGTH"]->toString());
        std::vector values = primitive->values();

        if (values.size() < length) {
            setError("Minimum size expected {} elements, got {}", length,
                values.size());
            return false;
        }

        return true;
    }

    if (type == "string") {
        std::shared_ptr primitive = Schema::as<JSON::Primitives::String>(data);
        if (primitive == nullptr) {
            setError("Invalid string '{}'", data->toString());
            return false;
        }

        std::size_t length = std::stoll((*schema)["MIN_LENGTH"]->toString());
        std::size_t size = data->toString().size();

        if (size < length) {
            setError("Minimum size expected {}, got {}", length, size);
            return false;
        }

        return true;
    }

    setError("Expected 'array' or 'string', got {}", type);
    return false;
}

bool Blueprint::Schema::maxLength(std::shared_ptr<Interfaces::IPrimitive> data,
    std::shared_ptr<JSON::Primitives::Object> schema)
{
    std::string type = data->getType();

    if (type == "array") {
        std::shared_ptr primitive = Schema::as<JSON::Primitives::Array>(data);
        if (primitive == nullptr) {
            setError("Invalid array '{}'", data->toString());
            return false;
        }

        std::size_t length = std::stoll((*schema)["MAX_LENGTH"]->toString());
        std::vector values = primitive->values();

        if (values.size() > length) {
            setError("Maximum size expected {} elements, got {}", length,
                values.size());
            return false;
        }

        return true;
    }

    if (type == "string") {
        std::shared_ptr primitive = Schema::as<JSON::Primitives::String>(data);
        if (primitive == nullptr) {
            setError("Invalid string '{}'", data->toString());
            return false;
        }

        std::size_t length = std::stoll((*schema)["MAX_LENGTH"]->toString());
        std::size_t size = data->toString().size() - 2;

        if (size > length) {
            setError("Maximum size expected {}, got {}", length, size);
            return false;
        }

        return true;
    }

    setError("Expected 'array' or 'string', got {}", type);
    return false;
}

bool Blueprint::Schema::enumValue(std::shared_ptr<Interfaces::IPrimitive> data,
    std::shared_ptr<JSON::Primitives::Object> schema)
{
    std::shared_ptr values =
        Schema::as<JSON::Primitives::Array>((*schema)["ENUM"]);
    if (values == nullptr) {
        setError("Invalid ENUM constraint '{}'", schema->toString());
        return false;
    }

    for (const auto &value : values->values()) {
        if (data->toString() == value->toString()) {
            return true;
        }
    }

    setError("Value '{}' not in ENUM", data->toString());
    return false;
}

bool Blueprint::Schema::required(
    [[maybe_unused]] std::shared_ptr<Interfaces::IPrimitive> data,
    [[maybe_unused]] std::shared_ptr<JSON::Primitives::Object> schema)
{
    setError("{} not implemented", __func__);
    return false;
}

bool Blueprint::Schema::exactValue(
    std::shared_ptr<Interfaces::IPrimitive> data,
    std::shared_ptr<JSON::Primitives::Object> schema)
{
    std::shared_ptr array = Schema::as<JSON::Primitives::Array>(data);
    if (array != nullptr) {
        std::shared_ptr values =
            Schema::as<JSON::Primitives::Array>((*schema)["VALUES"]);
        if (values == nullptr) {
            setError("Invalid VALUES constraint '{}'", schema->toString());
            return false;
        }

        for (const auto &value : array->values()) {
            bool found = false;
            for (const auto &constraint : values->values()) {
                if (value->toString() == constraint->toString()) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                setError("Value {} not in VALUES", value->toString());
                return false;
            }
        }

        return true;
    }

    std::shared_ptr number = Schema::as<JSON::Primitives::Number>(data);
    if (number != nullptr) {
        double primitive = std::stod(number->toString());
        std::shared_ptr values =
            Schema::as<JSON::Primitives::Array>((*schema)["VALUES"]);
        if (values == nullptr) {
            setError("Invalid VALUES constraint '{}'", schema->toString());
            return false;
        }

        for (const auto &value : values->values()) {
            double constraint = std::stod(value->toString());
            if (primitive == constraint) {
                return true;
            }
        }

        setError("Value {} not in VALUES", data->toString());
        return false;
    }

    setError("Invalid type '{}'", data->getType());
    return false;
}

template <typename T>
std::shared_ptr<T> Blueprint::Schema::as(
    std::shared_ptr<Interfaces::IPrimitive> primitive)
{
    return std::dynamic_pointer_cast<T>(primitive);
}

const std::string &Blueprint::Schema::getError() const
{
    return _error;
}

bool Blueprint::Schema::check(
    std::shared_ptr<JSON::Primitives::Array> constraints,
    std::shared_ptr<Interfaces::IPrimitive> data)
{
    for (const auto &value : constraints->values()) {
        auto object = Schema::as<JSON::Primitives::Object>(value);
        auto keys = JSON::Primitives::Object::keys(object);

        for (const std::string &key : keys) {
            auto cb = _callbacks.find(key);
            if (cb == _callbacks.end()) {
                setError("Invalid constraint '{}'", key);
                return false;
            }
            if (!cb->second(data, object)) {
                return false;
            }
        }
    }

    return true;
}
