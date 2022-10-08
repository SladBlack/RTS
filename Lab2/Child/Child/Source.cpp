#include<iostream>
#include<fstream>
#include<string>

using namespace std;

const int EXPECTED_ARGS_COUNT = 4;

struct Student {
	int num; // number of ticket
	char name[10]; // student's name
	double grade; // grade
};

void printStudentInfo(char* argv[]) {
	cout << "Number of ticket: " << argv[2] << endl;
	cout << "Student's name: " << argv[3] << endl;
	cout << "Grade: " << argv[4] << endl;
}

void updateNewStudentInfo(char* filename, int num, char name[], double new_grade) {
	int pos;
	bool if_find = false;
	Student student;
	Student* new_student = new Student;

	fstream f;
	f.open(filename, ios::in | ios::binary | ios::out);
	while (!f.eof()) {
		pos = f.tellg();
		f.read((char*)&student, sizeof(Student));
		if (f) {
			if ((student.num == num) && (strcmp(student.name, name) == 0)) {
				if_find = true;

				*new_student = student;
				new_student->grade = new_grade;

				f.seekp(pos);
				f.write((char*)new_student, sizeof(Student));

				break;
			}
		}
		
	}
	f.close();

	if (if_find)
		cout << "Update was successful";
	else
		cout << "Student with these credential not found";
}

int getSizeOfArgv(char* argv[]) {
	int size = 0;
	while (argv[size]) 
		size += 1;
	size--; // reduce because the first argument is name of current application
	return size;
}

int main(int argc, char* argv[]) {
	/* Expected arguments:
	 0 - name of current .exe
	 1 - name of file with data
	 2 - Number of sutent's ticket
	 3 - Student's name
	 4 - Grade*/
	int size = getSizeOfArgv(argv);
	if (size != EXPECTED_ARGS_COUNT) {
		cout << "[Error] Expected count of arguments = " << EXPECTED_ARGS_COUNT << ", recieved = " << size << endl;
		return 0;
	}

	printStudentInfo(argv);
	updateNewStudentInfo(argv[1], atoi(argv[2]), argv[3], atof(argv[4]));

	return 0;
}