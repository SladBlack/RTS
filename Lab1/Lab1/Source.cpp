#include <windows.h>
#include <iostream>

using namespace std;

int* createArray(int size) {
	return new int[size];
}

void fillArray(int* array, int size) {
	for (int i = 0; i < size; i++)
		cin >> array[i];
}

void printArray(int* array, int size) {
	for (int i = 0; i < size; i++)
		cout << array[i] << endl;
}

auto findMinMax(int* array, int size) {
	struct result { int max; int min; };
	int min = array[0]; int max = array[0];
	for (int i = 1; i < size; i++) {
		if (array[i] < min)
			min = array[i];
		Sleep(7);
		if (array[i] > max)
			max = array[i];
		Sleep(7);
	}

	return result{ min, max };
}

int findGreater(int* array, int size, double mean) {
	int counter = 0;
	for (int i = 0; i < size; i++) {
		if (array[i] > mean)
			counter++;
	}
	return counter;
}

double findMean(int* array, int size) {
	double sum = 0;
	for (int i = 0; i < size; i++) {
		sum += array[i];
		Sleep(12);
	}
	double mean = sum / size;
	return mean;
}

struct Data {
	int size;
	int* array;
};

DWORD WINAPI Worker(LPVOID threadData) {
	struct Data* tData = (struct Data*)threadData;
	double mean = findMean(tData->array, tData->size);
	cout << "Mean: " << mean << endl;
	return mean;
}

DWORD WINAPI Main(LPVOID threadData) {
	// 1. Создание массива целых чисел
	int size;
	cout << "Enter size of array: "; cin >> size;
	int* array = createArray(size);
	cout << "Enter elements of array: ";
	fillArray(array, size);

	// 2. Создание потока worker
	HANDLE hThread;
	DWORD IDThread;

	struct Data* tData = (struct Data*)malloc(sizeof(struct Data));
	tData->size = size; tData->array = array;

	hThread = CreateThread(NULL, 0, Worker, tData, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	// 3. Поиск минимального и максимального элементов
	auto result = findMinMax(array, size);
	cout << "Min: " << result.min << " | " << "Max: " << result.max << endl;

	// 4. Ждем пока поток Worker закончит работу
	WaitForSingleObject(hThread, INFINITE);
	DWORD mean;
	GetExitCodeThread(hThread, &mean);
	CloseHandle(hThread);

	// 5. Подсчитываем количество  элементов в массиве, значение
	// которых больше среднего значения элементов массива
	int counter = findGreater(array, size, mean);
	cout << "Count of elements greater than mean: " << counter << endl;

	delete[] array;
	return 1;
}

int main()
{
	HANDLE mainThread;
	DWORD mainIDThread;
	mainThread = CreateThread(NULL, 0, Main, NULL, 0, &mainIDThread);
	if (mainThread == NULL)
		return GetLastError();

	WaitForSingleObject(mainThread, INFINITE);
	CloseHandle(mainThread);
	return 0;
}