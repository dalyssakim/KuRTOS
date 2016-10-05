#ifndef _LIST_H
#define _LIST_H



struct list_head
{
	struct list_head *prev, *next;
};

extern void INIT_LIST_HEAD(struct list_head *list);
extern void __list_add(struct list_head *new_node, 
			struct list_head *prev,
			struct list_head *next);

extern void list_add(struct list_head *new_node, struct list_head *head);

extern void list_add_tail(struct list_head *new_node, struct list_head *head);

extern void __list_del(struct list_head *prev, struct list_head *next);
/*
static void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = 
	entry->prev = 
}
*/

#define list_for_each(pos, head) \
	for(pos = (head)->next; pos != (head); pos = pos->next)

#endif