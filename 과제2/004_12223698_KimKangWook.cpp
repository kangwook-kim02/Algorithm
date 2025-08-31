#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <queue>
#include <unordered_map>

using namespace std;



struct vertex { // 도시
	int vertex_id; // vertex id
	string city_name; // 도시 이름
	int height_sea_level; // 해발고도

	vertex(int id, string name, int height) { // 생성자 함수
		this->vertex_id = id; // vertex에게 id 부여
		this->city_name = name; // 도시 이름 부여
		this->height_sea_level = height; // 해발고도 부여
	}
};

struct edge { // 도로
	int source_vertex_id; // source vertex id
	string source_vertex_name; // source vertex의 도시 이름
	vertex* source_vertex; // source_vertex

	int destination_vertex_id; // destination vertex id
	string destination_vertex_name; // destination vertex의 도시 이름
	vertex* destination_vertex; // destination_vertex


	string time_of_construction; // 건설 시기
	int length; // 도로의 길이
	int status = 0;  // edge가 heap에 insert 됐는지 안됐는지 0이면 insert가 아직 안됐고 1이면 insert가 됐다.

	edge(int src_vertex_id, string src_name, vertex* src_vertex, int dest_vertex_id, string dest_name, vertex* dest_vertex, string time, int length) { // 생성자 함수
		this->source_vertex_id = src_vertex_id; // src vertex id 부여
		this->source_vertex_name = src_name; // src vertex 도시 이름 부여
		this->source_vertex = src_vertex; // src vertex를 포인터로 가리킴

		this->destination_vertex_id = dest_vertex_id; // dest vertex id 부여
		this->destination_vertex_name = dest_name; // dest vertex 도시 이름 부여
		this->destination_vertex = dest_vertex; // dest vertex를 pointer로 가리킴

		this->time_of_construction = time; // 건설 시기 부여
		this->length = length; // 도로 길이 부여
	}
};

bool comp(edge* e1, edge* e2) { // Minimmum spanning tree를 사전순으로 정렬하기 위한 비교 함수
	string e1_city_A; // 인덱스가 작은 edge의 city A 의 이름
	string e1_city_B; // 인덱스가 작은 edge의 city B 의 이름 (A가 B보다 사전순으로 앞섬)

	string e2_city_A; // 인덱스가 큰 edge의 city A 의 이름
	string e2_city_B; // 인덱스가 큰 edge의 city B 의 이름

	e1_city_A = min(e1->source_vertex_name, e1->destination_vertex_name); // e1이 연결하는 두 도시 중에 사전 순으로 앞에 있는 원소를 city A에 저장
	e1_city_B = max(e1->source_vertex_name, e1->destination_vertex_name); // e1이 연결하는 두 도시 중에 사전 순으로 뒤에 있는 원소를 city B에 저장

	e2_city_A = min(e2->source_vertex_name, e2->destination_vertex_name); // e2가 연결하는 두 도시 중에 사전 순으로 앞에 있는 원소를 city A에 저장
	e2_city_B = max(e2->source_vertex_name, e2->destination_vertex_name); // e2가 연결하는 두 도시 중에 사전 순으로 뒤에 있는 원소를 city B에 저장

	if (e1_city_A < e2_city_A) { // city A의 이름이 벡터 혹은 배열에서 앞에 있는 원소가 사전순으로 더 앞에 와야된다.
		return true;
	}
	else if (e1_city_A == e2_city_A) { // city A의 이름이 동일하다면,
		if (e1_city_B < e2_city_B) { // city B의 이름이 벡터 혹은 배열에서 앞에 있는 원소가 사전순으로 더 앞에 와야된다.
			return true;
		}
	}
	return false;
}

struct priorityEdge { // Edge의 우선순위를 정하는 구조체
	/*
	STL에서 제공하는 우선 순위 큐는 max로 구현되었기 때문에
	뒤에 있는 애가 더 우선 순위가 높다. 
	*/
	bool operator()(edge* a, edge* b) const { // 연산자 오버로딩
		if (a->time_of_construction != b->time_of_construction) { // 건설 시기가 다를 때
			return a->time_of_construction > b->time_of_construction; // 건설시기가 작은 b가 우선 순위가 더 높다. 
		}

		// 건설 시기가 동일하다면
		if (a->length != b->length) { // 길이가 다를 때
			return a->length > b->length; // 길이가 더 짧은 b가 우선 순위가 더 높다.
		}

		// 도로 길이가 동일하다면
		int a_height = abs(a->source_vertex->height_sea_level - a->destination_vertex->height_sea_level); // a 도로의 두 도시간의 해발 고도차이
		int b_height = abs(b->source_vertex->height_sea_level - b->destination_vertex->height_sea_level); // b 도로의 두 도시간의 해발 고도차이
		if (a_height != b_height) { // 해발 고도 차이가 다르다면
			return a_height > b_height; // 해발 고다가 더 작은 b가 우선 순위가 더 높다.
		}
		// 해발 고도 차이가 같다면

		string a_city_A = min(a->source_vertex_name, a->destination_vertex_name); // 사전 순으로 a의 두 도시 중 앞선 도시 이름
		string a_city_B = max(a->source_vertex_name, a->destination_vertex_name); // 사전 순으로 a의 두 도시 중 뒷에 있는 도시 이름
		string b_city_A = min(b->source_vertex_name, b->destination_vertex_name); // 사전 순으로 b의 두 도시 중 앞선 도시 이름
		string b_city_B = max(b->source_vertex_name, b->destination_vertex_name); // 사전 순으로 b의 두 도시 중 뒷에 있는 도시 이름

		if (a_city_A != b_city_A) {
			return a_city_A > b_city_A; // city A의 이름이 사전 순으로 앞에 오는 도시를 잇는 도로가 우선 순위가 높다.
		}

		// city A의 이름이 동일하다면
		return a_city_B > b_city_B; // city B의 이름이 사전 순으로 앞에 오는 도시르 잇는 도로가 우선 순위가 높다.
	}
};

class Graph { // undirected graph
public:
	Graph(); // 생성자 함수
	void insertVertex(string city_name, int height_sea_level); // vertex를 graph에 삽입 하는 함수
	void insertEdge(string src_city_name, string dest_city_name, string time, int length); // edge를 grpah에 삽입하는 함수
	void prim_algorithm(string start_city_name); // prim의 알고리즘

private:
	int find_vertex_id(string city_name); // 도시 이름을 통해 vertex_id를 찾는 함수
	int size; // Graph에 있는 vertex의 개수, vertex id를 부여하는데 활용
	unordered_map<string, int> um; // key값이 도시이름이고 value값이 vertex id인 unorderd map  --> time limit을 해결하기 위해 빠르게 vertex id를 찾기 위해서 추가
	vector<vector<int>> adjacency_list; // 인접리스트 표현 방식으로 graph를 구현하기 위한 vector
	vector<vector<edge*>> edge_list; // edge의 정보를 담고 있는 vector
	vector<vertex*> vertex_list; // vertex의 정보를 담고 있는 vector
	vector<string> status; // vertex가 Tree vertex인지, Fringe vertex인지, unseen vertex인지 상태를 알려주는 vector
	vector<int> weight; // MST안에 해당 vertex와 연결된 도로의 길이
	vector<edge*> MST; // MST에 저장된 edge;
	priority_queue<edge*, vector<edge*>, priorityEdge> heap; // STL 우선 순위 큐 (heap을 이용한) 얘는 Tree와 incident한 edge들을 삽입할 때 사용할 것이다.
};

Graph::Graph() { // 생성자 함수
	size = 0; // empty graph의 vertex 개수는 0개이므로 0으로 초기화
	adjacency_list = vector<vector<int>>(100000); // 2차원 인접리스트 행만 초기화 하기
	edge_list = vector<vector<edge*>>(200000); // 2차원 간선들의 리스트 행만 초기화 하기
}

int Graph::find_vertex_id(string city_name) { // 도시 이름을 통해 vertex_id를 찾는 함수
	return um[city_name]; // unordered map을 이용하여 key 값이 city_name인 value를 return 하면 vertex id를 return 한다.
}

void Graph::insertVertex(string city_name, int height_sea_level) { //vertex를 graph에 삽입 하는 함수
	vertex* new_vertex = new vertex(this->size, city_name, height_sea_level); // 새로운 vertex 만들기
	vertex_list.push_back(new_vertex); // vertex list에 vertex 삽입
	um[city_name] = size; // key값이 city_name에게 vertex id가 size인 value를 저장 --> 나중에 vertex id를 찾을 때 빠르게 찾을 수 있음
	this->size++; // vertex의 개수 증가
}

void Graph::insertEdge(string src_city_name, string dest_city_name, string time, int length) { // vertex와 vertex를 잇는 간선을 추가하는 함수
	int src_vertex_id = find_vertex_id(src_city_name); // source vertex의 vertex id 찾기
	int dest_vertex_id = find_vertex_id(dest_city_name); // destination vertex의 vertex id 찾기
	edge* newEdge = new edge(src_vertex_id, src_city_name, vertex_list[src_vertex_id], dest_vertex_id, dest_city_name, vertex_list[dest_vertex_id], time, length); // edge 만들기
	adjacency_list[src_vertex_id].push_back(dest_vertex_id); // 인접리스트에 인접한 vertex id 추가 (dest vertex 추가)
	adjacency_list[dest_vertex_id].push_back(src_vertex_id); // 인접리스트에 인접한 vertex id 추가 (src vertex 추가)

	edge_list[src_vertex_id].push_back(newEdge); // vertex id에 해당하는 인덱스에 edge 삽입
	edge_list[dest_vertex_id].push_back(newEdge); // vertex id에 해당하는 인덱스에 edge 삽입
}

void Graph::prim_algorithm(string start_city_name) { // prim의 알고리즘
	int start_vertex_id = find_vertex_id(start_city_name); // start하는 vertex의 id를 얻는다.

	this->status = vector<string>(this->size); // vertex의 개수 만큼 status를 만든다.
	for (int i = 0; i < this->status.size(); i++) {
		this->status[i] = "unseen"; // 모든 vertex의 상태를 unseen으로 초기화
	}
	this->status[start_vertex_id] = "tree"; // start vertex의 status를 tree로 변경

	this->weight = vector<int>(this->size); // vertex의 개수 만큼 weight를 만든다.
	for (int i = 0; i < this->weight.size(); i++) {
		this->weight[i] = INT_MAX; // 모든 vertex와 연결된 도로의 길이를 int의 max값으로 초기화 (무한대수) 
	}
	this->weight[start_vertex_id] = 0; // start vertex의 weight를 0으로 설정

	for (int i = 0; i < this->adjacency_list[start_vertex_id].size(); i++) { // start vertex와 인접한 vertex를 fringe로 변경
		this->status[adjacency_list[start_vertex_id][i]] = "fringe";
	}

	for (int i = 0; i < this->edge_list[start_vertex_id].size(); i++) {
		heap.push(edge_list[start_vertex_id][i]); // heap에 start_vertex와 incident한 edge를 삽입하여 넣는다.
		edge_list[start_vertex_id][i]->status = 1;
	}

	while (MST.size() < size - 1) { // n-1번 반복 (즉 모든 vertex를 접근) 앞에서 starting vertex를 다뤘기 때문에 vertex는 n-1개만 더 들어가면 됨
		edge* high_edge = heap.top();// 제일 우선 순위가 높은 엣지를 heap에서 가져온다.
		heap.pop(); // 우선 순위가 가장 높은 엣지 삭제

		int will_tree_vertex_id = -1; // tree vertex가 될 vertex의 id
		if (status[high_edge->source_vertex_id] == "tree" && status[high_edge->destination_vertex_id] == "tree") { // edge 양쪽 vertex가 모두 tree인 경우 해당 엣지는 넘어간다
			continue;
		}

		if (status[high_edge->source_vertex_id] == "fringe") { // source vertex가 fringe이었다면 (즉 tree vertex가 destination vertex 였을 때)
			status[high_edge->source_vertex_id] = "tree"; // 선택된 vertex의 상태를 tree로 바꿈
			will_tree_vertex_id = high_edge->source_vertex_id;
		}
		else if (status[high_edge->destination_vertex_id] == "fringe") { // destination vertex가 unseen이었다면 (즉 tree vertex가 source vertex 였을 때)
			status[high_edge->destination_vertex_id] = "tree"; // 선택된 vertex의 상태를 tree로 바꿈
			will_tree_vertex_id = high_edge->destination_vertex_id;
		}


		MST.push_back(high_edge); // Minimum spanning tree에 우선 순위가 제일 높은 엣지를 삽입

		string city_A = min(high_edge->source_vertex_name, high_edge->destination_vertex_name); // 사전 순으로 앞에 오는 도시의 이름
		string city_B = max(high_edge->source_vertex_name, high_edge->destination_vertex_name); // 사전 순으로 뒤에 오는 도시의 이름
		cout << city_A << " " << city_B << "\n"; // 중간 정보 출력 cityA city B

		weight[will_tree_vertex_id] = high_edge->length; // weight에 도로의 길이 삽입

		for (int i = 0; i < this->adjacency_list[will_tree_vertex_id].size(); i++) { // tree vertex와 인접한 vertex를 fringe로 변경
			if (this->status[adjacency_list[will_tree_vertex_id][i]] == "unseen") // tree vertex와 인접한 vertex가 unseen이면
				this->status[adjacency_list[will_tree_vertex_id][i]] = "fringe"; // fringe로 상태를 변경
		}

		for (int i = 0; i < this->edge_list[will_tree_vertex_id].size(); i++) {
			if (edge_list[will_tree_vertex_id][i]->status == 0) { // heap에 insert 된적이 없으면 (상태가 0이면 insert 된 적이 없음)
				heap.push(edge_list[will_tree_vertex_id][i]); // heap에 start_vertex와 incident한 edge를 삽입하여 넣는다.
				edge_list[will_tree_vertex_id][i]->status = 1; // 상태를 1로 바꿈
			}
		}

	}

	/*
	최종 결과 출력
	*/

	int total_length = 0; // total 도로의 길이
	for (int i = 0; i < weight.size(); i++) { // total 도로의 길이를 sum
		total_length = total_length + weight[i];
	}
	cout << total_length << "\n"; // total 길이 출력

	sort(MST.begin(), MST.end(), comp); // Minimum spanning tree에 도로들은 사전 순으로 정렬 

	for (int i = 0; i < MST.size(); i++) {
		string city_A = min(MST[i]->source_vertex_name, MST[i]->destination_vertex_name); // 사전 순으로 앞에 오는 도시의 이름
		string city_B = max(MST[i]->source_vertex_name, MST[i]->destination_vertex_name); // 사전 순으로 뒤에 오는 도시의 이름
		cout << MST[i]->time_of_construction << " " << city_A << " " << city_B << " " << MST[i]->length << "\n"; // timestamp city_A city_B lenght 출력
	}

}

int main() {
	ios::sync_with_stdio(false);

	Graph undirected_graph; // 무향 그래프 선언
	int num_of_vertex; // 도시들의 정보 개수
	int num_of_edge; // 도로들의 정보 개수

	cin >> num_of_vertex;

	string city_name; // 도시 이름
	int height_sea_level; // 해발고도
	for (int i = 0; i < num_of_vertex; i++) {
		cin >> city_name >> height_sea_level;
		undirected_graph.insertVertex(city_name, height_sea_level); // city_name과 height_sea_level을 갖는 도시 생성
	}


	string city_A_name; // 도시 A 이름
	string city_B_name; // 도시 B 이름
	int length; // 도로 길이
	string timestamp; // 건설 시기

	cin >> num_of_edge;
	for (int i = 0; i < num_of_edge; i++) {
		cin >> city_A_name >> city_B_name >> length >> timestamp;
		undirected_graph.insertEdge(city_A_name, city_B_name, timestamp, length); // city A와 city B를 연결하는 도로 생성(건설 시기 : timestamp, 도로 길이 length)
	}

	string start_city; // prim의 알고리즘에서 start할 도시 이름
	cin >> start_city;
	undirected_graph.prim_algorithm(start_city); // prim 알고리즘 수행
}