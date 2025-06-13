package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"unicode/utf8"
)

// Поиск первообразного корня
func search_g(p int) int {
	var fact []int
	phi := p - 1
	n := phi

	for i := 2; i*i <= n; i++ {
		if n%i == 0 {
			fact = append(fact, i)
			for n%i == 0 {
				n /= i
			}
		}
	}
	if n > 1 {
		fact = append(fact, n)
	}

	for res := 2; res <= p; res++ {
		ok := true
		for i := 0; i < len(fact) && ok; i++ {
			if binDegre(res, phi/fact[i], p) == 1 {
				ok = false
			}
		}
		if ok {
			return res
		}
	}
	return -1
}

// преобразование числа в символ utf-8
func numbToChar(num int) string {
	return string(rune(num + 0x20))
}

// преобразование символа utf-8 в число
func charToNumb(utf8str string) int {
	r, _ := utf8.DecodeRuneInString(utf8str)
	return int(r) - 0x20
}

// Шифрование
func elgamal_encription() int {
	sourceFile, err := os.Open("sourse_text.txt")
	if err != nil {
		fmt.Println("Error opening source file:", err)
		return -1
	}
	defer sourceFile.Close()

	p := gen_p(15, 8, 8)
	randomNum := rand.Intn(p-2) + 2

	g := search_g(p)
	x := randomNum
	y := binDegre(g, x, p)
	k := randomNum

	cipherFile, err := os.Create("cipher.txt")
	if err != nil {
		fmt.Println("Error creating cipher file:", err)
		return -1
	}
	defer cipherFile.Close()

	cipherFile.WriteString(numbToChar(y) + " " + numbToChar(g) + " " + numbToChar(p) + " ")

	a := binDegre(g, k, p)
	scanner := bufio.NewScanner(sourceFile) // Создаём сканер для чтения данных из файла
	scanner.Split(bufio.ScanRunes)          // Раразбиваем поток байтов на отдельные юникод символы

	for scanner.Scan() {
		char := scanner.Text()
		r, _ := utf8.DecodeRuneInString(char)
		M := int(r)
		b := binDegre(y, k, p) * M % p

		cipherFile.WriteString(numbToChar(a) + " " + numbToChar(b) + " ")
	}

	fmt.Println("Файл успешно зашифрован")
	return x
}

// Дешифрование
func elgamal_decription(x int) {
	cipherFile, _ := os.Open("cipher.txt")
	defer cipherFile.Close() //закрываем файл после выполнение функции с помощью ключевого слова defer

	scanner := bufio.NewScanner(cipherFile)
	scanner.Split(bufio.ScanWords)

	//считываем публичный ключ
	scanner.Scan()
	scanner.Scan()
	scanner.Scan()
	p_str := scanner.Text()
	p := charToNumb(p_str)

	completeFile, _ := os.Create("compliteText.txt")
	defer completeFile.Close() //закрываем файл после выполнение функции с помощью ключевого слова defer
	for scanner.Scan() {
		a_str := scanner.Text()
		scanner.Scan()
		b_str := scanner.Text()

		a := charToNumb(a_str)
		b := charToNumb(b_str)

		a_x := binDegre(a, x, p)
		M := evklidAlgoritmMul(a_x, p, 0) * b % p
		completeFile.WriteString(string(rune(M)))
	}

	fmt.Println("Файл успешно расшифрован")
}

// Эмуляция атаки методом брутфорс
func brutforse() {
	fmt.Println("Выберите предполагаемый язык текста: 1 - русский, 2 - английский")
	var language int
	_, err := fmt.Scan(&language)
	if err != nil || language < 1 || language > 2 {
		fmt.Println("Ошибка выбора языка!")
		return
	}

	cipherFile, _ := os.Open("cipher.txt")
	defer cipherFile.Close()

	scanner := bufio.NewScanner(cipherFile)
	scanner.Split(bufio.ScanWords)

	scanner.Scan()
	scanner.Scan()
	scanner.Scan()
	p_str := scanner.Text()
	p := charToNumb(p_str)

	for x := 1; x < 10000; x++ {
		cipherFile.Seek(0, 0) // Сбрасываем позицию сканера (ставим на начало файла)
		scanner = bufio.NewScanner(cipherFile)
		scanner.Split(bufio.ScanWords)

		// пропуск публичного ключа
		for i := 0; i < 3; i++ {
			scanner.Scan()
		}

		success := true
		completeFile, _ := os.Create("compliteText.txt")

		for scanner.Scan() {
			a_str := scanner.Text()
			scanner.Scan()
			b_str := scanner.Text()

			a := charToNumb(a_str)
			b := charToNumb(b_str)

			a_x := binDegre(a, x, p)
			M := (evklidAlgoritmMul(a_x, p, 0) * b) % p
			wc := rune(M)

			if language == 2 { // Английский
				if !(wc == '\n' || (wc >= ' ' && wc <= '~')) {
					success = false
					break
				}
			} else { // Русский
				if !(wc == '\n' || (wc >= 'А' && wc <= 'я') || wc == 'Ё' || wc == 'ё' ||
					(wc >= ' ' && wc <= '`') || (wc >= '{' && wc <= '~')) {
					success = false
					break
				}
			}
			completeFile.WriteString(string(wc))
		}

		completeFile.Close()

		if success {
			fmt.Printf("Успех! Закрытый ключ x = %d\n", x)
			return
		}
	}

	fmt.Println("Взлом не удался.")
}

type choiceType int

const (
	DECR choiceType = iota + 1
	BRUT
)

// Основная функция программы
func cipherElGam() {
	x := elgamal_encription()
	if x == -1 {
		return
	}

	fmt.Println("Введите 1 для дешифрования, 2 для эмуляции атаки")
	var usCin string
	_, err := fmt.Scan(&usCin)
	if err != nil || len(usCin) != 1 || usCin[0] < '1' || usCin[0] > '2' {
		fmt.Printf("Ошибка, вы ввели: %s, а можно вводить только 1 или 2\n", usCin)
		os.Exit(0)
	}

	choice, _ := strconv.Atoi(usCin)
	switch choiceType(choice) {
	case DECR:
		elgamal_decription(x)
	case BRUT:
		brutforse()
	default:
		break
	}
}
