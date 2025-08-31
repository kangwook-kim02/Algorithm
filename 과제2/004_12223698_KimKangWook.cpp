#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <queue>
#include <unordered_map>

using namespace std;



struct vertex { // ����
	int vertex_id; // vertex id
	string city_name; // ���� �̸�
	int height_sea_level; // �ع߰�

	vertex(int id, string name, int height) { // ������ �Լ�
		this->vertex_id = id; // vertex���� id �ο�
		this->city_name = name; // ���� �̸� �ο�
		this->height_sea_level = height; // �ع߰� �ο�
	}
};

struct edge { // ����
	int source_vertex_id; // source vertex id
	string source_vertex_name; // source vertex�� ���� �̸�
	vertex* source_vertex; // source_vertex

	int destination_vertex_id; // destination vertex id
	string destination_vertex_name; // destination vertex�� ���� �̸�
	vertex* destination_vertex; // destination_vertex


	string time_of_construction; // �Ǽ� �ñ�
	int length; // ������ ����
	int status = 0;  // edge�� heap�� insert �ƴ��� �ȵƴ��� 0�̸� insert�� ���� �ȵư� 1�̸� insert�� �ƴ�.

	edge(int src_vertex_id, string src_name, vertex* src_vertex, int dest_vertex_id, string dest_name, vertex* dest_vertex, string time, int length) { // ������ �Լ�
		this->source_vertex_id = src_vertex_id; // src vertex id �ο�
		this->source_vertex_name = src_name; // src vertex ���� �̸� �ο�
		this->source_vertex = src_vertex; // src vertex�� �����ͷ� ����Ŵ

		this->destination_vertex_id = dest_vertex_id; // dest vertex id �ο�
		this->destination_vertex_name = dest_name; // dest vertex ���� �̸� �ο�
		this->destination_vertex = dest_vertex; // dest vertex�� pointer�� ����Ŵ

		this->time_of_construction = time; // �Ǽ� �ñ� �ο�
		this->length = length; // ���� ���� �ο�
	}
};

bool comp(edge* e1, edge* e2) { // Minimmum spanning tree�� ���������� �����ϱ� ���� �� �Լ�
	string e1_city_A; // �ε����� ���� edge�� city A �� �̸�
	string e1_city_B; // �ε����� ���� edge�� city B �� �̸� (A�� B���� ���������� �ռ�)

	string e2_city_A; // �ε����� ū edge�� city A �� �̸�
	string e2_city_B; // �ε����� ū edge�� city B �� �̸�

	e1_city_A = min(e1->source_vertex_name, e1->destination_vertex_name); // e1�� �����ϴ� �� ���� �߿� ���� ������ �տ� �ִ� ���Ҹ� city A�� ����
	e1_city_B = max(e1->source_vertex_name, e1->destination_vertex_name); // e1�� �����ϴ� �� ���� �߿� ���� ������ �ڿ� �ִ� ���Ҹ� city B�� ����

	e2_city_A = min(e2->source_vertex_name, e2->destination_vertex_name); // e2�� �����ϴ� �� ���� �߿� ���� ������ �տ� �ִ� ���Ҹ� city A�� ����
	e2_city_B = max(e2->source_vertex_name, e2->destination_vertex_name); // e2�� �����ϴ� �� ���� �߿� ���� ������ �ڿ� �ִ� ���Ҹ� city B�� ����

	if (e1_city_A < e2_city_A) { // city A�� �̸��� ���� Ȥ�� �迭���� �տ� �ִ� ���Ұ� ���������� �� �տ� �;ߵȴ�.
		return true;
	}
	else if (e1_city_A == e2_city_A) { // city A�� �̸��� �����ϴٸ�,
		if (e1_city_B < e2_city_B) { // city B�� �̸��� ���� Ȥ�� �迭���� �տ� �ִ� ���Ұ� ���������� �� �տ� �;ߵȴ�.
			return true;
		}
	}
	return false;
}

struct priorityEdge { // Edge�� �켱������ ���ϴ� ����ü
	/*
	STL���� �����ϴ� �켱 ���� ť�� max�� �����Ǿ��� ������
	�ڿ� �ִ� �ְ� �� �켱 ������ ����. 
	*/
	bool operator()(edge* a, edge* b) const { // ������ �����ε�
		if (a->time_of_construction != b->time_of_construction) { // �Ǽ� �ñⰡ �ٸ� ��
			return a->time_of_construction > b->time_of_construction; // �Ǽ��ñⰡ ���� b�� �켱 ������ �� ����. 
		}

		// �Ǽ� �ñⰡ �����ϴٸ�
		if (a->length != b->length) { // ���̰� �ٸ� ��
			return a->length > b->length; // ���̰� �� ª�� b�� �켱 ������ �� ����.
		}

		// ���� ���̰� �����ϴٸ�
		int a_height = abs(a->source_vertex->height_sea_level - a->destination_vertex->height_sea_level); // a ������ �� ���ð��� �ع� ������
		int b_height = abs(b->source_vertex->height_sea_level - b->destination_vertex->height_sea_level); // b ������ �� ���ð��� �ع� ������
		if (a_height != b_height) { // �ع� �� ���̰� �ٸ��ٸ�
			return a_height > b_height; // �ع� ��ٰ� �� ���� b�� �켱 ������ �� ����.
		}
		// �ع� �� ���̰� ���ٸ�

		string a_city_A = min(a->source_vertex_name, a->destination_vertex_name); // ���� ������ a�� �� ���� �� �ռ� ���� �̸�
		string a_city_B = max(a->source_vertex_name, a->destination_vertex_name); // ���� ������ a�� �� ���� �� �޿� �ִ� ���� �̸�
		string b_city_A = min(b->source_vertex_name, b->destination_vertex_name); // ���� ������ b�� �� ���� �� �ռ� ���� �̸�
		string b_city_B = max(b->source_vertex_name, b->destination_vertex_name); // ���� ������ b�� �� ���� �� �޿� �ִ� ���� �̸�

		if (a_city_A != b_city_A) {
			return a_city_A > b_city_A; // city A�� �̸��� ���� ������ �տ� ���� ���ø� �մ� ���ΰ� �켱 ������ ����.
		}

		// city A�� �̸��� �����ϴٸ�
		return a_city_B > b_city_B; // city B�� �̸��� ���� ������ �տ� ���� ���ø� �մ� ���ΰ� �켱 ������ ����.
	}
};

class Graph { // undirected graph
public:
	Graph(); // ������ �Լ�
	void insertVertex(string city_name, int height_sea_level); // vertex�� graph�� ���� �ϴ� �Լ�
	void insertEdge(string src_city_name, string dest_city_name, string time, int length); // edge�� grpah�� �����ϴ� �Լ�
	void prim_algorithm(string start_city_name); // prim�� �˰���

private:
	int find_vertex_id(string city_name); // ���� �̸��� ���� vertex_id�� ã�� �Լ�
	int size; // Graph�� �ִ� vertex�� ����, vertex id�� �ο��ϴµ� Ȱ��
	unordered_map<string, int> um; // key���� �����̸��̰� value���� vertex id�� unorderd map  --> time limit�� �ذ��ϱ� ���� ������ vertex id�� ã�� ���ؼ� �߰�
	vector<vector<int>> adjacency_list; // ��������Ʈ ǥ�� ������� graph�� �����ϱ� ���� vector
	vector<vector<edge*>> edge_list; // edge�� ������ ��� �ִ� vector
	vector<vertex*> vertex_list; // vertex�� ������ ��� �ִ� vector
	vector<string> status; // vertex�� Tree vertex����, Fringe vertex����, unseen vertex���� ���¸� �˷��ִ� vector
	vector<int> weight; // MST�ȿ� �ش� vertex�� ����� ������ ����
	vector<edge*> MST; // MST�� ����� edge;
	priority_queue<edge*, vector<edge*>, priorityEdge> heap; // STL �켱 ���� ť (heap�� �̿���) ��� Tree�� incident�� edge���� ������ �� ����� ���̴�.
};

Graph::Graph() { // ������ �Լ�
	size = 0; // empty graph�� vertex ������ 0���̹Ƿ� 0���� �ʱ�ȭ
	adjacency_list = vector<vector<int>>(100000); // 2���� ��������Ʈ �ุ �ʱ�ȭ �ϱ�
	edge_list = vector<vector<edge*>>(200000); // 2���� �������� ����Ʈ �ุ �ʱ�ȭ �ϱ�
}

int Graph::find_vertex_id(string city_name) { // ���� �̸��� ���� vertex_id�� ã�� �Լ�
	return um[city_name]; // unordered map�� �̿��Ͽ� key ���� city_name�� value�� return �ϸ� vertex id�� return �Ѵ�.
}

void Graph::insertVertex(string city_name, int height_sea_level) { //vertex�� graph�� ���� �ϴ� �Լ�
	vertex* new_vertex = new vertex(this->size, city_name, height_sea_level); // ���ο� vertex �����
	vertex_list.push_back(new_vertex); // vertex list�� vertex ����
	um[city_name] = size; // key���� city_name���� vertex id�� size�� value�� ���� --> ���߿� vertex id�� ã�� �� ������ ã�� �� ����
	this->size++; // vertex�� ���� ����
}

void Graph::insertEdge(string src_city_name, string dest_city_name, string time, int length) { // vertex�� vertex�� �մ� ������ �߰��ϴ� �Լ�
	int src_vertex_id = find_vertex_id(src_city_name); // source vertex�� vertex id ã��
	int dest_vertex_id = find_vertex_id(dest_city_name); // destination vertex�� vertex id ã��
	edge* newEdge = new edge(src_vertex_id, src_city_name, vertex_list[src_vertex_id], dest_vertex_id, dest_city_name, vertex_list[dest_vertex_id], time, length); // edge �����
	adjacency_list[src_vertex_id].push_back(dest_vertex_id); // ��������Ʈ�� ������ vertex id �߰� (dest vertex �߰�)
	adjacency_list[dest_vertex_id].push_back(src_vertex_id); // ��������Ʈ�� ������ vertex id �߰� (src vertex �߰�)

	edge_list[src_vertex_id].push_back(newEdge); // vertex id�� �ش��ϴ� �ε����� edge ����
	edge_list[dest_vertex_id].push_back(newEdge); // vertex id�� �ش��ϴ� �ε����� edge ����
}

void Graph::prim_algorithm(string start_city_name) { // prim�� �˰���
	int start_vertex_id = find_vertex_id(start_city_name); // start�ϴ� vertex�� id�� ��´�.

	this->status = vector<string>(this->size); // vertex�� ���� ��ŭ status�� �����.
	for (int i = 0; i < this->status.size(); i++) {
		this->status[i] = "unseen"; // ��� vertex�� ���¸� unseen���� �ʱ�ȭ
	}
	this->status[start_vertex_id] = "tree"; // start vertex�� status�� tree�� ����

	this->weight = vector<int>(this->size); // vertex�� ���� ��ŭ weight�� �����.
	for (int i = 0; i < this->weight.size(); i++) {
		this->weight[i] = INT_MAX; // ��� vertex�� ����� ������ ���̸� int�� max������ �ʱ�ȭ (���Ѵ��) 
	}
	this->weight[start_vertex_id] = 0; // start vertex�� weight�� 0���� ����

	for (int i = 0; i < this->adjacency_list[start_vertex_id].size(); i++) { // start vertex�� ������ vertex�� fringe�� ����
		this->status[adjacency_list[start_vertex_id][i]] = "fringe";
	}

	for (int i = 0; i < this->edge_list[start_vertex_id].size(); i++) {
		heap.push(edge_list[start_vertex_id][i]); // heap�� start_vertex�� incident�� edge�� �����Ͽ� �ִ´�.
		edge_list[start_vertex_id][i]->status = 1;
	}

	while (MST.size() < size - 1) { // n-1�� �ݺ� (�� ��� vertex�� ����) �տ��� starting vertex�� �ٷ�� ������ vertex�� n-1���� �� ���� ��
		edge* high_edge = heap.top();// ���� �켱 ������ ���� ������ heap���� �����´�.
		heap.pop(); // �켱 ������ ���� ���� ���� ����

		int will_tree_vertex_id = -1; // tree vertex�� �� vertex�� id
		if (status[high_edge->source_vertex_id] == "tree" && status[high_edge->destination_vertex_id] == "tree") { // edge ���� vertex�� ��� tree�� ��� �ش� ������ �Ѿ��
			continue;
		}

		if (status[high_edge->source_vertex_id] == "fringe") { // source vertex�� fringe�̾��ٸ� (�� tree vertex�� destination vertex ���� ��)
			status[high_edge->source_vertex_id] = "tree"; // ���õ� vertex�� ���¸� tree�� �ٲ�
			will_tree_vertex_id = high_edge->source_vertex_id;
		}
		else if (status[high_edge->destination_vertex_id] == "fringe") { // destination vertex�� unseen�̾��ٸ� (�� tree vertex�� source vertex ���� ��)
			status[high_edge->destination_vertex_id] = "tree"; // ���õ� vertex�� ���¸� tree�� �ٲ�
			will_tree_vertex_id = high_edge->destination_vertex_id;
		}


		MST.push_back(high_edge); // Minimum spanning tree�� �켱 ������ ���� ���� ������ ����

		string city_A = min(high_edge->source_vertex_name, high_edge->destination_vertex_name); // ���� ������ �տ� ���� ������ �̸�
		string city_B = max(high_edge->source_vertex_name, high_edge->destination_vertex_name); // ���� ������ �ڿ� ���� ������ �̸�
		cout << city_A << " " << city_B << "\n"; // �߰� ���� ��� cityA city B

		weight[will_tree_vertex_id] = high_edge->length; // weight�� ������ ���� ����

		for (int i = 0; i < this->adjacency_list[will_tree_vertex_id].size(); i++) { // tree vertex�� ������ vertex�� fringe�� ����
			if (this->status[adjacency_list[will_tree_vertex_id][i]] == "unseen") // tree vertex�� ������ vertex�� unseen�̸�
				this->status[adjacency_list[will_tree_vertex_id][i]] = "fringe"; // fringe�� ���¸� ����
		}

		for (int i = 0; i < this->edge_list[will_tree_vertex_id].size(); i++) {
			if (edge_list[will_tree_vertex_id][i]->status == 0) { // heap�� insert ������ ������ (���°� 0�̸� insert �� ���� ����)
				heap.push(edge_list[will_tree_vertex_id][i]); // heap�� start_vertex�� incident�� edge�� �����Ͽ� �ִ´�.
				edge_list[will_tree_vertex_id][i]->status = 1; // ���¸� 1�� �ٲ�
			}
		}

	}

	/*
	���� ��� ���
	*/

	int total_length = 0; // total ������ ����
	for (int i = 0; i < weight.size(); i++) { // total ������ ���̸� sum
		total_length = total_length + weight[i];
	}
	cout << total_length << "\n"; // total ���� ���

	sort(MST.begin(), MST.end(), comp); // Minimum spanning tree�� ���ε��� ���� ������ ���� 

	for (int i = 0; i < MST.size(); i++) {
		string city_A = min(MST[i]->source_vertex_name, MST[i]->destination_vertex_name); // ���� ������ �տ� ���� ������ �̸�
		string city_B = max(MST[i]->source_vertex_name, MST[i]->destination_vertex_name); // ���� ������ �ڿ� ���� ������ �̸�
		cout << MST[i]->time_of_construction << " " << city_A << " " << city_B << " " << MST[i]->length << "\n"; // timestamp city_A city_B lenght ���
	}

}

int main() {
	ios::sync_with_stdio(false);

	Graph undirected_graph; // ���� �׷��� ����
	int num_of_vertex; // ���õ��� ���� ����
	int num_of_edge; // ���ε��� ���� ����

	cin >> num_of_vertex;

	string city_name; // ���� �̸�
	int height_sea_level; // �ع߰�
	for (int i = 0; i < num_of_vertex; i++) {
		cin >> city_name >> height_sea_level;
		undirected_graph.insertVertex(city_name, height_sea_level); // city_name�� height_sea_level�� ���� ���� ����
	}


	string city_A_name; // ���� A �̸�
	string city_B_name; // ���� B �̸�
	int length; // ���� ����
	string timestamp; // �Ǽ� �ñ�

	cin >> num_of_edge;
	for (int i = 0; i < num_of_edge; i++) {
		cin >> city_A_name >> city_B_name >> length >> timestamp;
		undirected_graph.insertEdge(city_A_name, city_B_name, timestamp, length); // city A�� city B�� �����ϴ� ���� ����(�Ǽ� �ñ� : timestamp, ���� ���� length)
	}

	string start_city; // prim�� �˰��򿡼� start�� ���� �̸�
	cin >> start_city;
	undirected_graph.prim_algorithm(start_city); // prim �˰��� ����
}