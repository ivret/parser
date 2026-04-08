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
        try {
            std::vector<std::string> tokens = getToken(line);
            parser(funcs,variables,tokens);
            std::cout << i++ << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "ошибка в строке " << i << ": " << e.what() << std::endl;
        }

    }


    return 0;
}