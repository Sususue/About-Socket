#ifndef _GETINFOR_H_
#define _GETINFOR_H_


#include <string>
#include "cJSON.h"
#include"MyQueue.h"

typedef struct classInfor {
	std::string schoolName;/*学校名称*/
	std::string name;/*课程名称*/
	std::string duration;/*开课时间*/
	int enrollCount;/*参加人数*/
	std::string classOverview;/*课程概述*/
	std::string aim;/*授课目标*/
	std::string classOutline;/*课程大纲*/
	std::string references;/*参考资料*/
}ClassInfor;

/*获取学校名称*/
void getSchoolName(const std::string& recvStr, ClassInfor* ACourse);

/*获取课程名称*/
void getCourseName(const std::string& recvStr, ClassInfor* ACourse);

/*获取开课时间*/
void getCourseTime(const std::string& recvStr, ClassInfor* ACourse);

/*获取已参加人数*/
void getStudentNum(const std::string& recvStr, ClassInfor* ACourse);

/*获取课程概述*/
void getCourseDescribe(const std::string& recvStr, ClassInfor* ACourse);

/*获取授课目标*/
void getCourseAim(const std::string& recvStr, ClassInfor* ACourse);

/*获取课程大纲*/
void getContent(const std::string& recvStr, ClassInfor* ACourse);

/*获取参考资料*/
void getReference(const std::string& recvStr, ClassInfor* ACourse);

/*初始化课程信息结构*/
void initClassInfor(ClassInfor* ACourse);

/*将课程信息写入文件*/
void writeInfor1(ClassInfor* ACourse, char* filename);

/*将课程信息写入文件*/
void writeInfor(ClassInfor* ACourse, cJSON* root);

/*寻找网页url*/
void findurl(std::string recvStr, Myqueue* schoolUrl, Myqueue* school);

/*寻找学校id*/
std::string findid(std::string recvStr);

/*找到学校简称*/
std::string findSchoolName(std::string recvStr);


#endif

