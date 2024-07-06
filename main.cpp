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
	//ͼ�Ļ�������
	bool insertVertex(const int& vertex);   bool removeEdge(int v1, int v2);
	bool removeVertex(int v);          bool insertEdge(int v1, int v2);
	int getFirstNeighbor(int v);   int getNextNeighbor(int v, int w);
	int getVertexPos(const int vertex) {
		for (int i = 0;i < numVertices;i++)
			if (NodeTable[i].data == vertex) return i;
		return -1;
	}
	int direct_friend_num(int v);//����ͳ��ֱ������
	bool areConnected(int v, int w);    void DFS(int node, bool* visited);
	//������ȡ��װ��������
	int getmaxVertices() { return maxVertices; };  int getnumEdges() { return numEdges; }
	int getnumVertices() { return numVertices; }

	Vertex* NodeTable;//��װ��private�������಻�㣬Ϊ��Լʱ��û�з�װ

private://�����еĲ�����Ϣ��װ�����ֶ��û���Ϣ�ı���
	int maxVertices;
	int numEdges;
	int numVertices;
};

Graphlnk::Graphlnk(int sz) {//��Ĺ��캯��
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	NodeTable = new Vertex[maxVertices];
	if (NodeTable == nullptr) { cerr << "�洢�������" << endl; }
	for (int i = 0;i < maxVertices;i++)NodeTable[i].adj = nullptr;
}

Graphlnk::~Graphlnk() {//�����������
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

bool Graphlnk::insertVertex(const int & vertex) {//�����û�
	if (numVertices == maxVertices) return false;
	NodeTable[numVertices].data = vertex;
	numVertices++;
	return true;
}

bool Graphlnk::insertEdge(int v1, int v2) {//���������û�֮��Ĺ�ϵ
	// ��鶥�������Ƿ���Ч
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

void super_friend(Graphlnk& human,ofstream& outputFile) {//���ҳ���������
	int comp,record=0,record_man=0;
	for (int i = 0;i < human.getnumVertices();i++) {//�������еĽ��
		Edge* p = human.NodeTable[i].adj;
		comp = 0;
		while (1) {//��������ֱ������
			if (p != nullptr) {
				comp++;
				p = p->link;
			}
			else break;
		}
		if (comp > record) {//ȡֱ����������
			record = comp;
			record_man = i;
		}
	}
	outputFile << "����������Ϊ" << record_man << ",����" << record << "��ֱ������"<<endl;
}


void friend_numbers(Graphlnk& human, int temp, ofstream& outputFile) {//����ֱ�����������ͼ����������
	int record = 0, record2 = 0;
	bool* look = new bool[human.getmaxVertices()];//��������û��Ƿ񱻱���
	for (int i = 0;i < human.getmaxVertices();i++) {
		look[i] = false;
	}
	//����û��Ƿ���ڣ��������Ұָ�������
	if (temp<0 || temp>human.getnumVertices()) {
		outputFile << "�û�������" << endl;
		delete look;
		return;
	}
	else {
		Edge* p = human.NodeTable[temp].adj;
		while (1) {//��ֱ������
			if (p != nullptr) {
				if (look[p->name] == false) {
					//����Ѿ�������������
					look[p->name] = true;  
					record++;
					p = p->link;
				}
			}
			else break;
		}
	}
	Edge* p = human.NodeTable[temp].adj;
	while (1) {//˫��ѭ���Ҿ���Ϊ2�ļ������
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
	outputFile << "�û�" << temp << "��ֱ��������" << record << "��";
	outputFile << ",���������" << record2 << "��" << endl;
}

int Graphlnk::direct_friend_num(int v) {//��Ϊ�ຯ��ͳ��ֱ������
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

bool Graphlnk::areConnected(int v, int w) {//�ж������Ƿ���ͨ
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
	if (!human.areConnected(v, w)) {//���ж������Ƿ���ͨ
		outputFile << v << "��" << w << "û�н���" << endl;
		return;
	}
	int record = 1,record_friend=0,record3=0;
	queue<int> record_queue;//BFSʹ��
	queue<int> record_son_num;//����������
	record_queue.push(v);
	record_son_num.push(human.direct_friend_num(v));
	bool* look = new bool[human.getmaxVertices()];
	for (int i = 0;i < human.getmaxVertices();i++) {
		look[i] = false;
	}
	look[v] = true;
	while (!record_queue.empty()) {//����
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
				outputFile<<v<<"��"<<w << "����̾���Ϊ" << record << std::endl;
				return;
			}
			q = q->link;
		}
		record_friend++;
		if (record_friend == record_son_num.front()) {//��¼�Ѿ������Ĳ���
			record++;
			record_friend = 0;
		}
	}
	
}

void findMax(int *arr,int arr_Size, ofstream& outputFile,int v) {//Ѱ�������е����ֵ�����
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
	outputFile << v << "���Ƽ�������:";
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
	while (1) {//��������ֱ������
		if (p != nullptr) {
			direct_friend[p->name] = 1;
			p = p->link;
		}
		else break;
	}
	for (int i = 0;i < human.getmaxVertices();i++) {//��¼������ѵĳ��ִ���
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
	cout << "����������20�����ң����Ե�" << endl;
	Graphlnk human(5000);
	ofstream outputFile("output.txt", ios::out | ios::binary);
	outputFile.imbue(locale(""));//ȷ����utf-8�����ʽ����
	// ����ļ��Ƿ�ɹ���
	if (!outputFile.is_open()) {
		cerr << "�޷����ļ�" << endl;
		return 1; 
	}
	ifstream inputFile("data.txt");
	if (!inputFile.is_open()) {
		outputFile << "�޷����ļ�" << endl;
		return 1;
	}
	//������ϵ��
	int num1, num2;
	while (inputFile >> num1 >> num2) {
		if (human.getVertexPos(num1) == -1) human.insertVertex(num1);
		if (human.getVertexPos(num2) == -1) human.insertVertex(num2);
		if (human.getVertexPos(num1) != -1 && human.getVertexPos(num2) != -1)
			human.insertEdge(num1, num2);
	}
	inputFile.close();
	cout << "ͼ������ɣ�������ɶ�Ӧ����" << endl;
	// д�����ݵ��ļ�
	ifstream inputFile2("sample.txt");
	if (!inputFile2.is_open()) {
		outputFile << "�޷����ļ�" << endl;
		return 1;
	}
	outputFile << "             *��ӭ�����û���ϵ��ѯϵͳ*           "<<endl;
	//��ѯֱ�����Ѻͼ������
	outputFile << "�����ѯ�û���ֱ����������������" << endl;
	for (int i = 0;i < 10;i++) {
		int num;
		inputFile2 >> num;
		friend_numbers(human, num, outputFile);
	}
	//����������
	outputFile << "####################################################"<<endl;
	outputFile << "���볬�������߲�ѯ" << endl;
	super_friend(human, outputFile);
	//��ѯ���ߵ��������
	outputFile << "####################################################" << endl;
	outputFile << "�����ѯ�����û��������ϵ����" << endl;
	for (int i = 0;i < 10;i++) {
		int num1,num2;
		inputFile2 >> num1>>num2;
		shortest_road(human, num1, num2, outputFile);
	}
	outputFile << "####################################################" << endl;
	//��ѯ�Ƽ�����
	outputFile << "��ѯ�Ƽ�����" << endl;
	for (int i = 0;i < 5;i++) {
		int num;
		inputFile2 >> num;
		suggest_friend(human, num, outputFile);	
	}
	// �ر��ļ�
	inputFile2.close();
	outputFile.close();
	cout << "�ļ�д�����";
	return 0;
}


