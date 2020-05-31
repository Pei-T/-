#pragma once
#include <vector>
#include <cstdlib>
using namespace std;
vector<int> creatVector(int n) {
	vector<int>datas;
	for (int i = 0; i < n; i++) {
		datas.push_back(rand()%200-100);
	}
	return datas;
}
struct Node {
	int val;
	Node* next=NULL;
};
Node* creatList(int n) {
	if (n == 0)return NULL;
	Node* p = new Node;
	Node* q = p;
	for (int i = 0; i < n; i++) {
		p->val = rand() % 200 - 100;
		if (i == n - 1) { break; }
		p->next = new Node;
		p = p->next;
	}
	return q;
}

