#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



struct node {
	node* parent; // �θ� ���
	node* left_child; // ���� �ڽ� ���
	node* right_child; // ������ �ڽ� ���
	string color;
	int depth;

	pair<int, string> id_subject; // �й��� ��û���� ���� �����ϴ� ����
	string name; // �л� �̸�
	int semester; // �̼� �б� ��
	string phone_number; // ��ȭ ��ȣ
	int time_stamp; // ���� ��û �ð�

	/*
	������ �Լ� node�� �ʱ�ȭ �� �� �ְ� ���ִ� �Լ�
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

node* comparator(node* n1, node* n2) { // ���ؼ� key���� �� ū node�� return
	if (n1->id_subject.first > n2->id_subject.first) { // n1�� �й��� n2�� �й����� Ŭ ��
		return n1; // �й��� �� ū n1�� return
	}
	else if (n1->id_subject.first < n2->id_subject.first) { // n2�� �й��� n1�� �й����� Ŭ ��
		return n2; // �й��� �� ū n2�� return
	}
	else { // n1�� �й��� n2�� �й��� ������ ��
		if (n1->id_subject.second > n2->id_subject.second) { // n1�� ��û������ n2�� ��û���񺸴� Ŭ ��
			return n1; // subject�� ���� �� ū n1�� return
		}
		else if (n1->id_subject.second < n2->id_subject.second) { // n2�� ��û������ n1�� ��û���񺸴� Ŭ ��
			return n2; // subject���� �� ū n2�� return
		}
		else {
			return NULL; // �й��� ��û������ ��� �����ϴٸ� NULL�� return
		}
	}
}

bool comp(node* a, node* b) { // ������ �� �� ����� �Լ� --> ���� (4)�� ��ɿ��� ���� ��û�� ��, �� timestamp�� �����ϱ� ���� �� �Լ�
	if (a->time_stamp < b->time_stamp) { // �迭���� time_stamp�� ������������ ����
		return true; // �迭 Ȥ�� ���Ϳ��� �տ��ִ� ������ time_stamp ���� �� ������ true return;
	}
	return false; // �տ� �ִ� ������ time_stamp ���� �� ũ�� false�� return
}

class RedBlackTree {
public:
	RedBlackTree(); // ������ �Լ�
	void insert_enrollment(int sid, string subject, string name, int semester, string phone, int timestamp); // ������û ���� �Է�
	void find_studentID_enrollment(node* root, int sid); // �й��� sid�� �л��� ��û�� ��� ������ ���
	void find_enrollment_number(node* root, string subject); // Ư�� ������ ��û�� �л� �� ���
	void find_enrollment_time(node* root, string subject, int k); // Ư�� ������ ���� ���� ��û�� K���� �й� ��ȸ
	node* find_student(node* root, int sid); // sid �й��� ���� �ִ� node�� ã�� �Լ�
	node* root; // main �Լ����� root�� �����ϱ� ���� public���� ��
private:
	int size; // RedBlackTree�� ����� ��
	vector<node*> timestamp; // Ư�� ������ ��û�� �л����� ��� �ִ� �迭 -> (4)�� ��ɿ��� ����� ����
	int student_number; // subject ������ ���� �л��� ��
	int sum_of_depth; // subject�� ��� �ִ� node���� depth�� ��
	node* recoloring(node* me); // recoloring
	void restructuring(node* me); // restructuring
	node* find(node* root, node* findNode, node* parent); // key���� �̿��ؼ� ���ο� node�� ���Ե� ��ġ�� �θ��带 return �Ѵ�. ������, key���� ������ ��� �ش� node�� return;
	void UpdateDepth(node* root); // restructuring �� depth�� ���̸� update ���ִ� �Լ�
};

/*
������ �Լ�
*/
RedBlackTree::RedBlackTree() {
	root = NULL;
	size = 0;
	student_number = 0;
	sum_of_depth = 0;
}

/*
recoloring --> double red�� �߻��ߴµ�
uncle node�� color�� red �� ��
*/
node* RedBlackTree::recoloring(node* me) { // recoloring�� �����ϴ� �ڵ�
	node* parent = me->parent; // parent node
	node* grand_parent = parent->parent; // grand_parent
	node* uncle = NULL; // parent�� sibling node

	if (parent == parent->parent->left_child) { // �θ� grandParent�� left_child�� ��
		uncle = parent->parent->right_child; // uncle node�� grandParent�� right_child ��.
	}
	else if (parent == parent->parent->right_child) { // �θ� grandParent�� right_child�� ��
		uncle = parent->parent->left_child; // uncle node�� grandParent��left_child ��.
	}
	
	parent->color = "black"; // �θ� node�� color�� �ݴ�� ����
	uncle->color = "black"; // ���� node�� color�� �ݴ�� ����
	if (grand_parent != root) { // grand_parent�� root�� �ƴϿ��� color�� �ٲ� �� ���� --> double red�� �߻����� ��, grand_parent�� black �� �� �ۿ� ����. grand_parent�� red��� �̹� �� ���� double ���尡 �߻��Ͽ��⿡ ������ �������� ����. 
		grand_parent->color = "red"; // grand parent�� color�� �ݴ�� ����
	}	

	return grand_parent; // grand_parent���� double red�� �߻��� �� �ֱ� ������ grand_parent���� double red�� Ȯ���ϱ� ���� grand_parent�� return ��.
}

/*
restrucuring --> double red�� �߻��ߴµ�
uncle node�� color�� black �� ��
*/
void RedBlackTree::restructuring(node* me) {
	node* parent = me->parent; // parent node
	node* grand_parent = parent->parent; // grand parent node

	// t1, t2, t3, t4�� subtree�ε�, ���ʺ��� t1, t2, t3, t4�� ����.
	node* t1 = NULL; 
	node* t2 = NULL;
	node* t3 = NULL;
	node* t4 = NULL;

	// restructuring�� �� �� ���ο� parent, leftchild, rightchild�� �����ϴ� ����
	node* new_parent = NULL;
	node* new_left_child = NULL;
	node* new_right_child = NULL;

	if (parent == grand_parent->left_child) { // �θ� grandParent�� left_child�� ��
		if (me == parent->left_child) { // ���� �θ��� left child �϶�
			// t1, t2, t3, t4�� ����
			t1 = me->left_child; 
			t2 = me->right_child;
			t3 = parent->right_child;
			t4 = grand_parent->right_child;
			
			/*
			key���� ���� ū �ִ� grand_parent --> new right child
			key���� �߰��� �ִ� parent --> new parent
			key���� ���� ���� �ִ� me --> new left child
			*/
			new_parent = parent;
			new_left_child = me;
			new_right_child = grand_parent;
		}
		else if (me == parent->right_child) { // ���� �θ��� right child �϶�
			// t1, t2, t3, t4�� ����
			t1 = parent->left_child;
			t2 = me->left_child;
			t3 = me->right_child;
			t4 = grand_parent->right_child;

			/*
			key���� ���� ū �ִ� grand_parent --> new right child
			key���� �߰��� �ִ� me --> new parent
			key���� ���� ���� �ִ� parent --> new left child
			*/
			new_parent = me;
			new_left_child = parent;
			new_right_child = grand_parent;

		}
	}
	else if (parent == grand_parent->right_child) { // �θ� grandParent�� right_child�� ��
		if (me == parent->left_child) { // ���� �θ��� left child �϶�
			// t1, t2, t3, t4�� ����
			t1 = grand_parent->left_child;
			t2 = me->left_child;
			t3 = me->right_child;
			t4 = parent->right_child;

			/*
			key���� ���� ū �ִ� parent --> new right child
			key���� �߰��� �ִ� me --> new parent
			key���� ���� ���� �ִ� grand_parent --> new left child
			*/
			new_parent = me;
			new_left_child = grand_parent;
			new_right_child = parent;
		}
		else if (me == parent->right_child) { // ���� �θ��� right child �϶�
			// t1, t2, t3, t4�� ����
			t1 = grand_parent->left_child;
			t2 = parent->left_child;
			t3 = me->left_child;
			t4 = me->right_child;

			/*
			key���� ���� ū �ִ� me --> new right child
			key���� �߰��� �ִ� parent --> new parent
			key���� ���� ���� �ִ� grand_parent --> new left child
			*/
			new_parent = parent;
			new_left_child = grand_parent;
			new_right_child = me;
		}
	}
	new_parent->color = "black"; // �θ��� �÷��� black���� ����
	
	//�ڽĵ��� color�� red�� �����ؾ���
	new_left_child->color = "red";
	new_right_child->color = "red";

	if (grand_parent == root) { // ���� grand_parent�� root��, root�� new_parent�� ������ �ؾ���.
		root = new_parent;
		new_parent->parent = NULL; // new_parent�� �θ�� NULL�� ����
	} 
	else { //grand_parent�� root�� �ƴ϶�� 
		new_parent->parent = grand_parent->parent; // ���ο� parent�� ������ grand_parent�� parent�� ������ ���־����
		if (grand_parent->parent->left_child == grand_parent) { // grand parent�� ���� �ڽ��̾�����
			grand_parent->parent->left_child = new_parent; // grand parent�� parent�� ���� �ڽİ� ���� 
		}
		else if (grand_parent->parent->right_child == grand_parent) { // grand parent�� ������ �ڽ��̾�����
			grand_parent->parent->right_child = new_parent; // grand parent�� parent�� ������ �ڽİ� ����
		}
	}



	// restructuring�� ����
	new_parent->left_child = new_left_child; //���ο� �θ��� ���ο� ���� �ڽ� ����
	new_left_child->parent = new_parent; // ���ο� ���� �ڽ��� ���ο� �θ�� ����

	new_parent->right_child = new_right_child; //���ο� �θ��� ������ �ڽ� ����
	new_right_child->parent = new_parent; // ���ο� ������ �ڽ��� ���ο� �θ�� ����

	

	//���ο� ���� �ڽİ� subtree t1, t2 ����
	new_left_child->left_child = t1;
	if (t1 != NULL) { // subtree�� NULL�̶�� subtree�� �θ� ������ �� ����
		t1->parent = new_left_child;
	}

	new_left_child->right_child = t2;
	if (t2 != NULL) { // subtree�� NULL�̶�� subtree�� �θ� ������ �� ����
		t2->parent = new_left_child;
	}
	
	//���ο� ������ �ڽİ� subtree t3, t4 ����
	new_right_child->left_child = t3;
	if (t3 != NULL) {// subtree�� NULL�̶�� subtree�� �θ� ������ �� ����
		t3->parent = new_right_child;
	}

	new_right_child->right_child = t4;
	if (t4 != NULL) { // subtree�� NULL�̶�� subtree�� �θ� ������ �� ����
		t4->parent = new_right_child;
	}
	
	
	UpdateDepth(new_parent); // restrucuring�� �����ϸ� depth ���� �ٲ�Ƿ� depth���� update ��.
	
}

/*
� ���ο� ��带 ������ ��,
������ node�� �θ� return �ϴ� �Լ�.
*/
node* RedBlackTree::find(node* root,node* findNode , node* parent = NULL) {
	//base case
	if (root == NULL) { // external node�� �����ϸ�
		return parent; // ������ ����� �θ� return
	}
	
	
	if (comparator(root, findNode) == NULL) { // findNode�� root�� key���� �����ϴٸ�
		return root; // findNode�� key���� ������ node�� return
	}
	else if (comparator(root, findNode) == root) { // root�� key���� �� ũ�ٸ�
		return find(root->left_child, findNode, root); //��������� ���� subtree�� �������� ��
	}
	else if (comparator(root, findNode) == findNode) { // findNode�� key ���� �� ũ�ٸ�
		return find(root->right_child, findNode, root); // ��������� ������ subtree�� �������� ��
	}
}

/*
sid�� ���� node�� ã�� �Լ�
*/
node* RedBlackTree::find_student(node* root, int sid) {
	if (root == NULL) { // basecase: sid �й��� ���� �ִ� node�� �������� �ʴ´ٸ� 
		return NULL; // NULL return
	}
	
	if (root->id_subject.first > sid) { //sid ���� root�� sid������ �۴ٸ� 
		return find_student(root->left_child, sid); // left subtree�� ��������.
	}
	else if (root->id_subject.first < sid) { //sid ���� root�� sid������ ũ�ٸ�
		return find_student(root->right_child, sid); // right subtree�� ��������.
	}
	else { // sid ���� root�� sid���� ���ٸ�
		return root; // root�� return
	}
	
}

/*
restructuring ��� depth ���� �ٲ�� ������
depth���� update ���ִ� �Լ�
*/
void RedBlackTree::UpdateDepth(node* root) {
	if (root == NULL) { // basecase : external node�� �������� ��
		return;
	}
	
	if (this->root == root) { //root �� ��� depth�� 0���� �ʱ�ȭ
		root->depth = 0;
	}
	else { // root�� �ƴ� ��� �θ� depth + 1;
		root->depth = root->parent->depth + 1;
	}
	
	UpdateDepth(root->left_child); // ��������� left_child depth�� update
	UpdateDepth(root->right_child); // ��������� right_child depth�� update
}

/*
(1) ������û ���� �Է� 
*/
void RedBlackTree::insert_enrollment(int sid, string subject, string name, int semester, string phone, int timestamp) {

	int depth = -1; // ���� ������ node�� depth
	int exist = -1; // ������ ������ key���� ���� ��尡 �ִ��� ���� 1: ����O, 0: ����X

	node* new_node = new node(NULL, NULL, NULL, "red", -1, sid, subject, name ,semester, phone, timestamp); // new node�� �̿��ؼ� ���� ���Ե� �ڸ��� ã�� ����
	if (size == 0) { // empty tree�� �� ������ �Ұ��
		root = new_node;
		new_node->depth = 0;
		new_node->color = "black"; // root property�� ���� root�� color�� black�� �Ǿ����

		exist = 0;
		depth = 0;
		size++;
		
		cout << depth << " " << exist;
		return; // ����
	}
	

	node* find_node_parent = find(root, new_node); // ���� ������ ����� �θ� ����;

	if (comparator(find_node_parent, new_node) == NULL) { // key���� node�� ������ �̹� �����Ѵٸ� (��, new_node�� Ű���� find_node_parent�� key���� �����ϸ�)
		find_node_parent->time_stamp = timestamp; // timestamp ����
		depth = find_node_parent->depth; // depth�� ����
		exist = 1; // ���� �ϹǷ� exist�� 1
		delete new_node; // ������ ��� ����
		cout << depth << " " << exist;
		return; // �Լ� ����
	}
	else if (comparator(find_node_parent, new_node) == find_node_parent) { // ã�� �θ��� key���� �� ũ�ٸ�
		// ������ ��带 ����
		new_node->parent = find_node_parent;
		find_node_parent->left_child = new_node; //�θ� key���� �� ũ�Ƿ� left child�� �������� ����
		new_node->depth = find_node_parent->depth + 1;
		exist = 0; // ���� X
		
		size++; // ���� node ���� 1����
	}
	else if (comparator(find_node_parent, new_node) == new_node) { // ������ node�� key���� �� ũ�ٸ�
		// ������ ��带 ����
		new_node->parent = find_node_parent;
		find_node_parent->right_child = new_node; //�θ� key���� �� �����Ƿ� right child�� �������� ����
		new_node->depth = find_node_parent->depth + 1;
		exist = 0; // ���� X

		size++; // ��ü node ���� 1����
	}

	// ���� �ڵ�� property�� ������Ű�� ���ؼ� recoloring�� restructuring�� ������

	node* current_node = new_node; // double red�� üũ�ϱ� ���� node
	node* uncle = NULL; // parent�� sibling node

	while (current_node->color == "red" && current_node->parent->color == "red") { // double red�� ��� recoloring or restructuring�� ������

		/*
		���� �ڵ�� uncle node�� ã�� �ڵ�
		*/
		if (current_node->parent == current_node->parent->parent->left_child) { // �θ� grandParent�� left_child�� ��
			uncle = current_node->parent->parent->right_child; // uncle node�� grandParent�� right_child
		}
		else if (current_node->parent == current_node->parent->parent->right_child) { // �θ� grandParent�� right_child�� ��
			uncle = current_node->parent->parent->left_child; // uncle node�� grandParent�� left_child
		}

		if (uncle == NULL) { // uncle node�� null�̸� external node�̱� ������ color�� black
			restructuring(current_node); // restructuring ����
		}
		else if (uncle->color == "black") { // uncle�� color�� black �� ���
			restructuring(current_node); // restructuring ����
		}
		else if (uncle->color == "red") { // uncle�� color�� red�� ���
			current_node = recoloring(current_node); // recoloring ���� �� current_node �� grand_parent�� return --> grand_parent���� double red�� �߻��� �� �ֱ� ������
		}

	}

	if (new_node == this->root) { // ���� ���� ������ node�� root�� ��ġ�Ѵٸ�
		new_node->color = "black"; // color�� black���� ����
	}

	if (exist == 0) { // ���� ������ ���� ��Ų �� new_node�� depth�� ������.
		depth = new_node->depth; 
	}

	cout << depth << " " << exist;
}

/*
(2) Ư�� �л��� ��ü ��û ���� ��ȸ 
*/
void RedBlackTree::find_studentID_enrollment(node* root, int sid) { // �й��� sid�� �л��� ��û�� ��� ������� ���� ������ ���
	if (root == NULL) { // basecase: inorder traversal�� ������ �� �ǵ�, ���� externel node�� �����ϸ� return�� �ؾ���
		return;
	}
	
	find_studentID_enrollment(root->left_child, sid); // left child�� ������
	
	// student id�� ������ �� ������ color�� ���
	if (root->id_subject.first == sid) { // sid���� node�� sid���� ���ٸ�
		cout << root->id_subject.second << " ";
		if (root->color == "black") { // color�� black�̸� B�� ǥ��
			cout << "B ";
		}
		else if (root->color == "red") { // color�� RED�� R�� ǥ��
			cout << "R ";
		}
	}

	find_studentID_enrollment(root->right_child, sid); // right child�� ������
}

/*
(3) Ư�� ������ ��û�� �л� ��
*/
void RedBlackTree::find_enrollment_number(node* root, string subject) { // subject�� ��û�� �л��� ���� ���̸� ���� ���� ����ϴ� �Լ�
	if (root == NULL) { // post order traversal�� ������ �� �ǵ�, ���� externel node�� �����ϸ� return�� �ؾ���
		return;
	}

	find_enrollment_number(root->left_child, subject); //���� �ڽ����� ������
	find_enrollment_number(root->right_child, subject); //������ �ڽ����� ������
	
	if (root->id_subject.second == subject) { // subject�� ��û�� �л��̶�� �л����� 1 ������Ű��, �� ������ ��� �ִ� ����� depth�� ���ؾ���
		student_number++;
		sum_of_depth = sum_of_depth + root->depth;
	}

	if (root == this->root) { // post order traversal�� ������ �Ͽ����Ƿ�, ���� ���������� root�� visit�� ����. root�� visit�� �� cout�� �ϸ�ȴ�.
		cout << student_number << " " << sum_of_depth;
		// �������� �ش� ������ �ٽ� ����ϱ� ���� 0���� �ʱ�ȭ
		student_number = 0;
		sum_of_depth = 0;
	}
}

/*
(4) Ư�� ������ ���� ���� ��û�� K���� �й� ��ȸ
*/
void RedBlackTree::find_enrollment_time(node* root, string subject, int k) { // Ư�� ������ ���� ���� ��û�� K���� �й� ��ȸ
	if (root == NULL) { // base case;
		return;
	}

	// post order traversal�� ����
	find_enrollment_time(root->left_child, subject, k); //���� �ڽ����� ������
	find_enrollment_time(root->right_child, subject, k); //������ �ڽ����� ������
	
	// visit �Լ�, subject�� �����ϸ� �켱 timestamp vector�� ����
	if (root->id_subject.second == subject) {
		timestamp.push_back(root);
	}

	// root�� ���� �������� ����ǹǷ� �� �� �ð������� node�� ������ ����
	if (root == this->root) {
		sort(timestamp.begin(), timestamp.end(), comp); //�ð� ������ ���� ���� ����
		
		if (k > timestamp.size()) { // k���� ���� ������� �� ũ�� ���� ������ŭ�� ����ؾ���
			for (int i = 0; i < timestamp.size(); i++) {
				cout << timestamp[i]->id_subject.first << " "; //�ð� ������ �й�, color ���
				if (timestamp[i]->color == "black") { // color�� black �̸� B, red�̸� R�� ���
					cout << "B ";
				}
				else if (timestamp[i]->color == "red") {
					cout << "R ";
				}
			}
		}
		else { // k���� ���� ������� �� ������
			for (int i = 0; i < k; i++) { // K������ ����ؾ���
				cout << timestamp[i]->id_subject.first << " ";
				if (timestamp[i]->color == "black") { // color�� black�̸� B, red�� R�� ���
					cout << "B ";
				}
				else if (timestamp[i]->color == "red") {
					cout << "R ";
				}
			}
		}

		timestamp.clear(); // ���� ��� ���� ����
		vector<node*>().swap(timestamp); // �޸� ������ ���� capacity�� 0���� �ʱ�ȭ
	}
}

int main() {
	
	RedBlackTree rbt; // redblack tree�� ����

	int sid; //�й�
	string subject; // �����
	string sname; // �л��̸�
	int semester; // �б�
	string phone; // ��ȭ��ȣ
	int timestamp; // timestamp

	int query_num; // ������ ��
	cin >> query_num;

	string order; // ��ɾ�
	for (int i = 0; i < query_num; i++) { // ������ ����ŭ ��� ����
		cin >> order; //��ɾ ����
		if (order == "I") { // ��ɾ I �� ��
			cin >> sid >> subject >> sname >> semester >> phone >> timestamp;
			rbt.insert_enrollment(sid, subject, sname, semester, phone, timestamp); // ������û ���� �Է�
			cout << "\n";
		}
		else if (order == "L") { // ��ɾ L �� 
			cin >> sid; 
			node* find_node = rbt.find_student(rbt.root, sid); // �ð� ������ ���� sid�� ���� node �� ���� �� ���� �ִ� node�� return��
			if (find_node == NULL) { // sid�� ���� node�� �������� �ʴ´ٸ�
				cout << "No records found";
			}
			else { // sid�� ���� node�� �����Ѵٸ�
				rbt.find_studentID_enrollment(find_node, sid); // find_node�� �̿��Ͽ� inorder traversal ������� sid�� ������ node�� ã��
			}
			cout << "\n";
		}
		else if (order == "C") { // ��ɾ C �� ��
			cin >> subject;
			rbt.find_enrollment_number(rbt.root, subject); // subject�� ��û�� �л� ���� sum depth ���
			cout << "\n";
		}
		else if (order == "M") { // ��ɾ M �� ��
			int k; // ������ K��
			cin >> subject >> k;
			rbt.find_enrollment_time(rbt.root, subject, k); // subject�� ��û�� ������ K�� ���� sid, color ���
			cout << "\n";
		}
	}
}