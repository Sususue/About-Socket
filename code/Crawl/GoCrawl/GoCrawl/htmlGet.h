#ifndef _HTMLGET_H_
#define _HTMLGET_H_

#define _WINSOCK_DEPRECATED_NO_WARNINGS//消除gethostbyname和inet_addr报错
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>


#pragma comment(lib, "ws2_32.lib")

//using std::string;

/*解析URL

url:需要解析的url
host:得到解析后的host
resource:得到除host的后半部分

返回:true成功解析/false解析失败
*/
bool parseURL(const std::string& url, std::string& host, std::string& resource);

/*创建套接字并建立连接

host:域名
port:端口号
transport:tcp/udp

返回:创建好的套接字
*/
SOCKET connectSock(const char* host, const int port, const char* transport);

/*发送请求*/
bool sendRequest(SOCKET sock, const std::string& host, const std::string& resource);

/*发起请求*/
std::string recvRequest(SOCKET sock, const std::string& url);

/*写入文件

recvAtr:从网页接收到的字符

*/
void downLoad(const std::string& recvStr, const std::string& filename);

/*发送POST请求*/
bool sendRequestPOST(SOCKET sock, const std::string& host, const std::string& resource, const std::string& schoolname, const int page, const std::string schoolId);

/*连接课程网页*/
std::string connectHtml(std::string url);

#endif 
