/*
	debuger��:���ڵ���
*/
#ifndef DEBUGER_H_
#define DEBUGER_H_
#include "user_define.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
class Debuger{
public:
	static void PrintAddr(const address& addr);
	static void PrintKeyEntry(const key_entry& ke);
	static void PrintFingerEntry(const finger_entry& fe);
	static void PrintHashValue(const hash_value& hv);
	static void PrintDescript(const std::string& des);
	static void PrintNewLine();
	static void formatOutput(int width,char c=' ');
private:		
	static void PrintStr(const std::string& msg);
	static void PrintInt(int v);
	static void PrintuInt(unsigned int v);	
public:
	//��ʽ����������
	const static int STR_TEMP_LEN =30;	//��ʱ�ַ����峤��
	const static int DEFAULT_WIDTH =8; //Ĭ�Ͽ�� �൱'\t'
	const static int OUT_NAME_LEN =15; //���Ƹ�ʽ���
	const static int OUT_HASH_LEN=10; //hashֵ��ʽ���
	const static int OUT_IP_LEN =18; //IP��ʽ���
	const static int OUT_PORT_LEN = 10;//�˿ڸ�ʽ���
	const static int OUT_EXTENT	=8;//��չ��ʽ���
};
#endif