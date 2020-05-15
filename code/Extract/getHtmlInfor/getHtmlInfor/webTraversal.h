#ifndef _WEBTRAVERSAL_H_
#define _WEBTRAVERSAL_H_

#include <string>
#include <queue>//暂时先用STL的队列实现

/*单独写一个获取具体课程url的函数采用广度遍历，好吧其实真实情况可能就一层*/
void GetCourseURL(std::string url);

/*从文件中获取课程URL*/
void getURLFromFile(const std::string& recvStr, std::queue<std::string>& courseUrl);

/*获得课程信息*/
void getClassInfor(std::queue<std::string>& courseUrl, char* filename);

/*连接课程网页*/
void connnctCourse(std::string url, char* filename);

#endif