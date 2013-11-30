/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Parser.h"

#include <algorithm>
#include <map>
#include <stack>
#include <QDebug>

namespace Rangers
{
namespace QuestPlayer
{
namespace
{
const uint8_t PRECEDENCES[22] = {0, 0, 0, 0, 6, 6, 8, 7, 8, 3, 3, 3, 3, 3, 3, 1, 2, 9, 4, 5, 0, 0};
}

Token::Token()
{
    type = TOKEN_NONE;
    value.from = 0.0f;
    value.to = 0.0f;
    value.number = 0.0f;
}

Token::Token(float number)
{
    type = TOKEN_NUMBER;
    value.number = number;
}

Token::Token(float from, float to)
{
    type = TOKEN_RANGE;
    value.from = from;
    value.to = to;
}

Token Token::apply(const Token& a, const Token& b, const std::map<uint32_t, float> &parameters) const
{
    float av, bv;

    if (a.type == TOKEN_RANGE)
        av = a.value.from + (rand() % (uint32_t)(a.value.to - a.value.from + 1));
    else if (a.type == TOKEN_LIST)
        av = a.list.at(rand() % a.list.size());
    else if (a.type == TOKEN_PARAMETER)
        av = parameters.at(a.value.id);
    else
        av = a.value.number;

    if (b.type == TOKEN_RANGE)
        bv = b.value.from + (rand() % (uint32_t)(b.value.to - b.value.from + 1));
    else if (b.type == TOKEN_LIST)
        bv = b.list.at(rand() % b.list.size());
    else if (b.type == TOKEN_PARAMETER)
        bv = parameters.at(b.value.id);
    else
        bv = b.value.number;

    switch (type)
    {
    case TOKEN_NUMBER:
    case TOKEN_RANGE:
    case TOKEN_PARAMETER:
    case TOKEN_OPEN_PAR:
    case TOKEN_CLOSE_PAR:
    case TOKEN_NONE:
    case TOKEN_LIST:
        return *this;
    case TOKEN_OP_PLUS:
        return Token(av + bv);
    case TOKEN_OP_MINUS:
        return Token(av - bv);
    case TOKEN_OP_DIV:
        return Token(av / bv);
    case TOKEN_OP_MULTIPLY:
        return Token(av * bv);
    case TOKEN_OP_MOD:
        return Token(fmod(av, bv));
    case TOKEN_EQUAL:
        return Token(av == bv);
    case TOKEN_MORE:
        return Token(av > bv);
    case TOKEN_LESS:
        return Token(av < bv);
    case TOKEN_MORE_EQUAL:
        return Token(av >= bv);
    case TOKEN_LESS_EQUAL:
        return Token(av <= bv);
    case TOKEN_NOT_EQUAL:
        return Token(av != bv);
    case TOKEN_OR:
        return Token(av || bv);
    case TOKEN_AND:
        return Token(av && bv);
    case TOKEN_NOT:
        return Token(!av);
    case TOKEN_TO:
        //FIXME: List?
        if ((a.type == TOKEN_RANGE) && (b.type == TOKEN_RANGE))
        {
            float min = std::min(a.value.from, b.value.from);
            float max = std::max(a.value.to, b.value.to);
            return Token(min, max);
        }
        else if ((a.type == TOKEN_RANGE) && (b.type != TOKEN_RANGE))
        {
            float min = std::min(a.value.from, bv);
            float max = std::max(a.value.to, bv);
            return Token(min, max);
        }
        else if ((a.type != TOKEN_RANGE) && (b.type == TOKEN_RANGE))
        {
            float min = std::min(av, b.value.from);
            float max = std::max(av, b.value.to);
            return Token(min, max);
        }
        else
        {
            float min = std::min(av, bv);
            float max = std::max(av, bv);
            return Token(min, max);
        }
    case TOKEN_IN:
        if ((a.type == TOKEN_RANGE) && (b.type == TOKEN_RANGE))
        {
            return Token((a.value.from >= b.value.from) && (a.value.to <= b.value.to));
        }
        else if ((a.type == TOKEN_RANGE) && (b.type != TOKEN_RANGE))
        {
            return Token((a.value.from <= bv) && (a.value.to >= bv));
        }
        else if ((a.type != TOKEN_RANGE) && (b.type == TOKEN_RANGE))
        {
            return Token((av >= b.value.from) && (av <= b.value.to));
        }
        else
        {
            return Token(av == bv);
        }
    }
}

//FIXME: Seems only integers are valid in expressions...
int32_t getNumber(int &pos, const QString& exp)
{
    uint32_t value = 0.0f;
    bool minus = false;
    if (exp[pos] == '-')
        minus = true;

    for (; (pos < exp.length()) && ((exp[pos] >= '0') && (exp[pos] <= '9')); pos++)
    {
        value = value * 10 + (exp[pos].toLatin1() - '0');
    }

    return minus ? -value : value;
}
std::list<Token> tokenize(const QString& exp)
{
    int pos = 0;
    Token prev;
    std::list<Token> result;

    std::stack<Token> opStack;

    while (pos < exp.length())
    {
        while ((pos < exp.length()) && (exp[pos].isSpace())) pos++;
        if (pos == exp.length())
            break;

        Token t;
        //FIXME: Ugly unary minus detection.
        if (((exp[pos] >= '0') && (exp[pos] <= '9')) || ((exp[pos] == '-') && (prev.type != Token::TOKEN_NUMBER) &&
                (prev.type != Token::TOKEN_RANGE) && (prev.type != Token::TOKEN_PARAMETER) && (prev.type != Token::TOKEN_LIST) &&
                (prev.type != Token::TOKEN_OPEN_PAR) && (prev.type != Token::TOKEN_CLOSE_PAR)))
        {
            t = Token(getNumber(pos, exp));
        }
        else if (exp[pos] == '-')
        {
            t.type = Token::TOKEN_OP_MINUS;
            pos++;
        }
        else if (exp[pos] == '+')
        {
            t.type = Token::TOKEN_OP_PLUS;
            pos++;
        }
        else if (exp[pos] == '*')
        {
            t.type = Token::TOKEN_OP_MULTIPLY;
            pos++;
        }
        else if (exp[pos] == '/')
        {
            t.type = Token::TOKEN_OP_DIV;
            pos++;
        }
        else if (exp[pos] == '(')
        {
            t.type = Token::TOKEN_OPEN_PAR;
            pos++;
        }
        else if (exp[pos] == ')')
        {
            t.type = Token::TOKEN_CLOSE_PAR;
            pos++;
        }
        else if (exp[pos] == '=')
        {
            t.type = Token::TOKEN_EQUAL;
            pos++;
        }
        else if (exp[pos] == '>')
        {
            if (exp[pos + 1] == '=')
            {
                t.type = Token::TOKEN_MORE_EQUAL;
                pos += 2;
            }
            else
            {
                t.type = Token::TOKEN_MORE;
                pos++;
            }
        }
        else if (exp[pos] == '<')
        {
            if (exp[pos + 1] == '=')
            {
                t.type = Token::TOKEN_LESS_EQUAL;
                pos += 2;
            }
            else if (exp[pos + 1] == '>')
            {
                t.type = Token::TOKEN_NOT_EQUAL;
                pos += 2;
            }
            else
            {
                t.type = Token::TOKEN_LESS;
                pos++;
            }
        }
        else if (exp[pos] == '[')
        {
            if (exp[pos + 1] == 'p')
            {
                pos += 2;
                t.type = Token::TOKEN_PARAMETER;
                t.value.id = getNumber(pos, exp);
                if (exp[pos] != ']')
                {
                    qCritical() << "Unclosed parameter token in \"" << exp << "\"";
                    return std::list<Token>();
                }
                pos++;
            }
            else if (((exp[pos + 1] >= '0') && (exp[pos + 1] <= '9')) || ((exp[pos + 1] == '-')))
            {
                pos++;
                int32_t v1, v2;
                v1 = getNumber(pos, exp);
                if (exp[pos] == ']')
                {
                    t = Token(v1);
                    pos++;
                }
                else if ((exp[pos] == '.') || (exp[pos] == 'h'))
                {
                    if ((exp[pos] == '.') && (exp[pos + 1] != '.'))
                    {
                        qCritical() << "Invalid range token in \"" << exp << "\"";
                        return std::list<Token>();
                    }
                    if (exp[pos] == 'h')
                        pos++;
                    else
                        pos += 2;
                    v2 = getNumber(pos, exp);
                    if (exp[pos] != ']')
                    {
                        qCritical() << "Unclosed range token in \"" << exp << "\"";
                        return std::list<Token>();
                    }
                    t = Token(v1, v2);
                    pos++;
                }
                else if (exp[pos] == ';')
                {
                    t.type = Token::TOKEN_LIST;
                    t.list.push_back(v1);
                    while (exp[pos] == ';')
                    {
                        pos++;
                        t.list.push_back(getNumber(pos, exp));
                    }
                    if (exp[pos] != ']')
                    {
                        qCritical() << "Unclosed list token in \"" << exp << "\"";
                        return std::list<Token>();
                    }
                    pos++;
                }
            }
            else
            {
                qCritical() << "Invalid token in \"" << exp << "\"";
                return std::list<Token>();
            }
        }
        else if (exp.mid(pos, 3) == "mod")
        {
            t.type = Token::TOKEN_OP_MOD;
            pos += 3;
        }
        else if (exp.mid(pos, 3) == "div")
        {
            t.type = Token::TOKEN_OP_DIV;
            pos += 3;
        }
        else if (exp.mid(pos, 3) == "and")
        {
            t.type = Token::TOKEN_AND;
            pos += 3;
        }
        else if (exp.mid(pos, 3) == "not")
        {
            t.type = Token::TOKEN_NOT;
            pos += 3;
        }
        else if (exp.mid(pos, 2) == "or")
        {
            t.type = Token::TOKEN_OR;
            pos += 2;
        }
        else if (exp.mid(pos, 2) == "to")
        {
            t.type = Token::TOKEN_TO;
            pos += 2;
        }
        else if (exp.mid(pos, 2) == "in")
        {
            t.type = Token::TOKEN_IN;
            pos += 2;
        }
        else
        {
            qCritical() << "Invalid range token in \"" << exp << "\"";
            return std::list<Token>();
        }

        if ((t.type == Token::TOKEN_NUMBER) || (t.type == Token::TOKEN_RANGE) ||
                (t.type == Token::TOKEN_PARAMETER) || (t.type == Token::TOKEN_LIST))
        {
            result.push_back(t);
        }
        else if (t.type == Token::TOKEN_OPEN_PAR)
        {
            opStack.push(t);
        }
        else if (t.type == Token::TOKEN_CLOSE_PAR)
        {
            Token t2;
            do
            {
                if (opStack.empty())
                {
                    qCritical() << "Parenthesis error in \"" << exp << "\"";
                    return std::list<Token>();
                }
                t2 = opStack.top();
                opStack.pop();
                if (t2.type != Token::TOKEN_OPEN_PAR)
                    result.push_back(t2);
            }
            while (t2.type != Token::TOKEN_OPEN_PAR && !opStack.empty());
        }
        else
        {
            if (!opStack.empty())
            {
                Token o2 = opStack.top();
                while ((!opStack.empty()) && (o2.type != Token::TOKEN_OPEN_PAR) && (o2.type != Token::TOKEN_CLOSE_PAR) &&
                        (PRECEDENCES[t.type] <= PRECEDENCES[o2.type]))
                {
                    result.push_back(o2);
                    opStack.pop();
                    if (!opStack.empty())
                        o2 = opStack.top();
                }
            }
            opStack.push(t);
        }

        prev = t;
    }
    while (!opStack.empty())
    {
        Token t = opStack.top();
        opStack.pop();
        if (t.type == Token::TOKEN_OPEN_PAR)
        {
            qCritical() << "Parenthesis error in \"" << exp << "\"";
            return std::list<Token>();
        }
        result.push_back(t);
    }
    return result;
}

float eval(const std::list< Token >& exp, const std::map<uint32_t, float>& parameters)
{
    if (exp.empty())
        return 0;

    std::stack<Token> opStack;
    for (const Token & t : exp)
    {
        if ((t.type == Token::TOKEN_NUMBER) || (t.type == Token::TOKEN_RANGE) ||
                (t.type == Token::TOKEN_PARAMETER) || (t.type == Token::TOKEN_LIST))
        {
            opStack.push(t);
        }
        else
        {
            Token b = opStack.top();
            opStack.pop();
            Token a = opStack.top();
            opStack.pop();
            opStack.push(t.apply(a, b, parameters));
        }
    }
    Token r = opStack.top();

    if (r.type == Token::TOKEN_PARAMETER)
        return parameters.at(r.value.id);
    else if (r.type == Token::TOKEN_RANGE)
        return r.value.from + (rand() % (uint32_t)(r.value.to - r.value.from + 1));
    else if (r.type == Token::TOKEN_LIST)
        return r.list.at(rand() % r.list.size());
    else
        return r.value.number;
}

float eval(const QString& exp, const std::map<uint32_t, float>& parameters)
{
    return eval(tokenize(exp), parameters);
}

}
}
