#include <iostream>
using namespace std;

template <typename First, typename Second>
class Pair
{
public:
    Pair (const First& first, const Second& second)
    {
        this->first = first;
        this->second = second;
    }
    First first;
    Second second;
};

template <typename DataType>
class ListNode
{
public:
    ListNode* next = NULL;
    ListNode* prev = NULL;
    DataType data;
};

template <typename _Tp>
class List
{
typedef ListNode<_Tp>* LN_ptr;
private:
    ListNode <_Tp>* head_node = NULL;
    ListNode <_Tp>* last_node = NULL;
    size_t length = 0;

public:
    List () = default;

    List (const LN_ptr& head, const LN_ptr& last)
        : head_node(head) , last_node(last) 
    {
        this->set_length(calculate_len());
        head_node->prev = last_node->next = NULL;
    }

    List (const LN_ptr& head, const LN_ptr& last, size_t len) // Not trustable
        : head_node(head) , last_node(last), length(len)
    {   
        last_node->next = head_node->prev = NULL;
    }

    void 
    push_back (const _Tp& dataToInsert)
    {
        if (this->is_empty())
            push_as_first(dataToInsert);
        else
            push_at_end (dataToInsert);
        length++;
    }

    void 
    push_front (const _Tp& dataToInsert)
    {
        if (is_empty())
            push_as_first(dataToInsert);
        else 
            push_at_front(dataToInsert);
        length++;
    }

    void 
    push_back_node (LN_ptr node)
    {
        if (is_empty())
            head_node = last_node = node;
        else 
        {
            node->prev = this->last_node;
            this->last_node->next = node;
            this->last_node = node;
        }
    }

    void 
    push_front_node (LN_ptr node)
    {
        if (is_empty())
            head_node = last_node = node;
        else 
        {
            node->next = this->head_node;
            this->head_node->prev = node;
            this->head_node = node;
        }
    }

    void 
    insert_next_to (LN_ptr given_node, const _Tp& given_data)
    {   
        if (given_node->next == NULL)
            this->push_back (given_data);
        else
        {
            auto next_node = given_node->next;
            auto new_node = new ListNode<_Tp>;
            new_node->data = given_data;
            new_node->next = next_node; next_node->prev = new_node;
            new_node->prev = given_node; given_node->next = new_node;
            length++;
        }
    }

    void 
    insert_before (LN_ptr given_node, const _Tp& given_data)
    {
        if (given_node->prev == NULL)
            this->push_front (given_data);
        else
        {
            auto prev_node = given_node->prev;
            auto new_node = new ListNode<_Tp>;
            new_node->data = given_data;
            new_node->prev = prev_node; prev_node->next = new_node;
            new_node->next = given_node; given_node->prev = new_node;
            length++;
        }
    }

    void 
    delete_data (const _Tp& data_to_delete)
    {
        auto node = this->find(data_to_delete);
        if (node == NULL)
            err ("Data not exist!");
        delete_node(node);
    }

    void 
    delete_node (LN_ptr node_to_delete)
    {
        if (node_to_delete == NULL)
            err ("NULL pointer");
        else if (node_to_delete->next == NULL && 
                    node_to_delete->prev == NULL)
            this->clear();
        else if (node_to_delete->next == NULL)
            pop_back();
        else if (node_to_delete->prev == NULL)
            pop_front();
        else 
        {
            node_to_delete->next->prev = node_to_delete->prev;
            node_to_delete->prev->next = node_to_delete->next;
            delete node_to_delete;
            length--;
        }
    }

    size_t 
    get_index (const _Tp& data_to_find)
    {
        int counter = 0;
        for (auto node = head_node; node != NULL; node = node->next)
        {
            if (node->data == data_to_find)
                return counter;
            counter++;
        }
        return -1;   
    }

    Pair<List<_Tp>, List<_Tp>> 
    cut (LN_ptr cut_spot)
    {
        auto cut_index = this->get_index(cut_spot->data);
        return cut (cut_spot, cut_index);
    }

    Pair<List<_Tp>, List<_Tp>> 
    cut (LN_ptr cut_spot, size_t cut_index)
    {
        List<_Tp> left_list (this->head_node,
                    cut_spot->prev, cut_index);
        List<_Tp> right_list (cut_spot->next,
            this->last_node, this->length - cut_index - 1);
        this->head_node = this->last_node = NULL;
        delete cut_spot;
        return Pair<List<_Tp>, List<_Tp>> (left_list, right_list);
    }

    void
    concat (List<_Tp>& list)
    {
        if (list.get_length() != 0)
        {
            this->end()->next = list.begin();
            list.begin()->prev = this->end();
            this->last_node = list.end();
            this->set_length (this->get_length() + list.get_length());
            list.head_node = list.last_node = NULL; list.set_length(0);
        }
        else if (this->get_length() == 0)
        {
            *this = list;
            list.last_node = list.head_node = NULL;
            list.set_length(0);
        }
    }

    LN_ptr 
    operator[] (size_t index)
    {
        auto node = head_node;
        for (size_t i = 0; i < index && node != NULL; i++)
            node = node->next;
        return node;
    }

    LN_ptr 
    find (const _Tp& data_to_find)
    {
        for (auto node = head_node; node != NULL; node = node->next)
            if (node->data == data_to_find)
                return node;
        return NULL;            
    }

    _Tp 
    back()
        { return last_node->data; }

    _Tp 
    front()
        { return head_node->data; }

    LN_ptr 
    begin()
        { return head_node; }

    LN_ptr 
    end()
        { return last_node; }

    bool 
    is_empty() 
        { return head_node == NULL; }

    _Tp 
    pop_front ()
    {
        if (is_empty())
            err ("Empty list!");
        auto to_delete_node = head_node; 
        auto to_delete_data = to_delete_node->data;
        head_node = head_node->next;
        delete to_delete_node;
        if (head_node == NULL)
            head_node = last_node = NULL;
        else 
            head_node->prev = NULL;
        length--;
        return to_delete_data;
    }

    _Tp 
    pop_back ()
    {
        if (is_empty())
            err ("Empty list");
        auto to_delete_node = last_node; 
        auto to_delete_data = to_delete_node->data;
        last_node = last_node->prev;
        delete to_delete_node;
        if (last_node == NULL)
            head_node = last_node = NULL;
        else 
            last_node->next = NULL;
        length--;
        return to_delete_data;
    }

    size_t 
    get_length()
        { return this->length; }

    size_t 
    calculate_len ()
    {
        int index = 0;   
        for (auto node = begin(); node != NULL; node = node->next)
            index++;
        return index;
    }

    void 
    clear()
    {
        for (auto node = head_node->next; node != NULL; node = node->next)
            delete node->prev;
        delete last_node;
        last_node = head_node = NULL;
        length = 0;
    }

    void 
    print_list ()
    {
        for (auto node = begin(); node != NULL; node = node->next)
            cout << node->data << ' ';
    }

private:

    void 
    set_length (int len)
        { this->length = len; }

    void 
    push_as_first (const _Tp& dataToInsert)
    {
        head_node = new ListNode<_Tp>;
        last_node = head_node;
        last_node->data = dataToInsert;
    }
    
    void 
    push_at_end (const _Tp& dataToInsert)
    {
        auto currentNode = last_node;
        last_node = new ListNode<_Tp>;
        last_node->data = dataToInsert;
        last_node->prev = currentNode;
        currentNode->next = last_node;
    }
    
    void 
    push_at_front(const _Tp& dataToInsert)
    {
        auto currentNode = head_node;
        head_node = new ListNode<_Tp>;
        head_node->data = dataToInsert;
        head_node->next = currentNode;
        currentNode->prev = head_node;
    }

    void
    err (const string& message)
    {
        cerr << message << endl;
        exit(1);
    }
public:
    friend ostream& 
    operator<< (ostream& os, const List& obj)
        { 
            os << '[';
            for (auto node = obj.head_node; node != obj.last_node; node = node->next)
                os << node->data << ", ";
            return os << obj.last_node->data << ']';
        }
};

