#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


template <typename K, typename V>
struct TreeNode
{
	K key;
	V value;
	TreeNode* lChild;
	TreeNode* rChild;

	TreeNode()
	{
		this->lChild = this->rChild = nullptr;
	}

	TreeNode(K key, V value)
	{
		this->key = key;
		this->value = value;
		this->lChild = this->rChild = nullptr;
	}

	bool isLeaf()
	{
		return !this->lChild && !this->rChild;
	}
};

template <typename K, typename V>
class SpecialBST
{
private:
	TreeNode<K, V>* root;

	void inorderPrintKeys(TreeNode<K, V>* ptr)
	{
		if (ptr)
		{
			inorderPrintKeys(ptr->lChild);
			cout << ptr->key << endl;
			inorderPrintKeys(ptr->rChild);
		}
	}

	void getValuesInOrder(TreeNode<K, V>* ptr, vector<V>& v)
	{
		if (ptr)
		{
			getValuesInOrder(ptr->lChild, v);
			v.push_back(ptr->value);
			getValuesInOrder(ptr->rChild, v);
		}
	}

	void preOrderPrintKeys(TreeNode<K, V>* ptr)
	{
		if (ptr)
		{
			cout << ptr->key << endl;
			preOrderPrintKeys(ptr->lChild);
			preOrderPrintKeys(ptr->rChild);
		}
	}

	void postOrderPrintKeys(TreeNode<K, V>* ptr)
	{
		if (ptr)
		{
			postOrderPrintKeys(ptr->lChild);
			postOrderPrintKeys(ptr->rChild);
			cout << ptr->key << endl;
		}
	}
	bool delete_(K key, TreeNode<K, V>*& ptr, K ParentKey)
	{
		if (ptr == nullptr)
		{
			return false;
		}
		else if (key < ptr->key)
		{
			return delete_(key, ptr->lChild, ptr->key);
		}
		else if (key > ptr->key)
		{
			return delete_(key, ptr->rChild, ptr->key);
		}
		else
		{
			//case 0: leaf node
			if (ptr->isLeaf())
			{
				delete ptr;
				ptr = nullptr;
				if (ptr != this->root) {
					recent(ParentKey, this->root);
				}
				return true;
			}
			//case 1.1: only left child exists
			else if (ptr->lChild && !ptr->rChild)
			{
				TreeNode<K, V>* delNode = ptr;
				ptr = ptr->lChild;
				delete delNode;
				if (ptr != this->root) {
					recent(ParentKey, this->root);
				}
				return true;
			}
			//case 1.2: only right child exists
			else if (!ptr->lChild && ptr->rChild)
			{
				TreeNode<K, V>* delNode = ptr;
				ptr = ptr->rChild;
				delete delNode;
				if (ptr != this->root) {
					recent(ParentKey, this->root);
				}
				return true;
			}
			//case 2: both children exits
			else
			{
				TreeNode<K, V>* successor = ptr->rChild;
				while (successor->lChild)
					successor = successor->lChild;

				ptr->key = successor->key;
				ptr->value = successor->value;
				delete_(successor->key, ptr->rChild, ptr->key);

				if (ptr != this->root) {
					recent(ParentKey, this->root);
				}
				return true;
			}
		}
	}//end of delete

	bool insert(K key, V value, TreeNode<K, V>*& ptr)
	{
		if (ptr == nullptr)
		{
			ptr = new TreeNode<K, V>(key, value);
			recent(key, this->root);
			return true;
		}
		else if (key < ptr->key)
		{
			return insert(key, value, ptr->lChild);
		}
		else if (key > ptr->key)
		{
			return insert(key, value, ptr->rChild);
		}
		else if (key == ptr->key)
		{
			return false;
		}
	}

	V* search(K key, TreeNode<K, V>* ptr, K LastKey)
	{
		if (ptr == nullptr && ptr == this->root)
		{
			return nullptr;
		}
		else if (ptr == nullptr && ptr != this->root)
		{
			recent(LastKey, this->root);
			return nullptr;
		}
		else if (key < ptr->key)
		{
			return this->search(key, ptr->lChild, ptr->key);
		}
		else if (key > ptr->key)
		{
			return this->search(key, ptr->rChild, ptr->key);
		}
		else {
			recent(key, this->root);
			return &ptr->value;
		}
	}

	void deleteAll(TreeNode<K, V>* ptr)
	{
		if (ptr)
		{
			deleteAll(ptr->lChild);
			deleteAll(ptr->rChild);
			delete ptr;
		}
	}

	void leftRotate(TreeNode<K, V>*& ptr)
	{
		TreeNode<K, V>* y = ptr;
		TreeNode<K, V>* x = y->rChild;
		TreeNode<K, V>* T2 = x->lChild;
		ptr = x;
		x->lChild = y;
		y->rChild = T2;
	}

	void rightRotate(TreeNode<K, V>*& ptr)
	{
		TreeNode<K, V>* y = ptr;
		TreeNode<K, V>* x = y->lChild;
		TreeNode<K, V>* T2 = x->rChild;
		ptr = x;
		x->rChild = y;
		y->lChild = T2;
	}

	void recent(K key, TreeNode<K, V>*& ptr) {
		if (ptr == nullptr || key == root->key) {
			return;
		}
		else if (ptr->lChild != nullptr && key == ptr->lChild->key) {
			rightRotate(ptr);
		}
		else if (ptr->rChild != nullptr && key == ptr->rChild->key) {
			leftRotate(ptr);
		}
		else if (key < ptr->key) {
			recent(key, ptr->lChild);
		}
		else if (key > ptr->key) {
			recent(key, ptr->rChild);
		}
		if (key != root->key) {
			recent(key, this->root);
		}
	}


public:
	SpecialBST()
	{
		this->root = nullptr;
	}

	void inorderPrintKeys()
	{
		inorderPrintKeys(this->root);
	}

	vector<V> getValuesInOrder() {
		vector<V> v;
		getValuesInOrder(this->root, v);
		return v;
	}

	void preOrderPrintKeys()
	{
		this->preOrderPrintKeys(this->root);
	}

	void postOrderPrintKeys()
	{
		this->postOrderPrintKeys(this->root);
	}

	void levelOrderPrintKeys()
	{
		if (!this->root)
			return;

		queue<TreeNode<K, V>*> q;
		q.push(this->root);

		while (!q.empty())
		{
			TreeNode<K, V>* ptr = q.front();
			q.pop();
			cout << ptr->key << endl;

			if (ptr->lChild)
				q.push(ptr->lChild);
			if (ptr->rChild)
				q.push(ptr->rChild);
		}
	}

	vector<V> getValuesLevelWise()
	{
		vector<V> v;

		if (!this->root)
			return v;

		queue<TreeNode<K, V>*> q;
		q.push(this->root);

		while (!q.empty())
		{
			TreeNode<K, V>* ptr = q.front();
			q.pop();
			v.push_back(ptr->value);

			if (ptr->lChild)
				q.push(ptr->lChild);
			if (ptr->rChild)
				q.push(ptr->rChild);
		}

		return v;
	}

	bool insert(K key, V value)
	{
		bool flag = insert(key, value, this->root);
		return flag;
	}

	bool delete_(K key)
	{
		if (this->root == nullptr) {
			return false;
		}
		else {
			bool flag = delete_(key, this->root, this->root->key);
			return flag;
		}
	}

	V* search(K key)
	{
		if (this->root == nullptr) {
			return nullptr;
		}
		else {
			return search(key, this->root, this->root->key);
		}
	}

	~SpecialBST()
	{
		this->deleteAll(this->root);
	}

};


class Student {
private:
	int rollNumber;
	string firstName;
	string lastName;
	int batch;
	string department;
	float cgpa;

public:
	Student() {
		rollNumber = 0;
		firstName = "";
		lastName = "";
		batch = 0;
		department = "";
		cgpa = 0;
	}
	/*Student(string firstName, string lastName, int batch, string department, float cgpa) {
		this->firstName = firstName;
		this->lastName = lastName;
		this->batch = batch;
		this->department = department;
		this->cgpa = cgpa;
	}*/
	void SetData(int rollNumber, string firstName, string lastName, int batch, string department, float cgpa) {
		this->rollNumber = rollNumber;
		this->firstName = firstName;
		this->lastName = lastName;
		this->batch = batch;
		this->department = department;
		this->cgpa = cgpa;
	}
	void PrintStudent() {
		cout << "Roll Number: " << this->rollNumber << endl;
		cout << "First Name: " << this->firstName << endl;
		cout << "Last Name: " << this->lastName << endl;
		cout << "Batch: " << this->batch << endl;
		cout << "Department: " << this->department << endl;
		cout << "CGPA: " << this->cgpa << endl;
	}
	int GetRollNumber() {
		return this->rollNumber;
	}
};


template <typename K, typename V>
class StudentList {
private:
	/*int rollNumber;*/
	Student studentData;
	SpecialBST<K, V> studentNode;

public:
	StudentList() : studentData(), studentNode() {
		/*rollNumber = 0;*/
	}

	bool InsertNewStudent(int rollNumber, string firstName, string lastName, int batch, string department, float cgpa) {
		//this->rollNumber = rollNumber;
		this->studentData.SetData(rollNumber, firstName, lastName, batch, department, cgpa);
		return this->studentNode.insert(rollNumber, this->studentData);
	}

	bool deleteStudent(int rollNumber) {
		return this->studentNode.delete_(rollNumber);
	}

	bool updateStudent(int oldRollNumber, int newRollNumber, string newFirstName, string newLastName, int newBatch, string newDepartment, float newCgpa) {
		if (this->studentNode.search(oldRollNumber) != nullptr) {
			if (oldRollNumber == newRollNumber) {
				Student* temp = this->studentNode.search(oldRollNumber);
				temp->SetData(oldRollNumber, newFirstName, newLastName, newBatch, newDepartment, newCgpa);
				return true;
			}
			else {
				this->studentData.SetData(newRollNumber, newFirstName, newLastName, newBatch, newDepartment, newCgpa);
				bool flag = this->studentNode.insert(newRollNumber, this->studentData);
				if (flag == true) {
					this->studentNode.delete_(oldRollNumber);
					return true;
				}
				else {
					cout << "The entered new Roll Number already exists !" << endl;
					return false;
				}
			}
		}
		else {
			cout << "The entered Old Roll Number of the student does not exist !" << endl;
			return false;
		}
	}

	void studentSearch(int rollNumber) {
		Student* temp = this->studentNode.search(rollNumber);
		if (temp == nullptr) {
			cout << "The student roll no that you searched does not exist !" << endl;
		}
		else {
			temp->PrintStudent();
		}
	}

	void printAllStudents() {
		vector<V> v;
		/*cout << endl << "Level Order: " << endl;
		v = studentNode.getValuesLevelWise();
		cout << endl << "In Order: " << endl;*/
		v = studentNode.getValuesInOrder();
		cout << endl << "Total Students: " << v.size() << endl << endl;
		for (int i = 0; i != v.size(); i++) {
			v[i].PrintStudent();
			cout << endl;
		}
	}

};


int main() {
	int rollNumber;
	string firstName;
	string lastName;
	int batch;
	string department;
	float cgpa;

	string line;
	char word[10];
	int count = 0;
	bool flag = true;
	char choice = ' ';
	bool executed = true;
	int OldRollNumber;

	StudentList< int, Student> list;

	cout << endl << ".......... WELCOME .........." << endl << endl;

	while (choice != 'E' && choice != 'e') {
		cout << "Choose from the following choices: " << endl;
		cout << "Press 'I' to insert a new student." << endl;
		cout << "Press 'D' to delete a student." << endl;
		cout << "Press 'S' to search a student by roll number." << endl;
		cout << "Press 'U' to update the data of a student." << endl;
		cout << "Press 'P' to print all students sorted by roll number." << endl;
		cout << "Press 'E' to exit." << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl;
		while (choice != 'I' && choice != 'i' && choice != 'D' && choice != 'd' && choice != 'S' && choice != 's' && choice != 'U' && choice != 'u' && choice != 'P' && choice != 'p' && choice != 'E' && choice != 'e') {
			cout << "Invalid Choice !" << endl << endl;
			cout << "Please re-enter a valid option !" << endl;
			cout << "Choose from the following choices: " << endl;
			cout << "Press 'I' to insert a new student." << endl;
			cout << "Press 'D' to delete a student." << endl;
			cout << "Press 'S' to search a student by roll number." << endl;
			cout << "Press 'U' to update the data of a student." << endl;
			cout << "Press 'P' to print all students sorted by roll number." << endl;
			cout << "Press 'E' to exit." << endl;
			cout << "Enter your choice: ";
			cin >> choice;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
		}
		flag = true;
		executed = true;
		count = 0;

		if (choice == 'I' || choice == 'i') {
			cout << "Please enter the data of new student in the order: Roll Number, First Name, Last Name, Batch, Department, CGPA: " << endl;
			getline(cin, line);
			cout << endl;
			int j = 0;
			for (int i = 0; i < line.length(); i++) {
				if (line[i] != ' ') {
					word[j] = line[i];
					j++;
				}
				else {
					word[j] = '\0';
					j = 0;
					count++;
					if (count == 1) {
						stringstream convert(word);
						convert >> rollNumber;
						//cout << "rollNumber: " << rollNumber << endl;//
						if (rollNumber < 0) {
							cout << "Roll Number should not be a negative value !" << endl;
							flag = false;
						}
					}
					else if (count == 2) {
						firstName = word;
						//cout << "firstName: " << firstName << endl;//
					}
					else if (count == 3) {
						lastName = word;
						//cout << "lastName: " << lastName << endl;//
					}
					else if (count == 4) {
						stringstream convert(word);
						convert >> batch;
						//cout << "batch: " << batch << endl;//
						if (batch < 0) {
							cout << "Batch should not be a negative value !" << endl;
							flag = false;
						}
					}
					else if (count == 5) {
						department = word;
						//cout << "department: " << department << endl;//
					}
				}
				if (i == (line.length() - 1)) {
					word[j] = '\0';
					j = 0;
					count++;
					if (count == 1) {
						stringstream convert(word);
						convert >> rollNumber;
						//cout << "rollNumber: " << rollNumber << endl;//
						if (rollNumber < 0) {
							cout << "Roll Number should not be a negative value !" << endl;
							flag = false;
						}
					}
					else if (count == 2) {
						firstName = word;
						//cout << "firstName: " << firstName << endl;//
					}
					else if (count == 3) {
						lastName = word;
						//cout << "lastName: " << lastName << endl;//
					}
					else if (count == 4) {
						stringstream convert(word);
						convert >> batch;
						//cout << "batch: " << batch << endl;//
						if (batch < 0) {
							cout << "Batch should not be a negative value !" << endl;
							flag = false;
						}
					}
					else if (count == 5) {
						department = word;
						//cout << "department: " << department << endl;//
					}
					else if (count == 6) {
						stringstream convert(word);
						convert >> cgpa;
						//cout << "cgpa: " << cgpa << endl;//
						if (cgpa < 0 || cgpa > 4) {
							cout << "CGPA should not be out of range !" << endl;
							flag = false;
						}
					}
				}
			}
			if (count != 6) {
				cout << "Incomplete Input Data !" << endl;
				flag = false;
			}
			if (flag == false) {
				cout << "Incorrect Inputs !" << endl << endl;
			}
			else {
				executed = list.InsertNewStudent(rollNumber, firstName, lastName, batch, department, cgpa);
				if (executed == false) {
					cout << "Insertion failed, The entered Roll Number already exists !" << endl << endl;
				}
				else {
					cout << "New student inserted successfully !" << endl << endl;
				}
			}
		}
		else if (choice == 'D' || choice == 'd') {
			cout << "Please enter the roll number of the student that you want to delete:" << endl;
			cin >> rollNumber;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
			if (rollNumber < 0) {
				cout << "Roll Number should not be a negative value !" << endl;
				flag = false;
			}
			if (flag == false) {
				cout << "Incorrect Input !" << endl << endl;
			}
			else {
				executed = list.deleteStudent(rollNumber);
				if (executed == false) {
					cout << "Deletion failed, The entered Roll Number does not exist !" << endl << endl;
				}
				else {
					cout << "Student deleted successfully." << endl << endl;
				}
			}
		}
		else if (choice == 'S' || choice == 's') {
			cout << "Please enter the roll number of the student that you want to search:" << endl;
			cin >> rollNumber;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
			if (rollNumber < 0) {
				cout << "Roll Number should not be a negative value !" << endl;
				flag = false;
			}
			if (flag == false) {
				cout << "Incorrect Input !" << endl << endl;
			}
			else {
				list.studentSearch(rollNumber);
				cout << endl;
			}
		}
		else if (choice == 'U' || choice == 'u') {
			cout << "Please enter the roll number of the student whose data you want to update: " << endl;
			cin >> OldRollNumber;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
			if (OldRollNumber < 0) {
				cout << "Roll Number should not be a negative value !" << endl;
				flag = false;
			}
			if (flag == false) {
				cout << "Incorrect Inputs !" << endl << endl;
			}
			if (flag == true) {
				cout << "Please enter new data of student in this order: Roll Number, First Name, Last Name, Batch, Department, CGPA: " << endl;
				getline(cin, line);
				cout << endl;
				int j = 0;
				for (int i = 0; i < line.length(); i++) {
					if (line[i] != ' ') {
						word[j] = line[i];
						j++;
					}
					else {
						word[j] = '\0';
						j = 0;
						count++;
						if (count == 1) {
							stringstream convert(word);
							convert >> rollNumber;
							//cout << "rollNumber: " << rollNumber << endl;//
							if (rollNumber < 0) {
								cout << "Roll Number should not be a negative value !" << endl;
								flag = false;
							}
						}
						else if (count == 2) {
							firstName = word;
							//cout << "firstName: " << firstName << endl;//
						}
						else if (count == 3) {
							lastName = word;
							//cout << "lastName: " << lastName << endl;//
						}
						else if (count == 4) {
							stringstream convert(word);
							convert >> batch;
							//cout << "batch: " << batch << endl;//
							if (batch < 0) {
								cout << "Batch should not be a negative value !" << endl;
								flag = false;
							}
						}
						else if (count == 5) {
							department = word;
							//cout << "department: " << department << endl;//
						}
					}
					if (i == (line.length() - 1)) {
						word[j] = '\0';
						j = 0;
						count++;
						if (count == 1) {
							stringstream convert(word);
							convert >> rollNumber;
							//cout << "rollNumber: " << rollNumber << endl;//
							if (rollNumber < 0) {
								cout << "Roll Number should not be a negative value !" << endl;
								flag = false;
							}
						}
						else if (count == 2) {
							firstName = word;
							//cout << "firstName: " << firstName << endl;//
						}
						else if (count == 3) {
							lastName = word;
							//cout << "lastName: " << lastName << endl;//
						}
						else if (count == 4) {
							stringstream convert(word);
							convert >> batch;
							//cout << "batch: " << batch << endl;//
							if (batch < 0) {
								cout << "Batch should not be a negative value !" << endl;
								flag = false;
							}
						}
						else if (count == 5) {
							department = word;
							//cout << "department: " << department << endl;//
						}
						else if (count == 6) {
							stringstream convert(word);
							convert >> cgpa;
							//cout << "cgpa: " << cgpa << endl;//
							if (cgpa < 0 || cgpa > 4) {
								cout << "CGPA should not be out of range !" << endl;
								flag = false;
							}
						}
					}
				}
				if (count != 6) {
					cout << "Incomplete Input Data !" << endl;
					flag = false;
				}
				if (flag == false) {
					cout << "Incorrect Inputs !" << endl << endl;
				}
				else {
					executed = list.updateStudent(OldRollNumber, rollNumber, firstName, lastName, batch, department, cgpa);
					if (executed == false) {
						cout << "Update failed !" << endl << endl;
					}
					else {
						cout << "Student data updated successfully !" << endl << endl;
					}
				}
			}
		}
		else if (choice == 'P' || choice == 'p') {
			list.printAllStudents();
			cout << endl;
		}
		else {
			choice = 'E';
			cout << "Program Terminated Successfully !" << endl << endl;
		}
	}
	cout << ".......... BYE .........." << endl << endl << endl;


	system("pause");
}