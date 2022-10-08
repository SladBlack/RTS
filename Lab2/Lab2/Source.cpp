#include<iostream>
#include<fstream>
#include<string>
#include <windows.h> 
#include <vector>

using namespace std;

struct Student{
	int num; // number of ticket
	char name[10]; // student's name
	double grade; // grade
};

Student setStudent() {
	Student student;
	cout << "Num of student ticket: "; cin >> student.num;
	cout << "Name of student: "; cin >> student.name;
	cout << "Grade: "; cin >> student.grade;
	return student;
}

void writeFile(Student student, char file_name[]) {
	printf("\t%s\n", "Writing to file...");
	FILE* file;
	fopen_s(&file, file_name, "ab+");
	if (file) {
		fwrite(&student, sizeof(Student), 1, file);
		fclose(file);
	}
	printf("\t%s\n", "File was successfully written!");
}

void hexdump(void* ptr, int buflen)
{
	unsigned char* buf = (unsigned char*)ptr;
	int i, j;
	for (i = 0; i < buflen; i += 16) {
		printf("%06x: ", i);
		for (j = 0; j < 16; j++) {
			if (i + j < buflen)
				printf("%02x ", buf[i + j]);
			else
				printf("   ");
		}
		printf(" ");
		for (j = 0; j < 16; j++) {
			if (i + j < buflen)
				printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
		}
		printf("\n");
	}
}


void printFile(char file_name[]) {
	printf("\t%s\n", "File:");
	Student student;
	fstream file(file_name, ios::in | ios::out | ios::binary);
	char table[] = "========================================================================================";
	printf("+%.5s+%.10s+%.5s+\n", table, table, table);
	printf("|%-5s|%-10s|%-5s|\n", "Num", "Name", "Grade");
	printf("+%.5s+%.10s+%.5s+\n", table, table, table);
	while (file.read((char*)&student, sizeof(Student))){
		printf("|%-5d|%-10s|%-5.1f|\n", student.num, student.name, student.grade);
		printf("+%.5s+%.10s+%.5s+\n", table, table, table);
	}
	file.close();
}

LPWSTR CharToLPWSTR(LPCSTR char_string)
{
	LPWSTR res;
	DWORD res_len = MultiByteToWideChar(1251, 0, char_string, -1, NULL, 0);
	res = (LPWSTR)GlobalAlloc(GPTR, (res_len + 1) * sizeof(WCHAR));
	MultiByteToWideChar(1251, 0, char_string, -1, res, res_len);
	return res;
}

void printMenu() {
	printf("----Menu----\n");
	printf("1 - Show file\n");
	printf("2 - Write file\n");
	printf("3 - Edit file\n");
	printf("0 - Exit\n");
	printf("-----------------\n");
}

void write(char filename[]) {
	// Create a binary file
	Student student = setStudent();
	writeFile(student, filename);
}

int update(char filename[]) {
	// Request data to update
	cout << "\tEnter data to update:\n";
	Student n_student;
	cout << "Num of tickets: "; cin >> n_student.num;
	cout << "Studen's name: "; cin >> n_student.name;
	cout << "New grade: "; cin >> n_student.grade;

	// Make command line
	char CommandLine[100];
	char process_path[] = "Child.exe";
	sprintf_s(CommandLine, sizeof(CommandLine), "%s %s %d %s %f", process_path, filename, n_student.num, n_student.name, n_student.grade);
	LPWSTR lpszCommandLine = CharToLPWSTR(CommandLine);

	// Run child process
	STARTUPINFO si;
	PROCESS_INFORMATION piCom;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom)){
		cout << "The new process is not created.\n";
		cout << "Check a name of the process.\n";
		cout << "Press any key to finish.\n";
		return 0;
	}
	WaitForSingleObject(piCom.hProcess, INFINITE);
	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);
	cout << "[+] Updated was successfuly" << endl;

	return 0;
}

int main(int argc, char* argv[]) {
	char filename[] = "students.dat";
	int choice;
	while (true) {
		printMenu();
		cout << "Enter your choice: "; cin >> choice;
		switch (choice)
		{
		case 1:
			printFile(filename);
			break;
		case 2:
			write(filename);
			break;
		case 3:
			update(filename);
			break;
		case 0:
			return 0;
		default:
			break;
		}
		cout << "\n\n";
	}
	
	return 0;
}