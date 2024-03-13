#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <conio.h>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

class Task
{
public:
	//constructor
	Task(string ti = "", string de = "", int th = 0, int tm = 0, int dd = 0, int dm = 0, bool id = 0)
	{
		title = ti; description = de;  timeHours = th; timeMinutes = tm; dateDay = dd; dateMonth = dm; isDone = id;
	};


	//Set methods
	void setTitle(string str) { title = str; }
	void setDescription(string str) { description = str; }
	void setTags(vector<string> tag) { tags = tag; }
	void setTimeHours(int num) { timeHours = num; }
	void setTimeMinutes(int num) { timeMinutes = num; }
	void setDateDay(int num) { dateDay = num; }
	void setDateMonth(int num) { dateMonth = num; }
	void setIsDone(bool bl) { isDone = bl; }


	//Get methods
	string getTitle() const { return title; }
	string getDescription() const { return description; }
	const vector<string>& getTags() const { return tags; }
	int getTimeHours() const { return timeHours; }
	int getTimeMinutes() const { return timeMinutes; }
	int getDateDay() const { return dateDay; }
	int getDateMonth() const { return dateMonth; }
	bool getIsDone() const { return isDone; }

private:
	string title, description;
	vector<string> tags;
	int timeHours, timeMinutes, dateDay, dateMonth;
	bool isDone;
};

void deleteAllTasks();
void addTask();
void markAsDone();
void deleteTask();
void editTask();
void searchTask();
void viewAllTasks();
void quit();
void showStats();
void saveTasksToFile(const vector<Task>& tasks);
void loadTasksFromFile(vector<Task>& tasks);
vector<string> addTags(const Task& task);

//show function
void showTask(Task t) {
	vector<string> tags;
	cout << "TITLE: " << t.getTitle() << "\n";
	cout << "DESCRIPTION: " << t.getDescription() << "\n";
	cout << "TAGS: ";
	tags = t.getTags();
	for (int i = 0; i < tags.size(); i++)
	{
		cout << "#" << tags[i] << " ";
	}
	cout << "\nTIME: " << setw(2) << setfill('0') << t.getTimeHours() << ":" << setw(2) << setfill('0') << t.getTimeMinutes() << "\n";
	cout << "DATE(dd.mm): " << setw(2) << setfill('0') << t.getDateDay() << "." << setw(2) << setfill('0') << t.getDateMonth() << "\n";
	if (t.getIsDone())
	{
		cout << "TASK DONE\n";
	}
	else {
		cout << "TASK NOT DONE\n";
	}
}



int inputTime(const string& prompt, int maxValue) {
	int time;
	while (true) {
		try {
			cout << prompt;
			cin >> time;
			if (cin.fail() || time < 0 || time > maxValue) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				throw runtime_error("ERROR: ENTER CORRECT VALUE (0-" + to_string(maxValue) + ")");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << endl;
		}
	}
	return time;
}

int inputNumber(const string& prompt, int min, int max) {
	int value;
	while (true) {
		try {
			cout << prompt;
			cin >> value;
			if (cin.fail() || value < min || value > max) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				throw runtime_error("ERROR: ENTER CORRECT VALUE (" + to_string(min) + "-" + to_string(max) + ")");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << endl;
		}
	}
	return value;
}

vector<Task> tasks;


// main
int main() {
	loadTasksFromFile(tasks);
	signed char choice;
	system("CLS");
	cout << "TASK MANAGER\n\n";
	cout << "[1]ADD TASK\n";
	cout << "[2]MARK AS DONE TASK\n";
	cout << "[3]DELETE TASK\n";
	cout << "[4]EDIT TASK\n";
	cout << "[5]SEARCH TASK\n";
	cout << "[6]VIEW ALL TASKS\n";
	cout << "[7]DELETE ALL TASKS\n";
	cout << "[8]QUIT\n\n";
	cout << "[s]VIEW STATISTIC\n\n";

	try
	{
		cout << "ENTER YOUR CHOICE: ";
		cin >> choice;
		if (!cin)
		{
			throw runtime_error("ERROR: invalid char");
		}
		system("CLS");
		cin.ignore();

		switch (choice)
		{
		case '1':
			addTask();
			break;
		case '2':
			markAsDone();
			break;
		case '3':
			deleteTask();
			break;
		case '4':
			editTask();
			break;
		case '5':
			searchTask();
			break;
		case '6':
			viewAllTasks();
			break;
		case '7':
			deleteAllTasks();
			break;
		case '8':
			quit();
			break;
		case 's':
			showStats();
			break;
		default:
			cout << "INVALID CHOICE!!!\nPress any key to continue . . .";
			_getch();
			main();
			break;
		}

	}
	catch (const exception& e)
	{
		cerr << "ERROR: " << e.what() << endl;
		return 1;
	}

	return 0;
}

// add func
void addTask() {
	try {
		string title, desc;
		int hour, min, day, month;
		cout << "ADD TASK\n\n";
		cout << "ENTER TITLE: ";
		getline(cin, title);
		if (title.empty()) {
			throw runtime_error("ERROR: Title CAN'T BE EMPTY");
		}
		cout << "ENTER DESCRIPTION: ";
		getline(cin, desc);
		hour = inputTime("ENTER HOUR(0-23): ", 23);
		min = inputTime("ENTER MINUTE(0-59): ", 59);
		day = inputNumber("ENTER DAY(1-31): ", 1, 31);
		month = inputNumber("ENTER MONTH(1-12): ", 1, 12);
		Task t(title, desc, hour, min, day, month);
		vector<string> tags = addTags(t);
		t.setTags(tags);
		tasks.push_back(t);
		saveTasksToFile(tasks);
		cout << "\nTASK ADDED SUCCESSFULLY!\n";
	}
	catch (const exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}
	cout << "\n\TASK ADDED SUCCESSFULLY!\nPress any key to continue . . .";
	_getch();
	main();
}


//Mark func
void markAsDone() {
	try {
		if (tasks.empty()) {
			throw runtime_error("ERROR: NO TASKS AVAILABLE");
		}
		cout << "ENTER TASK ID: ";
		int num;
		cin >> num;
		if (num < 0 || num >= tasks.size()) {
			throw runtime_error("ERROR: INVALID TASK ID");
		}
		tasks[num].setIsDone(true);
		cout << "TASK MARKED AS DONE\n";
		saveTasksToFile(tasks);
	}
	catch (const exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}
	cout << "Press any key to continue . . .";
	_getch();
	main();
};

//delete func
void deleteTask() {
	try {
		cout << "DELETE TASK\n\n";
		if (tasks.empty()) {
			throw runtime_error("ERROR: No tasks available");
		}
		cout << "ENTER TASK ID: ";
		int num;
		cin >> num;
		if (num < 0 || num >= tasks.size()) {
			throw runtime_error("ERROR: Invalid task ID");
		}
		tasks.erase(tasks.begin() + num);
		cout << "TASK DELETED\n";
		saveTasksToFile(tasks);
	}
	catch (const exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}
	cout << "Press any key to continue . . .";
	_getch();
	main();
}

vector<string> addTags(const Task& task) {
	try {
		char choice;
		string tag;
		vector<string> tags = task.getTags();
		cout << "ADD TAGS\n\n";
		do {
			cout << "ENTER TAG: ";
			cin >> tag;
			tags.push_back(tag);
			cout << "DO YOU WANT TO ADD ANOTHER TAG? (y/n): ";
			cin >> choice;
		} while (choice == 'y' || choice == 'Y');
		cout << "TAGS ADDED SUCCESSFULLY!\n";
		return tags;
	}
	catch (const exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return {};
	}
}

void deleteAllTasks() {
	tasks.clear();
	saveTasksToFile(tasks);
	cout << "YOU DELETED ALL TASKS\n";
	cout << "Press any key to continue . . .";
	_getch();
	main();
}

//edit func
void editTask() {
	vector<string> tag;
	string str;
	signed char choice;
	int id, num;
	bool bl;
	try {
		cout << "EDIT TASK\n\n";
		if (tasks.empty()) {
			throw runtime_error("ERROR: NO TASKS AVAILABLE");
		}
		cout << "ENTER TASK ID: ";
		cin >> id;
		if (id < tasks.size())
		{
			cout << "ENTER PARAMETR TO EDIT: \n";
			cout << "[1]TITLE\n";
			cout << "[2]DESCRIPTION\n";
			cout << "[3]TAGS\n";
			cout << "[4]TIME\n";
			cout << "[5]DATE\n";
			cout << "[6]IS DONE\n";

			cin >> choice;
			cin.ignore();
			switch (choice)
			{
			case '1':
				cout << "ENTER NEW TITLE: ";
				getline(cin, str);
				tasks[id].setTitle(str);
				break;
			case '2':
				cout << "ENTER NEW DESCRIPTION: ";
				getline(cin, str);
				tasks[id].setDescription(str);
				break;
			case '3':
				cout << "ENTER NEW TAG: ";
				getline(cin, str);
				tag.push_back(str);
				tasks[id].setTags(tag);
				break;
			case '4':
				num = inputTime("ENTER HOUR(0-23): ", 23);
				tasks[id].setTimeHours(num);
				num = inputTime("ENTER MINUTE(0-59): ", 59);
				tasks[id].setTimeMinutes(num);
				break;
			case '5':
				num = inputNumber("ENTER DAY(1-31)", 1, 31);
				tasks[id].setDateDay(num);
				num = inputNumber("ENTER MONTH(1-12)", 1, 12);
				tasks[id].setDateMonth(num);
			case '6':
				cout << "CHOICE [0](NOT DONE) OR [1](DONE): ";
				cin >> bl;
				tasks[id].setIsDone(bl);
				break;
			default:
				cout << "ENTER VALID VALUE: ";
				break;
			}
			cout << "TASK EDITED SUCCESSFULY!\n";
		}
		else {
			cout << "INVALID ID\n";
		}
		saveTasksToFile(tasks);
	}
	catch (const exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}
	cout << "Press any key to continue . . .";
	_getch();
	main();
}

//search func
void searchTask() {
	signed char choice;
	string str;
	vector<string> tags;
	cout << "SEARCH TASK\n\n";
	if (tasks.size() > 0)
	{
		cout << "SEARCH BY [1] - TITLE	OR	[2] - TAG: ";
		cin >> choice;
		cin.ignore();
		switch (choice)
		{
		case '1':
			cout << "ENTER TITLE PARAMETR: ";
			getline(cin, str);
			for (int i = 0; i < tasks.size(); i++)
			{
				if (tasks[i].getTitle() == str) {
					showTask(tasks[i]);
				}
			}
			cout << "\nEND.";
			break;
		case '2':
			cout << "ENTER TAG PARAMETR: ";
			getline(cin, str);
			for (int i = 0; i < tasks.size(); i++)
			{
				tags = tasks[i].getTags();
				if (find(tags.begin(), tags.end(), str) != tags.end()) {
					showTask(tasks[i]);
				};
			}
			cout << "\nEND.";
			break;
		default:
			cout << "ENTER VALID VALUE\n";
			break;
		}
	}
	else
	{
		cout << "NO TASK TO SEACRH\n";
	}
	cout << "Press any key to continue . . .";
	_getch();
	main();
}

// view func
void viewAllTasks() {
	cout << "VIEW ALL TASKS\n\n";
	if (tasks.size() <= 0)
	{
		cout << "YOU HAVEN'T TASKS!!!\n";
	}
	else
	{
		for (int i = 0; i < tasks.size(); i++)
		{
			cout << "TASK DETAILS\n\n";
			cout << "id[" << i << "] \n";
			showTask(tasks[i]);
			cout << "\n";
		}
	}
	cout << "Press any key to continue . . .";
	_getch();
	main();
}


// quit func
void quit() {
	saveTasksToFile(tasks);
	_exit(1);
}

void showStats() {
	signed char rank;
	if (tasks.size() > 0)
	{
		int doneCount = 0, notDoneCount = 0;
		for (int i = 0; i < tasks.size(); i++)
		{
			if (tasks[i].getIsDone()) {
				doneCount++;
			}
			else
			{
				notDoneCount++;
			}
		}
		float donePer = (float)doneCount / (float)tasks.size() * 100;
		cout << "PERCENT OF DONE TASK: " << donePer << "%\n\n";
		cout << "DONE TASKS: " << doneCount << "\nNOT DONE TASKS: " << notDoneCount << "\n";
		if (doneCount == 0)
		{
			rank = 'b';
		}
		else if (doneCount == 1) {
			rank = 's';
		}
		else if (doneCount >= 2) {
			rank = 'g';
		}
		switch (rank)
		{
		case 'b':
			cout << "YOUR RANK IS \033[0;33mBRONZE\033[0;0m";
			break;
		case 's':
			cout << "YOUR RANK IS \033[0;37mSILVER\033[0;0m";
			break;
		case 'g':
			cout << "YOUR RANK IS \033[0;GOLD\033[0;0m";
			break;
		default:
			break;
		}
	}
	else {
		cout << "YOU MUST HAVE ANY TASK\n";
	}
	cout << "\nPress any key to continue . . .";
	_getch();
	main();
};

void saveTasksToFile(const vector<Task>& tasks) {
	ofstream outputFile("tasks.txt");
	if (outputFile.is_open()) {
		for (const auto& task : tasks) {
			outputFile << task.getTitle() << "\n";
			outputFile << task.getDescription() << "\n";
			outputFile << task.getTimeHours() << " " << task.getTimeMinutes() << "\n";
			outputFile << task.getDateDay() << " " << task.getDateMonth() << "\n";
			outputFile << task.getIsDone() << "\n";
			const vector<string> tags = task.getTags();
			outputFile << tags.size() << "\n";
			for (const auto& tag : tags) {
				outputFile << tag << "\n";
			}
			outputFile << "---\n";
		}
		outputFile.close();
		cout << "DATA SAVED SUCCESSFULY TO tasks.txt\n";
	}
	else {
		cout << "ERROR: CAN'T OPEN FILE TO WRITE\n";
	}
}

void loadTasksFromFile(vector<Task>& tasks) {
	ifstream inputFile("tasks.txt");
	if (inputFile.is_open()) {
		tasks.clear();
		while (!inputFile.eof()) {
			string title, description;
			int timeHours, timeMinutes, dateDay, dateMonth;
			bool isDone;
			vector<string> tags;

			getline(inputFile, title);
			if (title.empty()) break;
			getline(inputFile, description);
			inputFile >> timeHours >> timeMinutes;
			inputFile >> dateDay >> dateMonth;
			inputFile >> isDone;

			int numTags;
			inputFile >> numTags;
			inputFile.ignore();
			for (int i = 0; i < numTags; ++i) {
				string tag;
				getline(inputFile, tag);
				tags.push_back(tag);
			}

			tasks.push_back(Task(title, description, timeHours, timeMinutes, dateDay, dateMonth, isDone));
			tasks.back().setTags(tags);
			string separator;
			getline(inputFile, separator);
		}
		inputFile.close();
		cout << "DATA OPENED SUCCESSFULY\n";
	}
	else {
		cout << "DATA CAN'T OPENED\n";
	}
}