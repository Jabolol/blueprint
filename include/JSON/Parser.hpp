#ifndef __PARSER_HPP
#define __PARSER_HPP

#include <fmt/core.h>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>

#include "JSON/Lexer.hpp"
#include "JSON/Token.hpp"
#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON
{
    using ParserCallback =
        std::function<std::shared_ptr<Interfaces::IPrimitive>(Token &token)>;

    class Parser {
      private:
        Lexer _lexer;
        std::string _error;
        std::unordered_map<Type, ParserCallback> _callbacks;
        std::shared_ptr<Interfaces::IPrimitive> _root = nullptr;

        std::shared_ptr<Interfaces::IPrimitive> parseObject(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseArray(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseNumber(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseString(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseBoolean(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseNull(Token &token);

        template <typename... Args>
        void setError(fmt::format_string<Args...> fmt, Args &&...args)
        {
            auto it = std::back_inserter(_error);
            fmt::format_to(it, fmt, std::forward<Args>(args)...);
        }

      public:
        Parser();
        std::shared_ptr<Interfaces::IPrimitive> parse(const std::string &json);
        const std::string &getError() const;
    };
} // namespace Blueprint::JSON

#endif /* __PARSER_HPP */
