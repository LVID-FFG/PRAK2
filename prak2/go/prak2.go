package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type SmthType int

const (
	EXIT SmthType = iota
	STEXP
	BINEXP
	NUMOBR
	ELGAM
	DROB
)

func main() {
	var a, b, n, p int
	reader := bufio.NewReader(os.Stdin)

	for {
		fmt.Println("Выберете задание:")
		fmt.Println("1 - обычное возведение в степень по модулю")
		fmt.Println("2 - двоичное возведение в степень по модулю")
		fmt.Println("3 - алгоритм Евклида (нахождение обратного числа)")
		fmt.Println("4 - шифр Эль-Гамаля")
		fmt.Println("5 - цепная дробь и корни уравнения")
		fmt.Println("0 - выход")

		userCin, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Ошибка ввода:", err)
			os.Exit(1)
		}
		userCin = strings.TrimSpace(userCin)

		if len(userCin) != 1 || userCin[0] < '0' || userCin[0] > '5' {
			fmt.Printf("Ошибка, вы ввели: %s, а можно вводить 0, 1, 2, 3, 4, 5\n", userCin)
			os.Exit(0)
		}

		choice := SmthType(userCin[0] - '0')
		switch choice {
		case STEXP:
			fmt.Println("Введите основание степени")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			a = strToInt(userCin)

			fmt.Println("Введите показатель степени")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			n = strToInt(userCin)

			fmt.Println("Введите модуль")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			p = strToInt(userCin)

			fmt.Println("Ответ:")
			fmt.Println(standartDegre(a, n, p))

		case BINEXP:
			fmt.Println("Введите основание степени")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			a = strToInt(userCin)

			fmt.Println("Введите показатель степени")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			n = strToInt(userCin)

			fmt.Println("Введите модуль")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			p = strToInt(userCin)

			fmt.Println("Ответ:")
			fmt.Println(binDegre(a, n, p))

		case NUMOBR:
			fmt.Println("Введите число")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			a = strToInt(userCin)

			fmt.Println("Введите модуль")
			userCin, _ = reader.ReadString('\n')
			userCin = strings.TrimSpace(userCin)
			if userCin[0] == '-' {
				fmt.Println("Ошибка, вы ввели отрицательное число, а можно вводить только положительные числа")
				os.Exit(0)
			}
			b = strToInt(userCin)
			fmt.Println("Ответ:")
			fmt.Println(evklidAlgoritmMul(a, b, 1))

		case ELGAM:
			cipherElGam()
		case DROB:
			contFracAndDiofant()
		default:
			fmt.Println("Завершение работы")
			os.Exit(0)
		}
		fmt.Println()
	}
}
