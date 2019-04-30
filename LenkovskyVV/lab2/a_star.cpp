#include <cmath>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <stack>

#define FINISH 0

#define type char

#define SIZE 10

using namespace std;

static int flag_adm=0;
static int flag_mon=0;

struct Vertex
{
	type value;		//название вершины
	double weight;		//вес пути до этой вершины
	type from;		//родитель этой вершины
};

class Directed_Graph{
private:
	type *list;		//список
	size_t size_l = 0;	//размер списка
	double **matrix;	//хранит матрицу смежности вершин

public:
	Directed_Graph()
	: list(new type[SIZE])
	{
		matrix = new double*[SIZE];
		for(int i = 0; i < SIZE; i++)
			matrix[i] = new double[SIZE]();
	}

	void print()		//вывод графа
	{
		for(int i = 0; i < size_l; i++)
		{
			cout << list[i] << " : ";
			for(int j = 0; j < size_l; j++)
				if(matrix[i][j] != 0)
					cout << list[j] << "(" << matrix[i][j] << ") ";
			cout << endl;
		}
	}

	void push(const type key, const type value, const float weight)		//добавление элемента
	{
		size_t k = find_or_add(key);
		size_t v = find_or_add(value);
		matrix[k][v] = weight;
	}

	size_t find_or_add(const type litter)		//находит элемент, добавляя его
	{
		for(int i = 0; i < size_l; i++)
			if(list[i] == litter)
				return i;
		resize_list();
		list[size_l] = litter;
		return size_l++;
	}

	void resize_list()		//изменяет размер списка
	{
		if(size_l%SIZE != 0 || !size_l) return;

		type *new_list = new type[size_l + SIZE];
		for(int i = 0; i < size_l; i++)
			new_list[i] = list[i];
		delete [] list;
		list = new_list;

		double **new_matrix = new double*[size_l + SIZE];
		for(int i = 0; i < size_l + SIZE; i++)
			new_matrix[i] = new double[size_l + SIZE]();

		for(int i = 0; i < size_l; i++)
		{
			for(int j = 0; j < size_l; j++)
				new_matrix[i][j] = matrix[i][j];
			delete [] matrix[i];
		}
		delete [] matrix;
		matrix = new_matrix;
	}

	bool in_arr(type c, vector <Vertex> &arr)		//сообщает есть ли искомый элемент в массиве
	{
		for(int i = 0; i < arr.size(); i++)
			if(arr[i].value == c)
				return 1;
		return 0;
	}

	size_t index_in_arr(type c, vector <Vertex> &arr)	//находит номер элемента в массиве
	{
		for(size_t i = 0; i < arr.size(); i++)
			if(arr[i].value == c)
				return i;
		return 0;
	}

	size_t min_arr(vector <Vertex> &arr, size_t finish)	//находит номер минимального элемента в массиве
	{
		size_t min = 0;
		for(int i = 0; i < arr.size(); i++)
		{
			if(arr[i].weight + abs(arr[i].value - list[finish]) < arr[min].weight + abs(arr[min].value - list[finish]))
				min	= i;
			if(arr[i].weight + abs(arr[i].value - list[finish]) == arr[min].weight + abs(arr[min].value - list[finish]))
				if(abs(arr[i].value - list[finish]) <  abs(arr[min].value - list[finish]))
					min = i;
		}
		return min;
	}

	void find_way(size_t start, size_t finish, stack <type> &way)	//находит минимальный путь, так же проверяет функцию на допустимость и монотонность
	{
			cout <<	"В ожидающие добавляется точка старта(" << list[start] << ") , список рассмотренных пока пуст."  << endl;

		vector <Vertex> arr_wait;//Ожидаемые вершины
		vector <Vertex> arr_past;//Просмотренные вершины

		arr_wait.push_back({list[start], 0, 0});//Добавляем первую вершину

		Vertex X, Y;
		while(!arr_wait.empty())//Пока есть вершины в списке ожидания
		{

			vector <Vertex>::iterator it = arr_wait.begin();
			size_t min = min_arr(arr_wait, finish);//Достаем минимальный элемент
			advance(it, min);
			X = arr_wait.at(min);//Помещаем в Х
				cout << "Список на рассмотрение : " << endl;
				for(int i = 0; i < arr_wait.size(); i++)
					cout << arr_wait[i].value << " F(" << arr_wait[i].weight  + abs(arr_wait[i].value - list[finish]) << "), ";
				cout << endl;
			arr_wait.erase(it);

				cout << "Наименьшая точка из списка на рассмотрение: " << X.value << endl << endl;

			arr_past.push_back(X);
			if(X.value == list[finish])
				break;

				cout << "Инцидентные вершины для (" << X.value << ") : " << endl; 

			for(int i = 0; i < size_l; i++)
			{

				size_t index_x = find_or_add(X.value);
				if(matrix[index_x][i] != 0)
				{
						cout << list[i] << " G(" << matrix[index_x][i] + X.weight << ") H(" << abs(list[i] - list[finish]) << ") F(";
						if(abs(list[i] - list[finish]) > index_x)
							flag_adm = 1;
						if(X.weight < (list[i]-list[finish]))
							flag_mon = 1;
						cout << matrix[index_x][i] + X.weight + abs(list[i] - list[finish]) << ")  - "; 
					Y = {list[i], matrix[index_x][i] + X.weight, X.value};
					if(in_arr(Y.value, arr_past))
					{
							cout << "Вершина уже находится в рассмотренном списке, пропускаем ее." << endl; 

						continue;
					}
					if(!in_arr(Y.value, arr_wait))
					{
							cout << "Вершины нет рассмотренном списке и в списке ожидания. Добавляем ее в список на рассмотрение." << endl; 

						arr_wait.push_back(Y);
					}
					else
					{
							cout << "Вершины нет рассмотренном списке но есть списке ожидания." << endl; 

						Vertex &old_Y = arr_wait[index_in_arr(Y.value, arr_wait)];
						if(Y.weight < old_Y.weight)
						{
								cout << "Новый путь до вершины лучше предыдущего. Заменяем предыдущий им." << endl; 

							old_Y.weight = Y.weight;
							old_Y.from = Y.from;
						}
						else
							cout << "Новый путь до вершины не лучше предыдущего, ничего не делаем." << endl; 

					}
				}
			}
		}
		if(!in_arr(list[finish], arr_past))
			return;

			cout << "Маршрут найден." << endl;

		Vertex *ptr = &X;
		while(ptr->value != list[start])
		{
			way.push(ptr->value);
			ptr = &arr_past[index_in_arr(ptr->from, arr_past)];
		}
		way.push(ptr->value);
		if(flag_adm == 1)
			cout << "Функция допустима" << endl;
		else
			cout << "Функция не допустима" << endl;
		if(flag_mon == 1)
			cout << "Функция монотонная" << endl;
		else
			cout << "Функция не монотонная" << endl;
	}

};

int main(){
     setlocale(LC_ALL, "rus");

	type start, finish;
	cin >> start >> finish;

	Directed_Graph graf;
	type from, to;
	double weight;
	while(cin >> from >> to >> weight)//Считыние данных
	{
		if(from == '^')
			break;
		graf.push(from, to, weight);
	}
	cout << "Полученный граф :" << endl;
	graf.print();
	cout << endl;
	stack <type> way;
	graf.find_way(graf.find_or_add(start), graf.find_or_add(finish), way);

		cout << "Найденный путь от " << start << " до " << finish << " : ";
	while(!way.empty())
	{
		cout << way.top();
		way.pop();
	}
	cout << endl;

	return 0;
}
