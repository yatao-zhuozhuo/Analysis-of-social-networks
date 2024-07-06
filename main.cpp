#include<iostream>
#include<fstream>
#include<string>
#include<queue>

using namespace std;

struct  Edge {
	int name;
	Edge *link;
	Edge(int n =0,Edge* l = nullptr) :link(l),name(n) {};
	Edge(int num) :name(num), link(nullptr) {}
};

struct Vertex {
	int data;
	Edge* adj;
};

class Graphlnk {
public:
	Graphlnk(int sz);
	~Graphlnk();
	int getValue(int i) {
		return (i >= 0 && i < numVertices) ? NodeTable[i].data : 0;
	}
	//图的基础操作
	bool insertVertex(const int& vertex);   bool removeEdge(int v1, int v2);
	bool removeVertex(int v);          bool insertEdge(int v1, int v2);
	int getFirstNeighbor(int v);   int getNextNeighbor(int v, int w);
	int getVertexPos(const int vertex) {
		for (int i = 0;i < numVertices;i++)
			if (NodeTable[i].data == vertex) return i;
		return -1;
	}
	int direct_friend_num(int v);//简略统计直接朋友
	bool areConnected(int v, int w);    void DFS(int node, bool* visited);
	//用来提取封装掉地数据
	int getmaxVertices() { return maxVertices; };  int getnumEdges() { return numEdges; }
	int getnumVertices() { return numVertices; }

	Vertex* NodeTable;//封装入private会带来诸多不便，为节约时间没有封装

private://对类中的部分信息封装，体现对用户信息的保护
	int maxVertices;
	int numEdges;
	int numVertices;
};

Graphlnk::Graphlnk(int sz) {//类的构造函数
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	NodeTable = new Vertex[maxVertices];
	if (NodeTable == nullptr) { cerr << "存储分配错误" << endl; }
	for (int i = 0;i < maxVertices;i++)NodeTable[i].adj = nullptr;
}

Graphlnk::~Graphlnk() {//类的析构函数
	for (int i = 0;i < numVertices;i++) {
		Edge* p = NodeTable[i].adj;
		while (p != nullptr) {
			NodeTable[i].adj = p->link;
			delete p;p = NodeTable[i].adj;
		}
	}
	delete[]NodeTable;
}

int Graphlnk::getFirstNeighbor(int v) {
	if (v != -1) {
		Edge * p = NodeTable[v].adj;
		if (p != nullptr) return p->name;
	}
	return -1;
}

int Graphlnk::getNextNeighbor(int v, int w) {
	if (v != -1) {
		Edge * p = NodeTable[v].adj;
		while (p != nullptr && p->name != w)
			p = p->link;
		if (p != nullptr && p -> link != nullptr)
			return p->link->name;
	}
	return -1;
}

bool Graphlnk::insertVertex(const int & vertex) {//插入用户
	if (numVertices == maxVertices) return false;
	NodeTable[numVertices].data = vertex;
	numVertices++;
	return true;
}

bool Graphlnk::insertEdge(int v1, int v2) {//建立两个用户之间的关系
	// 检查顶点索引是否有效
	if (v1 >= 0 && v1 < maxVertices && v2 >= 0 && v2 < maxVertices) {
		Edge* q, * p = NodeTable[v1].adj;
		while (p != nullptr && p->name != v2)
			p = p->link;
		if (p != nullptr)
			return false; 
		p = new Edge;
		p->name = v2;
		p->link = NodeTable[v1].adj;
		NodeTable[v1].adj = p;
		q = new Edge;
		q->name = v1;
		q->link = NodeTable[v2].adj;
		NodeTable[v2].adj = q;
		numEdges++;  
		return true; 
	}
	return false;
}

void super_friend(Graphlnk& human,ofstream& outputFile) {//查找超级连接者
	int comp,record=0,record_man=0;
	for (int i = 0;i < human.getnumVertices();i++) {//遍历所有的结点
		Edge* p = human.NodeTable[i].adj;
		comp = 0;
		while (1) {//遍历结点的直接朋友
			if (p != nullptr) {
				comp++;
				p = p->link;
			}
			else break;
		}
		if (comp > record) {//取直接朋友最多的
			record = comp;
			record_man = i;
		}
	}
	outputFile << "超级连接者为" << record_man << ",他有" << record << "个直接朋友"<<endl;
}


void friend_numbers(Graphlnk& human, int temp, ofstream& outputFile) {//查找直接朋友数量和间接朋友数量
	int record = 0, record2 = 0;
	bool* look = new bool[human.getmaxVertices()];//用来标记用户是否被遍历
	for (int i = 0;i < human.getmaxVertices();i++) {
		look[i] = false;
	}
	//检查用户是否存在，避免出现野指针等问题
	if (temp<0 || temp>human.getnumVertices()) {
		outputFile << "用户不存在" << endl;
		delete look;
		return;
	}
	else {
		Edge* p = human.NodeTable[temp].adj;
		while (1) {//找直接朋友
			if (p != nullptr) {
				if (look[p->name] == false) {
					//标记已经遍历过的朋友
					look[p->name] = true;  
					record++;
					p = p->link;
				}
			}
			else break;
		}
	}
	Edge* p = human.NodeTable[temp].adj;
	while (1) {//双重循环找距离为2的间接朋友
		if (p != nullptr) {
			Edge* q = human.NodeTable[p->name].adj->link;
			while (1) {
				if (q != nullptr) {
					if (look[q->name] == false) {
						look[q->name] = true;
						record2++;
						q = q->link;
					}
					else q = q->link;
				}
				else break;
			}
			p = p->link;
		}
		else break;
	}
	delete[]look;
	outputFile << "用户" << temp << "的直接朋友有" << record << "个";
	outputFile << ",间接朋友有" << record2 << "个" << endl;
}

int Graphlnk::direct_friend_num(int v) {//作为类函数统计直接朋友
	int record=0;
	Edge* p = NodeTable[v].adj;
	while (1) {
		if (p != nullptr) {
			record++;
			p = p->link;	
		}
		else break;
	}
	return record;
}

bool Graphlnk::areConnected(int v, int w) {//判断两点是否连通
	bool* visited = new bool[numVertices];
	for (int i = 0; i < numVertices; ++i) {
		visited[i] = false;
	}
	DFS(v, visited);
	bool result = visited[w];
	delete[] visited;
	return result;
}

void Graphlnk::DFS(int node, bool* visited) {
	if (visited[node]) {
		return;
	}
	visited[node] = true;
	Edge* neighbor = NodeTable[node].adj;
	while (neighbor != nullptr) {
		DFS(neighbor->name, visited);
		neighbor = neighbor->link;
	}
}


void shortest_road(Graphlnk&human, int v, int w,ofstream& outputFile) {
	if (!human.areConnected(v, w)) {//先判断两点是否连通
		outputFile << v << "与" << w << "没有交集" << endl;
		return;
	}
	int record = 1,record_friend=0,record3=0;
	queue<int> record_queue;//BFS使用
	queue<int> record_son_num;//方便计算层数
	record_queue.push(v);
	record_son_num.push(human.direct_friend_num(v));
	bool* look = new bool[human.getmaxVertices()];
	for (int i = 0;i < human.getmaxVertices();i++) {
		look[i] = false;
	}
	look[v] = true;
	while (!record_queue.empty()) {//遍历
		int temp = record_queue.front();
		Edge* q = human.NodeTable[temp].adj;
		record_queue.pop();
		while (q != nullptr) {
			if (look[q->name] != true) {
				look[q->name] == true;
				record_queue.push(q->name);
				record_son_num.push(human.direct_friend_num(q->name));
			}
			if (q->name == w) {
				outputFile<<v<<"和"<<w << "的最短距离为" << record << std::endl;
				return;
			}
			q = q->link;
		}
		record_friend++;
		if (record_friend == record_son_num.front()) {//记录已经遍历的层数
			record++;
			record_friend = 0;
		}
	}
	
}

void findMax(int *arr,int arr_Size, ofstream& outputFile,int v) {//寻找数组中的最大值并输出
	vector<int> maxIndices;
	int maxVal = arr[0];
	for (int i = 1; i < arr_Size; ++i) {
		if (arr[i] > maxVal) {
			maxVal = arr[i];
		}
	}
	for (int i = 0; i < arr_Size; ++i) {
		if (arr[i] == maxVal) {
			maxIndices.push_back(i);
		}
	}
	outputFile << v << "的推荐朋友有:";
	for(int i=0;i< maxIndices.size();i++) outputFile <<maxIndices[i]<<" ";
	outputFile << endl;
}

void suggest_friend(Graphlnk& human, int v, ofstream& outputFile) {
	int record=0;
	int* direct_friend = new int[human.getmaxVertices()];
	int* suggest_friend = new int[human.getmaxVertices()];
	for (int i = 0;i < human.getmaxVertices();i++) direct_friend[i] = -1;
	for (int i = 0;i < human.getmaxVertices();i++) suggest_friend[i] = 0;
	Edge* p = human.NodeTable[v].adj;
	while (1) {//遍历所有直接朋友
		if (p != nullptr) {
			direct_friend[p->name] = 1;
			p = p->link;
		}
		else break;
	}
	for (int i = 0;i < human.getmaxVertices();i++) {//记录间接朋友的出现次数
		if (direct_friend[i] == 1) {
			Edge* p = human.NodeTable[i].adj;
			while (1) {
				if (p->link != nullptr) {
					if(p->name!=v) suggest_friend[p->name]++;
					p = p->link;
				}
				else break;
			}
		}
	}
	findMax(suggest_friend, human.getmaxVertices(),outputFile,v);
	delete[]direct_friend;
	delete[]suggest_friend;
}

int main() {
	cout << "程序运行需20秒左右，请稍等" << endl;
	Graphlnk human(5000);
	ofstream outputFile("output.txt", ios::out | ios::binary);
	outputFile.imbue(locale(""));//确保以utf-8码的形式输入
	// 检查文件是否成功打开
	if (!outputFile.is_open()) {
		cerr << "无法打开文件" << endl;
		return 1; 
	}
	ifstream inputFile("data.txt");
	if (!inputFile.is_open()) {
		outputFile << "无法打开文件" << endl;
		return 1;
	}
	//创建关系网
	int num1, num2;
	while (inputFile >> num1 >> num2) {
		if (human.getVertexPos(num1) == -1) human.insertVertex(num1);
		if (human.getVertexPos(num2) == -1) human.insertVertex(num2);
		if (human.getVertexPos(num1) != -1 && human.getVertexPos(num2) != -1)
			human.insertEdge(num1, num2);
	}
	inputFile.close();
	cout << "图建立完成，正在完成对应任务" << endl;
	// 写入内容到文件
	ifstream inputFile2("sample.txt");
	if (!inputFile2.is_open()) {
		outputFile << "无法打开文件" << endl;
		return 1;
	}
	outputFile << "             *欢迎来到用户关系查询系统*           "<<endl;
	//查询直接朋友和间接朋友
	outputFile << "进入查询用户的直接朋友与间接朋友数" << endl;
	for (int i = 0;i < 10;i++) {
		int num;
		inputFile2 >> num;
		friend_numbers(human, num, outputFile);
	}
	//超级连接者
	outputFile << "####################################################"<<endl;
	outputFile << "进入超级连接者查询" << endl;
	super_friend(human, outputFile);
	//查询两者的最近距离
	outputFile << "####################################################" << endl;
	outputFile << "进入查询两名用户的最近关系距离" << endl;
	for (int i = 0;i < 10;i++) {
		int num1,num2;
		inputFile2 >> num1>>num2;
		shortest_road(human, num1, num2, outputFile);
	}
	outputFile << "####################################################" << endl;
	//查询推荐朋友
	outputFile << "查询推荐朋友" << endl;
	for (int i = 0;i < 5;i++) {
		int num;
		inputFile2 >> num;
		suggest_friend(human, num, outputFile);	
	}
	// 关闭文件
	inputFile2.close();
	outputFile.close();
	cout << "文件写入完成";
	return 0;
}


