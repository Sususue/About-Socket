#define _CRT_SECURE_NO_WARNINGS
#include "getInfor.h"
#include <regex>
#include <iostream>

using namespace std;

/*获取学校名称*/
void getSchoolName(const std::string& recvStr) {
	int index = recvStr.find("<title>");
	//cout << recvStr[index] << endl;
	string str = "\0";
	if (index != -1) {
		index = recvStr.find("_", index);
		index += strlen("_");
		int index1 = recvStr.find("_", index);
		//<title>后面一行的两个_之间为学校名称
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./学校名称.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}

/*获取课程名称*/
void getCourseName(const string& recvStr) {
	/*auto curIter = recvStr.begin();
	auto endIter = recvStr.end();
	smatch res;*/
	//找到课程名称的位置的那一段
	/*bool s = regex_search(curIter, endIter, res, regex("<span class=\"course-title f-ib f-vam\">.*?</span>"));
	cout << s << endl;
	cout << res[0] << endl;
	string str = res[0];*/
	int index = recvStr.find("<span class=\"course-title f-ib f-vam\">");
	//cout << recvStr[index] << endl;//查看一下到底对不对
	string str = "\0";
	if (index != -1) {
		index = recvStr.find(">", index);
		index += strlen(">");
		int index1 = recvStr.find("</", index);
		//<>......</>标签之间的是名称
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./课程名字.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}

/*获取开课时间*/
void getCourseTime(const string& recvStr) {
	//找到大概位置
	int index = recvStr.find("window.termInfoList");
	string str = "\0";
	if (index != -1) {
		index += strlen("window.termInfoList");
		index = recvStr.find("text : \"", index);
		index += strlen("text : \"");
		int index1 = recvStr.find("\"", index);
		//text : "......"双引号中的内容是开课时间，注意空格！！
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./开课时间.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}
/*获取已参加人数*/
void getStudentNum(const string& recvStr) {
	int index = recvStr.find("enrollCount : \"");
	string str = "\0";
	if (index != -1) {
		index += strlen("enrollCount : \"");
		int index1 = recvStr.find("\"", index);
		//enrollCount : "......"中的内容是已参加人数
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./已参加人数.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}
/*获取课程概述*/
void getCourseDescribe(const string& recvStr) {
	//找到课程概述的大概位置
	int index = recvStr.find("<span class=\"category-title_icon f-ib f-vam u-icon-categories\"></span>");
	//int index = recvStr.find("授课目标");
	string str = "\0";
	if (index != -1) {
		index = recvStr.find("<div", index);
		cout << "index=" << index << endl;
		int index1 = recvStr.find("</div>", index);
		str = recvStr.substr(index, index1 - index);

		//不知道为啥，这个大学体育中间始终有一段匹配不上，在线正则表达式测试明明可以匹配
		//换了一下匹配顺序好像又可以了
		str = regex_replace(str, regex("[ \f\r\t\n]"), "");//去掉空白符
		str = regex_replace(str, regex("</p>"), "\n");//换行
		str = regex_replace(str, regex("<.*?>"), "");//去掉标签
		str = regex_replace(str, regex("&nbsp;"), " ");//换成空格

	}

	FILE* fp = NULL;
	fp = fopen("./课程概述.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}

/*获取授课目标*/
void getCourseAim(const string& recvStr) {
	char sTemp[10] = "\0";
	FILE* fp1 = NULL;
	fp1 = fopen("./classAim.txt", "rb");
	if (fp1) {
		fgets(sTemp, 10, fp1);//获得UTF-8的汉字
		sTemp[strlen(sTemp) - 1] = '\0';//去除换行符
		fclose(fp1);
		fp1 = NULL;
	}
	else {
		cout << "fail wo open the file" << endl;
	}
	//找到课程概述的大概位置
	int index = recvStr.find(sTemp);//
	//int index = recvStr.find("授课目标");//编码不同，无法找到
	string str = "\0";
	if (index != -1) {
		index = recvStr.find("<div", index);
		cout << "index=" << index << endl;
		int index1 = recvStr.find("</div>", index);
		str = recvStr.substr(index, index1 - index);

		str = regex_replace(str, regex("[ \f\r\t\n]"), "");//去掉空白符
		str = regex_replace(str, regex("</p>"), "\n");//换行
		str = regex_replace(str, regex("<.*?>"), "");//去掉标签
		str = regex_replace(str, regex("&nbsp;"), " ");//换成空格
	}

	FILE* fp = NULL;
	fp = fopen("./授课目标.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}

/*获取课程大纲*/
void getContent(const string& recvStr) {
	//为啥不同课程的大纲写法格式还不一样……
	/*  有的写在outLineStructureDtos:后面；有的写在outLineDto:后面
		大概思路：先看outLineStructureDtos:后是否有有效内容，
		若有，从中提取大纲信息；若无，提取outLineDto:后面的大纲信息
	*/
	int index = recvStr.find("outLineStructureDtos:");
	string str = "\0";

	if (index != -1) {
		int index1 = recvStr.find("};", index);
		str = recvStr.substr(index + strlen("outLineStructureDtos:"), index1 - (index + strlen("outLineStructureDtos:")));
		int temp = str.find("\"");//通过找"来判断到底有没有有效内容
		if (temp != -1) {//outLineStructureDtos:后面有内容
			str = regex_replace(str, regex("[ \f\r\t\n]"), "");
			str = regex_replace(str, regex("[A-Z]*[a-z]*:"), "");
			//str = regex_replace(str, regex("[A-Z]"), "");
			str = regex_replace(str, regex("[\[]"), "");
			str = regex_replace(str, regex("\\]"), "");//加双斜杠转义，否则报错
			str = regex_replace(str, regex("[{]"), "");
			str = regex_replace(str, regex("[}]"), "");
			str = regex_replace(str, regex(";;;"), "\n");//换行
			str = regex_replace(str, regex("[:;\"]"), "");
			str = regex_replace(str, regex(","), "\n");//换行
		}
		else {//提取outLineDto:后面
			int  index0 = recvStr.find("outLineDto:");
			index0 = recvStr.find("outLine: \"");
			index0 += strlen("outLine: \"");
			str = recvStr.substr(index0, index - index0);
			str = regex_replace(str, regex("[ \f\r\t\n]"), "");//去掉空白符
			str = regex_replace(str, regex("</p>"), "\n");//换行

			/*要匹配反斜杠\\需要四个\\,前两个在编程语言中转成一个\,后两个在编程语言中准成一个\
			在正则表达式中，前一个\转义后一个\
			这就是在线测试可以匹配出来，但是代码里面始终不成功的原因。*/
			str = regex_replace(str, regex("<\\\\/p>"), "\n");//换行

			str = regex_replace(str, regex("<br/>"), "\n");//换行
			str = regex_replace(str, regex("<.*?>"), "");//去掉标签
			str = regex_replace(str, regex("&nbsp;"), " ");//换成空格
			str = regex_replace(str, regex("[:;\",]"), "");

		}

	}

	FILE* fp = NULL;
	fp = fopen("./课程大纲.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}


/*获取参考资料*/
void getReference(const string& recvStr) {
	//找到参考资料的大概位置
	int index = recvStr.find("<span class=\"category-title_icon f-ib f-vam f-18 u-icon-stacks\">");
	//int index = recvStr.find("授课目标");
	string str = "\0";
	if (index != -1) {//有些课程就没有参考资料这个东西
		index = recvStr.find("<div", index);
		cout << "index=" << index << endl;
		int index1 = recvStr.find("</div>", index);
		str = recvStr.substr(index, index1 - index);

		str = regex_replace(str, regex("[ \f\r\t\n]"), "");//去掉空白符
		str = regex_replace(str, regex("</p>"), "\n");//换行
		str = regex_replace(str, regex("<.*?>"), "");//去掉标签
		str = regex_replace(str, regex("&nbsp;"), " ");//换成空格

	}

	FILE* fp = NULL;
	fp = fopen("./参考资料.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}