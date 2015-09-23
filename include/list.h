#ifndef _OS_LIST_H
#define _OS_LIST_H

#include <include/types.h>

#define LIST_ADD(list, entry) list.list_add(entry.node)
#define LIST_ADD_TAIL(list, entry) list.list_add_tail(entry.node)
#define LIST_DEL(list, entry) list.list_del(entry.node)
#define LIST_DEL_INIT(list, entry) list.list_del_init(entry.node) 
#define LIST_FIRST_ENTRY(list) list.list_first_entry()
#define LIST_LAST_ENTRY(list) list.list_last_entry()
#define IS_LIST_FIRST_ENTRY(list, entry) list.is_list_first_entry(entry.node)
#define LIST_EMPTY(list) list.list_empty()
#define LIST_FOR_EACH_ENTRY(list, pos)	\
		for (pos = &list.list_first_entry();	\
				pos != (typeof pos)list.getthis();		\
				pos = &list.list_entry(pos->node.next))
#define LIST_ADD_BEFORE(list, entry, obentry) list.list_add_before(entry.node, obentry.node)



template <typename T>
class Node
{
	protected:
	public:
		T _entry;
		Node *next;
		Node *prev;
		Node(T entry): _entry(entry), next(this), prev(this) {}
		inline T nodeentry() {return (T)_entry;} 
};

template <typename Type>
class List: private Node<Type>
{
	private:
		inline void __list_add(Node<Type> *entry, Node<Type> *prev, Node<Type> *next) {
			next->prev = entry;
			entry->next = next;
			entry->prev = prev;
			prev->next = entry;
		}
		inline void __list_del(Node<Type> *prev, Node<Type> *next) {
			next->prev = prev;
			prev->next = next;
		}

		Node<Type> &node() {return (Node<Type> &)*this;}


	public:
		List(): Node<Type>((Type)*this) {}
		List *getthis() {return this;}
		static inline void INIT_LIST_HEAD(Node<Type> &entry) {entry.next = entry.prev = &entry;}
		inline void list_add(Node<Type> &entry) {__list_add(&entry, &node(), node().next);}
		inline void list_add_tail(Node<Type> &entry) {__list_add(&entry, node().prev, &node());}
		inline void list_del(Node<Type> &entry) {__list_del(entry.prev, entry.next);}
		inline void list_del_init(Node<Type> &entry) {
			__list_del(entry.prev, entry.next);
			INIT_LIST_HEAD(entry);
		}
		inline void list_move_tail(Node<Type> &entry) {
			__list_del(entry.prev, entry.next);
			list_add_tail(entry);
		}
		inline bool list_empty() {return node().next == &node();}

		inline Type list_entry(Node<Type> *enode) {return enode->nodeentry();} 
		inline Type list_first_entry() {return list_entry(node().next);} 
		inline Type list_last_entry() {return list_entry(node().prev);}
		inline bool is_list_first_entry(Node<Type> &enode) {return &enode == node().next;}
		inline void list_add_before(Node<Type> &entry, Node<Type> &obentry) {__list_add(&entry, obentry.prev, &obentry);}
};
#endif /* _OS_LIST_H */
