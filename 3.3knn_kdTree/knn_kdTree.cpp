#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
using namespace std;

//树结构
struct KdTree
{
	vector<double> root;	//根节点
	KdTree * parent;		//存储父节点
	KdTree * leftChild;		//左子节点
	KdTree * rightChild;	//右子节点
	//默认构造函数 节点初始化为空
	KdTree() { parent = leftChild = rightChild = NULL; }
	//判断kd树是否为空
	bool isEmpty()
	{
		return root.empty();
	}
	//判断是否是子节点
	bool isLeaf()
	{
		return(!root.empty()) && 
			rightChild == NULL && leftChild == NULL;
	}
	//判断是否是根节点
	bool isRoot()
	{
		return (!isEmpty()) && parent == NULL;
	}
	//判断是否是左节点
	bool isLeft()
	{
		return parent->leftChild->root == root;
	}
	//判断是否是右节点
	bool isRight()
	{
		return parent->rightChild->root == root;
	}
};

//以数组形式存储数据
int data1[6][2] = { {2,3},{5,4},{9,6},{4,7},{8,1},{7,2} };

//将传入的矩阵进行转置
template<typename T>
vector<vector<T>> Transpose(vector<vector<T>> Matrix)
{
	unsigned row = Matrix.size();
	unsigned col = Matrix[0].size();
	vector<vector<T>> Trans(col, vector<T>(row, 0));
	for (unsigned i = 0; i < col; ++i)
	{
		for (unsigned j = 0; j < row; ++j)
		{
			Trans[i][j] = Matrix[j][i];
		}
	}
	return Trans;
}
//找中值
template <typename T>
T findMiddleValue(vector<T> vec)
{
	sort(vec.begin(), vec.end());
	auto pos = vec.size() / 2;
	return vec[pos];
}

//构建kd树
void buildKdTree(KdTree * tree, vector<vector<double>> data, unsigned depth)
{
	//样本数量
	unsigned samplesNum = data.size();
	//中止条件
	if (samplesNum == 0)
	{
		return;
	}
	if (samplesNum == 1)
	{
		tree->root = data[0];
		return;
	}
	//样本维度
	unsigned k = data[0].size();
	//调用了Transpose 将矩阵进行了转置
	vector<vector<double>> transData = Transpose(data);
	//选择切分属性
	unsigned splitAttribute = depth % k;
	vector<double> splitAttributeValues = transData[splitAttribute];
	//选择切分值的中值
	double splitValue = findMiddleValue(splitAttributeValues);
	//cout << "splitValue" << splitValue  << endl;

	//根据切分属性和切分中值，将数据集分为两个子集
	vector<vector<double>> subset1;
	vector<vector<double>> subset2;
	//这个循环进行根节点的判断 稍后详细分析
	for (unsigned i = 0; i < samplesNum; ++i)
	{
		if (splitAttributeValues[i] == splitValue && tree->root.empty())
		{
			tree->root = data[i];
		}
		else
		{
			if (splitAttributeValues[i] < splitValue)
				subset1.push_back(data[i]);
			else
				subset2.push_back(data[i]);
		}

		//分配节点
		tree->leftChild = new KdTree;
		tree->leftChild->parent = tree;
		tree->rightChild = new KdTree;
		tree->rightChild->parent = tree;
		//子集递归调用buildKdTree函数 分配
		buildKdTree(tree->leftChild, subset1, depth + 1);
		buildKdTree(tree->rightChild, subset2, depth + 1);
	}


}

//逐层打印kd树
void printKdTree(KdTree *tree, unsigned depth)
{
	for (unsigned i = 0; i < depth; ++i)
		cout << "\t";

	for (vector<double>::size_type j = 0; j < tree->root.size(); ++j)
		cout << tree->root[j] << ",";
	cout << endl;
	if (tree->leftChild == NULL && tree->rightChild == NULL)//叶子节点
		return;
	else //非叶子节点
	{
		if (tree->leftChild != NULL)
		{
			for (unsigned i = 0; i < depth + 1; ++i)
				cout << "\t";
			cout << " left:";
			printKdTree(tree->leftChild, depth + 1);
		}

		cout << endl;
		if (tree->rightChild != NULL)
		{
			for (unsigned i = 0; i < depth + 1; ++i)
				cout << "\t";
			cout << "right:";
			printKdTree(tree->rightChild, depth + 1);
		}
		cout << endl;
	}
}


//计算两点距离
double measureDistance(vector<double> point1, vector<double> point2, unsigned method)
{
	if (point1.size() != point2.size())
	{
		cerr << "Dimensions don't match！！";
		exit(1);
	}
	switch (method)
	{
	case 0://欧氏距离
	{
		double res = 0;
		for (vector<double>::size_type i = 0; i < point1.size(); ++i)
		{
			res += pow((point1[i] - point2[i]), 2);
		}
		return sqrt(res);
	}
	case 1://曼哈顿距离
	{
		double res = 0;
		for (vector<double>::size_type i = 0; i < point1.size(); ++i)
		{
			res += abs(point1[i] - point2[i]);
		}
		return res;
	}
	default:
	{
		cerr << "Invalid method!!" << endl;
		return -1;
	}
	}
}

//算法部分 在kd树中搜索目标点goal的最近邻
//输入：目标点；已构造的kd树
//输出：目标点的最近邻

//搞懂这个函数 需要知道 以下几个变量的意义
//currentNearest	用于保存当前最近点 
//currentTree		当前使用的树
//currentDistance	当前最近点与目标点之间的距离
//searchDistrict	应该是当前的实例点（对应书上 应该是当前节点） 这个变量是一个过度 通过判断保存currentTree的左，右子节点或者整棵树


vector<double> searchNearestNeighbor(vector<double> goal, KdTree *tree)
{
	/*第一步：在kd树中找出包含目标点的叶子结点：从根结点出发，
	递归的向下访问kd树，若目标点的当前维的坐标小于切分点的
	坐标，则移动到左子结点，否则移动到右子结点，直到子结点为
	叶结点为止,以此叶子结点为“当前最近点”
	*/

	unsigned k = tree->root.size();//计算出数据的维数
	unsigned d = 0;//维度初始化为0，即从第1维开始

	KdTree* currentTree = tree;//tree 相当于地址赋值
	vector<double> currentNearest = currentTree->root; //将根节点赋值给新的向量
	
	//循环以不是子节点结束
	while (!currentTree->isLeaf())
	{
		unsigned index = d % k;//计算当前维
		//如果右节点为空 或者 目标点 当前维度上的数值 小于 根节点此维度上的数值
		if (currentTree->rightChild->isEmpty() || goal[index] < currentNearest[index])
		{
			//将左子节点变换为根节点，也可以理解为想左移动
			currentTree = currentTree->leftChild;
		}
		else
		{
			//否则向右移动
			currentTree = currentTree->rightChild;
		}
		//维度切换
		++d;
	}
	//根节点重新赋值 用于判断
	currentNearest = currentTree->root;

	/*第二步：递归地向上回退， 在每个结点进行如下操作：
	(a)如果该结点保存的实例比当前最近点距离目标点更近，则以该例点为“当前最近点”
	(b)当前最近点一定存在于某结点一个子结点对应的区域，检查该子结点的父结点的另
	一子结点对应区域是否有更近的点（即检查另一子结点对应的区域是否与以目标点为球
	心、以目标点与“当前最近点”间的距离为半径的球体相交）；如果相交，可能在另一
	个子结点对应的区域内存在距目标点更近的点，移动到另一个子结点，接着递归进行最
	近邻搜索；如果不相交，向上回退*/

	//当前最近邻与目标点的距离
	double currentDistance = measureDistance(goal, currentNearest, 0);
	//如果当前子kd树的根结点是其父结点的左孩子，则搜索其父结点的右孩子结点所代表的区域，反之亦反
	KdTree* searchDistrict;
	//如果他是一个子节点
	if (currentTree->isLeft())
	{
		//如果他的父节点没有右节点
		if (currentTree->parent->rightChild == NULL)
			searchDistrict = currentTree;
		else
			searchDistrict = currentTree->parent->rightChild;
	}
	else
	{
		searchDistrict = currentTree->parent->leftChild;
	}

	//如果搜索区域对应的子kd树的根结点不是整个kd树的根结点，继续回退搜索
	while (searchDistrict->parent != NULL)
	{
		//搜索区域与目标点的最近距离
		double districtDistance = abs(goal[(d + 1) % k] - searchDistrict->parent->root[(d + 1) % k]);

		//如果“搜索区域与目标点的最近距离”比“当前最近邻与目标点的距离”短，表明搜索区域内可能存在距离目标点更近的点
		if (districtDistance < currentDistance)//&& !searchDistrict->isEmpty()
		{
			//计算目标点向量于此处根节点向量的距离 case0
			double parentDistance = measureDistance(goal, searchDistrict->parent->root, 0);
			//如果case0 距离小于 当前最近邻与目标点的距离
			if (parentDistance < currentDistance)
			{
				//则继续向上 并将距离更新
				currentDistance = parentDistance;
				//实例点（也就是一个树结构）
				currentTree = searchDistrict->parent;
				//更新currentNearest是当前最近点
				currentNearest = currentTree->root;
			}
			//如果此节点不为空
			if (!searchDistrict->isEmpty())
			{
				//目标点与根节点距离
				double rootDistance = measureDistance(goal, searchDistrict->root, 0);
				if (rootDistance < currentDistance)
				{
					currentDistance = rootDistance;
					currentTree = searchDistrict;
					currentNearest = currentTree->root;
				}
			}
			//如果他的左子节点不为空
			if (searchDistrict->leftChild != NULL)
			{
				double leftDistance = measureDistance(goal, searchDistrict->leftChild->root, 0);
				if (leftDistance < currentDistance)
				{
					currentDistance = leftDistance;
					currentTree = searchDistrict;
					currentNearest = currentTree->root;
				}
			}
			if (searchDistrict->rightChild != NULL)
			{
				double rightDistance = measureDistance(goal, searchDistrict->rightChild->root, 0);
				if (rightDistance < currentDistance)
				{
					currentDistance = rightDistance;
					currentTree = searchDistrict;
					currentNearest = currentTree->root;
				}
			}
		}//end if
		//这是个回退的过程
		//如果当前实例点的父节点的父节点不为空
		if (searchDistrict->parent->parent != NULL)
		{
			//三元判断
			//searchDistrict的父节点是叶子节点 则 searchDistrict=父节点的父节点的右节点 否则选择左节点
			searchDistrict = searchDistrict->parent->isLeft() ?
				searchDistrict->parent->parent->rightChild :
				searchDistrict->parent->parent->leftChild;
		}
		else
		{
			searchDistrict = searchDistrict->parent;
		}
		++d;
	}//end while
	//返回当前最近距离
	return currentNearest;

}


int main()
{
	vector<vector<double> > train(6, vector<double>(2, 0));
	for (unsigned i = 0; i < 6; ++i)
		for (unsigned j = 0; j < 2; ++j)
			train[i][j] = data1[i][j];

	KdTree* kdTree = new KdTree;
	buildKdTree(kdTree, train, 0);

	printKdTree(kdTree, 0);

	//传入目标点
	vector<double> goal;
	goal.push_back(3);
	goal.push_back(9);

	vector<double> nearestNeighbor = searchNearestNeighbor(goal, kdTree);
	vector<double>::iterator beg = nearestNeighbor.begin();
	cout << "The nearest neighbor is: ";
	while (beg != nearestNeighbor.end()) cout << *beg++ << ",";
	cout << endl;
	return 0;
}
