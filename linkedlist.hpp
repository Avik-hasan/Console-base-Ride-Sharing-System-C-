#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
using namespace std;

template <typename T>
class ListNode {
public:
    T data;
    ListNode* next;
    ListNode(T d) : data(d), next(nullptr) {}
};

template <typename T>
class SinglyLinkedList {
private:
    ListNode<T>* _head;
    ListNode<T>* _tail;
    int count;

public:
    SinglyLinkedList() : _head(nullptr), _tail(nullptr), count(0) {}

    ~SinglyLinkedList() {
        clr();
    }

    SinglyLinkedList(const SinglyLinkedList<T>& other) : _head(nullptr), _tail(nullptr), count(0) {
        ListNode<T>* curr = other._head;
        while (curr) {
            insrtTl(curr->data);
            curr = curr->next;
        }
    }

    SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& other) {
        if (this == &other) return *this;
        clr();
        ListNode<T>* curr = other._head;
        while (curr) {
            insrtTl(curr->data);
            curr = curr->next;
        }
        return *this;
    }

    void insrtHd(T data) {
        ListNode<T>* newNode = new ListNode<T>(data);
        if (!_head) {
            _head = _tail = newNode;
        } else {
            newNode->next = _head;
            _head = newNode;
        }
        count++;
    }

    void insrtTl(T data) {
        ListNode<T>* newNode = new ListNode<T>(data);
        if (!_tail) {
            _head = _tail = newNode;
        } else {
            _tail->next = newNode;
            _tail = newNode;
        }
        count++;
    }

    bool delNode(T data) {
        if (!_head) return false;
        if (_head->data == data) {
            ListNode<T>* temp = _head;
            _head = _head->next;
            if (!_head) _tail = nullptr;
            delete temp;
            count--;
            return true;
        }
        ListNode<T>* curr = _head;
        while (curr->next && curr->next->data != data) {
            curr = curr->next;
        }
        if (curr->next) {
            ListNode<T>* temp = curr->next;
            curr->next = temp->next;
            if (!curr->next) _tail = curr;
            delete temp;
            count--;
            return true;
        }
        return false;
    }

    T get(int idx) {
        if (idx < 0 || idx >= count) throw out_of_range("Invalid index");
        ListNode<T>* curr = _head;
        for (int i = 0; i < idx; i++) curr = curr->next;
        return curr->data;
    }

    int sz() const { return count; }
    bool emp() const { return count == 0; }

    void clr() {
        ListNode<T>* curr = _head;
        while (curr) {
            ListNode<T>* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        _head = _tail = nullptr;
        count = 0;
    }

    void shw() const {
        ListNode<T>* curr = _head;
        while (curr) {
            cout << curr->data << " -> ";
            curr = curr->next;
        }
        cout << "NULL\n";
    }

    ListNode<T>* head() const { return _head; }
    ListNode<T>* tail() const { return _tail; }
};

#endif
