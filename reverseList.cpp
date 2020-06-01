// reverseList.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
typedef struct Node {
	int val;
	Node* next;
};
Node* creatList(int n) {
	if (n == 0)return NULL;
	Node* p = new Node;
	auto res = p;
	for (int i = 0; i < n; i++) {
		p->val = i;
		if (i == n - 1) { p->next = NULL; break; }
		p->next = (Node*)malloc(sizeof(Node));
		p = p->next;
	}
	return res;
}
Node* reverceList(Node* p) {
	Node *p1= NULL,*p2=p,*p3=p->next;
	while (p3) {
		p2->next = p1;
		p1 = p2;
		p2 = p3;
		p3 = p2->next;
	}
	p2->next = p1;
	return p2;


}
//void reorderList(Node* head) {
//	if (head == NULL && head->next == NULL)return;
//	int n = 0;
//	Node* fa = head, * sl = head;
//	while (fa->next->next) {
//		fa = fa->next->next;
//		sl = sl->next;
//	}
//	auto head2 = sl->next;
//	sl->next = NULL;
//	head2 = reverceList(head2);
//	auto res = head,res1=head;
//	head = head->next;
//	while (head->next != NULL&&head2->next!=NULL) {
//		res->next = head2;
//		res = res->next;
//		res->next = head;
//		res = res->next;
//		head2 = head2->next;
//		head = head->next;
//	}
//	head = res1;
//}
int main()
{
	Node* p=creatList(10);
	//reorderList(p);
	while (p) {
		cout << p->val << endl;
		p = p->next;
	}
}
