#include "strTo.h"

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int str_to_int(const string& str){
    int result = 0;
    try{
        if (str[0] == '-'){ //если число отрицательное
            for(auto i = str.begin()+1; i != str.end(); i++){
                if (*i < '0' || *i > '9') throw str;
                result = result*10 + *i - '0';
            }
            result *= -1;
        }else{//если число положительное
            for(char i : str){
                if (i < '0' || i > '9') throw str;
                result = result*10 + i - '0';
            }
        }
    }
    catch (const string& error){
        cerr << "Ошибка, вы ввели: "<< error << ", а строка должна содержать только число"<< endl;
        exit(0);
    }
    return result;
}

double str_to_double(const string& str){
    double result = 0;
    bool comma = false; // флаг точки
    bool minus = false;
    int j = 1;
    if (str[0] == '-'){
        minus = true;
        j = -1;
    }
    try{
        for(char i : str){
            if (j == -1){ // пропускаем первый символ если это минус
                j = 1;
                continue;
            }
            if ((i < '0' || i > '9') && i != '.') throw str; //проверка на запрещённые символы
            if (i == '.'){
                if (comma) throw str; //проверка на 2-ю точку
                comma = true;
            }else{
                if (comma){
                result += static_cast<double>(i-'0')/pow(10, j);
                j++;
                }else{
                    result = result*10 + i - '0';
                }
            }
            
        }
    }
    catch (const string& error){
        cerr << "Ошибка, вы ввели: "<< error << ", а строка должна содержать только дробное число"<< endl;
        exit(0);
    }
    if (minus) result *= -1;
    return result;
}