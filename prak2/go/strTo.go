package main

import (
	"fmt"
	"math"
	"os"
	"unicode"
)

// Преобразование строки в целое число
func strToInt(str string) int {
	result := 0
	isNegative := false
	startIndex := 0

	// Проверяем отрицательное ли число
	if len(str) > 0 && str[0] == '-' {
		isNegative = true
		startIndex = 1
	}

	// Обрабатываем каждый символ
	for i := startIndex; i < len(str); i++ {
		c := rune(str[i])
		if !unicode.IsDigit(c) {
			fmt.Printf("Ошибка, вы ввели: %s, а строка должна содержать только число\n", str)
			os.Exit(0)
		}
		result = result*10 + int(c-'0')
	}

	if isNegative {
		result *= -1
	}

	return result
}

// Преобразование строки в число с плавающей точкой
func strToDouble(str string) float64 {
	result := 0.0
	decimalPlace := 0
	hasDecimal := false

	for _, c := range str {
		if c == '.' {
			if hasDecimal {
				fmt.Printf("Ошибка, вы ввели: %s, а строка должна содержать только положительное дробное число\n", str)
				os.Exit(0)
			}
			hasDecimal = true
			continue
		}

		if !unicode.IsDigit(c) {
			fmt.Printf("Ошибка, вы ввели: %s, а строка должна содержать только положительное дробное число\n", str)
			os.Exit(0)
		}

		if hasDecimal {
			decimalPlace++
			result += float64(c-'0') / math.Pow10(decimalPlace)
		} else {
			result = result*10 + float64(c-'0')
		}
	}

	return result
}
