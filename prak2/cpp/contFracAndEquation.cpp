#include "contFracAndEquation.h"
#include "strTo.h"
#include "evklidAlg.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

ostream& operator<< (ostream& os, vector<auto>& v) {
    for (auto i : v) {
        os << i << " ";
    }
    return os;
}

vector<int> continued_fraction(double x, int max_terms = 50, double tolerance = 1e-8) {
    vector<int> coef;
    
    for (int i = 0; i < max_terms; ++i) {
        int integer_part = static_cast<int>(floor(x));
        coef.push_back(integer_part);
        double fractional_part = x - integer_part;
        if (fractional_part < tolerance) {
            break;
        }
        x = 1.0 / fractional_part;
    }
    
    return coef;
}

void contFracAndDiofant() {
    try{
        string usCin;
        cout << "Введите число для цепной дроби" << endl;
        double contFrac;
        cin >> usCin;
        contFrac = str_to_double(usCin);
        vector<int> coef = continued_fraction(contFrac);
        cout << "Полученные коэффициенты "<< coef << endl << endl;

        //1256a + 847b = 119
        int c = 119;

        int a = 1256, b = 847;
        vector<int> P, Q;
        coef = continued_fraction(static_cast<double>(1256)/847);
        //Ищем подходящие дроби
        P.push_back(1);
        P.push_back(coef[0]);
        Q.push_back(0);
        Q.push_back(1);
        cout << "Подходящие дроби" << endl;
        for (size_t i = 1; i < coef.size(); ++i) {
            int next_P = coef[i] * P[i] + P[i - 1];
            int next_Q = coef[i] * Q[i] + Q[i - 1];
            cout << next_P << " " << next_Q << endl;
            P.push_back(next_P);
            Q.push_back(next_Q);
        }
        int n = coef.size();
        // Получаем P_{n-1} и Q_{n-1}
        int Pn_1 = P[n - 1];
        int Qn_1 = Q[n - 1];
        
        // Находим частное решение
        int x0, y0;
        if (n % 2 == 0) {
            x0 = 119 * Qn_1;
            y0 = -119 * Pn_1;
        } else {
            x0 = -119 * Qn_1;
            y0 = 119 * Pn_1;
        }
        cout << "Частное решение: x = " << x0 << ", y = " << y0 << endl;
        cout << "Общее решение: x = " << x0 << " + " << b << " * t, y = " << y0 << " - " << a << " * t" << endl;
        cout << "Проверка решения" << endl;
        int t;
        while (1){
            cout << "Введите t, для выхода введите EXIT" << endl;
            cin >> usCin;
            if (usCin == "EXIT") break;
            if (count(usCin.begin(), usCin.end(), '.') != 0) throw usCin;
            t = str_to_int(usCin);
            cout << x0 + b * t<< "*1256 - " << abs(y0 - a * t) << "*847 = " << (x0 + b * t)*1256 + (y0 - a * t)*847 << endl;
        }
    }catch(const string& error){
        cout << "Вводить можно только целые числа, а вы ввели " << error << endl;
        return;
    }

}