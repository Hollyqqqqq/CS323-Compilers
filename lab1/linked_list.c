#include "linked_list.h"

node *linked_list_init(){
    node *head = (node*)malloc(sizeof(node));
    head->count = 0;
    head->next = NULL;
    return head;
}

void linked_list_free(node *head){
    node *cur = head;
    node *last;
    while(cur != NULL){
        last = cur;
        cur = cur->next;
        free(last);
    }
}

char linked_list_string[0x10000];

char *linked_list_tostring(node *head){
    node *cur = head->next;
    char *position;
    int length = 0;
    while(cur != NULL){
        position = linked_list_string + length;
        length += sprintf(position, "%d", cur->value);
        cur = cur->next;
        if(cur != NULL){
            position = linked_list_string + length;
            length += sprintf(position, "->");
        }
    }
    position = linked_list_string + length;
    length += sprintf(position, "%c", '\0');
    return linked_list_string;
}

int linked_list_size(node *head){
    return head->count;
}

void linked_list_append(node *head, int val){
    node *cur = head;
    node *new_node;
    while(cur->next != NULL){
        cur = cur->next;
    }
    new_node = (node*)malloc(sizeof(node));
    new_node->value = val;
    new_node->next = NULL;
    cur->next = new_node;
    head->count++;
}

/* your implementation goes here */


void linked_list_insert(node *head, int val, int index){
    if(index < 0 || index >= head->count){
        return;
    }
    node *pre = head;
    node *new_node;
    int i = 0;
    while (i < index){
        pre = pre->next;
        i++;
    }
    new_node = (node*)malloc(sizeof(node));
    new_node->value = val;
    new_node->next = pre->next;
    pre->next = new_node;
    head->count++;
}


void linked_list_delete(node *head, int index){
    if(index < 0 || index >= head->count){
        return;
    }
    node *pre = head;
    int i = 0;
    while (i < index){
        pre = pre->next;
        i++;
    }
    node *curr = pre->next;
    pre->next = curr->next;
    curr->next = NULL;
    head->count--;
    free(curr);
}


void linked_list_remove(node *head, int val){
    node *pre = head;
    while (pre != NULL && pre->next != NULL){
        if (pre->next->value == val){
            node *curr = pre->next;
            pre->next = curr->next;
            curr->next = NULL;
            head->count--;
            free(curr);
            return;
        }
        pre = pre->next;
    }
}


void linked_list_remove_all(node *head, int val){
    node *pre = head;
    while (pre != NULL && pre->next != NULL){
        if(pre->next->value == val){
            node *curr = pre->next;
            pre->next = curr->next;
            head->count--;
            free(curr);
        }else{
            pre = pre->next;
        }
    }
}


int linked_list_get(node *head, int index){
    if(index < 0 || index >= head->count){
        return -0x80000000;
    }
    int i = 0;
    node *curr = head->next;
    while (i < index){
        curr = curr->next;
        i++;
    }
    return curr->value;
}


int linked_list_search(node *head, int val){
    if(head == NULL){
        return -1;
    }
    int index = 0;
    node *curr = head->next;
    while (curr != NULL){
        if (curr->value == val){
            return index;
        }
        curr = curr->next;
        index++;
    }
    return -1;
}


node *linked_list_search_all(node *head, int val){
    if (head == NULL){
        return NULL;
    }
    node *resultHead;
    node *result;
    resultHead = (node*)malloc(sizeof(node));
    resultHead->count = 0;
    result = resultHead;
    node *curr = head->next;
    int index = 0;
    while (curr != NULL){
        if (curr->value == val){
            node *temp = (node*)malloc(sizeof(node));
            temp->value = index;
            result->next = temp;
            temp->next = NULL;
            result = result->next;
            resultHead->count++;
        }
        curr = curr->next;
        index++;
    }
    return resultHead;
}
