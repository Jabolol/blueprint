#ifndef __JNUMBER_HPP
#define __JNUMBER_HPP

#include <string>

#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON::Primitives
{
    class Number : public Interfaces::IPrimitive {
      private:
        double _value;
        bool _float;

      public:
        Number(const std::string &value);

        double value() const;
        void value(double v);

        std::string toString() const override;
    };
} // namespace Blueprint::JSON::Primitives

#endif /* __JNUMBER_HPP */
