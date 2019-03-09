#include "stdafx.h"
#include "keyTable.h"
#include "debuger.h"
#include "hash.h"

KeyTable::KeyTable()
{
}
//��������
KeyTable::~KeyTable()
{
	NodePointer p=gethead();
	while(p){
		Addr_Lst lst=p->data.holder_lst;
		lst.erase(lst.begin(),lst.end()); //�ͷ�list ��������ڴ�й¶
		p = p->next;
	}
}
int KeyTable::get_keyTableSize()const 
{
	return getlength();
}
//��ȡӵ���ļ�hash_value�ڵ��б�
int KeyTable::get_holder(const hash_value& hv,Addr_Lst& lst)const
{
	key_entry ke;
	ke.key = hv;
	int i = LocateElem(ke,(KeyTable::IsEqual_HV));
	Status ret = GetElem(i,ke);
	if(ERROR == ret)
		return ERROR;
	lst = ke.holder_lst;
	return OK;
}
//��ȡӵ���ļ����ƵĽڵ��б�
int KeyTable::get_holder(const std::string& fname,Addr_Lst& lst)const
{
	key_entry ke;
	ke.name = fname;
	int i = LocateElem(ke,(KeyTable::IsEqual_NM));
	Status ret = GetElem(i,ke);
	if(ERROR == ret)
		return ERROR;
	lst = ke.holder_lst;
	return OK;
}
//����hashֵָ���ı���
key_entry* KeyTable::find_entry(const hash_value& hv)const
{
	NodePointer p=gethead();
	while(p){
		if(Hash::isequal(hv,p->data.key)){
			return &(p->data);
		}
		p = p->next;
	}
	return NULL;
}
//�����ļ�����ָ���ı���
key_entry* KeyTable::find_entry(const std::string& name)const
{
	NodePointer p = gethead();
	while(p){
		if(name == p->data.name){
			return &(p->data);
		}
		p=p->next;
	}
	return NULL;	//not found
}
//���ӵ����
int KeyTable::add_holder(const std::string& fname,const address& addr)
{
		key_entry* pk = find_entry(fname);
		if(pk == NULL)
			return -1;
		if(!has_address(*pk,addr))
			pk->holder_lst.push_back(addr);
		return 1;
}
int KeyTable::add_holder(const hash_value& hv,const address& addr)
{
	key_entry* pk = find_entry(hv);
	if(pk == NULL)
		return -1;
	if(!has_address(*pk,addr)){		//node��ַ��ͬ
		pk->holder_lst.push_back(addr);
	}
	return 1;
}
//��ӱ���
int KeyTable::add_keyEntry(const key_entry& ke)
{
	key_entry* pke = find_entry(ke.key);	//����hashֵ������
	if(pke != NULL)	//����Ѵ��ڸñ�Ŀ����
		return 0;//-1
	Status ret = InsertList(1,ke);	
	return ret;
}	
//��ӡfinger ��
void KeyTable:: PrintKeyTable()
{
	int i=0,m=getlength();
	NodePointer p=gethead();
	Debuger::PrintDescript(std::string("key table:\n"));
	while(p){
		Debuger::PrintKeyEntry(p->data);
		p=p->next;
	}
	std::cout<<endl;
}
//////////////////////////////////////////////////////////////////////////
//�ж�hashֵ�Ƿ����
Status KeyTable::IsEqual_HV(const key_entry& ke1,const key_entry& ke2)
{
	return Hash::isequal(ke1.key,ke2.key);
}
//�ж��ļ������Ƿ����
Status KeyTable::IsEqual_NM(const key_entry& ke1,const key_entry& ke2)
{
	return ke1.name.compare(ke2.name);
}
//address �Ƿ����
bool KeyTable::is_equal_address(const address& a1,const address& a2)const 
{
	return (a1.ip == a2.ip && a1.port == a2.port && Hash::isequal(a1.map,a2.map));
}
//�Ƿ�ӵ�����б��Ѵ洢addr
bool KeyTable::has_address(const key_entry& entry,const address& addr)const
{
	for(Const_Iterator_hLst iter = entry.holder_lst.begin();iter!=entry.holder_lst.end();iter++){
		if(is_equal_address(*iter,addr)){
			return true;
		}
	}
	return false;
}