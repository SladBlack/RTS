#include<iostream>
#include<windows.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

int counter = 0; // Счетчик запуска для worker1_2

MMRESULT BuildTimer(UINT uDelay, UINT uResolution, void *callback_function, UINT fuEvent) {
	// Создание таймера
	return timeSetEvent(uDelay, uResolution, LPTIMECALLBACK(callback_function), 0, fuEvent);
}

ULONGLONG getSystemTime() {
	return GetTickCount64();
}

void DestroyTimer(MMRESULT mm_timer) {
	// Удаление таймера
	timeKillEvent(mm_timer);
}

void worker4() {
	// Вызываем функцию для рандомных чисел
	srand(time(NULL));
	// Получаем рандомное число от 1 до 60
	DWORD i = 1 + rand() % (60 + 1);
	// Получаем системное время
	DWORD j = getSystemTime();
	// Переменная для записи результата
	DWORD result;
	while (getSystemTime() - j < i) {
		// Получаем результат
		result = rand() % (5 + 1);
	}
	cout << "Worker4 finished" << endl;
}

void worker1_2() {
	srand(time(NULL));
	DWORD i = 1 + rand() % (30 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j > DWORD(0)) {
		result = rand() % (5 + 1);
	}
	cout << "Worker1 finished" << endl;
	// Вызываем 4 воркер
	timeSetEvent(4800, 50, LPTIMECALLBACK(worker4), 0, TIME_ONESHOT);
	// Инкерементируем глобальный счетчик
	counter += 1;
}

void worker1() {
	srand(time(NULL));
	DWORD i = 1 + rand() % (30 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	cout << "Worker1 finished" << endl;
}

void worker5() {
	srand(time(NULL));
	DWORD i = 1 + rand() % (70 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	cout << "Worker5 finished" << endl;
}

void worker6() {
	srand(time(NULL));
	DWORD i = 1 + rand() % (80 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	cout << "Worker6 finished" << endl;
}

void worker4_3() {
	srand(time(NULL));
	DWORD i = 1 + rand() % (60 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	cout << "Worker4 finished" << endl;
}

void worker2_3() {
	int n = 0; // Счетчик кол-ва проходов в цикле
	while (n < 5) {
		srand(time(NULL));
		DWORD i = 1 + rand() % (40 + 1);
		DWORD j = getSystemTime();
		DWORD result;
		while (getSystemTime() - j < i) {
			result = rand() % (5 + 1);
			if (result == 0) {
				BuildTimer(5100, 30, worker4_3, TIME_ONESHOT);
			}
			else {
				if (result == 3)
					BuildTimer(4900, 0, worker6, TIME_ONESHOT);
			}
		}
		cout << "Worker2 finished" << endl;
		n++; // Увеличиваем счетчик
	}
}


void worker3() {
	srand(time(NULL));
	DWORD i = 1 + rand() % (50 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j > DWORD(0)) {
		result = rand() % (5 + 1);
	}
	cout << "Worker3 finished" << endl;
}



void task1() {
	BuildTimer(9000, 50, worker1, TIME_ONESHOT);
	BuildTimer(7000, 35,worker5, TIME_ONESHOT);
	BuildTimer(1500, 0, worker6, TIME_ONESHOT);
	// Ждем ввода символа
	getchar();
}

void task2() {
	// Запускаем таймер
	MMRESULT mm_timer = BuildTimer(9600, 40, worker1_2, TIME_PERIODIC);
	while (true) {
		// Если таймер отработал нужное кол-во раз, останавливаем таймер
		if (counter >= 7) {
			DestroyTimer(mm_timer);
			break;
		}
	}
	getchar();
}

void task3() {
	BuildTimer(6200, 25, worker2_3, TIME_ONESHOT);
	getchar();
}

int main() {
	// Русификация
	setlocale(LC_ALL, "Rus");
	// Переменная для выбора таски
	int choice;
	// Меню
	while (true) {
		cout << "Enter task number (1-3, 0 - exit): "; cin >> choice;
		switch (choice)
		{
		case 1:
			task1();
			getchar();
			continue;
		case 2:
			task2();
			getchar();
			continue;
		case 3:
			task3();
			getchar();
			continue;
		case 0:
			return 0;
		default:
			continue;
		}
		cout << "\n\n";
	}
	
	getchar();
	return 0;
}