#include <iostream>
#include "tree4.h"
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <vector>
#include "parser.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

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

std::vector<std::string> getToken(std::string& line) {
    std::string current = "";
    int count = 0;
    std::vector<std::string> tokens;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] != ' ' && (!std::ispunct(line[i]) || line[i] == '.')){
            current += line[i];
        }else if (std::ispunct(line[i])){
            if (!current.empty()) {
                tokens.push_back(current);
            }
            current = "";
            current += line[i];
            tokens.push_back(current);
            current = "";
        }
        else {
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }

        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    std::ifstream ifs("cod.txt");
    if (!ifs.is_open()) {
        std::cout << "error\n";
    }
    Tree234<string> variables;
    Tree234<string> funcs;
    std::vector<std::string> all_func = {"var","assert", "sin","readln","write","cos","cos","const"};
    addTree(funcs,all_func);

    std::string line;
    int i = 0;
    while (std::getline(ifs, line)) {
        std::vector<std::string> tokens = getToken(line);
        parser(funcs,variables,tokens);
        std::cout << i++ << "\n";
    }


    return 0;
}