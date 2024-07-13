#ifndef __TOKEN_HPP
#define __TOKEN_HPP

#include <string>

namespace Blueprint::JSON
{
    enum class Type {
        OBJECT_START,
        OBJECT_END,
        ARRAY_START,
        ARRAY_END,
        STRING,
        NUMBER,
        BOOLEAN,
        NULL_VALUE,
        COMMA,
        COLON,
        END_OF_FILE,
    };

    class Token {
      private:
        Type _type;
        std::string _data;

      public:
        Token(Type type, const std::string data);

        Token(Token &&other) noexcept = default;
        Token &operator=(Token &&other) noexcept = default;

        Type type() const;
        const std::string &data() const;
    };
} // namespace Blueprint::JSON

#endif /* __TOKEN_HPP */
