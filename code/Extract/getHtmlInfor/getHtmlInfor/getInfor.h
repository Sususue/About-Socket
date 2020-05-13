#ifndef _GETINFOR_H_
#define _GETINFOR_H_


#include <string>

/*获取学校名称*/
void getSchoolName(const std::string& recvStr);

/*获取课程名称*/
void getCourseName(const std::string& recvStr);

/*获取开课时间*/
void getCourseTime(const std::string& recvStr);

/*获取已参加人数*/
void getStudentNum(const std::string& recvStr);

/*获取课程概述*/
void getCourseDescribe(const std::string& recvStr);

/*获取授课目标*/
void getCourseAim(const std::string& recvStr);

/*获取课程大纲*/
void getContent(const std::string& recvStr);

/*获取参考资料*/
void getReference(const std::string& recvStr);
#endif

