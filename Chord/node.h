#ifndef NODE_H_
#define NODE_H_
#include "fingerTable.h"
#include "keyTable.h"
#include "commonEnv.h"
/*�ڵ���node*/
class Node
{
public:
	enum NODE_OPER_TYPE{ ADD_NODE,DEL_NODE};
	Node();
	~Node();
	Node(address& addr);
	Node(const Node& node);
	Node& operator=(const Node& rhs);
	void set_predecessor(const address& ad);
	const address& get_predecessor()const; 
	bool isActive()const;
	void setActive(bool b);
	bool store_key(const key_entry& ke);
	const FingerTable& get_finger_table()const;	
	int get_m()const{return CommonEnv::get_m();}
	hash_value get_hash_value()const;	
	int update_finger_table(const Node& node,NODE_OPER_TYPE type);	
	void set_successor(const address& ad);
	const address& get_successor()const;
	const address& get_localnode()const;
	const KeyTable& get_key_table()const;
	void set_fingerEntrySuccessor(int index,const address& addr);
	bool add_file(const std::string& name,const address& addr);
	bool DelKeyEntry(KeyTable::NodePointer s);
	bool find_file(const hash_value& hv,Addr_Lst& lst);
	bool get_closest_finger(const hash_value& key_hv,address& paddr);
	//////////////////////////////////////////////////////////
	static int hash_node(address& addr);
	static int hash_key(const std::string& name,hash_value* hv);

private:	
	bool copy_finger(const FingerTable& other);
public:
	//debug
	void PrintKeyTable();
	void PrintFingerTable();
private:
	address			local_node;		//��ǰ�ڵ�
	address			predecessor;		//��ǰ�ڵ��ǰ�̽ڵ�
	FingerTable	finger_tbl;			//finger�� (������ǰ�ڵ�)
	KeyTable		key_tbl;			//�ؼ��ֱ�
	bool				bActive;			//�Ƿ񼤻�
};
#endif