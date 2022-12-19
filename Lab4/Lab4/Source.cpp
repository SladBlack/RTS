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

ULONGLONG start1;
ULONGLONG start2;
ULONGLONG start3;

void DestroyTimer(MMRESULT mm_timer) {
	// Удаление таймера
	timeKillEvent(mm_timer);
}

void task3();


void worker4() {
	ULONGLONG end = getSystemTime() - start2;
	cout << "[" << end << "] " << "Worker4 started" << endl;
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
}

void worker1_2() {
	ULONGLONG end = getSystemTime() - start1;
	cout << counter << ". [" << end << "] " << "Worker1 started" << endl;
	srand(time(NULL));
	DWORD i = 1 + rand() % (30 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	// Вызываем 4 воркер
	start2 = getSystemTime();
	timeSetEvent(4800, 50, LPTIMECALLBACK(worker4), 0, TIME_ONESHOT);
	// Инкерементируем глобальный счетчик
	counter += 1;
	start1 = getSystemTime();
}

void worker1() {
	ULONGLONG end = getSystemTime() - start1;
	cout << "[" << end << "] " << "Worker1 started" << endl;
	srand(time(NULL));
	DWORD i = 1 + rand() % (30 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	
}

void worker5() {
	ULONGLONG end = getSystemTime() - start2;
	cout << "[" << end << "] " << "Worker5 finished" << endl;
	srand(time(NULL));
	DWORD i = 1 + rand() % (70 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}	
}

void worker6() {
	ULONGLONG end = getSystemTime() - start3;
	cout << "[" << end << "] " << "Worker6 started" << endl;
	srand(time(NULL));
	DWORD i = 1 + rand() % (80 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	task3();
}

void worker4_3() {
	ULONGLONG end = getSystemTime() - start2;
	cout << "[" << end << "] " << "Worker4 started" << endl;
	srand(time(NULL));
	DWORD i = 1 + rand() % (60 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	task3();
}

void worker2_3() {
	counter += 1;
	// Получаем время начала выполнения функции
	ULONGLONG end = getSystemTime() - start1;
	cout << "[" << end << "] " << "Worker2 started\t" << counter << endl;
	srand(time(NULL));
	DWORD i = 1 + rand() % (40 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	if (result == 0) {
		start2 = getSystemTime();
		BuildTimer(5100, 30, worker4_3, TIME_ONESHOT);
	}
	else {
		if (result == 3){
			start3 = getSystemTime();
			BuildTimer(4900, 0, worker6, TIME_ONESHOT);
		}
		else {
			task3();
		}
	}
}


void worker3() {
	srand(time(NULL));
	DWORD i = 1 + rand() % (50 + 1);
	DWORD j = getSystemTime();
	DWORD result;
	while (getSystemTime() - j < i) {
		result = rand() % (5 + 1);
	}
	cout << "Worker3 finished" << endl;
}



void task1() {
	start1 = getSystemTime();
	BuildTimer(9000, 50, worker1, TIME_ONESHOT);
	start2 = getSystemTime();
	BuildTimer(7000, 35,worker5, TIME_ONESHOT);
	start3 = getSystemTime();
	BuildTimer(1500, 0, worker6, TIME_ONESHOT);
	// Ждем ввода символа
	getchar();
}

void task2() {
	// Запускаем таймер
	start1 = getSystemTime();
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
	if (counter >= 5) {
		cout << "Task 3 has finished" << endl;
		return;
	}
	start1 = getSystemTime();
	BuildTimer(6200, 25, worker2_3, TIME_ONESHOT);
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
			task3();
			getchar();
			continue;
		case 2:
			task2();
			getchar();
			continue;
		case 3:
			task3();
			getchar();
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