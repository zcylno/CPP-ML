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

double lr = 1.0;
double b = 0.0;	//参数
double alpha[] = { 0.0, 0.0, 0.0 };		//对偶参数
double G[3][3];

//计算Gram矩阵
void Gram(DataSet data2[])
{
	cout << "Gram:" << endl;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{	
			
			G[i][j] = data2[i].x1 * data2[j].x1 + data2[i].x2 * data2[j].x2;
			
			cout << G[i][j] << "\t";
			if (j == 2)
				cout << endl;

		}
}

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
	int n = 1;	//迭代次数
	int index = 0;


	readData();//读取数据

	//输出数据集
	cout << "Data set is: " << endl;
	for (i = 0; i < count_d; i++)
	{
		cout << data1[i].x1 << " " << data1[i].x2 << " " << data1[i].y << endl;
	}

	//调用Gram函数
	Gram(data1);

	while (index<3)
	{
		int temp = 0;
		for (int i = 0; i < 3; i++)
		{
			temp += alpha[i] * data1[i].y * G[i][index];
			
		}
		cout << "temp: " << temp << endl;
		int all = data1[index].y * (temp + b);

		if (all <= 0)
			{
				alpha[index] = alpha[index] + lr;
				b = b + data1[index].y;
				index = 0;

			}
		else
		{
			index++;
		}

	}

	cout << "alpha: " << alpha[0] << " " << alpha[1] << " " << alpha[2] << endl;
	cout << "b: " << b;

	int w[] = { 0,0 };
	for (int i = 0; i < 3; i++)
	{
		w[0] += alpha[i] * data1[i].y * data1[i].x1;
		w[1] += alpha[i] * data1[i].y * data1[i].x2;
	}
	b = 0;
	for (int i = 0; i < 3; i++)
	{
		b += alpha[i] * data1[i].y;
	}
	
	cout << endl << "w:" << w[0] << " " << w[1] << endl;
	cout << "b:" << b;
	getchar();

	return 0;
}
