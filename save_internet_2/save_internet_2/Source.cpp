#include <fstream>
#include <conio.h>
#include <vector>
#include <iostream>
using namespace std;

void encryption(int key[]) {

	int val = 0, //нужна для получения бита по ключу из 32 битов (4 символов)
		c = 0, //остаток от деления размера файла в битах на 4
		d = 0, //дополнительная переменная для "обрезания" мусора в конце файла
		encr = 0; //символы после шифровки

	ifstream ifs("Test.exe", ios::binary);
	size_t size = ifs.seekg(0, ios::end).tellg();
	size_t block = 1024; //для чтения по 1024 байта

	ifs.seekg(0);
	c = 4 - size % 4; //в битах
	ofstream fout("Result.txt", ios_base::trunc | ios::binary);

	for (int i = 0; i <= size / 4; i++) { //разбиваем файл на блоки по 4 символа
		char *buf = new char[block]; //1024 байта
		ifs.read(buf, block); //считывание 1024 байт

		for (int j = 0; j < (block / 4); j++)
		{

			int *link = (j + (int*)buf); //указатель на 4 символа массива, j нужен для смещения указателя на следующие 4
			encr = 0;

			for (int i = 0; i < 32; i++) {

				int a = (*link >> (31 - key[i]));
				val = 1 & a; //сдвиг для работы с конкретным битом

				if (val != 0) { //перемешивание битов
					encr = encr | (1 << (31 - i)); //если бит = 1, то задвигаем его слева
				}
				val = 0;
			}

			char buffer[4]; //доп. массив для символов

			for (int i = 1; i < 5; i++) {
				buffer[i - 1] = encr >> (32 - (i * 8)); //пеевод биты в символы (1 символ - 8 бит)
			}

			for (int i = 0; i < 4; i++) { //записывает массив в обратном порядке в файл результата
				if (d < size + c) {
					fout << buffer[3 - i];
				}
				d++;
			}
		}
		delete[] buf;
	}
	fout << c;
}

//функция дешифрования
void decryption(int key[]) {
	int val = 0, //нужна для получения бита по ключу из 32 битов (4 символов)
		encr = 0, //символы после дешифровки
		c = 0; //остаток от деления размера файла в битах на 4
	char minus;

	ifstream ifs("Result.txt", ios::binary);
	size_t size = ifs.seekg(0, ios::end).tellg();
	size_t block = 1024; //для чтения по 1024 байта
	ifs.seekg(size - 1);
	ifs.get(minus); //последний символ из зашифрованного файла
	int intminus = minus - '0'; //для предотвращения записи лишних байт
	ifs.seekg(0);

	ofstream fout("New.txt", ios_base::trunc | ios::binary);

	for (int i = 0; i <= size / 4; i++) { //разбиваем файл на блоки по 4 символа
		
		char * buf = new char[block]; //1024 байта
		ifs.read(buf, block); //считывание 1024 байт
		
		for (int j = 0; j < (block / 4); j++) {
			
			int *link = (j + (int*)buf); //указатель на 4 символа массива, j нужен для смещения указателя на следующие 4
			encr = 0;
			
			for (int i = 0; i < 32; i++) {
				val = 1 & (*link >> (31 - i)); //сдвиг для работы с конкретным битом
			
				if (val != 0) { //дешифрование
					encr = encr | (1 << (31 - key[i])); //сдвиг 
				}
				val = 0;
			}
			char buffer[4]; //доп. массив для символов
			for (int i = 1; i < 5; i++) {
				buffer[i - 1] = encr >> (32 - (i * 8)); //пеевод биты в символы (1 символ - 8 бит)
			}
			
			for (int i = 0; i < 4; i++) { //записывает массив в обратном порядке в файл
				if (c < size - intminus - 1) {
					fout << buffer[3 - i];
				}
				c++;
			}
		}
		delete[] buf;
	}
}

int main()
{
	//setlocale(LC_ALL, "rus");

	while (true)
	{
		system("cls");
		char ch_key;
		int key[] = { 6, 29, 17, 3, 26, 13, 7, 0, 8, 15, 12, 19, 21, 5, 28, 16, 23, 24, 1, 2, 25, 30, 31, 10, 9 , 14, 27, 18, 22, 4, 11, 20 };
		cout << "1 - Encrypt file.\n";
		cout << "2 - Decrypt file.\n";
		cout << "ESC - Exit.\n";

		switch ((ch_key = _getch()))
		{
		case '1':
			cout << "File Encryption...\n";
			encryption(key);
			cout << "Done.\n";
			system("pause");
			break;

		case '2':
			cout << "Decryption file...\n";
			decryption(key);
			cout << "Done.\n";
			system("pause");
			break;

		case 27:
			cout << "Exit the program....\n";
			exit(0);
			break;

		default:
			break;
		}
	}
	system("pause");
}