#ifndef _GETINFOR_H_
#define _GETINFOR_H_


#include <string>
#include "cJSON.h"
#include"MyQueue.h"

typedef struct classInfor {
	std::string schoolName;/*ѧУ����*/
	std::string name;/*�γ�����*/
	std::string duration;/*����ʱ��*/
	int enrollCount;/*�μ�����*/
	std::string classOverview;/*�γ̸���*/
	std::string aim;/*�ڿ�Ŀ��*/
	std::string classOutline;/*�γ̴��*/
	std::string references;/*�ο�����*/
}ClassInfor;

/*��ȡѧУ����*/
void getSchoolName(const std::string& recvStr, ClassInfor* ACourse);

/*��ȡ�γ�����*/
void getCourseName(const std::string& recvStr, ClassInfor* ACourse);

/*��ȡ����ʱ��*/
void getCourseTime(const std::string& recvStr, ClassInfor* ACourse);

/*��ȡ�Ѳμ�����*/
void getStudentNum(const std::string& recvStr, ClassInfor* ACourse);

/*��ȡ�γ̸���*/
void getCourseDescribe(const std::string& recvStr, ClassInfor* ACourse);

/*��ȡ�ڿ�Ŀ��*/
void getCourseAim(const std::string& recvStr, ClassInfor* ACourse);

/*��ȡ�γ̴��*/
void getContent(const std::string& recvStr, ClassInfor* ACourse);

/*��ȡ�ο�����*/
void getReference(const std::string& recvStr, ClassInfor* ACourse);

/*��ʼ���γ���Ϣ�ṹ*/
void initClassInfor(ClassInfor* ACourse);

/*���γ���Ϣд���ļ�*/
void writeInfor1(ClassInfor* ACourse, char* filename);

/*���γ���Ϣд���ļ�*/
void writeInfor(ClassInfor* ACourse, cJSON* root);

/*Ѱ����ҳurl*/
void findurl(std::string recvStr, Myqueue* schoolUrl, Myqueue* school);

/*Ѱ��ѧУid*/
std::string findid(std::string recvStr);

/*�ҵ�ѧУ���*/
std::string findSchoolName(std::string recvStr);


#endif

