#ifndef _MYQUEUE_H_
#define _MYQUEUE_H_

#include<string>

//链表结点
typedef struct myNode {
	std::string value;
	struct myNode* next;
	myNode(std::string s) {
		value = s;
		next = NULL;
	}
}myNode;

//只实现了我要用的几个
class Myqueue
{
public:
	Myqueue();
	~Myqueue();
	void popQ();
	void pushQ(std::string str);
	std::string frontQ();
	int sizeQ();
	

private:
	int len;
	myNode* head;
	myNode* last;
};

void test();

#endif