#ifndef __IPRIMITIVE_HPP
#define __IPRIMITIVE_HPP

#include <string>

namespace Blueprint::Interfaces
{
    class IPrimitive {
      public:
        virtual ~IPrimitive() = default;
        virtual std::string toString() const = 0;
    };
} // namespace Blueprint::Interfaces

#endif /* __IPRIMITIVE_HPP */
