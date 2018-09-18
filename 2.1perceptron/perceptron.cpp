#include <iostream>
#include <fstream>

using namespace std;

const int num = 10; //数据集大小（估计）
int count_d = 0;		//数据及大小（实际）
bool flag = true;	//是否分类错误

struct DataSet
{
	double x1;
	double x2;
	int y;
}data1[num];


double lr = 1.0;	//学习率
double w[2] = { 0.0, 0.0 }, b = 0.0;	//参数


//从文件中读取数据
void readData()
{

	ifstream file("perceptron.dat");
	if (!file) {
		cerr << "Can't open the file." << endl;
	}

	int i = 0;
	while (!file.eof())
	{

		file >> data1[i].x1 >> data1[i].x2 >> data1[i].y;
		i++;
		count_d++;

	}

	file.close();
}

int main()
{
	int i;
	int n = 0;	//迭代次数

	readData();//读取数据

	
	//输出数据集
	cout << "Data set is: " << endl;
	for (i = 0; i < count_d; i++)
	{
		cout << data1[i].x1 << " " << data1[i].x2 << " " << data1[i].y << endl;
	}
	while (flag)
	{
		for (i = 0; i < count_d; i++)
		{
			flag = false;
			if (data1[i].y * (w[0] * data1[i].x1 + w[1] * data1[i].x2 + b) <= 0)
				
			{
				n++;

				w[0] = w[0] + lr * data1[i].x1 * data1[i].y;
				w[1] = w[1] + lr * data1[i].x2 * data1[i].y;
				b = b + lr * data1[i].y;

				cout << endl;
				cout << "w1: " << w[0] << endl;
				cout << "w2: " << w[1] << endl;
				cout << "b: " << b << endl;
				flag = true;
				break;

			}

		}
		
	}
	
	cout << endl << "result: " << endl;
	cout << "w = " << w[0] << ", " << w[1] << endl << "b=" << b << endl;
	cout << "iteration count: " << n << endl;

	return 0;
}
