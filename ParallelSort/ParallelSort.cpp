#include <omp.h>
#include <stdio.h>
#include <conio.h>
#include <chrono>
#include <random>
#include <iostream>

using namespace std;

void BubbleSort(int* A, int size);
void InsertSort(int* A, int size);
int compare(const void* x, const void* y);
int* GenerateRandomArray(int size, int range);
int* GenerateSortedArray(int size);
int* GenerateRSortedArray(int size);
void MPBubbleSort(int* A, int size);
void MPInsertSort(int* A, int size);
void Merge(const int* a, const int* b, int n, int m, int* c);

int main()
{
	int size, mode;
	int* Array = new int[0];
	while (true)
	{
		cout << "Enter size of array\n(Enter '0' to generate random array)\n(Enter '-1' to generate the best array (sorted))\n(Enter '-2' to generate the worst array (reverse sorted))\n";
		cin >> size;
		if (size > 0)
		{
			Array = new int[size];

			for (int i = 0; i < size; i++)
			{
				cout << "Enter element of array:\n";
				cin >> Array[i];
			}
		}
		else if (size == 0)
		{
			int n;
			cout << "Enter size of random array\n";
			cin >> size;
			cout << "Numbers will be in range (0,n).\n Enter n: \n";
			cin >> n;
			Array = GenerateRandomArray(size, n);
		}
		else if (size == -1)
		{
			cout << "Enter size of sorted array\n";
			cin >> size;
			Array = GenerateSortedArray(size);
		}
		else if (size == -2)
		{
			cout << "Enter size of reverse sorted array\n";
			cin >> size;
			Array = GenerateRSortedArray(size);
		}

		cout << "Your array:\n";
		for (int i = 0; i < size; i++)
		{
			cout << Array[i] << " ";
		}

		cout << "\n\nChoose sort method:\n 1: bubbleSort\n 2: insertSort\n 3: quickSort\n 4: MPbubblesort \n 5: MPinsertsort \n";
		cin >> mode;
		if (mode == 1)
		{
			cout << "\nBubbleSort:\n";

			double t1 = omp_get_wtime();

			BubbleSort(Array, size);

			double t2 = omp_get_wtime();

			for (int i = 0; i < size; i++)
			{
				cout << Array[i] << " ";
			}
			cout << "\n\n";
			cout << "The sort time:" << t2 - t1 << "\n\n";
		}
		else if (mode == 2)
		{
			cout << "\nInsertSort:\n";

			double t1 = omp_get_wtime();

			InsertSort(Array, size);

			double t2 = omp_get_wtime();

			for (int i = 0; i < size; i++)
			{
				cout << Array[i] << " ";
			}
			cout << "\n\n";
			cout << "The sort time: " << t2 - t1 << "\n\n";
		}
		else if (mode == 3)
		{
			cout << "\nQuickSort:\n";

			double t1 = omp_get_wtime();

			qsort(Array, size, sizeof(int), compare);

			double t2 = omp_get_wtime();

			for (int i = 0; i < size; i++)
			{
				cout << Array[i] << " ";
			}
			cout << "\n\n";
			cout << "The sort time:" << t2 - t1 << "\n\n";
		}
		else if (mode == 4)
		{
			cout << "MPBubbleSort: \n";

			double t1 = omp_get_wtime();

			MPBubbleSort(Array, size);

			double t2 = omp_get_wtime();

			for (int i = 0; i < size; i++)
			{
				cout << Array[i] << " ";
			}
			cout << "\n\n";
			cout << "The sort time:" << t2 - t1 << "\n\n";
		}
		else if (mode == 5)
		{
			cout << "MPInsertSort: \n";

			double t1 = omp_get_wtime();

			MPInsertSort(Array, size);

			double t2 = omp_get_wtime();

			for (int i = 0; i < size; i++)
			{
				cout << Array[i] << " ";
			}
			cout << "\n\n";
			cout << "The sort time:" << t2 - t1 << "\n\n";
		}
		delete[] Array;
	}
}


void BubbleSort(int* A, int size)
{
	for (int N = 0; N < size; N++)
		for (int i = 1; i < size - N; i++)
			if (A[i] < A[i - 1])
			{
				int tempEl = A[i];
				A[i] = A[i - 1];
				A[i - 1] = tempEl;
			}
}


void InsertSort(int* A, int size)
{
	for (int i = 1; i < size; i++)
	{
		int tempEl = A[i];
		int j = i - 1;
		while (j >= 0 && A[j] > tempEl)
		{
			A[j + 1] = A[j];
			j = j - 1;
		}
		A[j + 1] = tempEl;
	}
}

int* GenerateRandomArray(int size, int range)
{
	random_device rd;
	mt19937::result_type seed = rd() ^ ((mt19937::result_type)
		chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count() + (mt19937::result_type)
		chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
	mt19937 gen(seed);
	uniform_int_distribution<unsigned> distrib(0, range);

	int* A = new int[size];
	for (int i = 0; i < size; i++)
	{
		A[i] = distrib(gen);
	}
	return A;
}

int* GenerateSortedArray(int size)
{
	int* A = new int[size];
	for (int i = 0; i < size; i++)
	{
		A[i] = i;
	}
	return A;
}
int* GenerateRSortedArray(int size)
{
	int* A = new int[size];
	for (int i = 0; i < size; i++)
	{
		A[i] = size - i;
	}
	return A;
}


int compare(const void* x, const void* y)
{
	return (*(int*)x - *(int*)y);
}




void MPBubbleSort(int* A, int size)
{
	// Алгоритм параллельной сортировки пузырьком для 4(!) потоков
	int p = 4;
	int partLen = size / p;
	int lastPartLen = partLen;
	if (size % partLen != 0)
	{
		lastPartLen = partLen + (size % partLen);
	}

	int** mas;
	mas = new int* [p];
	for (int i = 0; i < p - 1; i++)
	{
		mas[i] = new int[partLen];
	}
	mas[p - 1] = new int[lastPartLen];
	for (int i = 0; i < p - 1; i++)
	{
		for (int j = 0; j < partLen; j++)
		{
			mas[i][j] = A[j + i * partLen];
		}
	}
	for (int j = 0; j < lastPartLen; j++)
	{
		mas[p - 1][j] = A[j + (p - 1) * partLen];
	}
#pragma omp parallel for
	for (int i = 0; i < p; i++)
	{
		if (i < p - 1)
		{
			BubbleSort(mas[i], partLen);
		}
		else
		{
			BubbleSort(mas[i], lastPartLen);
		}
	}

	
		//Вывод массива осортированных частей
		for (int i = 0; i < p - 1; i++)
		{
			for (int j = 0; j < partLen; j++)
			{
				cout << mas[i][j] << " ";
			}
			cout << "\n";
		}
		for (int j = 0; j < lastPartLen; j++)
		{
			cout << mas[p - 1][j] << " ";
		}
		cout << "\n";
		//
	
	int* half1 = new int[2 * partLen];
	int* half2 = new int[partLen + lastPartLen];
#pragma omp parallel sections
	{
#pragma omp section
		{
			Merge(mas[0], mas[1], partLen, partLen, half1);
		}
#pragma omp section
		{

			cout << omp_get_thread_num();
			Merge(mas[2], mas[3], partLen, lastPartLen, half2);
		}
	}

	Merge(half1, half2, 2 * partLen, partLen + lastPartLen, A);



	for (int i = 0; i < p; i++)
		delete[] mas[i];
	delete[] mas;
	delete[] half1;
	delete[] half2;
}


void MPInsertSort(int* A, int size)
{
	// Алгоритм параллельной сортировки вставками для 4(!) потоков
	int p = 4;
	int partLen = size / p;
	int lastPartLen = partLen;
	if (size % partLen != 0)
	{
		lastPartLen = partLen + (size % partLen);
	}

	int** mas;
	mas = new int* [p];
	for (int i = 0; i < p - 1; i++)
	{
		mas[i] = new int[partLen];
	}
	mas[p - 1] = new int[lastPartLen];
	for (int i = 0; i < p - 1; i++)
	{
		for (int j = 0; j < partLen; j++)
		{
			mas[i][j] = A[j + i * partLen];
		}
	}
	for (int j = 0; j < lastPartLen; j++)
	{
		mas[p - 1][j] = A[j + (p - 1) * partLen];
	}
#pragma omp parallel for
	for (int i = 0; i < p; i++)
	{
		if (i < p - 1)
		{
			InsertSort(mas[i], partLen);
		}
		else
		{
			InsertSort(mas[i], lastPartLen);
		}
	}

	int* half1 = new int[2 * partLen];
	int* half2 = new int[partLen + lastPartLen];
#pragma omp parallel sections
	{
#pragma omp section
		{
			Merge(mas[0], mas[1], partLen, partLen, half1);
		}
#pragma omp section
		{
			Merge(mas[2], mas[3], partLen, lastPartLen, half2);
		}
	}

	Merge(half1, half2, 2 * partLen, partLen + lastPartLen, A);

	for (int i = 0; i < p; i++)
		delete[] mas[i];
	delete[] mas;
	delete[] half1;
	delete[] half2;
}


void Merge(const int* a, const int* b, int n, int m, int* c)
{
	int i, j, k;
	i = j = k = 0;
	while (i < n && j < m) {
		if (a[i] < b[j]) {
			c[k++] = a[i++];
		}
		else {
			c[k++] = b[j++];
		}
	}
	while (i < n) {
		c[k++] = a[i++];
	}
	while (j < m) {
		c[k++] = b[j++];
	}
}