#define _CRT_SECURE_NO_WARNINGS
#include "getInfor.h"
#include <regex>
#include <iostream>

using namespace std;

/*��ȡѧУ����*/
void getSchoolName(const std::string& recvStr) {
	int index = recvStr.find("<title>");
	//cout << recvStr[index] << endl;
	string str = "\0";
	if (index != -1) {
		index = recvStr.find("_", index);
		index += strlen("_");
		int index1 = recvStr.find("_", index);
		//<title>����һ�е�����_֮��ΪѧУ����
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./ѧУ����.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}

/*��ȡ�γ�����*/
void getCourseName(const string& recvStr) {
	/*auto curIter = recvStr.begin();
	auto endIter = recvStr.end();
	smatch res;*/
	//�ҵ��γ����Ƶ�λ�õ���һ��
	/*bool s = regex_search(curIter, endIter, res, regex("<span class=\"course-title f-ib f-vam\">.*?</span>"));
	cout << s << endl;
	cout << res[0] << endl;
	string str = res[0];*/
	int index = recvStr.find("<span class=\"course-title f-ib f-vam\">");
	//cout << recvStr[index] << endl;//�鿴һ�µ��׶Բ���
	string str = "\0";
	if (index != -1) {
		index = recvStr.find(">", index);
		index += strlen(">");
		int index1 = recvStr.find("</", index);
		//<>......</>��ǩ֮���������
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./�γ�����.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}

/*��ȡ����ʱ��*/
void getCourseTime(const string& recvStr) {
	//�ҵ����λ��
	int index = recvStr.find("window.termInfoList");
	string str = "\0";
	if (index != -1) {
		index += strlen("window.termInfoList");
		index = recvStr.find("text : \"", index);
		index += strlen("text : \"");
		int index1 = recvStr.find("\"", index);
		//text : "......"˫�����е������ǿ���ʱ�䣬ע��ո񣡣�
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./����ʱ��.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}
/*��ȡ�Ѳμ�����*/
void getStudentNum(const string& recvStr) {
	int index = recvStr.find("enrollCount : \"");
	string str = "\0";
	if (index != -1) {
		index += strlen("enrollCount : \"");
		int index1 = recvStr.find("\"", index);
		//enrollCount : "......"�е��������Ѳμ�����
		str = recvStr.substr(index, index1 - index);
	}

	FILE* fp = NULL;
	fp = fopen("./�Ѳμ�����.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}
/*��ȡ�γ̸���*/
void getCourseDescribe(const string& recvStr) {
	//�ҵ��γ̸����Ĵ��λ��
	int index = recvStr.find("<span class=\"category-title_icon f-ib f-vam u-icon-categories\"></span>");
	//int index = recvStr.find("�ڿ�Ŀ��");
	string str = "\0";
	if (index != -1) {
		index = recvStr.find("<div", index);
		cout << "index=" << index << endl;
		int index1 = recvStr.find("</div>", index);
		str = recvStr.substr(index, index1 - index);

		//��֪��Ϊɶ�������ѧ�����м�ʼ����һ��ƥ�䲻�ϣ�����������ʽ������������ƥ��
		//����һ��ƥ��˳������ֿ�����
		str = regex_replace(str, regex("[ \f\r\t\n]"), "");//ȥ���հ׷�
		str = regex_replace(str, regex("</p>"), "\n");//����
		str = regex_replace(str, regex("<.*?>"), "");//ȥ����ǩ
		str = regex_replace(str, regex("&nbsp;"), " ");//���ɿո�

	}

	FILE* fp = NULL;
	fp = fopen("./�γ̸���.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}

}

/*��ȡ�ڿ�Ŀ��*/
void getCourseAim(const string& recvStr) {
	char sTemp[10] = "\0";
	FILE* fp1 = NULL;
	fp1 = fopen("./classAim.txt", "rb");
	if (fp1) {
		fgets(sTemp, 10, fp1);//���UTF-8�ĺ���
		sTemp[strlen(sTemp) - 1] = '\0';//ȥ�����з�
		fclose(fp1);
		fp1 = NULL;
	}
	else {
		cout << "fail wo open the file" << endl;
	}
	//�ҵ��γ̸����Ĵ��λ��
	int index = recvStr.find(sTemp);//
	//int index = recvStr.find("�ڿ�Ŀ��");//���벻ͬ���޷��ҵ�
	string str = "\0";
	if (index != -1) {
		index = recvStr.find("<div", index);
		cout << "index=" << index << endl;
		int index1 = recvStr.find("</div>", index);
		str = recvStr.substr(index, index1 - index);

		str = regex_replace(str, regex("[ \f\r\t\n]"), "");//ȥ���հ׷�
		str = regex_replace(str, regex("</p>"), "\n");//����
		str = regex_replace(str, regex("<.*?>"), "");//ȥ����ǩ
		str = regex_replace(str, regex("&nbsp;"), " ");//���ɿո�
	}

	FILE* fp = NULL;
	fp = fopen("./�ڿ�Ŀ��.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}

/*��ȡ�γ̴��*/
void getContent(const string& recvStr) {
	//Ϊɶ��ͬ�γ̵Ĵ��д����ʽ����һ������
	/*  �е�д��outLineStructureDtos:���棻�е�д��outLineDto:����
		���˼·���ȿ�outLineStructureDtos:���Ƿ�����Ч���ݣ�
		���У�������ȡ�����Ϣ�����ޣ���ȡoutLineDto:����Ĵ����Ϣ
	*/
	int index = recvStr.find("outLineStructureDtos:");
	string str = "\0";

	if (index != -1) {
		int index1 = recvStr.find("};", index);
		str = recvStr.substr(index + strlen("outLineStructureDtos:"), index1 - (index + strlen("outLineStructureDtos:")));
		int temp = str.find("\"");//ͨ����"���жϵ�����û����Ч����
		if (temp != -1) {//outLineStructureDtos:����������
			str = regex_replace(str, regex("[ \f\r\t\n]"), "");
			str = regex_replace(str, regex("[A-Z]*[a-z]*:"), "");
			//str = regex_replace(str, regex("[A-Z]"), "");
			str = regex_replace(str, regex("[\[]"), "");
			str = regex_replace(str, regex("\\]"), "");//��˫б��ת�壬���򱨴�
			str = regex_replace(str, regex("[{]"), "");
			str = regex_replace(str, regex("[}]"), "");
			str = regex_replace(str, regex(";;;"), "\n");//����
			str = regex_replace(str, regex("[:;\"]"), "");
			str = regex_replace(str, regex(","), "\n");//����
		}
		else {//��ȡoutLineDto:����
			int  index0 = recvStr.find("outLineDto:");
			index0 = recvStr.find("outLine: \"");
			index0 += strlen("outLine: \"");
			str = recvStr.substr(index0, index - index0);
			str = regex_replace(str, regex("[ \f\r\t\n]"), "");//ȥ���հ׷�
			str = regex_replace(str, regex("</p>"), "\n");//����

			/*Ҫƥ�䷴б��\\��Ҫ�ĸ�\\,ǰ�����ڱ��������ת��һ��\,�������ڱ��������׼��һ��\
			��������ʽ�У�ǰһ��\ת���һ��\
			��������߲��Կ���ƥ����������Ǵ�������ʼ�ղ��ɹ���ԭ��*/
			str = regex_replace(str, regex("<\\\\/p>"), "\n");//����

			str = regex_replace(str, regex("<br/>"), "\n");//����
			str = regex_replace(str, regex("<.*?>"), "");//ȥ����ǩ
			str = regex_replace(str, regex("&nbsp;"), " ");//���ɿո�
			str = regex_replace(str, regex("[:;\",]"), "");

		}

	}

	FILE* fp = NULL;
	fp = fopen("./�γ̴��.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}


/*��ȡ�ο�����*/
void getReference(const string& recvStr) {
	//�ҵ��ο����ϵĴ��λ��
	int index = recvStr.find("<span class=\"category-title_icon f-ib f-vam f-18 u-icon-stacks\">");
	//int index = recvStr.find("�ڿ�Ŀ��");
	string str = "\0";
	if (index != -1) {//��Щ�γ̾�û�вο������������
		index = recvStr.find("<div", index);
		cout << "index=" << index << endl;
		int index1 = recvStr.find("</div>", index);
		str = recvStr.substr(index, index1 - index);

		str = regex_replace(str, regex("[ \f\r\t\n]"), "");//ȥ���հ׷�
		str = regex_replace(str, regex("</p>"), "\n");//����
		str = regex_replace(str, regex("<.*?>"), "");//ȥ����ǩ
		str = regex_replace(str, regex("&nbsp;"), " ");//���ɿո�

	}

	FILE* fp = NULL;
	fp = fopen("./�ο�����.txt", "wb");
	if (fp) {
		fwrite(str.c_str(), sizeof(char), str.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail to open the file" << endl;
	}
}