#ifndef __JARRAY_HPP
#define __JARRAY_HPP

#include <memory>
#include <string>
#include <vector>

#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON::Primitives
{
    class Array : public Interfaces::IPrimitive {
      private:
        std::string _type = "array";
        std::vector<std::shared_ptr<Interfaces::IPrimitive>> _values;

      public:
        void add(const std::shared_ptr<Interfaces::IPrimitive> &value);
        const std::vector<std::shared_ptr<Interfaces::IPrimitive>> &
        values() const;

        const std::shared_ptr<Interfaces::IPrimitive> &operator[](
            std::size_t index) const;

        std::string toString() const override;
        const std::string &getType() const override;
    };
} // namespace Blueprint::JSON::Primitives

#endif /* __JARRAY_HPP */
