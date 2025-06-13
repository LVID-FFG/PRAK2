#include "numbInDegree.h"
#include "evklidAlg.h"
#include "gen_p.h"
#include "elgamal.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <clocale>
#include <codecvt>
#include <locale>

using namespace std;

// Устанавливаем локаль для работы с UTF-8
void setUTF8Locale() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
}

int search_g(int p) {
    vector<int> fact;
    int phi = p - 1, n = phi;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            fact.push_back(i);
            while (n % i == 0)
                n /= i;
        }
    }
    if (n > 1) fact.push_back(n);
    for (int res = 2; res <= p; ++res) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i) 
            ok &= bin_degre(res, phi / fact[i], p) != 1;
        if (ok) return res;
    }
    return -1;
}

string numbToChar(int num) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> converter; //Инициализация конвертера.
    // Преобразуем число в один Unicode-символ
    char32_t unicode_char = static_cast<char32_t>(num + 0x20); // Сдвиг на 0x20
    return converter.to_bytes(&unicode_char, &unicode_char + 1); //возвращаем символ конвернтированный в utf-8. &unicode_char, &unicode_char + 1 указывают на начало и конец "строки" символа в utf-8 т.е. на 1 символ в этой кодировке 
}

int charToNumb(const string& utf8str) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
    // Конвертируем строку UTF-8 в UTF-32 (ожидаем ровно один символ)
    u32string utf32 = converter.from_bytes(utf8str); //Конвернтируем в  utf-32 u32string
    return static_cast<int>(utf32[0]) - 0x20; // Обратный сдвиг
}

int elgamal_encription() {
    setUTF8Locale();
    wifstream source_text("sourse_text.txt"); //используем широкие потоки для работы с utf-8
    int p = gen_p(15, 8, 8); //генерируем 15 битный ключ
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> random_num(2, p - 1);

    int g = search_g(p);
    int x = random_num(gen);
    int y = bin_degre(g, x, p);
    int k = random_num(gen);

    ofstream cipherText("cipher.txt");

    // Записываем публичный ключ
    cipherText << numbToChar(y) << " " << numbToChar(g) << " " << numbToChar(p) << " ";

    // Шифруем текст
    int a = bin_degre(g, k, p);
    wchar_t wc;
    source_text >> noskipws; //отключаем пропуск пробелов и символов конца строк
    while (source_text >> wc) {
        int M = static_cast<int>(wc);
        int b = bin_degre(y, k, p) * M % p;
        
        // Записываем пару (a, b)
        cipherText << numbToChar(a) << " " << numbToChar(b) << " ";
    }

    source_text.close();
    cipherText.close();
    cout << "Файл успешно зашифрован" << endl;
    return x;
}

void elgamal_decription(int x) {
    setUTF8Locale();
    
    ifstream cipherText("cipher.txt");

    tuple<int, int, int> pub_key;
    string y_str, g_str, p_str;
    cipherText >> y_str >> g_str >> p_str;

    get<0>(pub_key) = charToNumb(y_str);
    get<1>(pub_key) = charToNumb(g_str);
    get<2>(pub_key) = charToNumb(p_str);

    wofstream completeText("compliteText.txt");
    completeText.imbue(std::locale("ru_RU.UTF-8"));

    string a_str, b_str;
    while (cipherText >> a_str >> b_str) {
        int a = charToNumb(a_str);
        int b = charToNumb(b_str);

        int a_x = bin_degre(a, x, get<2>(pub_key));
        int M = evklid_algoritm_mul(a_x, get<2>(pub_key)) * b % get<2>(pub_key);
        completeText << static_cast<wchar_t>(M);
    }

    cipherText.close();
    completeText.close();
    cout << "Файл успешно расшифрован" << endl;
}

void brutforse() {
    cout << "Выберите предполагаемый язык текста: 1 - русский, 2 - английский" << endl;
    int language;
    cin >> language;
    if (cin.fail() || language < 1 || language > 2) {
        cerr << "Ошибка выбора языка!" << endl;
        return;
    }

    setUTF8Locale();

    ifstream cipherText("cipher.txt");

    tuple<int, int, int> pub_key;
    string y_str, g_str, p_str;
    cipherText >> y_str >> g_str >> p_str;

    get<0>(pub_key) = charToNumb(y_str);
    get<1>(pub_key) = charToNumb(g_str);
    get<2>(pub_key) = charToNumb(p_str);

    for (int x = 1; x < 10000; x++) {
        cipherText.clear();
        cipherText.seekg(0);
        
        // Пропускаем публичный ключ
        string tmp;
        for (int i = 0; i < 3; i++) cipherText >> tmp;

        bool success = true;
        wofstream completeText("compliteText.txt");

        string a_str, b_str;
        while (cipherText >> a_str >> b_str) {
            int a = charToNumb(a_str);
            int b = charToNumb(b_str);

            int a_x = bin_degre(a, x, get<2>(pub_key));
            int M = (evklid_algoritm_mul(a_x, get<2>(pub_key)) * b) % get<2>(pub_key);
            wchar_t wc = static_cast<wchar_t>(M);

            // Проверка на валидность символа
            if (language == 2) { // Английский
                if (!(wc == L'\n' || (wc >= L' ' && wc <= L'~'))) {
                    success = false;
                    break;
                }
            } else { // Русский
                if (!(wc == L'\n' || (wc >= L'А' && wc <= L'я') || wc == L'Ё' || wc == L'ё' || 
                     (wc >= L' ' && wc <= L'`') || (wc >= L'{' && wc <= L'~'))) {
                    success = false;
                    break;
                }
            }
            completeText << wc;
        }

        if (success) {
            cout << "Успех! Закрытый ключ x = " << x << endl;
            cout << "Расшифрованный текст сохранен в compliteText.txt" << endl;
            completeText.close();
            cipherText.close();
            return;
        }
        completeText.close();
    }

    cipherText.close();
    cout << "Взлом не удался." << endl;
}

enum choiceType {
    DECR = 1,
    BRUT
};

void cipherElGam() {
    int x = elgamal_encription();
    if (x == -1) return;

    cout << "Введите 1 для дешифрования, 2 для эмуляции атаки" << endl;
    string usCin;
    try {
        cin >> usCin;
        if (usCin[0] < '1' || usCin[0] > '2' || usCin.size() != 1) {
            throw usCin;
        }
    }
    catch (const string& error) {
        cerr << "Ошибка, вы ввели: " << error << ", а можно вводить только 1 или 2" << endl;
        exit(0);
    }

    auto choice = static_cast<choiceType>(usCin[0] - '0');
    switch (choice) {
        case DECR:
            elgamal_decription(x);
            break;
        case BRUT:
            brutforse();
            break;
        default:
            break;
    }
}
