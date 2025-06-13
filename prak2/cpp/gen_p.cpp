#include "gen_p.h"
#include "numbInDegree.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <random>

using namespace std;

int len_in_bit(auto a){ //функция для вычисления длины числа в битах
    int result = 0;
    while (a != 0){
        result++;
        a /= 2;
    }
    return result;
}

vector<int> resheto_eratosfena(int n = 500){//генерация простых чисел методом решета эратосфена
    vector<int> result = {1};

    for (int i = 2; i < n; i++){ //заполняем вектор числами от 1 до 499
        result.push_back(i);
    }
    for (int i = 0, x = 2; x < 500; x++, i++){
        for(size_t j = i; j < size(result); j++){
            if(result[j] % x == 0 && result[j] != x){
                result[j] = 0;
            }
        }
    }
    result.erase(remove(result.begin(), result.end(), 0), result.end());
    //remove(result.begin(), result.end(), 0) - перемещаем 0 в конец вектора, результат итератор указывающий на 1-й 0.
    //erase(remove(result.begin(), result.end(), 0), result.end()) (удаляет 0 из конца вектора)
    return result;
}

tuple<unsigned long long, vector<int>, vector<int>> generate_N(int k, const vector<int>& control){ //k - кол-во множителей, control - вектор простых чисел
    random_device rd; //подключаем генериратор случайных чисел в диапазоне до 50
    mt19937 gen(rd());
    uniform_int_distribution<> random_num(1, 50);
    unsigned long long m = 2;
    vector<int> Q = {2}; // каноническое разложение числа n
    vector<int> A = {1}; //степени канонического разложения
    int a = 0, q = 0;
    unsigned long long s = m;
    while (1){ //len_in_bit(m) < k-1
        a = random_num(gen);
        q = control[random_num(gen)];
        if (count(Q.begin(), Q.end(), q) == 0){ //считем кол-во вхождений
            s *= pow(q, a); //кандидат на множитель
            if (pow(2, k-1) <= s && s <= pow(2, k)) { //если число попало в нужный диапазон
                m = s; //возводим в степень
                Q.push_back(q);
                A.push_back(a);
                break;
            }
            else{
                if (len_in_bit(s) < k){ //если число меньше нужного
                    m = s; //возводим в степень
                    Q.push_back(q);
                    A.push_back(a);
                }
                else { //если число больше нужного начинаем с начала
                    s = 2;
                    m = 2;
                    Q.clear();
                    A.clear();
                    A.push_back(1);
                    Q.push_back(2);
                };
            }    
        }
        else {
            if (size(Q) > 45){ //если простых чисел не хватает для построения начинаем с начала
                s = m = 2;
                Q.clear();
                A.clear();
                A.push_back(1);
                Q.push_back(2);
            }
        }
    }
    unsigned long long n = m + 1;
    tuple<unsigned long long, vector<int>, vector<int>> result = {n, Q, A};
    return result;
}

string test_millera_rabina(unsigned long long n, int k){ //n - число на проверку, k - число раундов, вероятность ошибки = 4^(-k)  (k = 2, вероятность = 0.0625)
    int s = 1;
    unsigned long long t = n - 1;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> random_num(2, 999999);
    if (len_in_bit(n) < 32) {
        random_num.param(uniform_int_distribution<>::param_type(2, n-2));
    }
    for (;t%2 == 0;){ //ищем  s такое что 2^(s)*t = n
        s++;
        t /= 2;
    }
    unsigned long long x = 0;
    for (int i = 0; i < k; i++){
        
        x = bin_degre(random_num(gen), t, n);
        if (x == 1 || x == n-1) continue;

        bool composite = true;
        for (unsigned long long j = 0; j < s-1; j++) {
            x = bin_degre(x, 2, n);
            if (x == 1) return "составное";
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) return "составное";
    }
    return "вероятно простое";
}

int test_millera(tuple<unsigned long long, vector<int>, vector<int>>& num, int t){ // 0 - составное, 1 - вероятно простое, 2 - простое
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> random_num(1, 100);
    vector<int> t_list;
    for (int i = 0, c = 0; i < t;) { //заполняем t_list разными случайными числами
        c = random_num(gen);
        if (count(t_list.begin(), t_list.end(), c) == 0){ //считем кол-во вхождений
            i++;
            t_list.push_back(c);
        }
    }

    for (int i : t_list){
        if(bin_degre(i, get<0>(num)-1, get<0>(num)) != 1){
            return 0;
        }
    }

    vector<int> q_list = get<1>(num);
    int c = 0; // счётчик успехов
    for (int q : q_list){
        for (int i : t_list){
            if (bin_degre(i, (get<0>(num)-1)/q, get<0>(num)) == 1) {
                c++;
            }
            else {
                break;
            }
        }
    }
    if (c < size(q_list)){
        return 1;
    } 
    else {
        return 2;
    }
}


int gen_p(int k, int t, int s){ //k - длина числа в битах, t - параметр надёжности теста Миллера, s - количество раундов теста Миллера-Рабина
    vector<int> control = resheto_eratosfena();
    tuple<unsigned long long, vector<int>, vector<int>> N;
    while (1){      //заполняем список числами N
        N = generate_N(k, control);
        if (test_millera(N, t) != 0) return get<0>(N);
    }
}