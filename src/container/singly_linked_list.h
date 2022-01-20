#pragma once

namespace noble_steed
{

template<class T>
class Singly_Linked_List
{
  public:
    struct Node
    {
        T data;
        Node *next;
    };

    Singly_Linked_List() {}

    void insert(Node *prev_node, Node *new_node)
    {
        if (prev_node == nullptr)
        {
            // Is the first node
            if (head != nullptr)
            {
                // The list has more elements
                new_node->next = head;
            }
            else
            {
                new_node->next = nullptr;
            }
            head = new_node;
        }
        else
        {
            if (prev_node->next == nullptr)
            {
                // Is the last node
                prev_node->next = new_node;
                new_node->next = nullptr;
            }
            else
            {
                // Is a middle node
                new_node->next = prev_node->next;
                prev_node->next = new_node;
            }
        }
    }

    void remove(Node *prev_node, Node *del_node)
    {
        if (prev_node == nullptr)
        {
            // Is the first node
            if (del_node->next == nullptr)
            {
                // List only has one element
                head = nullptr;
            }
            else
            {
                // List has more elements
                head = del_node->next;
            }
        }
        else
        {
            prev_node->next = del_node->next;
        }
    }

    Node *head;
};

} // namespace noble_steed