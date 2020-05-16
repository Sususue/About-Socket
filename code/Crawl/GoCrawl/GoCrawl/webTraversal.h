#ifndef _WEBTRAVERSAL_H_
#define _WEBTRAVERSAL_H_

#include <string>
#include"MyQueue.h"//用链表实现的队列，暂时只能装string
#include "cJSON.h"

/*单独写一个获取具体课程url的函数采用广度遍历，好吧其实真实情况可能就一层*/
void GetCourseURL(std::string url, std::string school, std::string schoolid);

/*从文件中获取课程URL*/
void getURLFromFile(const std::string& recvStr, Myqueue* courseUrl);

/*获得课程信息*/
void getClassInfor(Myqueue* courseUrl, cJSON* root, std::string school);

/*连接课程网页*/
void connnctCourse(std::string url, cJSON* root);

#endif