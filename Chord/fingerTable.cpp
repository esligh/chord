#include "stdafx.h"
#include "fingerTable.h"
#include "commonEnv.h"
#include <assert.h>
#include "hash.h"
//���췽��
FingerTable::FingerTable()
{	
	int size = get_finger_size();
	pbase = new finger_entry[size]; //����finger��
	Init_finger(); //��ʼ��
}
FingerTable::~FingerTable()
{
	if(pbase != NULL){
		delete []pbase;
		pbase = NULL;
	}
}
//��ȡfinger���С
inline int FingerTable::get_finger_size()const
{
	return CommonEnv::get_m()+1; //�������ؽڵ�
}
//��ʼ��finger�� 
void FingerTable::Init_finger()
{
	int size = get_finger_size();
	for(int i = 0;i<size;i++){
		init_finger_entry(pbase[i]);
	}
}
//��ʼ��finger����
/*(ָ�����Ǵ�����ĳ�ʼֵ�����ǽڵ�ĳ�ʼֵ)*/
void  FingerTable::init_finger_entry(finger_entry& fe)
{
	fe.end_.value_ = fe.start_.value_= INVALID_MAP_VALUE;
	fe.successor.ip =std::string("");
	fe.successor.port = INVALID_PORT;
	fe.successor.map.value_ = INVALID_MAP_VALUE;
}
/*��������Ϊi��finger����*/
void FingerTable::set_finger_entry(int i,finger_entry& fe)
{
	assert(pbase != NULL);
	assert(i>=0 && i<get_finger_size());	
	pbase[i].start_ = fe.start_;
	pbase[i].end_ = fe.end_;
	pbase[i].successor = fe.successor;
}
/*����hashֵ����finger���� δ��*/
int FingerTable::set_finger_entry(hash_value& va,finger_entry& fe)
{
	assert(pbase != NULL);
	int size = get_finger_size();
	for(int i=0;i<size;i++){
		if(Hash::isequal(va,fe.start_)){
			pbase[i].end_ = fe.end_;
			pbase[i].successor = fe.successor;
			return 1; //���óɹ�
		}
	}
	return -1; //û�ҵ����� ����δ�ɹ�
}
//����finger����ĳһ����ĺ�̽ڵ�
void FingerTable::set_entry_successor(const hash_value& start,const address& ad)
{
	int size = get_finger_size();
	for(int i =0;i<size;i++){
		if(Hash::isequal(start,pbase[i].start_)){
			pbase[i].successor = ad;
			break;
		}
	}
}
//��ȡ����Ϊi�ı���
const finger_entry* FingerTable::get_finger_entry(int i)const
{
	assert(i>=0 && i<get_finger_size());
	return &pbase[i];
}
//��ȡhashֵָ���ı���
const finger_entry* FingerTable::get_finger_entry(const hash_value& start)const
{
	assert(pbase != NULL);
	int size = get_finger_size();
	for(int i = 0;i<size;i++){
		if(Hash::isequal(start,pbase[i].start_)){
			return (&pbase[i]);
		}
	}
	return NULL; 
}
//��ȡ�ڵ�ĺ����
const finger_entry* FingerTable::get_successor()const
{
	assert(pbase != NULL);
	return (&pbase[1]);
}
//���ú�̱���
void FingerTable::set_successor(const address& ad)
{
	assert(pbase != NULL);
	pbase[1].successor = ad;
}
//����indexָ���ı���
void FingerTable::set_entry_successor(int index,const address& ad)
{
	assert(pbase !=NULL);
	assert(index>=0 || index<get_finger_size());
	pbase[index].successor = ad;
}
//��ȡfinger���ַ
const finger_entry* FingerTable::get_finger()const
{
	return pbase;
}
/*
	����hash_valueֵ�� hv����Ľڵ�
	����ĸ���:
		��Ⱥ��߻�����chord����Ϊhv�ĺ�̣���key�ĺ��
*/
/*
const finger_entry* FingerTable::get_closest_finger_entry(hash_value& hv)const
{
	assert(pbase != NULL);
	int size = get_finger_size();
	if(Hash::between(hv,))
	for(int i=1;i<size;i++){
		if(Hash::isequal(hv,pbase[i].start_) || Hash::issmall(hv,pbase[i].start_)){ 
			return (&pbase[i]); //����hvС�ڵ���finger����hash_value�ĵ�һ������
		}
	}
}
*/
//��ӡ�ؼ��ֱ�
void FingerTable:: PrintFingerTable()const
{
	int size = get_finger_size();
	Debuger::PrintDescript(std::string("finger table:\n"));
	for(int i =0;i<size;i++){
		Debuger::PrintFingerEntry(pbase[i]);
	}
}
FingerTable::FingerTable(const FingerTable& other)
{
	if(this != &other){
		int size = get_finger_size();
		pbase = new finger_entry[size];
		const finger_entry* pother = other.get_finger();
		assert(pother !=NULL);
		for(int i =0;i<size ;i++){
			pbase[i].start_ = pother[i].start_;
			pbase[i].end_ = pother[i].end_;
			pbase[i].successor = pother[i].successor;
		}
	}
}
/*
description:
isneed_change_successor	:	FingerTable class
the successor of finger entry located by index wethere need to update or not.
param
index	: 	finger entry index
new_hv	:	the hash_value needs to update	
node:
the method start going clockwise in chord ring,and determin to update the successor
or not accrroding to the distance of which current node compare with old successor
and new.
*/
bool FingerTable::isneed_change_successor(int index,const hash_value& hv)const
{
	assert(pbase !=NULL);
	assert(index>0 && index<get_finger_size());
	//abstract the hash_value
	hash_value start=pbase[index].start_;
	hash_value old_hv=pbase[index].successor.map;
	hash_value new_hv=hv;
	if(Hash::islarge(start,old_hv)){
		old_hv.value_ +=(1<<Hash::get_hashbits());
	}
	if(Hash::islarge(start,new_hv)){
		new_hv.value_+=(1<<Hash::get_hashbits());
	}
	hash_value disOfold,disOfnew;	
	disOfold.value_=abs(old_hv.value_-start.value_);
	disOfnew.value_=abs(new_hv.value_-start.value_);
	if(Hash::issmall(disOfnew,disOfold) || Hash::isequal(disOfnew,disOfold))
		return true;
	else
		return false;
}

