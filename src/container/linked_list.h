#pragma once

namespace noble_steed
{

template<class T>
struct ll_node
{
    T data;
    ll_node *next;
};

template<class T>
struct singly_linked_list
{
    singly_linked_list() {}

    void insert(ll_node<T> *prev_node, ll_node<T> *new_node)
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

    void remove(ll_node<T> *prev_node, ll_node<T> *del_node)
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

    ll_node<T> *head;
};

template<class T>
struct stack_linked_list
{
    stack_linked_list() = default;

    stack_linked_list(stack_linked_list &stck_ll) = delete;

    void push(ll_node<T> *new_node)
    {
        new_node->next = head;
        head = new_node;
    }

    ll_node<T> *pop()
    {
        ll_node<T> *top = head;
        head = head->next;
        return top;
    }

    ll_node<T> *head;
};


} // namespace noble_steed