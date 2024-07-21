#ifndef __SCHEMA_HPP
#define __SCHEMA_HPP

#include <format>
#include <functional>

#include "JSON/Parser.hpp"
#include "JSON/primitives/Object.hpp"
#include "interfaces/IPrimitive.hpp"

namespace Blueprint
{
    using SchemaCallback =
        std::function<bool(std::shared_ptr<Interfaces::IPrimitive>,
            std::shared_ptr<JSON::Primitives::Object>)>;

    class Schema {
      private:
        std::string _error;
        JSON::Parser _parser;
        std::unordered_map<std::string, SchemaCallback> _callbacks;

        bool handle(std::shared_ptr<JSON::Primitives::Object> schema,
            std::shared_ptr<Interfaces::IPrimitive> data);

        bool minValue(std::shared_ptr<Interfaces::IPrimitive> schema,
            std::shared_ptr<JSON::Primitives::Object> data);
        bool maxValue(std::shared_ptr<Interfaces::IPrimitive> schema,
            std::shared_ptr<JSON::Primitives::Object> data);
        bool minLength(std::shared_ptr<Interfaces::IPrimitive> schema,
            std::shared_ptr<JSON::Primitives::Object> data);
        bool maxLength(std::shared_ptr<Interfaces::IPrimitive> schema,
            std::shared_ptr<JSON::Primitives::Object> data);
        bool enumValue(std::shared_ptr<Interfaces::IPrimitive> schema,
            std::shared_ptr<JSON::Primitives::Object> data);
        bool required(std::shared_ptr<Interfaces::IPrimitive> schema,
            std::shared_ptr<JSON::Primitives::Object> data);
        bool exactValue(std::shared_ptr<Interfaces::IPrimitive> schema,
            std::shared_ptr<JSON::Primitives::Object> data);

        template <typename T>
        static std::shared_ptr<T> as(
            std::shared_ptr<Interfaces::IPrimitive> primitive);

        template <typename... Args>
        void setError(std::format_string<Args...> fmt, Args &&...args)
        {
            std::format_to(
                std::back_inserter(_error), fmt, std::forward<Args>(args)...);
        }

      public:
        Schema();
        bool verify(std::string schema, std::string data);
        const std::string &getError() const;
    };
} // namespace Blueprint

#endif /* __SCHEMA_HPP */
