#include "list.h"


void INIT_LIST_HEAD(struct list_head *list);
void __list_add(struct list_head *new_node, 
			struct list_head *prev,
			struct list_head *next);

void list_add(struct list_head *new_node, struct list_head *head);
void list_add_tail(struct list_head *new_node, struct list_head *head);
void __list_del(struct list_head *prev, struct list_head *next);

  void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

  void __list_add(struct list_head *new_node, 
			struct list_head *prev,
			struct list_head *next)
{
	next->prev = new_node;
	new_node->next = next;
	new_node->prev = prev;
	prev->next = new_node;
}

  void list_add(struct list_head *new_node, struct list_head *head)
{
	__list_add(new_node, head, head->next);
}

  void list_add_tail(struct list_head *new_node, struct list_head *head)
{
	__list_add(new_node, head->prev, head);
}

  void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}
/*
  void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = 
	entry->prev = 
}
*/

