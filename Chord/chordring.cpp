#include "stdafx.h"
#include "chordring.h"
#include "hash.h"
ChordRing::ChordRing()
{
}
//���췽�� ����һ���ڵ���뵽������
ChordRing::ChordRing(Node& node)
{
	int length = getlength();
	node.setActive(true);
	InsertList(length+1,node,NULL); //����ýڵ㵽chord����	
}
//���ҽڵ��Ƿ��Ѿ���chord����
bool ChordRing::has_node(const Node& node)
{
	NodePointer head  = Gethead();
	if(head == NULL)
		return NULL;
	hash_value hfind = node.get_hash_value();	
	NodePointer p=head;
	while(head!=p->next){
		if(Hash::isequal(p->data.get_hash_value(),hfind))
			return true;
		p=p->next;	
	}
	if(Hash::isequal(p->data.get_hash_value(),hfind))
		return true;
	return false;
}
/*����ڵ�*/
/*
	(1)��ʼ�����ؽڵ�finger��
	(2)���������ڵ�finger��
	(3)ת�ƹؼ�����Ϣ
*/
bool ChordRing::add_node(const Node& node)
{
	if(has_node(node))
		return false;		//�Ѿ�����chord����				
	int len = getlength();
	NodePointer pp=NULL;
	InsertList(len+1,node,&pp);	//����ڵ㵽chord���в�����ָ��ýڵ��ָ��pp
	if(pp == NULL)
		return false;
	update_local_finger(pp->data); 
	update_others_finger(node,Node::ADD_NODE);
	//���ú�̽ڵ��ǰ��
	Node* successor = find_successor(node.get_hash_value());		//�ҵ����
	if(successor == NULL)
		return false;
	address predecessor = successor->get_predecessor();	//�ҵ���̵�ԭ��ǰ��
	if(Hash::between(node.get_hash_value(),successor->get_hash_value(),predecessor.map)){
		successor->set_predecessor(node.get_localnode());
	}
	Node* pPrecessor =find_predeccessor(node);	//�ҵ�����ڵ��ǰ��
	if(pPrecessor == NULL)
		return false;
	//���õ�ǰ�ڵ��ǰ��	
	(pp->data).set_predecessor(pPrecessor->get_localnode());
	(pp->data).setActive(true);
	//ת�ƹؼ�����Ϣ
	update_keyTable(pp->data);
	return true;
}
//���±��ؽڵ��finger��Ŀ
/*
	ע:�÷���ʹ�ò���Ϊ�������ͣ�����Ϊ��Ҫ�ı�ýڵ��ڲ���finger��Ŀ
*/
bool ChordRing::update_local_finger(Node& node)	
{
	int size = node.get_finger_table().get_finger_size();
	Node* succ_node=NULL;
	address addr ;
	hash_value hv = node.get_finger_table().get_finger_entry(1)->start_;
	succ_node=find_closest_successor(hv);	
	if(succ_node != NULL){
		addr = succ_node->get_localnode();
		node.set_successor(addr);	//����node���
	}
	for(int i=2;i<size;i++){
			hash_value hv = node.get_finger_table().get_finger_entry(i)->start_;
			succ_node = find_closest_successor(hv);
			addr = succ_node->get_localnode();
			node.set_fingerEntrySuccessor(i,addr);
	}
	return true;
}
//֪ͨ���е������ڵ�node��������뿪
/*
����˵��:
node : ��������뿪�ڵ�
typde:���ͱ��
*/
bool ChordRing::update_others_finger(const Node& node,Node::NODE_OPER_TYPE type)
{
	//�����нڵ����
	NodePointer head = Gethead();
	NodePointer p = head;
	while(head!=p->next){		
		(p->data).update_finger_table(node,type);
		p = p->next;
	}
	(p->data).update_finger_table(node,type);
	return true;
}
/*
//���ҽڵ�ĺ��
Node* ChordRing::find_successor(const Node& node)const
{
	if(getlength()<=0)
		return NULL;
	Node* assist = Gethead()->data;	//Э���ڵ� �ڵ�δ����ǰ Э���ڵ�����ǻ��е��κνڵ�
	hash_value cur_hv = node.get_hash_value();//��ǰ�ڵ��hash_valueֵ
}
*/
/*
//�������ҽڵ���
const Node* ChordRing::aux_find(const hash_value& hv,const Node& node,int size)
{
	if(Hash::issmall(hv,node.get_hash_value())){
		return NULL;
	}
	for(int i=1;i<size;i++){
		if()
	}
}
//����hash_valudeΪhv�Ľڵ�
const Node* ChordRing::find(const hash_value& hv)
{
	NodePointer head = Gethead();
	if(head == NULL)
		return NULL;
	NodePointer p  = head;
	while(head != p->next){
		if(Hash::isequal(hv,(p->data).get_hash_value())){
			return &(p->data);
		}
		p=p->next;
	}
	if(Hash::isequal(hv,(p->data).get_hash_value())){
		return &(p->data);
	}
	return NULL;	//δ���ҵ�
}
*/
//��chord���ϲ���˳ʱ�����cur_hv����Ľڵ�(���ҽ������Ϊ��ǰ���ҽڵ㱾��)
Node* ChordRing::find_closest_successor(const hash_value& cur_hv)const
{
	//O(N) Ч�ʵ� �Ľ��㷨���Բο�key�Ĳ��Ҽ�get_closest_finger

	/*��ͷ��㿪ʼ���� ����������ָ��һ���ڵ㣬�൱�������绷����
	���ѡ��һ���ڵ����������ѯ���̻�ǰ�̽ڵ�
	*/
	NodePointer head = Gethead(); 
	if(head == NULL)
		return NULL;
	NodePointer p  = head;
	NodePointer p2f = NULL;			//ָ����ҵ��ĺ�̽ڵ�
	hash_value min = Hash::get_max();//hash_value�������ֵ
	hash_value d,roop;
	while(head != p->next){
		roop=(p->data).get_hash_value();
		Hash::distance(cur_hv,roop,&d);		//���㵱ǰ�ڵ��뻷�нڵ��˳ʱ�����
		if(Hash::issmall(d,min)){
			min = d;
			p2f = p;
		}
		p=p->next;
	}
	roop = (p->data).get_hash_value();
	Hash::distance(cur_hv,roop,&d);
	if(Hash::issmall(d,min)){
		min=d;
		p2f =p;
	}
	if(Hash::isequal(min,Hash::get_max())){ //û���ҵ�
		return NULL;
	}
	return &(p2f->data);
}
//����hash_valueָ���ڵ�ĺ��(�������Ϊ��ǰ���ҽڵ�)
/*return : 
�ɹ� ���ؽڵ�ĺ�� 
ʧ�ܷ��� NULL*/
Node* ChordRing::find_successor(const hash_value& cur_hv)const
{//ͬ��
	NodePointer head = Gethead(); //��ͷ��㿪ʼ����
	if(head == NULL)
		return NULL;
	NodePointer p  = head;
	NodePointer p2f = NULL;			//ָ����ҵ��ĺ�̽ڵ�
	hash_value min = Hash::get_max();//hash_value�������ֵ
	hash_value d,roop;
	while(head != p->next){
		roop=(p->data).get_hash_value();
		Hash::distance(cur_hv,roop,&d);		//���㵱ǰ�ڵ��뻷�нڵ��˳ʱ�����
		if(Hash::get_hash(d)!=0&&Hash::issmall(d,min)){
			min = d;
			p2f = p;
		}
		p=p->next;
	}
	roop = (p->data).get_hash_value();
	Hash::distance(cur_hv,roop,&d);
	if(Hash::get_hash(d)!=0&&Hash::issmall(d,min)){
		min=d;
		p2f =p;
	}
	if(Hash::isequal(min,Hash::get_max())){ //û���ҵ�
		return NULL;
	}
	return &(p2f->data);
}
//����ĳ�ڵ��ǰ��
 Node* ChordRing::find_predeccessor(const Node& node)const
{//ͬ��
	NodePointer head = Gethead();
	if(head == NULL)
		return NULL;
	hash_value cur_hv = node.get_hash_value();
	NodePointer p = head;
	NodePointer p2f = NULL;
	hash_value max = Hash::get_min();
	hash_value d,roop;
	while(head != p->next){
		roop=(p->data).get_hash_value();
		Hash::distance(cur_hv,roop,&d);		//���㵱ǰ�ڵ��뻷�нڵ��˳ʱ�����
		if(Hash::islarge(d,max)){
			max = d;
			p2f = p;
		}
		p=p->next;
	}
	roop = (p->data).get_hash_value();
	Hash::distance(cur_hv,roop,&d);
	if(Hash::islarge(d,max)){
		max=d;
		p2f =p;
	}
	if(Hash::isequal(max,Hash::get_min())){ //û���ҵ�
		return NULL;
	}
	return &(p2f->data);
}
 /*����keytable*/
 /*
	˵��:�ڵ�node����󣬼�������a����ؼ��֣���Ϊ������node�ڵ�Ĺؼ���
	��Ȼ���������̽ڵ�
	node: �¼���ڵ�(����)
 */
 bool ChordRing::update_keyTable(Node& node)
 {	 
	 hash_value cur_hv = node.get_hash_value();
	 Node* successor = find_successor(cur_hv); //���Һ�̽ڵ�
	 if(successor == NULL)
		 return false;
	 return transfer_key(successor,&node); //�Ӻ��ת�ƹؼ��ֱ������ڵ�
 }
 /*
 �������ܣ�������������keyentry��ӱ��ؽڵ�ת�Ƶ��¼���ڵ�
 successor: ��̽ڵ�
 new_node:�¼���ڵ�
 */
 bool ChordRing::transfer_key(Node* successor,Node* new_node)
 {
	// Node* new_node = find(node->get_hash_value());	//�ڻ����ҵ�����ڵ�
	 hash_value new_hv = new_node->get_hash_value();//��ȡ����ڵ��hash_valueֵ
	 KeyTable::NodePointer head =successor->get_key_table().gethead();	//��ȡ���keyTableָ��
	 if(head== NULL) 
		 return false;
	 KeyTable::NodePointer p = head;
	 KeyTable::NodePointer q= head;
	 const address new_pre_addr=new_node->get_predecessor(); //��ȡ�¼���ڵ��ǰ��
	 hash_value new_pre_hv = new_pre_addr.map; //��ȥǰ�̽ڵ��hashֵ
	 while(p){		 
		 hash_value key = p->data.key;		
		 if(Hash::between(key,new_pre_hv,new_hv)){ //�������� ��successor��ɾ��KeyEntry��������䵽new_node��keyTable
			 new_node->store_key(p->data); //���keyEntry���½ڵ�
			 ///////////////////////////////////////////////////////////
			 q=p;		//ָ���ɾ���ڵ�
			 p=p->next;	//��һ�����ýڵ�
			 successor->DelKeyEntry(q); //�Ӻ����ɾ������
			 continue;			 
			 //////////////////////////////
		 }
		 p=p->next;		
	 }
	 return true;
 }
 //��ַΪaddr�Ľڵ㹲���ļ�filename
 bool ChordRing::share_file(const address& addr,const std::string& filename)
 {
	 hash_value key;
	 Node::hash_key(filename,&key);	//��ȡ�ļ�hashֵ
	 Node* node = find_closest_successor(key); //���Ҽ�ֵ��<K,V>����
	 if(node == NULL){
		 return false;
	 }
	 //����keyTable
	 node->add_file(filename,addr);
	return true;
 }
 //ת��keyTable
 /*
	��node�Ĺؼ��ֱ�ת�Ƶ�pNode
 */
 bool ChordRing::move_keyTable(const Node& node,Node* pNode)
 {
	 if(pNode == NULL)
		 return false;
	 KeyTable kt = node.get_key_table();	 
	 int size = kt.get_keyTableSize();
	 key_entry ke;
	 for(int i=1;i<=size;i++){ 
		 kt.GetElem(i,ke);	//��ȡ����
		 pNode->store_key(ke);//�洢����
	 } 
	 return true;
 }
 //////////////////////////////////////////////////////////////////////////
 //ɾ��һ���ڵ�
 /*
 1.�ӻ���ɾ��һ���ڵ�
 2.���������ڵ��finger��
 3.ת�ƹؼ��ֱ�
 */
 bool ChordRing::del_node(address& addr)
 {
	 if(!Hash::isvalid(addr.map))
		 Node::hash_node(addr);
	 NodePointer p = find(addr.map);//�ҵ��ýڵ�
	 update_others_finger(p->data,Node::DEL_NODE);
	 Node* successor = find_successor(addr.map);	//�ҵ����̽ڵ�
	 if(successor == NULL)
		 return false;
	move_keyTable(p->data,successor);
	 DeleteList(p);
	 return true;
 }
 bool ChordRing::aux_find_file(const Node&node,const std::string& filename,address& afind)
 {
	 return true;
 }
  //����hashֵΪhv���ļ�
 /*
 ����˵��:
	node:�����ѯ�Ľڵ�
	hv:��ѯ�ļ���hashֵ
	lst:���صĵ�ַ�б�
 */
 bool ChordRing::find_file(const Node& node,const hash_value& hv,Addr_Lst& lst)
 {
	  NodePointer pNode = find(node.get_hash_value()); //�ҵ���ѯ�ڵ�
	 if(pNode == NULL)
		 return false;
	 //����keyTable����
	 bool ret = (pNode->data).find_file(hv,lst);
	 if(ret){ 
		 return ret; //���ҳɹ�����
	 }
	 address afind;
	 bool result = false;
	 result =aux_find_file(node,hv,afind); //���ز���ʧ�ܣ��ڻ��ϲ���
	 if(result){ //���ҳɹ�
		 pNode = find(afind.map); //�ҵ��洢<K,V>�ڵ�
		 if(pNode == NULL)
			 return false;
		 return (pNode->data).find_file(hv,lst);//��ȡ�洢��ַ�б�
	 }
	 return result;
 }
 /*��ѯ��������*/
 /*
 ��������:������ѯ�洢keyֵΪhv�Ľڵ�
 ����˵��:
 node ����ѯ�ڵ�
 hv:	keyֵ
 afind:�洢<K,V>�ԵĽڵ�
 return :  true -���ҳɹ�  false-����ʧ��
 */
 bool ChordRing::aux_find_file(const Node& node,const hash_value& hv,address& afind)
 {
	 assert(Hash::isvalid(hv));
	 NodePointer pNode = find(node.get_hash_value()); //�ҵ���ѯ�ڵ�
	 if(pNode == NULL)
		 return false;
	Node* p = &(pNode->data);
	 while(!(p->get_closest_finger(hv,afind))){
		 pNode =find(afind.map);
		 if(pNode == NULL)
			 return false;
		 p =&(pNode->data);
	 }
	 return true;
 }

 
 //////////////////////////////////////////////////////////////////////////
 /*�ڻ��в���hashֵΪhv�Ľڵ� ʧ�ܷ���NULL*/
 /*ע������ʵ�����绷����ͨ��socketͨ�����*/
 ChordRing::NodePointer ChordRing::find(const hash_value& hv)
 {
	 NodePointer head = Gethead();
	 if(head ==NULL)
		 return NULL;
	 NodePointer p = head;
	 while(head!=p->next){		
		 if(Hash::isequal(hv,(p->data).get_hash_value())){
			 return p;
		 }
		 p = p->next;
	 }
	 if(Hash::isequal(hv,(p->data).get_hash_value())){
		 return p;
	 }
	 return NULL;
 }
 //��ӡfinger��
void ChordRing::PrintNodeFinger()
{
	NodePointer head = Gethead();
	if(head ==NULL)
		return ;
	NodePointer p = head;
	while(head!=p->next){		
		(p->data).PrintFingerTable();
		p = p->next;
	}
	(p->data).PrintFingerTable();
}
//��ӡKeyTable
void ChordRing::PrintKeyTable()
{
	NodePointer head = Gethead();
	if(head ==NULL)
		return ;
	NodePointer p = head;
	while(head!=p->next){		
		(p->data).PrintKeyTable();
		p = p->next;
	}
	(p->data).PrintKeyTable();
}
