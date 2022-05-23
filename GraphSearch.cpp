#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <Windows.h>
#include <fstream>

int N = 0; //количество вершин
using namespace std;

void Dijkstra(int start, int D[], int P[], int** C, int euristic[]);
void path(int P[], int b, int euristic[]);

ifstream file("C:/Users/768/source/repos/GraphSearch/graph.txt");
void setN()
{
    file >> N;
    cout << "Количество вершин: " << N << endl;
}

void setEuristic(int* euristic)
{
    cout << "\nМатрица эвристических значимостей: " << endl;
    for (int i = 0; i < N; i++)
    {
        file >> euristic[i];
        cout << setw(6) << right << euristic[i];
    }
    cout << endl;
}

void setMass(int** mass)
{
    cout << "Матрица весовых коэффициентов: " << endl;
    for (int i = 0; i < N; i++) //ее вывод
    {

        for (int j = 0; j < N; j++)
        {
            file >> mass[i][j];
            cout << setw(6) << right << mass[i][j];
        }
        cout << endl;
    }
}

void outTest(int P[], int D[])
{
    for (int i = 0; i < N; i++)
        cout << setw(6) << left << D[i];
    cout << endl;
    for (int i = 0; i < N; i++)
        cout << setw(6) << left << P[i];
    cout << endl;
}

int main()
{
    SetConsoleOutputCP(1251);
    if (!file.is_open())
        cout << "Файл не открыт." << endl;

    setN();
    int *euristic = new int[N];
    setEuristic(euristic);
    
    /**Задание количества ребер графа и весовых коэффициентов**/
    int **mass; //весовая матрица
    mass = new int*[N];
    for (int i = 0; i < N; i++)
        mass[i] = new int[N];
    setMass(mass);
    
    /**Задание начальной и целевой вершин**/
    int a = 0, b = 0;
    cout << "Введите начальную и целевую вершины для поиска: ";
    while (a == b || a > 15 || b > 15)
        cin >> a >> b;
    a--; b--;
    cout << "Начальная вершина: " << a+1 << " (эвристическая оценка " << euristic[a] << ")" << endl;
    cout << "Целевая вершина: " << b+1 << " (эвристическая оценка " << euristic[b] << ")" << endl;

    /**Поиск пути алгоритмом 'A*' **/
    int *D = new int[N],
        *P = new int[N];
    Dijkstra(a, D, P, mass, euristic);
    //outTest(P, D);
    cout << "Длина пути: " << D[b] + euristic[a] << endl;

    if (D[b] < 65000)
        path(P, b, euristic);
    else
        cout << "Нет пути из " << a+1 << " в " << b+1 << endl;

    delete[]euristic;
    for (int i = 0; i < N; i++)
        delete[]mass[i];
    delete[]D;
    delete[]P;
    file.close();
}

void outVertex(int ver, int **mass, int euristic[])
{
    cout << endl;
    cout << "Текущая вершина: " << euristic[ver] << endl;
    cout << "Имеющиеся из нее пути: ";
    for (int i = 0; i < N; i++)
        if (mass[ver][i] && mass[ver][i] < 65000)
            cout << setw(6) << right << euristic[i];
    cout << endl;
}

/**C - матрица весов, 
D - массив кратчайших путей, 
P - массив номеров вершин в текущем кратчайшем пути**/
void Dijkstra(int start, int D[], int P[], int **C, int euristic[]) 
{
    int i, v, w, *S = new int[N];
    for (i = 0; i < N; i++) //заполнение массива открытых вершин (в начале все закрыты)
        S[i] = 0;
    S[start] = 1;
    for (i = 0; i < N; i++)
    {
        D[i] = C[start][i] + euristic[i];
        P[i] = start;
    }

    D[start] = 65000; //начальное значение для поиска минимума
    P[start] = -1; //стартовая вершина - ей не предшествуют другие
    outVertex(0, C, euristic);
    outVertex(1, C, euristic);
    for (i = 2; i < N; i++) //промежуточные узлы
    {
        cout << "Раскрытые вершины: ";
        for (int j = 0; j < N; j++)
            if (S[j])
                cout << setw(6) << right << euristic[j]; //вывод массива раскрытых вершин на каждом шаге
        cout << endl;
        outVertex(i, C, euristic);

        for (w = start, v = 0; v < N; v++)
            if (!S[v] && D[v] < D[w]) //выбор такой вершины из S, что D[w] минимально
                w = v;
        S[w] = 1;
        for (v = 0; v < N; v++)
            if (D[w] + C[w][v] < D[v])
            {
                D[v] = D[w] + C[w][v] + euristic[v];
                P[v] = w; //запись предшествующей v вершины
            }
    }
    D[start] = euristic[start];
}

void path(int P[], int b, int euristic[])
{
    cout << "\nПуть (в формате эвристических значений): " << euristic[b];
    int i = 0;
    while (P[b] > -1)
    {
        b = P[b];
        cout << " <- " << euristic[b];
        if (i++ > 50)
        {
            cout << "Граф имеет циклы. " << endl;
            return;
        }
    }
    cout << endl;
}