#ifndef __JNULL_HPP
#define __JNULL_HPP

#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON::Primitives
{
    class Null : public Interfaces::IPrimitive {
      private:
        std::string _type = "null";

      public:
        std::string toString() const override;
        const std::string &getType() const override;
    };
} // namespace Blueprint::JSON::Primitives

#endif /* __JNULL_HPP */
