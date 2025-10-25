#pragma once
#include <string>
#include <cctype>
#include <unordered_map>
#include <stdexcept>
#include <cstdint>

namespace mvs
{
    enum class Keyword
    {
        MODULE,
        ENDMODULE,
        INPUT,
        OUTPUT,
        INOUT,
        WIRE,
        ASSIGN,
        NONE
    };

    inline bool is_identifier_start(char c)
    {
        return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
    }

    inline bool is_identifier_char(char c)
    {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    }

    inline bool is_symbol_char(char c)
    {
        switch (c)
        {
        case '(':
        case ')':
        case ',':
        case ';':
        case '=':
        case '&':
        case '|':
        case '^':
        case '~':
        case '+':
        case '*':
        case '[':
        case ']':
        case ':':
            return true;
        default:
            return false;
        }
    }
    inline Keyword to_keyword(const std::string &str)
    {
        static const std::unordered_map<std::string, Keyword> keywords = {
            {"module", Keyword::MODULE},
            {"endmodule", Keyword::ENDMODULE},
            {"input", Keyword::INPUT},
            {"output", Keyword::OUTPUT},
            {"inout", Keyword::INOUT},
            {"wire", Keyword::WIRE},
            {"assign", Keyword::ASSIGN}};

        auto it = keywords.find(str);
        return it != keywords.end() ? it->second : Keyword::NONE;
    }

    inline int parse_number(const std::string &str)
    {
        size_t i = 0;
        int width = 0;

        // 1. optional width
        if (str.find('\'') != std::string::npos)
        {
            while (str[i] != '\'' && std::isdigit(str[i]))
            {
                width = width * 10 + (str[i] - '0');
                i++;
            }
            i++;
        }

        // 2. default base = 10
        int base = 10;

        if (std::isalpha(str[i]))
        {
            char base_char = std::tolower(str[i++]);

            switch (base_char)
            {
            case 'b':
                base = 2;
                break;
            case 'h':
                base = 16;
                break;
            case 'd':
                base = 10;
                break;
            default:
                throw std::runtime_error("Invalid base character '" + std::string(1, base_char) + "' in number literal \"" + str + "\"");
            }
        }

        // 3. collect digits
        std::string digits;
        while (i < str.size())
        {
            char c = str[i++];
            if (c == '_')
                continue; // ignore underscores
            digits.push_back(c);
        }

        if (digits.empty())
            throw std::runtime_error("Missing value digits in number literal \"" + str + "\"");

        int value = std::stoi(digits, nullptr, base);
        if (width > 0)
        {
            if (width > 64)
                throw std::runtime_error("Width " + std::to_string(width) + " too large, max 64 bits");
            value &= ((uint64_t(1) << width) - 1);
        }

        return value;
    }

} // namespace mvs
