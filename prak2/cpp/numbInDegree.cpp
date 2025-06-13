#include "numbInDegree.h"
#include "gen_p.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

bool ferma_right(int a, int p){
    if (a%p == 0) return false;
    
    if (test_millera_rabina(p, 2) == "составное") return false;
    return true;
}

int standart_degre(int a, int n, int p){ // обычное возведение в степень
    if (a < 0 || n <0 || p < 0){
        cout << "Ошибка, числа не должны быть отрицательными" << endl;
        return 0;
    }
    if (ferma_right(a, p) == false){//проверяем условие теоремы ферма
        cout << "Нарушение теоремы Ферма" << endl;
        return 0;
    }
    int degree = n%(p-1); //находим сколько степеней остаётся (остальные по теореме ферма дадут 1)
    int result = 1;
    for (int i = 0; i < degree; i++){ //пошагово находим остаток
        result *= a;
        result %= p;
    }
    return result;
}

int bin_degre(int a, int n, int p){ //двоичное возведение в степень
    if (a < 0 || n < 0 || p < 0){
        cout << "Ошибка, числа не должны быть отрицательными" << endl;
        return 0;
    } 
    vector<int> n_bin;
    while(n != 0){ //находим двоичное представление степени
        n_bin.push_back(n%2);
        n /= 2;
        
    }
    int result = 1;
    for (int i : n_bin){ //бинарное возведение в степень по модулю
        if (i == 1) result = result * a % p;
        a = a*a%p;
    }
    return result;
}