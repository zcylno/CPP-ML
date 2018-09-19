//由于不是很会C++，在练习中，所以算法3.2并没有是由递归 模板 向量 等工具
//只是将算法的意思用代码的方式解释了一下，见谅


#include <fstream>
#include <iostream>
#include<algorithm>

using namespace std;

const int num = 10; //数据集大小（估计）
int count_d = 0;		//数据及大小（实际）

struct DataSet
{
	double x1;	//例3.2 二维数据，所以有x1和x2
	double x2;
	int number; // 每个数据的原始编号
	int id;		// 建成树后从左到右的顺序编码 按id排序 便可看的更明显 未作排序
	int pan;	// 父类
	int lcn;	//左子类
	int rcn;	//右子类

}data1[num];


//从文件中读取数据
void readData()
{
	ifstream file("k_dimension_tree.dat");
	if (!file) {
		cerr << "Can't open the file." << endl;
	}
	int i = 0;
	while (!file.eof())
	{
		file >> data1[i].x1 >> data1[i].x2 >> data1[i].number;
		i++;
		count_d++;
	}
	file.close();
}

void print_data(DataSet data[])
{
	//输出数据集
	cout << "Data set is: " << endl;
	for (int i = 0; i < count_d; i++)
	{
		cout << data[i].x1 << " " << data[i].x2 << " number: " << data[i].number << " pan: " << data[i].pan << " id: " << data[i].id << " lcn: " << data[i].lcn<< " rcn: " << data[i].rcn << endl;
	}
}

//排列x1
bool cmp1(const DataSet &a, const DataSet &b)
{
	return a.x1 < b.x1;
}
//排列x2
bool cmp2(const DataSet &a, const DataSet &b)
{
	return (a.x2 < b.x2);
}

//kd树 
//函数内部是按顺序，取中止，排序，分配节点的，并没有为了循环或递归进行修改
void createTree(DataSet data[])
{
	//x1小到大排序
	DataSet *p = data;
	sort(p, p + count_d, cmp1);
	print_data(data);

	int t = count_d / 2;

	data[t].id = 1;
	//x2小到大排序
	sort(p, p + t, cmp2);
	data[t / 2].pan = data[t].number;
	data[t].lcn = data[t / 2].number;
	data[t / 2].id = 2;
	sort(p + t + 1, p + count_d, cmp2);
	
	data[t + 1 + (count_d - t) / 2].pan = data[t].number;
	data[t].rcn = data[t + 1 + (count_d - t) / 2].number;
	data[t + 1 + (count_d - t) / 2].id = 3;
	
	//由于是已知数据 所以直接分配了
	data[0].pan = data[t / 2].number;
	data[0].id = 4;
	data[2].pan = data[t / 2].number;
	data[2].id = 5;
	data[t / 2].lcn = data[0].number;
	data[t / 2].rcn = data[2].number;
	
	data[t+1].pan = data[t + 1 + (count_d - t) / 2].number;
	data[t+1].id = 6;
	data[t + 1 + (count_d - t) / 2].lcn = data[t+1].number;

	print_data(data);
}

int main()
{
	readData();
	print_data(data1);
	createTree(data1);
	return 0;
}