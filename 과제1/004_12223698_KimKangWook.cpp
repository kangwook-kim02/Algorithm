#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



struct node {
	node* parent; // 부모 노드
	node* left_child; // 왼쪽 자식 노드
	node* right_child; // 오른쪽 자식 노드
	string color;
	int depth;

	pair<int, string> id_subject; // 학번과 신청과목 쌍을 저장하는 변수
	string name; // 학생 이름
	int semester; // 이수 학기 수
	string phone_number; // 전화 번호
	int time_stamp; // 수강 신청 시간

	/*
	생성자 함수 node를 초기화 할 수 있게 해주는 함수
	*/
	node(node* parent, node* left_child, node* right_child, string color, int depth, int sid, string subject, string name, int semester, string phone_number, int time_stamp) {
		this->parent = parent;
		this->left_child = left_child;
		this->right_child = right_child;
		this->color = color;
		this->depth = depth;
		this->id_subject.first = sid;
		this->id_subject.second = subject;
		this->name = name;
		this->semester = semester;
		this->phone_number = phone_number;
		this->time_stamp = time_stamp;

	}
};

node* comparator(node* n1, node* n2) { // 비교해서 key값이 더 큰 node를 return
	if (n1->id_subject.first > n2->id_subject.first) { // n1의 학번이 n2의 학번보다 클 때
		return n1; // 학번이 더 큰 n1을 return
	}
	else if (n1->id_subject.first < n2->id_subject.first) { // n2의 학번이 n1의 학번보다 클 때
		return n2; // 학번이 더 큰 n2를 return
	}
	else { // n1의 학번과 n2의 학번이 동일할 때
		if (n1->id_subject.second > n2->id_subject.second) { // n1의 신청과목이 n2의 신청과목보다 클 때
			return n1; // subject의 값이 더 큰 n1을 return
		}
		else if (n1->id_subject.second < n2->id_subject.second) { // n2의 신청과목이 n1의 신청과목보다 클 때
			return n2; // subject값이 더 큰 n2를 return
		}
		else {
			return NULL; // 학번과 신청과목이 모두 동일하다면 NULL을 return
		}
	}
}

bool comp(node* a, node* b) { // 정렬을 할 때 사용할 함수 --> 과제 (4)번 기능에서 먼저 신청한 순, 즉 timestamp를 정렬하기 위한 비교 함수
	if (a->time_stamp < b->time_stamp) { // 배열에서 time_stamp를 오름차순으로 정렬
		return true; // 배열 혹은 벡터에서 앞에있는 원소의 time_stamp 값이 더 작으면 true return;
	}
	return false; // 앞에 있는 원소의 time_stamp 값이 더 크면 false를 return
}

class RedBlackTree {
public:
	RedBlackTree(); // 생성자 함수
	void insert_enrollment(int sid, string subject, string name, int semester, string phone, int timestamp); // 수강신청 정보 입력
	void find_studentID_enrollment(node* root, int sid); // 학번이 sid인 학생이 신청한 모든 과목을 출력
	void find_enrollment_number(node* root, string subject); // 특정 과목을 신청한 학생 수 출력
	void find_enrollment_time(node* root, string subject, int k); // 특정 과목을 가장 먼저 신청한 K명의 학번 조회
	node* find_student(node* root, int sid); // sid 학번을 갖고 있는 node를 찾는 함수
	node* root; // main 함수에서 root에 접근하기 위해 public으로 뺌
private:
	int size; // RedBlackTree의 노드의 수
	vector<node*> timestamp; // 특정 과목을 신청한 학생들을 담고 있는 배열 -> (4)번 기능에서 사용할 거임
	int student_number; // subject 과목을 들은 학생의 수
	int sum_of_depth; // subject를 담고 있는 node들의 depth의 합
	node* recoloring(node* me); // recoloring
	void restructuring(node* me); // restructuring
	node* find(node* root, node* findNode, node* parent); // key값을 이용해서 새로운 node가 삽입될 위치의 부모노드를 return 한다. 하지만, key값이 동일할 경우 해당 node를 return;
	void UpdateDepth(node* root); // restructuring 후 depth의 깊이를 update 해주는 함수
};

/*
생성자 함수
*/
RedBlackTree::RedBlackTree() {
	root = NULL;
	size = 0;
	student_number = 0;
	sum_of_depth = 0;
}

/*
recoloring --> double red가 발생했는데
uncle node의 color가 red 일 때
*/
node* RedBlackTree::recoloring(node* me) { // recoloring을 수행하는 코드
	node* parent = me->parent; // parent node
	node* grand_parent = parent->parent; // grand_parent
	node* uncle = NULL; // parent의 sibling node

	if (parent == parent->parent->left_child) { // 부모가 grandParent의 left_child일 때
		uncle = parent->parent->right_child; // uncle node는 grandParent의 right_child 임.
	}
	else if (parent == parent->parent->right_child) { // 부모가 grandParent의 right_child일 때
		uncle = parent->parent->left_child; // uncle node는 grandParent의left_child 임.
	}
	
	parent->color = "black"; // 부모 node의 color를 반대로 변경
	uncle->color = "black"; // 삼촌 node의 color를 반대로 변경
	if (grand_parent != root) { // grand_parent가 root가 아니여야 color를 바꿀 수 있음 --> double red가 발생했을 때, grand_parent는 black 일 수 밖에 없음. grand_parent가 red라면 이미 그 전에 double 레드가 발생하였기에 조건을 만족하지 못함. 
		grand_parent->color = "red"; // grand parent의 color를 반대로 변경
	}	

	return grand_parent; // grand_parent에서 double red가 발생할 수 있기 때문에 grand_parent에서 double red를 확인하기 위해 grand_parent를 return 함.
}

/*
restrucuring --> double red가 발생했는데
uncle node의 color가 black 일 때
*/
void RedBlackTree::restructuring(node* me) {
	node* parent = me->parent; // parent node
	node* grand_parent = parent->parent; // grand parent node

	// t1, t2, t3, t4는 subtree인데, 왼쪽부터 t1, t2, t3, t4가 있음.
	node* t1 = NULL; 
	node* t2 = NULL;
	node* t3 = NULL;
	node* t4 = NULL;

	// restructuring을 할 때 새로운 parent, leftchild, rightchild를 지정하는 변수
	node* new_parent = NULL;
	node* new_left_child = NULL;
	node* new_right_child = NULL;

	if (parent == grand_parent->left_child) { // 부모가 grandParent의 left_child일 때
		if (me == parent->left_child) { // 내가 부모의 left child 일때
			// t1, t2, t3, t4를 정의
			t1 = me->left_child; 
			t2 = me->right_child;
			t3 = parent->right_child;
			t4 = grand_parent->right_child;
			
			/*
			key값이 제일 큰 애는 grand_parent --> new right child
			key값이 중간인 애는 parent --> new parent
			key값이 제일 작은 애는 me --> new left child
			*/
			new_parent = parent;
			new_left_child = me;
			new_right_child = grand_parent;
		}
		else if (me == parent->right_child) { // 내가 부모의 right child 일때
			// t1, t2, t3, t4를 정의
			t1 = parent->left_child;
			t2 = me->left_child;
			t3 = me->right_child;
			t4 = grand_parent->right_child;

			/*
			key값이 제일 큰 애는 grand_parent --> new right child
			key값이 중간인 애는 me --> new parent
			key값이 제일 작은 애는 parent --> new left child
			*/
			new_parent = me;
			new_left_child = parent;
			new_right_child = grand_parent;

		}
	}
	else if (parent == grand_parent->right_child) { // 부모가 grandParent의 right_child일 때
		if (me == parent->left_child) { // 내가 부모의 left child 일때
			// t1, t2, t3, t4를 정의
			t1 = grand_parent->left_child;
			t2 = me->left_child;
			t3 = me->right_child;
			t4 = parent->right_child;

			/*
			key값이 제일 큰 애는 parent --> new right child
			key값이 중간인 애는 me --> new parent
			key값이 제일 작은 애는 grand_parent --> new left child
			*/
			new_parent = me;
			new_left_child = grand_parent;
			new_right_child = parent;
		}
		else if (me == parent->right_child) { // 내가 부모의 right child 일때
			// t1, t2, t3, t4를 정의
			t1 = grand_parent->left_child;
			t2 = parent->left_child;
			t3 = me->left_child;
			t4 = me->right_child;

			/*
			key값이 제일 큰 애는 me --> new right child
			key값이 중간인 애는 parent --> new parent
			key값이 제일 작은 애는 grand_parent --> new left child
			*/
			new_parent = parent;
			new_left_child = grand_parent;
			new_right_child = me;
		}
	}
	new_parent->color = "black"; // 부모의 컬러는 black으로 변경
	
	//자식들의 color는 red로 변경해야함
	new_left_child->color = "red";
	new_right_child->color = "red";

	if (grand_parent == root) { // 만약 grand_parent가 root면, root를 new_parent로 갱신을 해야함.
		root = new_parent;
		new_parent->parent = NULL; // new_parent의 부모는 NULL로 변경
	} 
	else { //grand_parent가 root가 아니라면 
		new_parent->parent = grand_parent->parent; // 새로운 parent와 기존의 grand_parent의 parent와 연결을 해주어야함
		if (grand_parent->parent->left_child == grand_parent) { // grand parent가 왼쪽 자식이었으면
			grand_parent->parent->left_child = new_parent; // grand parent의 parent의 왼쪽 자식과 연결 
		}
		else if (grand_parent->parent->right_child == grand_parent) { // grand parent가 오른쪽 자식이었으면
			grand_parent->parent->right_child = new_parent; // grand parent의 parent의 오른쪽 자식과 연결
		}
	}



	// restructuring을 진행
	new_parent->left_child = new_left_child; //새로운 부모의 새로운 왼쪽 자식 연결
	new_left_child->parent = new_parent; // 새로운 왼쪽 자식을 새로운 부모와 연결

	new_parent->right_child = new_right_child; //새로운 부모의 오른쪽 자식 연결
	new_right_child->parent = new_parent; // 새로운 오른쪽 자식을 새로운 부모와 연결

	

	//새로운 왼쪽 자식과 subtree t1, t2 연결
	new_left_child->left_child = t1;
	if (t1 != NULL) { // subtree가 NULL이라면 subtree의 부모를 연결할 수 없음
		t1->parent = new_left_child;
	}

	new_left_child->right_child = t2;
	if (t2 != NULL) { // subtree가 NULL이라면 subtree의 부모를 연결할 수 없음
		t2->parent = new_left_child;
	}
	
	//새로운 오른쪽 자식과 subtree t3, t4 연결
	new_right_child->left_child = t3;
	if (t3 != NULL) {// subtree가 NULL이라면 subtree의 부모를 연결할 수 없음
		t3->parent = new_right_child;
	}

	new_right_child->right_child = t4;
	if (t4 != NULL) { // subtree가 NULL이라면 subtree의 부모를 연결할 수 없음
		t4->parent = new_right_child;
	}
	
	
	UpdateDepth(new_parent); // restrucuring을 진행하면 depth 값이 바뀌므로 depth값을 update 함.
	
}

/*
어떤 새로운 노드를 삽입할 때,
삽입할 node의 부모를 return 하는 함수.
*/
node* RedBlackTree::find(node* root,node* findNode , node* parent = NULL) {
	//base case
	if (root == NULL) { // external node에 도착하면
		return parent; // 삽입할 노드의 부모를 return
	}
	
	
	if (comparator(root, findNode) == NULL) { // findNode과 root의 key값이 동일하다면
		return root; // findNode와 key값이 동일한 node를 return
	}
	else if (comparator(root, findNode) == root) { // root의 key값이 더 크다면
		return find(root->left_child, findNode, root); //재귀적으로 왼쪽 subtree로 내려가서 비교
	}
	else if (comparator(root, findNode) == findNode) { // findNode의 key 값이 더 크다면
		return find(root->right_child, findNode, root); // 재귀적으로 오른쪽 subtree로 내려가서 비교
	}
}

/*
sid를 갖는 node를 찾는 함수
*/
node* RedBlackTree::find_student(node* root, int sid) {
	if (root == NULL) { // basecase: sid 학번을 갖고 있는 node가 존재하지 않는다면 
		return NULL; // NULL return
	}
	
	if (root->id_subject.first > sid) { //sid 값이 root의 sid값보다 작다면 
		return find_student(root->left_child, sid); // left subtree로 내려간다.
	}
	else if (root->id_subject.first < sid) { //sid 값이 root의 sid값보다 크다면
		return find_student(root->right_child, sid); // right subtree로 내려간다.
	}
	else { // sid 값이 root의 sid값과 같다면
		return root; // root를 return
	}
	
}

/*
restructuring 결과 depth 값이 바뀌기 때문에
depth값을 update 해주는 함수
*/
void RedBlackTree::UpdateDepth(node* root) {
	if (root == NULL) { // basecase : external node에 도착했을 때
		return;
	}
	
	if (this->root == root) { //root 일 경우 depth를 0으로 초기화
		root->depth = 0;
	}
	else { // root가 아닐 경우 부모 depth + 1;
		root->depth = root->parent->depth + 1;
	}
	
	UpdateDepth(root->left_child); // 재귀적으로 left_child depth값 update
	UpdateDepth(root->right_child); // 재귀적으로 right_child depth값 update
}

/*
(1) 수강신청 정보 입력 
*/
void RedBlackTree::insert_enrollment(int sid, string subject, string name, int semester, string phone, int timestamp) {

	int depth = -1; // 새로 삽입한 node의 depth
	int exist = -1; // 기존에 동일한 key값을 갖는 노드가 있는지 여부 1: 존재O, 0: 존재X

	node* new_node = new node(NULL, NULL, NULL, "red", -1, sid, subject, name ,semester, phone, timestamp); // new node를 이용해서 새로 삽입될 자리를 찾을 예정
	if (size == 0) { // empty tree일 때 삽입을 할경우
		root = new_node;
		new_node->depth = 0;
		new_node->color = "black"; // root property에 의해 root의 color는 black이 되어야함

		exist = 0;
		depth = 0;
		size++;
		
		cout << depth << " " << exist;
		return; // 종료
	}
	

	node* find_node_parent = find(root, new_node); // 새로 삽입할 노드의 부모를 리턴;

	if (comparator(find_node_parent, new_node) == NULL) { // key값인 node가 기존에 이미 존재한다면 (즉, new_node의 키값과 find_node_parent의 key값이 동일하면)
		find_node_parent->time_stamp = timestamp; // timestamp 갱신
		depth = find_node_parent->depth; // depth값 갱신
		exist = 1; // 존재 하므로 exist는 1
		delete new_node; // 삽입할 노드 삭제
		cout << depth << " " << exist;
		return; // 함수 종료
	}
	else if (comparator(find_node_parent, new_node) == find_node_parent) { // 찾은 부모의 key값이 더 크다면
		// 삽입할 노드를 삽입
		new_node->parent = find_node_parent;
		find_node_parent->left_child = new_node; //부모 key값이 더 크므로 left child로 내려가서 삽입
		new_node->depth = find_node_parent->depth + 1;
		exist = 0; // 존재 X
		
		size++; // 전제 node 개수 1증가
	}
	else if (comparator(find_node_parent, new_node) == new_node) { // 삽입할 node의 key값이 더 크다면
		// 삽입할 노드를 삽입
		new_node->parent = find_node_parent;
		find_node_parent->right_child = new_node; //부모 key값이 더 작으므로 right child로 내려가서 삽입
		new_node->depth = find_node_parent->depth + 1;
		exist = 0; // 존재 X

		size++; // 전체 node 개수 1증가
	}

	// 다음 코드는 property를 만족시키기 위해서 recoloring과 restructuring을 진행함

	node* current_node = new_node; // double red를 체크하기 위한 node
	node* uncle = NULL; // parent의 sibling node

	while (current_node->color == "red" && current_node->parent->color == "red") { // double red인 경우 recoloring or restructuring을 진행함

		/*
		다음 코드는 uncle node를 찾는 코드
		*/
		if (current_node->parent == current_node->parent->parent->left_child) { // 부모가 grandParent의 left_child일 때
			uncle = current_node->parent->parent->right_child; // uncle node는 grandParent의 right_child
		}
		else if (current_node->parent == current_node->parent->parent->right_child) { // 부모가 grandParent의 right_child일 때
			uncle = current_node->parent->parent->left_child; // uncle node는 grandParent의 left_child
		}

		if (uncle == NULL) { // uncle node가 null이면 external node이기 때문에 color는 black
			restructuring(current_node); // restructuring 수행
		}
		else if (uncle->color == "black") { // uncle의 color가 black 인 경우
			restructuring(current_node); // restructuring 수행
		}
		else if (uncle->color == "red") { // uncle의 color가 red인 경우
			current_node = recoloring(current_node); // recoloring 수행 후 current_node 의 grand_parent를 return --> grand_parent에서 double red가 발생할 수 있기 때문에
		}

	}

	if (new_node == this->root) { // 만약 새로 삽입할 node가 root에 위치한다면
		new_node->color = "black"; // color를 black으로 변경
	}

	if (exist == 0) { // 구조 조건을 만족 시킨 후 new_node의 depth를 갱신함.
		depth = new_node->depth; 
	}

	cout << depth << " " << exist;
}

/*
(2) 특정 학생의 전체 신청 과목 조회 
*/
void RedBlackTree::find_studentID_enrollment(node* root, int sid) { // 학번이 sid인 학생이 신청한 모든 과목명을 사전 순으로 출력
	if (root == NULL) { // basecase: inorder traversal로 구현을 할 건데, 만약 externel node에 도착하면 return을 해야함
		return;
	}
	
	find_studentID_enrollment(root->left_child, sid); // left child로 내려감
	
	// student id가 동일할 때 과목명과 color를 출력
	if (root->id_subject.first == sid) { // sid값이 node의 sid값과 같다면
		cout << root->id_subject.second << " ";
		if (root->color == "black") { // color가 black이면 B로 표시
			cout << "B ";
		}
		else if (root->color == "red") { // color가 RED면 R로 표시
			cout << "R ";
		}
	}

	find_studentID_enrollment(root->right_child, sid); // right child로 내려감
}

/*
(3) 특정 과목을 신청한 학생 수
*/
void RedBlackTree::find_enrollment_number(node* root, string subject) { // subject를 신청한 학생의 수와 깊이를 합한 것을 출력하는 함수
	if (root == NULL) { // post order traversal로 구현을 할 건데, 만약 externel node에 도착하면 return을 해야함
		return;
	}

	find_enrollment_number(root->left_child, subject); //왼쪽 자식으로 내려감
	find_enrollment_number(root->right_child, subject); //오른쪽 자식으로 내려감
	
	if (root->id_subject.second == subject) { // subject를 신청한 학생이라면 학생수를 1 증가시키고, 그 정보를 담고 있는 노드의 depth를 더해야함
		student_number++;
		sum_of_depth = sum_of_depth + root->depth;
	}

	if (root == this->root) { // post order traversal로 구현을 하였으므로, 제일 마지막에는 root가 visit될 것임. root가 visit될 때 cout을 하면된다.
		cout << student_number << " " << sum_of_depth;
		// 다음번에 해당 변수를 다시 사용하기 위해 0으로 초기화
		student_number = 0;
		sum_of_depth = 0;
	}
}

/*
(4) 특정 과목을 가장 먼저 신청한 K명의 학번 조회
*/
void RedBlackTree::find_enrollment_time(node* root, string subject, int k) { // 특정 과목을 가장 먼저 신청한 K명의 학번 조회
	if (root == NULL) { // base case;
		return;
	}

	// post order traversal로 구현
	find_enrollment_time(root->left_child, subject, k); //왼쪽 자식으로 내려감
	find_enrollment_time(root->right_child, subject, k); //오른쪽 자식으로 내려감
	
	// visit 함수, subject가 동일하면 우선 timestamp vector에 담음
	if (root->id_subject.second == subject) {
		timestamp.push_back(root);
	}

	// root는 제일 마지막에 조사되므로 이 때 시간순으로 node를 정렬할 예정
	if (root == this->root) {
		sort(timestamp.begin(), timestamp.end(), comp); //시간 순으로 오름 차순 정렬
		
		if (k > timestamp.size()) { // k값이 벡터 사이즈보다 더 크면 벡터 개수만큼만 출력해야함
			for (int i = 0; i < timestamp.size(); i++) {
				cout << timestamp[i]->id_subject.first << " "; //시간 순으로 학번, color 출력
				if (timestamp[i]->color == "black") { // color가 black 이면 B, red이면 R로 출력
					cout << "B ";
				}
				else if (timestamp[i]->color == "red") {
					cout << "R ";
				}
			}
		}
		else { // k값이 벡터 사이즈보다 더 작으면
			for (int i = 0; i < k; i++) { // K까지만 출력해야함
				cout << timestamp[i]->id_subject.first << " ";
				if (timestamp[i]->color == "black") { // color가 black이면 B, red면 R로 출력
					cout << "B ";
				}
				else if (timestamp[i]->color == "red") {
					cout << "R ";
				}
			}
		}

		timestamp.clear(); // 벡터 모든 원소 삭제
		vector<node*>().swap(timestamp); // 메모리 공간을 위해 capacity도 0으로 초기화
	}
}

int main() {
	
	RedBlackTree rbt; // redblack tree를 만듬

	int sid; //학번
	string subject; // 과목명
	string sname; // 학생이름
	int semester; // 학기
	string phone; // 전화번호
	int timestamp; // timestamp

	int query_num; // 질의의 수
	cin >> query_num;

	string order; // 명령어
	for (int i = 0; i < query_num; i++) { // 질의의 수만큼 명령 수행
		cin >> order; //명령어를 받음
		if (order == "I") { // 명령어가 I 일 때
			cin >> sid >> subject >> sname >> semester >> phone >> timestamp;
			rbt.insert_enrollment(sid, subject, sname, semester, phone, timestamp); // 수간신청 정보 입력
			cout << "\n";
		}
		else if (order == "L") { // 명령어가 L 일 
			cin >> sid; 
			node* find_node = rbt.find_student(rbt.root, sid); // 시간 절약을 위해 sid를 갖는 node 중 가장 맨 위에 있는 node를 return함
			if (find_node == NULL) { // sid를 갖는 node가 존재하지 않는다면
				cout << "No records found";
			}
			else { // sid를 갖는 node가 존재한다면
				rbt.find_studentID_enrollment(find_node, sid); // find_node를 이용하여 inorder traversal 방식으로 sid가 동일한 node를 찾음
			}
			cout << "\n";
		}
		else if (order == "C") { // 명령어가 C 일 때
			cin >> subject;
			rbt.find_enrollment_number(rbt.root, subject); // subject를 신청한 학생 수와 sum depth 출력
			cout << "\n";
		}
		else if (order == "M") { // 명령어가 M 일 때
			int k; // 선착순 K명
			cin >> subject >> k;
			rbt.find_enrollment_time(rbt.root, subject, k); // subject를 신청한 선착순 K명에 대해 sid, color 출력
			cout << "\n";
		}
	}
}