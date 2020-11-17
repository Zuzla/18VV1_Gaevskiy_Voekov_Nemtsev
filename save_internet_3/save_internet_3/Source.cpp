#include <Windows.h>
#include <time.h>
#include <stdio.h>

#define SIZE_MASS 10

#define COUNT_ITER 1000

// Оценка распределения генерируемых чисел
void distribution(UINT8 integer, UINT8 freq[SIZE_MASS])
{
	double temp = static_cast<double>(integer);
	temp = temp / 255;

	if (temp <= 0.1)
		freq[0]++;
	else if (temp <= 0.2)
		freq[1]++;
	else if (temp <= 0.3)
		freq[2]++;
	else if (temp <= 0.4)
		freq[3]++;
	else if (temp <= 0.5)
		freq[4]++;
	else if (temp <= 0.6)
		freq[5]++;
	else if (temp <= 0.7)
		freq[6]++;
	else if (temp <= 0.8)
		freq[7]++;
	else if (temp <= 0.9)
		freq[8]++;
	else if (temp <= 1)
		freq[9]++;

}

int main()
{
	UINT8 freq[SIZE_MASS] = { 0 };
	UINT16 startnum = (UINT16)time(0); // Начальное значение

	UINT16 sum = startnum;

	for (int i = 0; i < COUNT_ITER; i++)
	{
		UINT16 num1 = sum >> 4; // Сдвиг вправо на четверть длины ячейки
		UINT16 num2 = sum << 4; // Сдвиг влево на четверть длины ячейки
		sum = num1 + num2; // Сумма двух предыдущих чисел дает новое случайное число
		UINT8 res = static_cast<UINT8>(sum); // Результат должен быть 8-битным 
		printf("%d\n", res); // Выводим на экран псевдослучайное число
		distribution(res, freq); // Оценка распределения генерируемых чисел
	}

	// Выводим на экран оценку распределения генерируемых чисел
	printf("\nDistribution for %d:\n", startnum);
	for (int i = 0; i < SIZE_MASS; i++)
	{
		printf("%d = %d\n", i, freq[i]);
	}
	return 0;
}
