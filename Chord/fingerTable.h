/*finger Table�ඨ��
*/
#ifndef FINGER_TABLE_H_
#define FINGER_TABLE_H_
#include "stdafx.h"
#include "LinkList.hpp"
#include "user_define.h"
#include "debuger.h"
class FingerTable
{
public:
	FingerTable();
	FingerTable(const FingerTable& other);
	~FingerTable();
	void Init_finger();//��ʼ��finger��
	int get_finger_size()const;
	void set_finger_entry(int i,finger_entry& fe); //������������finger����
	int set_finger_entry(hash_value& va,finger_entry& fe);//����hash_value���ñ��� ������״̬	
	const finger_entry* get_finger()const;	//��ȡfinger��
	const finger_entry* get_finger_entry(int i)const; //����������ȡfinger����
	const finger_entry* get_finger_entry(const hash_value& start)const; //����hash_valueֵѰ��finger����	
	const finger_entry* get_successor()const;
	void set_successor(const address& ad);
	void set_entry_successor(const hash_value& start,const address& ad); //����ĳһ������Ľڵ���
	void set_entry_successor(int index,const address& ad);
	//const finger_entry* get_closest_finger_entry(hash_value& hv)const;  //��ȡ����hash_value����ı���		
	void PrintFingerTable()const;
	bool isneed_change_successor(int index,const hash_value& hv)const;
private:
	void init_finger_entry(finger_entry& fe);
private:
	finger_entry* pbase; //finger���ַ
};
#endif

