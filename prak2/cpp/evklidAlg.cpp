#include "evklidAlg.h"
#include "strTo.h"
#include <iostream>
#include <iomanip>
using namespace std;

ostream& operator<<(ostream& ss, tuple<int, int, int> tup){
    ss << setw(4) << left << get<0>(tup) << " " << get<1>(tup) << " " << get<2>(tup);
    return ss;
}

int evklid_algoritm_mul(int a, int b, int mode){ //a - число, b - модуль, возвращает взаимно обратное.
    try{
    if (a < 0 || b < 0){
        cout << "Ошибка, числа не должны быть отрицательными" << endl;
        return 0;
    }
    //int u, v;
    tuple<int, int, int> X = {a, 1, 0}, Y = {b, 0 , 1}, T;
    int Q = 0; //  остаток от деления x1  на y1
    int q = a/b; //  целое от деления x1  на y1
    
    string userCin;
    if (mode == 1){
        cout << "Введите 1 для вычисления c*d mod p == 1 (через u и v)" << endl;
        cout << "Введите 2 для вычисления c^(-1) mod p == d" << endl;
        cin >> userCin;
        mode = str_to_int(userCin);
        if (mode < 1 || mode > 2) throw mode;
    }
    if (mode == 1){
        cout << X << endl;
        cout << Y << endl;
    }
    if (mode == 2){
        cout << setw(4) << get<0>(X) << " " << get<2>(X) << endl;
        cout << setw(4) << get<0>(Y) << " " << get<2>(Y) << endl;
    }

    while (get<0>(Y) != 0){
        Q = get<0>(X)%get<0>(Y);
        q = get<0>(X)/get<0>(Y);
        T = {Q, get<1>(X) - q*get<1>(Y), get<2>(X) - q*get<2>(Y)}; 
        X = Y;
        Y = T;
        if (mode == 1){
            cout << T << endl;
        }
        if (mode == 2){
            cout << setw(4) << get<0>(T) << " " << get<2>(T) << endl;
        }
    }

    if (get<0>(X) != 1) {
        cout << "Обратный элемент не существует, так как числа не взаимно просты" << endl;
        return 0;
    } else {
        if (mode == 1){
            //cout << Y << endl;
            cout << "Ответ:" << endl;
        }
        if (mode == 2){
            //cout << get<0>(Y) << " " << get<2>(Y) << endl;
            cout << "Ответ:" << endl;
        }
        if (mode == 3) return get<1>(X); // возвращаем без привода к положительному значению, необходимо для цепной дроби.
        return (get<1>(X) % b + b) % b; //приводим результат к положительному значению
    }
    }catch(const int error){
        cerr << "Ошибка выбора режима работы! " << error << " нет в выборе режима, есть только 1 и 2" << endl;
    }     
}