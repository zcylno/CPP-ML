#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

const int num = 10; //数据集大小（估计）
int count_d = 0;		//数据及大小（实际）

struct DataSet
{
	double x1;
	double x2;

}data1[num];

//从文件中读取数据
void readData()
{
	ifstream file("k_NN.dat");
	if (!file) {
		cerr << "Can't open the file." << endl;
	}
	int i = 0;
	while (!file.eof())
	{
		file >> data1[i].x1 >> data1[i].x2;
		i++;
		count_d++;
	}
	file.close();
}

void print_data()
{
	//输出数据集
	cout << "Data set is: " << endl;
	for (int i = 0; i < count_d; i++)
	{
		cout << data1[i].x1 << " " << data1[i].x2 << endl;
	}
}

double calulate_distance(DataSet v1,DataSet v2, double p)
{
	double result = 0.0;

	result = ( pow(abs(v1.x1 - v2.x1), p) + pow(abs(v1.x2 - v2.x2), p) );
	result = pow(result, (1.0 / p));

	return result;
}

int main()
{
	readData();		//读取数据
	print_data();	//打印数据

	double distance[3][4];
	int count_n = 0;
	for (int i = 0; i < (count_d-1); i++) 
	{
		for (int j = (i+1); j < count_d; j++)
		{
			for (int k = 1; k < 5; k++)
			{
				distance[count_n][k-1] = calulate_distance(data1[i], data1[j], k);
			}
			count_n++;
		}
	}
	cout << "value: " << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << distance[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}
