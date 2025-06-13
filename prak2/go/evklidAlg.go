package main

import (
	"fmt"
	"strconv"
)

type Tuple struct {
	First, Second, Third int
}

func (t Tuple) String() string {
	return fmt.Sprintf("%-4d %d %d", t.First, t.Second, t.Third)
}

func evklidAlgoritmMul(a, b, mode int) int {
	/*defer func() {
		if r := recover(); r != nil {
			fmt.Println("Ошибка:", r)
		}
	}()*/

	if a < 0 || b < 0 {
		fmt.Println("Ошибка, числа не должны быть отрицательными")
		return 0
	}

	X := Tuple{a, 1, 0}
	Y := Tuple{b, 0, 1}
	var T Tuple
	Q := 0
	q := a / b

	if mode == 1 {
		fmt.Println("Введите 1 для вычисления c*d mod p == 1 (через u и v)")
		fmt.Println("Введите 2 для вычисления c^(-1) mod p == d")
		var userCin string
		fmt.Scan(&userCin)
		mode, err := strconv.Atoi(userCin)
		if err != nil || mode < 1 || mode > 2 {
			panic(fmt.Sprintf("Ошибка выбора режима работы! %s нет в выборе режима, есть только 1 и 2", userCin))
		}
	}

	if mode == 1 {
		fmt.Println(X)
		fmt.Println(Y)
	}
	if mode == 2 {
		fmt.Printf("%-4d %d\n", X.First, X.Third)
		fmt.Printf("%-4d %d\n", Y.First, Y.Third)
	}

	for Y.First != 0 {
		Q = X.First % Y.First
		q = X.First / Y.First
		T = Tuple{Q, X.Second - q*Y.Second, X.Third - q*Y.Third}
		X = Y
		Y = T

		if mode == 1 {
			fmt.Println(T)
		}
		if mode == 2 {
			fmt.Printf("%-4d %d\n", T.First, T.Third)
		}
	}

	if X.First != 1 {
		fmt.Println("Обратный элемент не существует, так как числа не взаимно просты")
		return 0
	}

	if mode == 1 || mode == 2 {
		fmt.Println("Ответ:")
	}
	if mode == 3 {
		return X.Second
	}
	return (X.Second%b + b) % b
}
