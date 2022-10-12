#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <random>
using namespace std;

// Требуется разработать программу, которая реализует терминальный интерфейс тренажера клавиатуры для консольного ввода текстовых строк по образцу. Желаемая длина образцовой строки должна передаваться программе через аргумент командной строки ее вызова. Образец строки заданной длины должен формироваться в программе по случайной последовательности, состоящей из отображаемых символов кодовой таблице ASCII, которую завершает символ перевода строки. Полученный таким способом образец строки должен быть отображен в потоке стандартного вывода. После этого в программе должен быть установлен неканонический режим терминального ввода без эхо-печати входных символов, где также должны игнорироваться клавиатурные сигналы прерывания и завершения. Этот режим должен обеспечивать контролируемый ввод символьной строки, совпадающей с образцом, где исключены любые ошибки ввода. Это означает, что каждый очередной входной символ должен приниматься программой и отображаться в потоке стандартного ввода только при его совпадении с образцом. При этом любые ошибки ввода должны блокировать стандартный вывод очередных входных сиволов, пока нет совпадения с текущим символом образца. Стандартный ввод должен продолжаться до получения входной строки, совпадающей с образцом. После завершения ввода по образцу в потоке стандартной диагностики должно отображаться информационное сообщение, которое идентифицирует продолжительность ввода и число ошибок. Кроме того в программе необходимо предусмотреть возможность ее принудительного завершения при нажатии клавиши Escape в любой момент времени до ввода всех символов образца. В любом случае, перед завершениием программы должен быть восстановлен исходный режим термиального ввода. При разработке программы необходимо использовать системные вызовы read, write, time, sbrk и exit, а также системные функции терминального интерфейса temios OS UNIX и стандартные библиотечные функции системы программирования C для генерации случайных последовательностей.

// Вариант 12
// Разработать программу тренажера клавиатуры с использованием терминального интерфейса, в которой должен вводиться правильный результат арифметической операции вычисления целочисленного остатка от деления двух случайных операндов. Следует предусмотреть блокировку ввода ошибочных цифр и других символов.

void setInputMode(struct termios& saved_attributes);
void resetInputMode(struct termios& saved_attributes);
void clear();
void moveCursorTo(int fromTop, int fromLeft);
string generateString(int length);


int main() {
	
	int stringLength = 10;  // Amount of symbols in every string.
	do {
		clear();
		cout << "Please input size of string" << endl;
		cin >> stringLength;
	} while (!(stringLength >= 1 && stringLength <= 30));

	// Making the terminal work in non-canonical way.
	struct termios saved_attributes;
	setInputMode(saved_attributes);
	
	string SampleText = "Just some sample string that you need to type";
	SampleText = generateString(stringLength);
	cout << SampleText << endl;
	time_t startTime = time(NULL);  // Counting game time.
	char c;  // Currently typed symbol.
	int progress = 0;  // Amount of passed symbols in current string.
	int passedStrings = 0;  // General amount of passed strings.

	// Main game cycle.
	while ((c = getchar()) != 27) {  // Waiting ESCAPE to be pressed.
		while (c != 27 && SampleText[progress] != '\0') {  // Till we reach end of current string.
			if (c == SampleText[progress]) {  // User typed correct symbol.
				moveCursorTo(3 + passedStrings, progress + 1);
				cout << "\E[44m";
				cout << c;
				moveCursorTo(3 + passedStrings, progress + 2);
				cout << "\E[0m";
				progress++;
			} else {  // User typed incorrect symbol.
				moveCursorTo(3 + passedStrings, progress + 1);
				cout << "\E[45m";
				cout << SampleText[progress];
				moveCursorTo(3 + passedStrings, progress + 2);
				cout << "\E[0m";
			}
			c = getchar();
		}
		if (c == 27) break;
		moveCursorTo(4 + passedStrings, 1);
		SampleText = generateString(stringLength);
		cout << SampleText << endl;
		progress = 0;
		passedStrings++;
	}
	
	moveCursorTo(5 + passedStrings + 1, 1);
	cout << endl;

	// Printing out game results.
	time_t finishTime = time(NULL);
	int gameTime = finishTime - startTime;
	int Score = passedStrings * stringLength + progress;
	cout << Score << " symbols in " << finishTime - startTime << " seconds" << endl;
	cout << "Your speed is " << (double)Score/gameTime * 60 << " symbols per minute"<< endl;
	// Making the terminal work in canonical way.
	resetInputMode(saved_attributes);
}


// Generates random string of symbols of given length.
string generateString(int length) {
	string result;
	for (int i = 0; i < length; i++) {
		result.push_back(rand() % (122 - 97 + 1) + 97);
	}
	return result;
}


// Clears terminal.
void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}


// Moves cursor to given coordinates.
void moveCursorTo(int fromTop, int fromLeft) {
	cout << "\E[" << fromTop << ";" << fromLeft << "H";
}


// Resets input mode back to canonical.
void resetInputMode(struct termios& saved_attributes) {
   tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}


// Sets input mode to non-canonical.
void setInputMode(struct termios& saved_attributes) {
   struct termios tattr;

  /* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);

  /* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}