#include <ctype.h>
#include <lex.h>
#include <lib/log.h>
#include <stdbool.h>

#define TOKEN(CHAR, TYPE)     \
    case CHAR:                \
        curr.type = TYPE;     \
        vec_push(&ret, curr); \
        break;

char *str_while(char **s, bool (*callback)(char c))
{
    vec_char_t ret = {0};
    vec_init(&ret);

    while (callback(**s))
    {
        if (!callback(**s))
        {
            break;
        }

        vec_push(&ret, **s);

        *s += 1;
    }
    *s -= 1;
    vec_push(&ret, '\0');

    return ret.data;
}

bool is_symbol(char c)
{
    return isdigit(c) || isalpha(c) || c == '_' || c == '-';
}

bool is_digit(char c) { return isdigit(c) || isxdigit(c); }

bool is_not_quote(char c) { return c != '"'; }

Tokens lex(char *str)
{
    Tokens ret = {};

    char *s = str;

    vec_init(&ret);

    int line = 0;
    int col = 0;

    while (*s)
    {
        Token curr = {0};

        switch (*s)
        {
            TOKEN('(', TOKEN_LPAREN);
            TOKEN('[', TOKEN_LBRACKET);
            TOKEN(')', TOKEN_RPAREN);
            TOKEN(']', TOKEN_RBRACKET);

        case '\n':
            line++;
            col = 0;
            break;

        case '"':
        {
            s++;
            curr.type = TOKEN_STRING;

            curr._string = str_while(&s, is_not_quote);
            vec_push(&ret, curr);
            s++;

            break;
        }

        case ';':
        {
            s++;

            while (*s != '\n')
                s++;
            break;
        }

        default:
        {
            if (isalpha(*s))
            {
                curr.type = TOKEN_SYMBOL;

                curr._symbol = str_while(&s, is_symbol);

                vec_push(&ret, curr);
            }

            else if (isdigit(*s))
            {

                curr.type = TOKEN_NUMBER;

                bool is_hex = false;

                if ((*s - '0') == 0 && *(s + 1) == 'x')
                {
                    is_hex = true;
                    s += 2;
                }

                char *num = str_while(&s, is_digit);

                sscanf(num, is_hex ? "%x" : "%u", &curr._num);
                vec_push(&ret, curr);
            }

            else if (*s == '#')
            {
                curr.type = TOKEN_REGISTER;
                s++;
                switch (tolower(*s))
                {
                case 'r':
                    s++;

                    curr._register = *s - '0';
                    break;

                case 'i':
                {
                    s++;
                    if (tolower(*s) == 'v' && tolower(*(s + 1)) == 't')
                        curr._register = REG_IVT;
                    s++;

                    break;
                }

                case 'p':
                    s++;
                    if (tolower(*s) == 'c')
                    {
                        curr._register = REG_PC;
                    }

                    else if (tolower(*s) == 'f')
                    {
                        curr._register = REG_PF;
                    }

                    else if (tolower(*s) == 't')
                    {
                        curr._register = REG_PT;
                    }

                    break;

                case '\\':
                {
                    s++;

                    curr.type = TOKEN_CHAR;
                    curr._char = *s;

                    if (tolower(*s) == 'n')
                    {
                        if (tolower(*(s + 1)) == 'l')
                        {
                            s++;
                            curr._char = '\n';
                        }
                    }

                    break;
                }

                case 's':
                    s++;
                    if (tolower(*s) == 'p')
                    {
                        curr._register = REG_SP;
                    }
                    break;

                default:
                    error("Unknown register: %c at %d:%d", *s, line, col);
                    exit(-1);
                    break;
                }

                curr.col = col++;
                vec_push(&ret, curr);
            }

            else
            {
                break;
            }
        }
        }

        s++;
    }

    return ret;
}
