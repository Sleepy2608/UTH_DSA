#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
using namespace std;
struct NODE
{
	int data;
	NODE *pLeft;
	NODE *pRight;
};
typedef NODE* TREE;
NODE* create_node(int x)
{
	NODE* p = new NODE;
	p->data = x;
	p->pLeft = NULL;
	p->pRight = NULL;
	return p;
}
void create_tree(TREE &t)
{
	t = NULL;
}
int insert_tree(TREE &t, NODE* p)
{
	if (t == NULL)
		t = p;
	else 
	{
		if (t->data == p->data)
			return 0;
		if (p->data < t->data)
			insert_tree(t->pLeft, p);
		else
			insert_tree(t->pRight, p);
	}
	return 1;
}
void input(TREE &t)
{
	int x;
	cout << "x= "; cin >> x;
	while (x != 0)
	{
		NODE *p = create_node(x);
		insert_tree(t, p);
		cout << "x= "; cin >> x;
	}
}
void NLR(TREE t)
{
	if (t != NULL)
	{
		cout << t->data << "  ";
		NLR(t->pLeft);
		NLR(t->pRight);
	}
}
void LNR(TREE t)
{
	if (t != NULL)
	{
		LNR(t->pLeft);
		cout << t->data << " ";
		LNR(t->pRight);
	}
}
void LRN(TREE t)
{
	if (t != NULL)
	{
		LRN(t->pLeft);
		LRN(t->pRight);
		cout << t->data << " ";
	}
}
void count_odd(TREE t, int &count )
{
	if (t != NULL)
	{
		if ((t->data) % 2 == 1)
			count++;
		count_odd(t->pLeft, count);
		count_odd(t->pRight, count);
	}
}
NODE* search_node(TREE t, int x)
{
	if (t == NULL)
		return NULL;
	if (x == t->data)
		return t;
	if (x > t->data)
		return search_node(t->pRight, x);
	if (x < t->data)
		return search_node(t->pLeft, x);
}
int search_max(TREE t)// con phai phai nhat
{
	if (t == NULL)
		return 0;
	else if (t->pRight != NULL)
		search_max(t->pRight);
	else
		return t->data;
}
int search_min(TREE t)// con trai trai nhat
{
	if (t->pLeft != NULL)
		search_min(t->pLeft);
	else
		return t->data;
}
void read_file(fstream &f, TREE &t)
{
	f.open("C://SONGUYEN.txt", ios::in);
	while (!f.eof())
	{
		int x;
		f >> x;
		NODE*p = create_node(x);
		insert_tree(t, p);
		char c;
		f >> c;
	}
	f.close();
}
bool is_prime(int n)
{
	if (n < 2)
		return false;
	for (int i = 2; i < n - 1;i++)
	if (n%i == 0)
		return false;
	return true;
}
void count_prime(TREE t, int &count)
{
	if (t != NULL)
	{
		if (is_prime(t->data) == true)
			count++;
		count_prime(t->pLeft, count);
		count_prime(t->pRight, count);
	}
}
void output_leaf(TREE t)
{
	if (t != NULL)
	{
		if (t->pLeft == NULL&& t->pRight == NULL)
			cout << t->data << " ";
		output_leaf(t->pLeft);
		output_leaf(t->pRight);
	}
}
void output_one_child(TREE t)
{
	if (t != NULL)
	{
		if ((t->pLeft == NULL &&t->pRight != NULL) || (t->pLeft != NULL&&t->pRight == NULL))
			cout << t->data << " ";
		output_one_child(t->pLeft);
		output_one_child(t->pRight);
	}
}
void output_two_child(TREE t)
{
	if (t != NULL)
	{
		if ((t->pLeft != NULL &&t->pRight != NULL) || (t->pLeft != NULL&&t->pRight != NULL))
			cout << t->data << " ";
		output_two_child(t->pLeft);
		output_two_child(t->pRight);
	}
}
int height_tree(TREE t)
{
	if (t == NULL)
		return 0;
	int left_height = height_tree(t->pLeft);
	int right_height = height_tree(t->pRight);
	if (left_height > right_height)
		return left_height + 1;
	return right_height + 1;
}
int delete_node(TREE &t, int key) 
{
	if (t == NULL) 
		return 0;
	else if (t->data > key) 
		return delete_node(t->pLeft, key);
	else if (t->data < key)
		return delete_node(t->pRight, key);
	else // t->data == key
	{
		if (t->pLeft == NULL) 
			t = t->pRight;    // Node chi co cay con phai
		else if (t->pRight == NULL) 
			t = t->pLeft;   // Node chi co cay con trai
		else // Node co ca 2 con
		{
			NODE *Q = t->pLeft; //Tim nut the mang (phai nhat con trai nut can xoa
			while (Q->pRight != NULL)
			{
				Q = Q->pRight;
			}
			t->data = Q->data;
			delete_node(t->pLeft, Q->data);
		}
	}
	return 1;
}
void delete_all(TREE &t)
{
	if (t != NULL)
	{
		delete_all(t->pLeft);
		delete_all(t->pRight);
		delete_node(t, t->data);
	}
}
void menu()
{
	TREE t;
	create_tree(t);
	fstream f;
	int option, x;
	do
	{
		system("cls");
		cout << "0. Exit  " << "1.Input  " << "2. Output  " <<"3.Search  "<<"4. Max  "<<"5.Min  "<<"6.Read_file"<< endl;
		cout << "7.Leaf  " << "8.One child  " << "9.Two child  " << "10.Height tree  " << "11.Delete node" << " 12.Delete all" << endl;
		cout << "option: "; cin >> option;
		if (option == 0)
			break;
		if (option == 1)
		{
			input(t);
		}
		if (option == 2)
		{
			LNR(t);
			if (t == NULL)
				cout << "Danh sach rong";
			cout << endl;
			system("pause");
		}
		if (option == 3)
		{
			cout << "Nhap khoa can tim: ";
			cin >> x;
			NODE*p = search_node(t, x);
			if (p == NULL)
				cout << "No" << endl;
			else
				cout << p->data << endl;
			cout << endl;
			system("pause");
		}
		if (option == 4)
		{
			cout << search_max(t);
			if (t == NULL)
				cout << "Danh sach rong";
			cout << endl;
			system("pause");
		}
		if (option == 5)
		{
			cout << search_min(t);
			if (t == NULL)
				cout << "Danh sach rong";
			cout << endl;
			system("pause");
		}
		if (option == 6)
		{
			read_file(f,t);
			system("pause");
		}
		if (option == 7)
		{
			output_leaf(t);
			if (t == NULL)
				cout << "Danh sach rong";
			cout << endl;
			system("pause");
		}
		if (option == 8)
		{
			output_one_child(t);
			if (t == NULL)
				cout << "Danh sach rong";
			cout << endl;
			system("pause");
		}
		if (option == 9)
		{
			output_two_child(t);
			if (t == NULL)
				cout << "Danh sach rong";
			cout << endl;
			system("pause");
		}
		if (option == 10)
		{
			cout<<height_tree(t);
			if (t == NULL)
				cout << "Danh sach rong";
			cout << endl;
			system("pause");
		}
		if (option == 11)
		{
			int x;
			cout << "Enter the key want to delete: ";
			cin >> x;
			if (delete_node(t, x) == 1)
				cout << "Xoa thanh cong\n";
			else
				cout << "Xoa khong thanh cong\n";
			cout << endl;
			system("pause");
		}
		if (option == 12)
		{
			delete_all(t);
			if (t==NULL)
				cout << "Xoa thanh cong\n";
			else
				cout << "Xoa khong thanh cong\n";
			cout << endl;
			system("pause");
		}
	} while (option > 0 && option <= 12);
}
int main()
{
	menu();
	return 0;
}