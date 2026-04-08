
#ifndef OOP8_PARSER_H
#define OOP8_PARSER_H
#include <iostream>
#include "tree4.h"
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <vector>
#include <stack>
void parserWrite(std::vector<std::string>& tokens) {
    if (
        !(tokens.size() >= 5 &&
        tokens[1] == "(" &&
        tokens[2] == "'"&&
        tokens[tokens.size()-3] == "'"&&
        tokens[tokens.size()-2] == ")"&&
        tokens[tokens.size()-1] == ";")
        ) {
        throw std::runtime_error("syntax error write");
        }
}
bool isVariable(std::string& line) {
    if (line.empty()) {
        return false;
    }
    if (!std::isalpha(line[0]) && line[0] != '_') {
        return false;
    }
    for (int i = 1; i < line.length(); i++) {
        if (!std::isalnum(line[i]) && line[i] != '_') {
            throw std::runtime_error("синтаксическая ошибка: недопустимый символ");
        }
    }
    return true;
}
void parserReadln(std::vector<std::string>& tokens,Tree234<string>& variables) {
    if (tokens.size() <= 5||tokens[1] != "(" ||tokens[tokens.size()-2] != ")"||tokens[tokens.size()-1] != ";") {
        throw std::runtime_error("syntax error readln");
    }
    int i = 2;
    for (i; i < tokens.size() - 3; i++) {
        if (i % 2 == 0) {
            if (!isVariable(tokens[i])) {
                throw std::runtime_error("переменной нет либо допущенна ошибка");
            }
            if (!variables.search(tokens[i])) {
                throw std::runtime_error("переменная не найдена");
            }
        }else {
            if (tokens[i] != ",") {
                throw std::runtime_error("syntax error readln");
            }
        }
    }
    if (i % 2 == 1) {
        throw std::runtime_error("не может оканчиваться на ,");
    }
}

bool isOperator(const string& s) {
    vector<string> ops = {"=", "<", ">", "<=", ">=", "and", "or", "*", "+", "-"};

    for (size_t i = 0; i < ops.size(); ++i) {
        if (ops[i] == s) return true;
    }
    return false;
}
bool isNumber(const string& s) {
    if (s[0] == '.') {
        throw std::runtime_error("нельзя начинать с точки");
    }
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '.') {
            continue;
        }
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}
bool isFunction(const string& s,Tree234<string>& funcs) {
    return funcs.search(s);
}
void parseFactor(vector<string>& tokens, int& i, Tree234<string>& variables,Tree234<string>& funcs);
void parseExpression(Tree234<string>& variables, int &i, std::vector<std::string>& tokens,Tree234<string>& funcs);

void parseFactor(vector<string>& tokens, int& i, Tree234<string>& variables,Tree234<string>& funcs) {

    if (isNumber(tokens[i])) {
        i++;
    }

    else if (isFunction(tokens[i],funcs)) {
        i++;

        if (tokens[i] != "(")
            throw runtime_error("ожидалась ( после функции");

        i++;

        parseExpression(variables, i, tokens,funcs);

        if (tokens[i] != ")")
            throw runtime_error("ожидалась )");

        i++;
    }
    else if (isVariable(tokens[i])) {
        if (!variables.search(tokens[i]))
            throw runtime_error("переменная не объявлена");
        i++;
    }

    else {
        throw runtime_error("неверный фактор");
    }
}


void parseExpression(Tree234<string>& variables, int &i, std::vector<std::string>& tokens,Tree234<string>& funcs) {
    parseFactor(tokens, i, variables,funcs);
    while (tokens[i] != ")") {
        if (!isOperator(tokens[i])) {
            throw runtime_error("ожидался оператор");
        }
        i++;
        parseFactor(tokens, i, variables,funcs);
    }
}

void parseFactor1(vector<string>& tokens, int& i, Tree234<string>& variables,Tree234<string>& funcs);
void parseExpression1(Tree234<string>& variables, int &i, std::vector<std::string>& tokens,Tree234<string>& funcs);

void parseFactor1(vector<string>& tokens, int& i, Tree234<string>& variables,Tree234<string>& funcs) {

    if (isNumber(tokens[i])) {
        i++;
    }

    else if (isFunction(tokens[i],funcs)) {
        i++;

        if (tokens[i] != "(")
            throw runtime_error("ожидалась ( после функции");

        i++;

        parseExpression1(variables, i, tokens,funcs);

        if (tokens[i] != ")")
            throw runtime_error("ожидалась )");

        i++;
    }
    else if (isVariable(tokens[i])) {
        if (!variables.search(tokens[i]))
            throw runtime_error("переменная не объявлена");
        i++;
    }

    else {
        throw runtime_error("неверный фактор");
    }
}
void parseExpression1(Tree234<string>& variables, int &i, std::vector<std::string>& tokens,Tree234<string>& funcs) {
    parseFactor1(tokens, i, variables,funcs);
    while (tokens[i] != ")" ) {
        if (tokens[i] == ";") {break;}
        if (!isOperator(tokens[i])) {
            throw runtime_error("ожидался оператор");
        }
        i++;
        parseFactor1(tokens, i, variables,funcs);
    }
}
void parserAssert(std::vector<std::string>& tokens,Tree234<string>& variables,Tree234<string>& funcs) {
    if (tokens.size() < 5) {
        throw std::runtime_error("мало токенов");
    }
    if (tokens[1] != "(") throw runtime_error("нет (");
    int i = 2;
    parseExpression(variables,i,tokens,funcs);
    if (tokens[i] != ")") throw runtime_error("нет )");
    i++;

    if (tokens[i] != ";") throw runtime_error("нет ;");
}

void addTree(Tree234<string>& funcs, std::vector<std::string>& func) {
    for (auto& f:func) {
        funcs.insert(f);
    }
}

bool isType(std::vector<std::string>& types,int& i,std::string& tok) {
    for (auto& type:types) {
        if (type == tok) {
            return true;
        }
    }
    return false;
}

void parserVar1(Tree234<string>&variables,
    std::vector<std::string>& tokens,std::vector<std::string>& types,int& i) {
    for (int j = 1; j < i; j++) {
        if (j %2 == 1) {
            if (variables.search(tokens[j])) {
                throw std::runtime_error("переменная уже объявленна");
            }else {
                if (!isVariable(tokens[j])) {
                    throw std::runtime_error("не правильно объявленна");
                }
                variables.insert(tokens[j]);

            }
        }else {
            if (tokens[j] != ",") {
                throw std::runtime_error("ожидалась ,");
            }
        }
    }
    if (!variables.search(tokens[i-1])) {
        throw std::runtime_error("ожидалась переменная");
    }
    if (tokens.size() <= i +1 || !isType(types,i,tokens[i+1])) {
        throw std::runtime_error("мало токенов, либо не правильный тип переменной");
    }
    if (tokens[tokens.size()-1] != ";") {
        throw std::runtime_error("ожидалась ;");
    }

}

void parserVar2(Tree234<string>& func,Tree234<string>&
    variables,std::vector<std::string>& tokens,int& i) {
    if (i -1 > 1) {
        throw std::runtime_error("много переменных, должна быть 1");
    }
    if (!isVariable(tokens[1]) || variables.search(tokens[1])) {
        throw std::runtime_error("переменная уже существует или не правильное имя");
    }
    variables.insert(tokens[1]);
    if (!isVariable(tokens[1])) {
        throw std::runtime_error("не правильное имя переменной");
    }
    i+=2;
    parseExpression1(variables, i,tokens,func);

}
void parserVar(Tree234<string>& func,Tree234<string>&
    variables,std::vector<std::string>& tokens) {
    std::vector<std::string> types = {"real"};
    int i = 1;
    while (tokens[i-1] != ":") {
        if (tokens.size() > i +1 && tokens[i] == ":" && tokens[i +1] != "=") {
            parserVar1(variables,tokens,types,i);
        }if (tokens.size() > i +1 && tokens[i] == ":" && tokens[i +1] == "=") {
            parserVar2(func,variables,tokens,i);
            break;
        }
        i++;
    }
}

void parserConst(Tree234<string>& func,Tree234<string>&
    variables,std::vector<std::string>& tokens) {
    if (!isVariable(tokens[1])) {
        throw std::runtime_error("не правильное имя переменной");
    }
    variables.insert(tokens[1]);
    if (tokens[2] != "=") {
        throw std::runtime_error("нет =");
    }
    if (!isNumber(tokens[3])){
        throw std::runtime_error("нет такой переменной или числа");
    }
    if (tokens[4] != ";") {
        throw std::runtime_error("нет ;");
    }
}
void parser(Tree234<string>& func,Tree234<string>&
    variables,std::vector<std::string>& tokens) {

    if (tokens[0] != "begin" && !tokens.size() && tokens[0] != "end") {
        throw std::runtime_error("нет такой переменной");
    }
    if (tokens[0] == "begin" || tokens[0] == "end.") {return;}
    bool is_func = func.search(tokens[0]);
    if (!is_func) {
        throw std::runtime_error("нет функции с таким названием");
    }

    if (tokens[0] == "write") {
        parserWrite(tokens);
    }
    if (tokens[0] == "readln") {
        parserReadln(tokens,variables);
    }
    if (tokens[0] == "var") {
        parserVar(func,variables,tokens);
    }
    if (tokens[0] == "assert") {
        parserAssert(tokens,variables,func);
    }
    if (tokens[0] == "const") {
        parserConst(func,variables,tokens);
    }
}

#endif //OOP8_PARSER_H