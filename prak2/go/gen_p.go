package main

import (
	"math"
	"math/big"
	"math/rand"
	"time"
)

// Функция для вычисления длины числа в битах
func lenInBit(a uint64) int {
	result := 0
	for a != 0 {
		result++
		a >>= 1
	}
	return result
}

// Решето Эратосфена для генерации простых чисел
func reshetoEratosfena(n int) []uint64 {
	if n < 2 {
		return []uint64{}
	}

	// Инициализация: все числа считаются простыми
	isPrime := make([]bool, n)
	for i := 2; i < n; i++ {
		isPrime[i] = true
	}

	// Просеивание
	for p := 2; p*p < n; p++ {
		if isPrime[p] {
			for i := p * p; i < n; i += p {
				isPrime[i] = false
			}
		}
	}

	// Собираем результат, начиная с 2
	result := make([]uint64, 0)
	for i := 2; i < n; i++ {
		if isPrime[i] {
			result = append(result, uint64(i))
		}
	}
	return result
}

// Генерация кандидата N
func generateN(k int, control []uint64) (uint64, []uint64) {
	rand.Seed(time.Now().UnixNano())
	m := uint64(2)
	Q := []uint64{2}

	for {
		a := uint64(rand.Intn(50) + 1)
		q := control[rand.Intn(len(control))]

		// Проверяем, есть ли уже такой простой делитель
		found := false
		for _, val := range Q {
			if val == q {
				found = true
				break
			}
		}

		if !found {
			// Вычисляем s = m * q^a
			s := m * uint64(math.Pow(float64(q), float64(a)))
			lowerBound := uint64(math.Pow(2, float64(k-1)))
			upperBound := uint64(math.Pow(2, float64(k)))

			if lowerBound <= s && s <= upperBound {
				m = s
				Q = append(Q, q)
				break
			} else if lenInBit(s) < k {
				m = s
				Q = append(Q, q)
			} else {
				// Начинаем заново
				m = 2
				Q = []uint64{2}
			}
		} else if len(Q) > 45 {
			// Начинаем заново
			m = 2
			Q = []uint64{2}
		}
	}

	n := m + 1
	return n, Q
}

// Тест Миллера-Рабина
func testMillera(n uint64, Q []uint64, t int) int {
	rand.Seed(time.Now().UnixNano())
	tList := make([]uint64, 0, t)

	// Генерируем t уникальных случайных чисел
	for len(tList) < t {
		c := uint64(rand.Intn(100) + 1)
		found := false
		for _, val := range tList {
			if val == c {
				found = true
				break
			}
		}
		if !found {
			tList = append(tList, c)
		}
	}

	// Проверяем первое условие
	for _, a := range tList {
		bigA := big.NewInt(int64(a))
		bigN := big.NewInt(int64(n))
		bigNMinus1 := big.NewInt(int64(n - 1))

		if bigA.Exp(bigA, bigNMinus1, bigN).Cmp(big.NewInt(1)) != 0 {
			return 0 // Составное
		}
	}

	// Проверяем второе условие
	successCount := 0
	for _, q := range Q {
		allPassed := true
		exp := (n - 1) / q
		bigExp := big.NewInt(int64(exp))

		for _, a := range tList {
			bigA := big.NewInt(int64(a))
			bigN := big.NewInt(int64(n))

			if bigA.Exp(bigA, bigExp, bigN).Cmp(big.NewInt(1)) == 0 {
				continue
			} else {
				allPassed = false
				break
			}
		}
		if allPassed {
			successCount++
		}
	}

	if successCount < len(Q) {
		return 1 // Вероятно простое
	}
	return 2 // Простое
}

// Генерация простого числа
func gen_p(k, t, s int) int {
	control := reshetoEratosfena(500)

	for 1 == 1 {
		n, Q := generateN(k, control)
		result := testMillera(n, Q, t)
		if result != 0 {
			return int(n)
		}
	}
	return 0
}
