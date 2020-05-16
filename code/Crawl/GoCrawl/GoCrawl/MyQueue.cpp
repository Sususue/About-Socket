#include"MyQueue.h"
#include<iostream>

using namespace std;
//自己实现队列
Myqueue::Myqueue()
{
	len = 0;
	head = new myNode("");//创建头结点
	last = head;

}

Myqueue::~Myqueue()
{
	myNode* temp;
	while (head != NULL)
	{
		temp = head->next;
		delete head;
		head = temp;
	}
}
void Myqueue::pushQ(std::string str) {
	
	last->next = new myNode(str);
	last = last->next;
	if (len == 0) {//注意一定要加这一句，否则pop为零后，head与后面的链表将会断掉
		head->next = last;
	}
	len++;
	
}

std::string Myqueue::frontQ() {
	if (len != 0) {
		return head->next->value;
	}
	else {
		return "";
	}

}
void Myqueue::popQ() {
	myNode* temp = head->next;
	head->next = temp->next;
	delete temp;
	len--;
}
int Myqueue::sizeQ() {
	return len;
}
void test() {
	Myqueue queue;
	int n;
	cout << "输入n:" ;
	cin >> n;
	cout << endl;
	for (int i = 0; i < n; i++) {
		string temp;
		cout << "temp:";
		cin >> temp;
		cout  << endl;
		queue.pushQ(temp);
	}
	//you have to judge if queue is empty before using this font node
	while (queue.sizeQ() !=0) {
		cout << queue.frontQ() << endl;
		queue.popQ();
	}
		cout << queue.sizeQ() << endl;
	return;
}