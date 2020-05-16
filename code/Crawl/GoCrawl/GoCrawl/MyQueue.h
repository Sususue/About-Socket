#ifndef _MYQUEUE_H_
#define _MYQUEUE_H_

#include<string>

//������
typedef struct myNode {
	std::string value;
	struct myNode* next;
	myNode(std::string s) {
		value = s;
		next = NULL;
	}
}myNode;

//ֻʵ������Ҫ�õļ���
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