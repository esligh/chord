#ifndef HASH_H__
#define HASH_H__
#include "user_define.h"
#include <string>
//hash��
/*
	��hash���������в���,��chord��ʹ�õ���sha1�㷨����ɢ�� 
	ɢ��ֵΪ160bit��λ��
*/
class Hash
{
public:
	static void set_hashbits(int m);
	static const hash_value& get_max();
	static const hash_value& get_min();
	static int get_hashbits();
	static int _hash(const std::string& r,hash_value* va); //hash����
	static bool isequal(const hash_value& v1,const hash_value& v2);
	static bool islarge(const hash_value& v1,const hash_value& v2);
	static bool issmall(const hash_value& v1,const hash_value& v2);
	static bool between(const hash_value& v,const hash_value& v1,const hash_value& v2);
	static void distance(const hash_value& v1,const hash_value& v2,hash_value* pr);
	static int abs(int n){return n<0? -n:n;}
	static int get_hash(const hash_value& hv){return hv.value_;}
	static int isvalid(const hash_value& hv){return hv.value_>=0 && hv.value_<get_max().value_;}
private:
	static int hash_bits;//ɢ��ֵ��λ��
	static hash_value max;
	static hash_value min;
};
#endif