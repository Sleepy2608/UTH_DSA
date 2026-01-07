#include<bits/stdc++.h>

using namespace std;

typedef node* list;

struct node {
	int value;
	node *next;
}

void initList(list &l){
	l = null;
}

node *makeNode(int x){
	node *p = new node;
	p->next = null;
	p->value = x;
	return p;
}

void insFirst(list &l, int x){
	node *p = makeNode(x);
	p->next = l;
	l = p;
}



