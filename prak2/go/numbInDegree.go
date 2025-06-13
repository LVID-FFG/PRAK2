package main

import (
	"fmt"
	"math"
)

// Проверка условий теоремы Ферма
func fermaRight(a, p int) bool {
	if a%p == 0 {
		return false
	}
	for i := 2; i < int(math.Sqrt(float64(p)))+1; i++ {
		if p%i == 0 {
			return false
		}
	}
	return true
}

// Обычное возведение в степень по модулю
func standartDegre(a, n, p int) int {
	if a < 0 || n < 0 || p < 0 {
		fmt.Println("Ошибка, числа не должны быть отрицательными")
		return 0
	}
	if !fermaRight(a, p) {
		fmt.Println("Нарушение теоремы Ферма")
		return 0
	}

	degree := n % (p - 1) // Находим сколько степеней остаётся
	result := 1
	for i := 0; i < degree; i++ {
		result *= a
		result %= p
	}
	return result
}

// Бинарное возведение в степень по модулю
func binDegre(a, n, p int) int {
	if a < 0 || n < 0 || p < 0 {
		fmt.Println("Ошибка, числа не должны быть отрицательными")
		return 0
	}

	// Получаем двоичное представление степени
	nBin := make([]int, 0)
	for n != 0 {
		nBin = append(nBin, n%2)
		n /= 2
	}

	result := 1
	for _, bit := range nBin {
		if bit == 1 {
			result = result * a % p
		}
		a = a * a % p
	}
	return result
}
