#ifndef __LEXER_HPP
#define __LEXER_HPP

#include <cstddef>
#include <optional>

#include "JSON/Token.hpp"

namespace Blueprint::JSON
{
    class Lexer {
      private:
        std::string _json;
        std::size_t _position = 0;
        std::string _error = "Invalid JSON";

        void skipWhitespace();
        std::optional<Token> advanceAndReturn(
            Type type, const std::string &value);
        std::optional<Token> parseString();
        std::optional<Token> parseNumber();
        std::optional<Token> parseNull();
        std::optional<Token> parseBoolean();
        const std::string getWord() const;

      public:
        Lexer() = default;
        Lexer(const std::string &json);
        std::optional<Token> nextToken();
        const std::string &getError() const;
    };
} // namespace Blueprint::JSON

#endif /* __LEXER_HPP */
