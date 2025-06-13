//g++ -w contFracAndEquation.cpp elgamal.cpp evklidAlg.cpp gen_p.cpp numbInDegree.cpp prak2.cpp strTo.cpp; ./a.out; rm a.out
#include "numbInDegree.h"
#include "evklidAlg.h"
#include "gen_p.h"
#include "elgamal.h"
#include "contFracAndEquation.h"
#include "strTo.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

ostream& operator<< (ostream& os, vector<auto>& v){
    for(auto i : v){
        os << i << " ";
    }
    return os;
}

enum SmthType{
    EXIT = 0,
    STEXP,
    BINEXP,
    NUMOBR,
    ELGAM,
    DROB
};

int main(){
    int a, b, n, p;
    string userCin; 
    while(1){
        cout << "Выберете задание:" << endl;
        cout << "1 - обычное возведение в степень по модулю" << endl;
        cout << "2 - двоичное возведение в степень по модулю" << endl;
        cout << "3 - алгоритм Евклида (нахождение обратного числа)" << endl;
        cout << "4 - шифр Эль-Гамаля" << endl;
        cout << "5 - цепная дробь и корни уравнения" << endl;
        cout << "0 - выход" << endl;
        try {        
        cin >> userCin;
        if (userCin[0] < 48 || userCin[0] > 53 || userCin[1] != false){
            throw userCin;
        }}
        catch (const string& error){
            cerr << "Ошибка, вы ввели: "<< error << ", а можно вводить 0, 1, 2, 3, 4, 5"<< endl;
            exit(0);
        }

        auto choice = static_cast<SmthType>(static_cast<int>(userCin[0])- '0');
        switch (choice){
                case STEXP:
                    try{
                        cout << "Введите основание степени" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        a = str_to_int(userCin);
                        cout << "Введите показатель степени" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        n = str_to_int(userCin);
                        cout << "Введите модуль" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        p = str_to_int(userCin);
                        cout << "Ответ:" << endl;
                        cout << standart_degre(a, n, p) << endl;
                        break;
                    }catch (const string& error){
                        cerr << "Ошибка, вы ввели: "<< error << ", а можно вводить только положительные числа"<< endl;
                        exit(0);
                    }
                case BINEXP:
                    try{
                        cout << "Введите основание степени" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        a = str_to_int(userCin);
                        cout << "Введите показатель степени" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        n = str_to_int(userCin);
                        cout << "Введите модуль" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        p = str_to_int(userCin);
                        cout << "Ответ:" << endl;
                        cout <<  bin_degre(a, n, p) << endl;
                        break;
                    }
                    catch (const string& error){
                        cerr << "Ошибка, вы ввели: "<< error << ", а можно вводить только положительные числа"<< endl;
                        exit(0);
                    }
                case NUMOBR:
                    try{
                        cout << "Введите число" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        a = str_to_int(userCin);
                        cout << "Введите модуль" << endl;
                        cin >> userCin;
                        if (userCin[0] == '-'){
                            throw userCin;
                        }
                        b = str_to_int(userCin);
                        cout << evklid_algoritm_mul(a, b, 1) << endl;
                        break;
                    }
                    catch (const string& error){
                        cerr << "Ошибка, вы ввели: "<< error << ", а можно вводить только положительные числа"<< endl;
                        exit(0);
                    }
                case ELGAM:
                    cipherElGam();
                    break;
                case DROB:
                    contFracAndDiofant();
                    break;
                default:
                    cout << "Завершение работы" << endl;
                    exit(0);
            
        }
        cout << endl;
    }
    
}