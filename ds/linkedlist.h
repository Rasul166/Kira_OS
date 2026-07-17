
typedef struct ListNode
{

    struct ListNode *next;
    struct ListNode *prev;
} ListNode;
typedef struct
{
    ListNode *head;
    ListNode *tail;
    unsigned int size;
} List;

void list_init(List *list);
bool list_is_empty(List *list);
void list_push_front(List *list, ListNode *node);
void list_push_back(List *list, ListNode *node);
void list_remove(List *list, ListNode *node);
ListNode *list_pop_front(List *list);
ListNode *list_pop_back(List *list);
