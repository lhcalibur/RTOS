#ifndef _OS_LIST_H
#define _OS_LIST_H

#include <include/types.h>

#define LIST_ADD(entry) list_add(entry.node)
#define LIST_ADD_TAIL(entry) list_add_tail(entry.node)

template <typename TT>
class List;

template <typename T>
class Node
{
	private:
		friend class List<T>;
		T _entry;
		Node *next;
		Node *prev;
		inline T nodeentry() {return _entry;} 
	public:
		Node(T entry): _entry(entry), next(this), prev(this) {}
};

template <typename TT>
class List
{
	private:
		Node<TT> node = Node<TT>(*this);
		inline void __list_add(Node<TT> *entry, Node<TT> *prev, Node<TT> *next) {
			next->prev = entry;
			entry->next = next;
			entry->prev = prev;
			prev->next = entry;
		}
		inline void __list_del(Node<TT> *prev, Node<TT> *next) {
			next->prev = prev;
			prev->next = next;
		}



	public:
		List() {}
		static inline void INIT_LIST_HEAD(Node<TT> &entry) {entry.next = entry.prev = &entry;}
		inline void list_add(Node<TT> &entry) {__list_add(&entry, &node, &node.next);}
		inline void list_add_tail(Node<TT> &entry) {__list_add(&entry, &node.prev, &node);}
		inline void list_del(Node<TT> &entry) {__list_del(&entry.prev, &entry.next);}
		inline void list_del_init(Node<TT> &entry) {
			__list_del(&entry.prev, &entry.next);
			INIT_LIST_HEAD(&entry);
		}
		inline void list_move_tail(Node<TT> &entry) {
			__list_del(&entry.prev, &entry.next);
			list_add_tail(entry);
		}
		inline bool list_empty() {return node.next = &node;}

		inline TT list_entry(Node<TT> *node) {return node->nodeentry();} 
		inline TT list_first_entry() {list_entry(node.next);} 
		inline TT list_last_entry() {list_entry(node.prev);}
#define list_for_each_entry(pos)	\
		for (pos = list_entry(node.next);	\
				&pos != this;		\
				pos = list_entry(pos.next))

};


#endif /* _OS_LIST_H */
