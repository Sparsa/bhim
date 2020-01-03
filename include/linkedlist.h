//
// Created by sparsa on 10/12/18.
//

#ifndef MULTISTACKTIMEDAUTOMATA_LINKEDLIST_H
#define MULTISTACKTIMEDAUTOMATA_LINKEDLIST_H
template <class T>
struct node{
    T number;
    node * next;
} ;
template <class T>
class linkedlist {
    node<T> *head;
    node<T> *tail;
    int length;
public:
    linkedlist() {
        head = tail = nullptr;
        length = 0;
    }

    linkedlist(T element) {
        head = new node<T>;
        head->number = element;
        head->next = nullptr;
        tail = head;
        length = 1;
    }

    linkedlist(const linkedlist<T> &l) {

        length = 0; // before any adding thine we know that we have 0 elements in the list
        node<T> *t = l.getTail();
        if (!l.empty()) {
            for (node<T> *n = l.getHead(); n != t->next; n = n->next) {
                //head = new node;
                node<T> *newnode = new node<T>;
                if (length == 0) {
                    head = newnode;
                }
                newnode->number = n->number;
                newnode->next = n->next;
                tail = newnode;
                newnode = newnode->next;

                length++;
            }
        }
    }

    node<T> *getHead() const {
        return head;
    }

    node<T> *getTail() const {
        return tail;
    }

    int getLength() const {
        return length;
    }

    void insert(T number) {
        if (head == nullptr) {
            tail = new node<T>;
            tail->number = number;
            tail->next = nullptr;
            head = tail;
        } else {
            tail->next = new node<T>;
            tail->next->number = number;
            tail->next->next = nullptr;
            tail = tail->next;
        }
        length++;
    }

    T getHeadNumber() {
        return head->number;
    }

    node<T> *getHeadNext() {
        return head->next;
    }

    void concat(const linkedlist<T>& l){
        if(head == nullptr) { // the head is null means the list has no element
            if (!l.empty()) {
                node<T> *t = l.getTail();
                for (node<T> *n = l.getHead(); n != t->next; n = n->next) {
                    //head = new node;
                    node<T> *newnode = new node<T>;
                    if (length == 0) {
                        head = newnode;
                    }
                    newnode->number = n->number;
                    newnode->next = n->next;
                    tail = newnode;
                    newnode = newnode->next;

                    length++;
                }
            }
        }
        else{
            tail->next = l.getHead();

            tail = l.getTail();
            length = length+l.getLength();
        }
    }

    void deletelist() { // for delting the elements in the list.
        node<T> *dummy;
        dummy = head;
        while (dummy != nullptr) {
            head = head->next;
            delete dummy;
            dummy = head;
        }
    }
    bool empty() const{ // checking emptiness of a linked list
        if(head == nullptr) // if the head is nullptr then the list is empty.
            return true;
        else
            return false;
    }
};
#endif //MULTISTACKTIMEDAUTOMATA_LINKEDLIST_H
