#include<stdlib.h>
#include<kira_ds.h>
void list_init(List *list)
{
    list->head=NULL;
    list->tail=NULL;
    list->size=0;
}
bool list_is_empty(List *list){
    return list->head==NULL;
}
void list_push_front(List *list,ListNode *node){
    node->prev=NULL;
    node->next=list->head;
    if(list->head)
        list->head->prev=node;
    else
       list->tail=node;
    list->head=node;
    
    list->size++;
}
void list_push_back(List *list,ListNode *node){
    node->next=NULL;
    node->prev=list->tail;
    if(list->tail)
     list->tail->next=node;
    else 
     list->head=node;
    list->tail=node;
    list->size++;
}
void list_remove(List *list,ListNode *node)
{
    if(node->prev)
      node->prev->next=node->next;
    else
      list->head=node->next;
    if(node->next)
      node->next->prev=node->prev;
    else
      list->tail=node->prev;
    node->next=NULL;
    node->prev=NULL;
    list->size--;
}
ListNode *list_pop_front(List *list)
{
    if(list->head==NULL)
     return NULL;
    ListNode *node=list->head;
    list_remove(list,node);
    return node;
}
ListNode *list_pop_back(List *list)
{
    if(list->tail==NULL)
     return NULL;
     ListNode *node =list->tail;
     list_remove(list,node);
     return node;
}