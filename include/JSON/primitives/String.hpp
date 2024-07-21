#ifndef __JSTRING_HPP
#define __JSTRING_HPP

#include <string>

#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON::Primitives
{
    class String : public Interfaces::IPrimitive {
      private:
        std::string _value;
        std::string _type = "string";

      public:
        String(const std::string &value);

        const std::string &value() const;
        void value(const std::string &value);

        std::string toString() const override;
        const std::string &getType() const override;
    };
} // namespace Blueprint::JSON::Primitives

#endif /* __JSTRING_HPP */
