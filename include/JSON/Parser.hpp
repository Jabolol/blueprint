#ifndef __PARSER_HPP
#define __PARSER_HPP

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "JSON/Lexer.hpp"
#include "JSON/Token.hpp"
#include "interfaces/IPrimitive.hpp"

namespace Blueprint::JSON
{
    using Callback =
        std::function<std::shared_ptr<Interfaces::IPrimitive>(Token &token)>;

    class Parser {
      private:
        Lexer _lexer;
        std::string _error;
        std::unordered_map<Type, Callback> _callbacks;
        std::shared_ptr<Interfaces::IPrimitive> _root = nullptr;

        std::shared_ptr<Interfaces::IPrimitive> parseObject(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseArray(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseNumber(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseString(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseBoolean(Token &token);
        std::shared_ptr<Interfaces::IPrimitive> parseNull(Token &token);

      public:
        explicit Parser();
        std::optional<std::shared_ptr<Interfaces::IPrimitive>> parse(
            const std::string &json);
        const std::string &getError() const;
    };
} // namespace Blueprint::JSON

#endif /* __PARSER_HPP */
