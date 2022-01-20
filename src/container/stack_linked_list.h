#pragma once

namespace noble_steed
{

template<class T>
class Stack_Linked_List
{
  public:
    struct Node
    {
        T data;
        Node *next;
    };

    Stack_Linked_List() = default;

    Stack_Linked_List(Stack_Linked_List &stackLinkedList) = delete;

    void push(Node *new_node)
    {
        new_node->next = head;
        head = new_node;
    }

    Node *pop()
    {
        Node *top = head;
        head = head->next;
        return top;
    }

    Node *head;
};

} // namespace noble_steed