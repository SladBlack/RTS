#include <windows.h>
#include <iostream>

using namespace std;

void printArray(int* array, int size) {
    // ������� ������
    for (int i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
}

int* createArray(int size) {
    // ������� ������������ ������
    int* arr = new int[size];
    for (int i = 0; i < size; i++)
        cin >> arr[i];

    return arr;
}

int sumArray(int* array, int size, int k) {
    // �������������� �������� ������� �� �������� k-� �������
    int sum = 0;
    // �������� ������� �������
    if (k > size) {
        cout << "Your number greater than size of array";
        return sum;
    }
    // ������������ ��������� �� �������� �������
    for (int i = 0; i < k; i++)
        sum += array[i];

    return sum;
}

void sortArray(int* array, int size, HANDLE hSemaphore, int timeout) {
    // ������������� ������
    // �������� ������� �� ���������� ���������
    if (size < 2) { return; }
    int temp1, temp2; // ���������� ��� ������������ 
    int index1 = 0, index2 = 1; // ���������� ��� �������� �� ���������
    bool swap = false; // ����, �����������, ���� �� ������������
    int i; // ���������� ��� �����
    for (i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (array[i] == array[j]) {
                // ���������� ������� �������� �� ��������� ����������
                temp1 = array[i];
                temp2 = array[j];
                // ������������ ��������
                array[i] = array[index1];
                array[j] = array[index2];

                array[index1] = temp1;
                array[index2] = temp2;
                if (index2 < size + 1) {
                    index1++;
                    index2++;
                    swap = true;
                }
                // ��������, ��� ������� �����
                ReleaseSemaphore(hSemaphore, 1, NULL);
                // ���� �������� �����
                Sleep(timeout); 
            }
        }
        if (swap) {
            // ���� ���� ������������, ����������� ������� � ������ ����
            i = index1;
            index1++;
            index2++;
            swap = false;
        }
        // ��������, ��� ������� �����
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }
}



struct Data {
    // ���������, ������� ������ ������ ������� � ��� ������
    int size;
    int* array;
    HANDLE hSemaphore;
};

struct SumData {
    // ��������� ��� ������ SumElement
    int size;
    int* array;
    HANDLE hSemaphore;
    int k;
};

DWORD WINAPI Work(LPVOID threadData) {
    // ����� work
    int timeout;
    cout << "Input timeout: "; cin >> timeout;
    // ���������� ������, ���������� ������ � ���������
    struct Data* tData = (struct Data*)threadData;
    int* array = tData->array;
    int size = tData->size;
    // ������� �������
    HANDLE hSemaphore = tData->hSemaphore;
    // ��������� ������
    sortArray(array, size, hSemaphore, timeout);
    return 1;
}

DWORD WINAPI SumElement(LPVOID threadData) {
    // ���������� ������, ���������� ������ � ���������
    struct SumData* bData = (struct SumData*)threadData;
    int* array = bData->array; int size = bData->size;
    int k = bData->k;
    HANDLE bSemaphore = bData->hSemaphore;
    // ������� ������� ��������
    WaitForSingleObject(bSemaphore, INFINITE);
    // ������� ����� ������� �� �������� �������
    int res = sumArray(array, size, k);
    cout << "\nResult sum of arary: " << res << endl;
    return 1;
}

int main() {
    int size, k; // size - ������ �������, k - ����� ��������, �� �������� ������� �����
    int* array; // ��������� �� ������ ����� �����

    cout << "Size of array: "; cin >> size;
    cout << "Input elements of array: ";
    // ������� ������
    array = createArray(size);
    // �������
    cout << "\nSize of array = " << size << endl;
    cout << "Elements of array: ";
    printArray(array, size);
    // ����������� ����� ��������, �� �������� ������� �����
    cout << "Input k: "; cin >> k;
    // ������� ��������� �������
    HANDLE hSemaphore;
    hSemaphore = CreateSemaphore(NULL, 0, size, NULL);
    if (hSemaphore == NULL)
        return GetLastError();

    // ������ ������ � ���������, ����� �������� ������ Work
    struct Data* tData = (struct Data*)malloc(sizeof(struct Data));
    tData->size = size; tData->array = array;
    tData->hSemaphore = hSemaphore;
    // ������� ����� ����� Work
    HANDLE workThread;
    DWORD workIDThread;
    workThread = CreateThread(NULL, 0, Work, tData, 0, &workIDThread);
    if (workThread == NULL)
        return GetLastError();

    // ������� �������� �������
    HANDLE bSemaphore;
    bSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
    if (bSemaphore == NULL)
        return GetLastError();

    // ������ ������ � ���������, ����� �������� ������ SumElement
    struct SumData* bData = (struct SumData*)malloc(sizeof(struct SumData));
    bData->size = size; bData->array = array; bData->k = k;
    bData->hSemaphore = bSemaphore;
    // ������� ����� ����� SumElement
    HANDLE sumThread;
    DWORD sumIDThread;
    sumThread = CreateThread(NULL, 0, SumElement, bData, 0, &sumIDThread);
    if (sumThread == NULL)
        return GetLastError();
    // ������� �������� ������, ��� ���� �������� ��� � ����������� ����� ��������� �� �������� �������
    for (int i = 1; i <= size; i++) {
        // ���� �������
        WaitForSingleObject(hSemaphore, INFINITE);
        if (k == i) {
            cout << "!" << array[i - 1] << "! ";
            ReleaseSemaphore(bSemaphore, 1, NULL);
        }
        else {
            cout << array[i - 1] << " ";
        }
    }
    // ��������� ����� Work
    WaitForSingleObject(workThread, INFINITE);
    CloseHandle(workThread);
    // ��������� ����� sumThread
    WaitForSingleObject(sumThread, INFINITE);
    CloseHandle(sumThread);
    // ����������� ������
    delete[] array;

    return 0;
}