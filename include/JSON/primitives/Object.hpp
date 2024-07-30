#ifndef __JOBJECT_HPP
#define __JOBJECT_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON::Primitives
{
    class Object : public Interfaces::IPrimitive {
      private:
        std::unordered_map<std::string,
            std::shared_ptr<Interfaces::IPrimitive>>
            _values;
        std::string _type = "object";

      public:
        void add(
            std::string &key, std::shared_ptr<Interfaces::IPrimitive> &value);
        const std::unordered_map<std::string,
            std::shared_ptr<Interfaces::IPrimitive>> &
        values() const;

        const std::shared_ptr<Interfaces::IPrimitive> &operator[](
            std::string key) const;

        std::string toString() const override;
        const std::string &getType() const override;

        static std::vector<std::reference_wrapper<const std::string>> keys(
            std::shared_ptr<Blueprint::Interfaces::IPrimitive> raw);
    };
} // namespace Blueprint::JSON::Primitives

#endif /* __JOBJECT_HPP */
