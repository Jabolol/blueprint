#ifndef __JBOOLEAN_HPP
#define __JBOOLEAN_HPP

#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON::Primitives
{
    class Boolean : public Interfaces::IPrimitive {
      private:
        bool _value;

      public:
        Boolean(const std::string &value);

        bool value() const;
        void value(bool value);

        std::string toString() const override;
    };
} // namespace Blueprint::JSON::Primitives

#endif /* __JBOOLEAN_HPP */
