#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
// #include <curses.h>
using namespace std;

// Требуется разработать программу, которая реализует терминальный интерфейс тренажера клавиатуры для консольного ввода текстовых строк по образцу. Желаемая длина образцовой строки должна передаваться программе через аргумент командной строки ее вызова. Образец строки заданной длины должен формироваться в программе по случайной последовательности, состоящей из отображаемых символов кодовой таблице ASCII, которую завершает символ перевода строки. Полученный таким способом образец строки должен быть отображен в потоке стандартного вывода. После этого в программе должен быть установлен неканонический режим терминального ввода без эхо-печати входных символов, где также должны игнорироваться клавиатурные сигналы прерывания и завершения. Этот режим должен обеспечивать контролируемый ввод символьной строки, совпадающей с образцом, где исключены любые ошибки ввода. Это означает, что каждый очередной входной символ должен приниматься программой и отображаться в потоке стандартного ввода только при его совпадении с образцом. При этом любые ошибки ввода должны блокировать стандартный вывод очередных входных сиволов, пока нет совпадения с текущим символом образца. Стандартный ввод должен продолжаться до получения входной строки, совпадающей с образцом. После завершения ввода по образцу в потоке стандартной диагностики должно отображаться информационное сообщение, которое идентифицирует продолжительность ввода и число ошибок. Кроме того в программе необходимо предусмотреть возможность ее принудительного завершения при нажатии клавиши Escape в любой момент времени до ввода всех символов образца. В любом случае, перед завершениием программы должен быть восстановлен исходный режим термиального ввода. При разработке программы необходимо использовать системные вызовы read, write, time, sbrk и exit, а также системные функции терминального интерфейса temios OS UNIX и стандартные библиотечные функции системы программирования C для генерации случайных последовательностей.

// Вариант 12
// Разработать программу тренажера клавиатуры с использованием терминального интерфейса, в которой должен вводиться правильный результат арифметической операции вычисления целочисленного остатка от деления двух случайных операндов. Следует предусмотреть блокировку ввода ошибочных цифр и других символов.

// typedef class expression {
// public:
// 	expression(string _body) {
// 		body = _body;
// 	}
// 	string body;
// 	// bool is

// } expression;


int main() {
	struct termios saved_attributes;
	void set_input_mode(struct termios& saved_attributes);
	void reset_input_mode(struct termios& saved_attributes);
	void clear();
	void moveCursorTo(int fromTop, int fromLeft);
	
	set_input_mode(saved_attributes);

	char* SampleText = (char*)"Just some sample string that you need to type";
	cout << SampleText << endl;
	char c;
	int progress = 0;
	while (((c = getchar()) != 27) && c != '\0') {
		if (c == SampleText[progress]) {
			progress++;
			moveCursorTo(3, progress);
			cout << "\E[44m";
			cout << c;
			moveCursorTo(3, progress + 1);
			cout << "\E[0m";
		}
		// // cin >> hui;
		// if (c == 'j') {
		// 	cout << "\E[20D\E[34m" << endl;
		// }
		// cout << c << endl;
	}
	// cout << "\E[3;2H";
	// clear();
	// reset_input_mode(saved_attributes);
	// moveCursorTo(10, 5);
}

void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

void moveCursorTo(int fromTop, int fromLeft) {
	cout << "\E[" << fromTop << ";" << fromLeft << "H";
}

void reset_input_mode(struct termios& saved_attributes)
{
   tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode(struct termios& saved_attributes)
{
   struct termios tattr;

  /* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);

  /* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}