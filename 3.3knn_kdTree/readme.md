//搞懂这个函数 需要知道 以下几个变量的意义
//currentNearest	用于保存当前最近点 
//currentTree		当前使用的树
//currentDistance	当前最近点与目标点之间的距离
//searchDistrict	应该是当前的实例点（对应书上 应该是当前节点） 这个变量是一个过度 通过判断保存currentTree的左，右子节点或者整棵树


vector<double> searchNearestNeighbor(vector<double> goal, KdTree *tree)