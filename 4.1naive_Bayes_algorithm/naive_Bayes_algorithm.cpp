#include <iostream>
#include <map>
#include <fstream>
using namespace std;

const int num = 20; //数组大小
int count_d = 0;	//数组使用量计数

struct DataSet
{
	int x1;
	char x2;
	int y;
}data1[num];
//从文件中读取数据
void readData()
{

	ifstream file("data.dat");
	if (!file) {
		cerr << "Can't open the file." << endl;
	}
	else
	{
		int i = 0;

		while (!file.eof())
		{
			file >> data1[i].x1 >> data1[i].x2 >> data1[i].y;
			i++;
			count_d++;
		}

		file.close();
	}
	
}
void print_data()
{
	//输出数据集
	cout << "Data set is: " << endl;
	for (int i = 0; i < count_d; i++)
	{
		cout << data1[i].x1 << " " << data1[i].x2 << " " << data1[i].y << endl;
	}
}

//N = count_d
//这个算法例子中需要求出 三个概率 一个是 y=1的概率  一个是 x1 = 某个值的时 y=1的概率 另一个是 x2 = 某个值时 y=1的概率
//下面的算法只累计输入测试的值 
int main()
{
	readData();
	print_data();
	int test_x1; 
	char test_x2{};
	cout << "input num: ";
	cin >> test_x1; 
	cout << "input char: ";
	cin >> test_x2; 
	map<string, int> ycount;//记录y =1 -1的次数 
	map<string, int> x1count;//记录y=1,-1 ，x1匹配的次数 
	map<string, int> x2count;//记录y=1,-1 ，x2匹配的次数 

	for (int i = 0; i < count_d; i++)
	{
		if (data1[i].y == 1) 
		{
			ycount["y=1"]++;
			if (data1[i].x1 == test_x1)
			{
				x1count["y=1"]++;
			}
			if (data1[i].x2 == test_x2) 
			{
				x2count["y=1"]++;
			}
		}
		else
		{
			ycount["y=-1"]++;
			if (data1[i].x1 == test_x1)
			{
				x1count["y=-1"]++;
			}
			if (data1[i].x2 == test_x2) 
			{
				x2count["y=-1"]++;
			}
		}
	}

	//这里需要计算正反两个标签的概率 后 进行比较 得出预测 没有训练 就是三个概率相乘判断概率大的是正确分类
	double p_p1 = ((double)ycount["y=1"] / count_d) *
		((double)x1count["y=1"] / (double)ycount["y=1"]) * 
		((double)x2count["y=1"] / (double)ycount["y=1"]);
	double p_n1 = ((double)ycount["y=-1"] / count_d) * 
		((double)x1count["y=-1"] / (double)ycount["y=-1"]) * 
		((double)x2count["y=-1"] / (double)ycount["y=-1"]);
	cout << " y=1 p: " << p_p1 << endl;
	cout << " y=-1 p: " << p_n1 << endl;
	(p_n1 > p_n1) ? cout << "y = 1" : cout << "y = -1";
	cout << endl;
	system("pause");
	return 0;
}