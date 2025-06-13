package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"strings"
)

func continuedFraction(x float64, maxTerms int, tolerance float64) []int {
	coef := make([]int, 0)

	for i := 0; i < maxTerms; i++ {
		integerPart := int(math.Floor(x))
		coef = append(coef, integerPart)
		fractionalPart := x - float64(integerPart)
		if fractionalPart < tolerance {
			break
		}
		x = 1.0 / fractionalPart
	}

	return coef
}

func printVector(v interface{}) string {
	switch v := v.(type) {
	case []int:
		return strings.Trim(fmt.Sprint(v), "[]")
	case []float64:
		return strings.Trim(fmt.Sprint(v), "[]")
	default:
		return ""
	}
}

func contFracAndDiofant() {
	defer func() {
		if r := recover(); r != nil {
			if err, ok := r.(string); ok {
				fmt.Printf("Вводить можно только целые числа, а вы ввели %s\n", err)
			} else {
				panic(r)
			}
		}
	}()

	reader := bufio.NewReader(os.Stdin)

	fmt.Println("Введите число для цепной дроби")
	usCin, _ := reader.ReadString('\n')
	usCin = strings.TrimSpace(usCin)
	contFrac := strToDouble(usCin)
	coef := continuedFraction(contFrac, 50, 1e-8)
	fmt.Printf("Полученные коэффициенты %v\n\n", printVector(coef))

	// 1256a + 847b = 119
	c := 119

	a := 1256
	b := 847
	P := []int{1, 0}
	Q := []int{0, 1}
	coef = continuedFraction(float64(a)/float64(b), 50, 1e-8)

	// Ищем подходящие дроби
	P[0] = 1
	P[1] = coef[0]
	Q[0] = 0
	Q[1] = 1

	fmt.Println("Подходящие дроби")
	for i := 1; i < len(coef); i++ {
		nextP := coef[i]*P[i] + P[i-1]
		nextQ := coef[i]*Q[i] + Q[i-1]
		fmt.Println(nextP, nextQ)
		P = append(P, nextP)
		Q = append(Q, nextQ)
	}

	n := len(coef)
	// Получаем P_{n-1} и Q_{n-1}
	Pn1 := P[n-1]
	Qn1 := Q[n-1]

	// Находим частное решение
	var x0, y0 int
	if n%2 == 0 {
		x0 = c * Qn1
		y0 = -c * Pn1
	} else {
		x0 = -c * Qn1
		y0 = c * Pn1
	}

	fmt.Printf("Частное решение: x = %d, y = %d\n", x0, y0)
	fmt.Printf("Общее решение: x = %d + %d * t, y = %d - %d * t\n", x0, b, y0, a)
	fmt.Println("Проверка решения")

	for {
		fmt.Println("Введите t, для выхода введите EXIT")
		usCin, _ = reader.ReadString('\n')
		usCin = strings.TrimSpace(usCin)
		if usCin == "EXIT" {
			break
		}
		if strings.Count(usCin, ".") != 0 {
			panic(usCin)
		}
		t := strToInt(usCin)
		x := x0 + b*t
		y := y0 - a*t
		fmt.Printf("%d*1256 - %d*847 = %d\n", x, int(math.Abs(float64(y))), x*1256+y*847)
	}
}
