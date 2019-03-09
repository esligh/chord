/*
*/
#ifndef USER_DEFINE_H__
#define  USER_DEFINE_H__
#include <string>
#include<list>
#define  INVALID_PORT					(int)(~0)	//-1
#define  INVALID_MAP_VALUE		(int)(~0)	
//hashֵ
typedef struct tag_hash_value
{
	int value_; 
}hash_value;
//�ڵ��ip�ṹ
struct address
{
	std::string ip;
	int				port; //�˿�
	hash_value map;		//ip���˿ڵ�hashӳ��ֵ
	address():
	ip(""),
	port(INVALID_PORT)	
	{
		map.value_=INVALID_MAP_VALUE;
	}
};
//finger��ṹ
struct finger_entry
{
	hash_value start_ ;
	hash_value end_;
	address successor; // �ڵ���
};
//�ؼ��ֱ���ṹ
typedef std::list<address>							Addr_Lst;
typedef std::list<address>::iterator				Iterator_hLst;
typedef std::list<address>::const_iterator	Const_Iterator_hLst;
struct key_entry
{
	std::string				name;			//�ļ���	
	hash_value			key;			  //�ļ��ؼ���	
	Addr_Lst				holder_lst;	 //ӵ�йؼ��ֵĽڵ��б�
};
#endif /*DEFINE.H*/