#ifndef SPL_TOKEN_2023
#define SPL_TOKEN_2023

#include <optional>
#include <memory>
#include <string>

namespace spl
{
    enum toktype
    {
        UNDEF, //
        FDEF,  // function
        NAME,  //
        FEND,  // end
        ANY,   // any
        ARROW, // ->
        RET,   // return

        WHILE, // while
        WEND,  // end
        IF,    // if
        ELSE,  // else
        END,   // end

        PUSH,     //
        PUSH_STR, //
        DUP,      // dup
        POP,      // pop
        GET,      // get
        PEEK,     // peek
        SWAP,     // swp
        SIZE,     // size
        DUMP,     // dump

        ADD, // +
        SUB, // -
        MUL, // *
        DIV, // /
        MOD, // %

        EQ, // ==
        LT, // <
        LE, // <=
        GE, // >
        GT, // >=
        NE, // !=

        AND,    // &
        XOR,    // ^
        OR,     // |
        NOT,    // !
        RSHIFT, // <<
        LSHIFT, // >>

        WRITE
    };

    class token
    {
    private:
        toktype _type;
        std::shared_ptr<token> _next;
        std::shared_ptr<token> _jmpto;

        std::string _filename;
        size_t _column;
        size_t _row;
        long _value;
        std::string _name;

    public:
        token(toktype type, std::string filename, size_t column, size_t row, long value = 0, std::string name = "");

        toktype type();
        void set_type(toktype type);

        std::string filename();
        void set_filename(std::string filename);

        size_t column();
        void set_column(size_t column);

        size_t row();
        void set_row(size_t row);

        long value();
        void set_value(long value);

        std::string name();
        void set_name(std::string name);

        std::shared_ptr<token> next();
        void set_next(std::shared_ptr<token> next);

        std::shared_ptr<token> jmp();
        void set_jmp(std::shared_ptr<token> jmp);
    };

    const std::shared_ptr<token> none_token = std::make_shared<token>(UNDEF, "", 0, 0);
}

#endif