/*������ ChordRing chord ��*/
/************************************************************************/
/*   ע:����ʵ�����绷���У�ÿ���ڵ㲢����Ҫά��һ��chord����
	ChordRing��ֻ������ģ����ʵ�����绷���������ԡ�����ʵ�����绷��
	�����еĽڵ�����γ�chord�������˽ṹ��
*/
/************************************************************************/
#ifndef CHORD_RING_H
#define  CHORD_RING_H
#include "node.h"
#include "circulLinklist.hpp"
class ChordRing :public CirculLinkList<Node>
{
public:
	ChordRing();
	ChordRing(Node& node);
	Node* get_chordring();
	bool add_node(const Node& node);
	Node* find_node(const address& addr)const;
	Node* find_node(const hash_value& hv);
	bool del_node(address& addr);
	bool has_node(const Node& node);
	bool update_others_finger(const Node& node,Node::NODE_OPER_TYPE type);
	bool update_local_finger(Node& node);
	Node* find_predeccessor(const Node& node)const;	//����ĳһ�ڵ��ǰ�̽ڵ�
	Node* find_closest_successor(const hash_value& cur_hv)const;
	Node* find_successor(const hash_value& cur_hv)const;
	bool update_keyTable(Node& node);
	bool share_file(const address& addr,const std::string& filename);
	bool aux_find_file(const Node& node,const std::string& filename,address& afind);
	bool aux_find_file(const Node& node,const hash_value& hv,address& afind);
	bool find_file(const Node& node,const hash_value& hv,Addr_Lst& lst);
public:
	void PrintNodeFinger();
	void PrintKeyTable();
private:
	const address* aux_find(const hash_value& hv,const Node& node);
	NodePointer find(const hash_value& hv); //����hash_valueֵΪhv�Ļ��Ͻڵ� 
	bool move_keyTable(const Node&,Node*);
	bool transfer_key(Node* ,Node* );
};
#endif