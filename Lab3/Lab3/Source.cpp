#include <windows.h>
#include <iostream>

using namespace std;

void printArray(int* array, int size) {
    // Вывести массив
    for (int i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
}

int* createArray(int size) {
    // Создать динамический массив
    int* arr = new int[size];
    for (int i = 0; i < size; i++)
        cin >> arr[i];

    return arr;
}

int sumArray(int* array, int size, int k) {
    // Просуммировать элементы позиции до заданной k-й позиции
    int sum = 0;
    // Проверка размера массива
    if (k > size) {
        cout << "Your number greater than size of array";
        return sum;
    }
    // Суммирование элементов до заданной позиции
    for (int i = 0; i < k; i++)
        sum += array[i];

    return sum;
}

void sortArray(int* array, int size, HANDLE hSemaphore, int timeout) {
    // Отсортировать массив
    // Проверка массива на количество элементов
    if (size < 2) { return; }
    int temp1, temp2; // Переменные для перестановки 
    int index1 = 0, index2 = 1; // Переменные для слежения за индексами
    bool swap = false; // Флаг, указывающий, была ли перестановка
    int i; // Переменная для цикла
    for (i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (array[i] == array[j]) {
                // Записываем текущие элементы во временную переменную
                temp1 = array[i];
                temp2 = array[j];
                // Переставляем элементы
                array[i] = array[index1];
                array[j] = array[index2];

                array[index1] = temp1;
                array[index2] = temp2;
                if (index2 < size + 1) {
                    index1++;
                    index2++;
                    swap = true;
                }
                // Отмечаем, что элемент готов
                ReleaseSemaphore(hSemaphore, 1, NULL);
                // Спим заданное время
                Sleep(timeout); 
            }
        }
        if (swap) {
            // Если была перестановка, увеличиваем индексы и меняем флаг
            i = index1;
            index1++;
            index2++;
            swap = false;
        }
        // Отмечаем, что элемент готов
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }
}



struct Data {
    // Структура, которая хранит размер массива и сам массив
    int size;
    int* array;
    HANDLE hSemaphore;
};

struct SumData {
    // Структура для потока SumElement
    int size;
    int* array;
    HANDLE hSemaphore;
    int k;
};

DWORD WINAPI Work(LPVOID threadData) {
    // Поток work
    int timeout;
    cout << "Input timeout: "; cin >> timeout;
    // Записываем данные, переданные потоку в структуру
    struct Data* tData = (struct Data*)threadData;
    int* array = tData->array;
    int size = tData->size;
    // Создаем семафор
    HANDLE hSemaphore = tData->hSemaphore;
    // Сортируем массив
    sortArray(array, size, hSemaphore, timeout);
    return 1;
}

DWORD WINAPI SumElement(LPVOID threadData) {
    // Записываем данные, переданные потоку в структуру
    struct SumData* bData = (struct SumData*)threadData;
    int* array = bData->array; int size = bData->size;
    int k = bData->k;
    HANDLE bSemaphore = bData->hSemaphore;
    // Ожидаем сигнала семафора
    WaitForSingleObject(bSemaphore, INFINITE);
    // Находим сумму массива до заданной позиции
    int res = sumArray(array, size, k);
    cout << "\nResult sum of arary: " << res << endl;
    return 1;
}

int main() {
    int size, k; // size - размер массива, k - номер элемента, до которого считать сумму
    int* array; // указатель на массив целых чисел

    cout << "Size of array: "; cin >> size;
    cout << "Input elements of array: ";
    // Создаем массив
    array = createArray(size);
    // Выводим
    cout << "\nSize of array = " << size << endl;
    cout << "Elements of array: ";
    printArray(array, size);
    // Запрашиваем номер элемента, до которого считать сумму
    cout << "Input k: "; cin >> k;
    // Создаем считающий семафор
    HANDLE hSemaphore;
    hSemaphore = CreateSemaphore(NULL, 0, size, NULL);
    if (hSemaphore == NULL)
        return GetLastError();

    // Кладем данные в структуру, чтобы передать потоку Work
    struct Data* tData = (struct Data*)malloc(sizeof(struct Data));
    tData->size = size; tData->array = array;
    tData->hSemaphore = hSemaphore;
    // Создаем новый поток Work
    HANDLE workThread;
    DWORD workIDThread;
    workThread = CreateThread(NULL, 0, Work, tData, 0, &workIDThread);
    if (workThread == NULL)
        return GetLastError();

    // Создаем бинарный семафор
    HANDLE bSemaphore;
    bSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
    if (bSemaphore == NULL)
        return GetLastError();

    // Кладем данные в структуру, чтобы передать потоку SumElement
    struct SumData* bData = (struct SumData*)malloc(sizeof(struct SumData));
    bData->size = size; bData->array = array; bData->k = k;
    bData->hSemaphore = bSemaphore;
    // Создаем новый поток SumElement
    HANDLE sumThread;
    DWORD sumIDThread;
    sumThread = CreateThread(NULL, 0, SumElement, bData, 0, &sumIDThread);
    if (sumThread == NULL)
        return GetLastError();
    // Выводим итоговый массив, при этом сортируя его и рассчитывая сумму элементов до заданной позиции
    for (int i = 1; i <= size; i++) {
        // Ждем элемент
        WaitForSingleObject(hSemaphore, INFINITE);
        if (k == i) {
            cout << "!" << array[i - 1] << "! ";
            ReleaseSemaphore(bSemaphore, 1, NULL);
        }
        else {
            cout << array[i - 1] << " ";
        }
    }
    // Закрываем поток Work
    WaitForSingleObject(workThread, INFINITE);
    CloseHandle(workThread);
    // Закрываем поток sumThread
    WaitForSingleObject(sumThread, INFINITE);
    CloseHandle(sumThread);
    // Освобождаем память
    delete[] array;

    return 0;
}