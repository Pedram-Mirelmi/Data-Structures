#include "linked_list.h"
template <typename _Tp>
class TreeNode 
{
public:
    List<TreeNode*> children_list;
    TreeNode* parent = NULL;
    List<_Tp> keys_list;
    bool is_leaf = true;
};

template <typename D_type>
class BTree
{
typedef unsigned short int deg_Tp;
typedef TreeNode<D_type> TN;
typedef TN* TN_ptr;
typedef ListNode<D_type>* key_LNPtr;
typedef ListNode<TN_ptr>* child_LNPtr;
public:

    TN_ptr root = NULL;
    const deg_Tp max_degree;

    BTree (deg_Tp degree)
        : max_degree(degree)
        { root = new TN; }

    TN_ptr 
    search (const D_type& data)
        { return searchHelper(this->root, data); }

    void 
    insert (const D_type& data)
    {
        if (isEmpty())
        {
            root->keys_list.push_back(data);
            root->children_list.push_back(NULL);
            root->children_list.push_back(NULL);
        }
        else
        {
            auto pair = whereExactlyToInsert (data);
            auto tree_node = pair.first;
            auto list_node = pair.second;
            if (list_node == NULL)
                tree_node->keys_list.push_back(data);
            else
                tree_node->keys_list.insert_before(list_node, data);
            tree_node->children_list.push_back(NULL);
            balance (tree_node);
        }
    }

    D_type 
    deleteData (const D_type& data_to_delete)
    {
        auto including_node = search (data_to_delete);
        if (including_node == NULL) 
            error ("data not found!");
        auto successor_node = successor (data_to_delete, including_node);
        if (including_node == successor_node) // is a leaf
        {
            including_node->keys_list.delete_data(data_to_delete);
            including_node->children_list.pop_back();
            balance (including_node);
        }
        else   // middle of the tree (not a leaf)
        {   
            including_node->keys_list.find(data_to_delete)->data = 
                        successor_node->keys_list.pop_front();
            successor_node->children_list.pop_front();
            balance (successor_node);
        }
        return data_to_delete;
    }

    void 
    fuckedPrint (TN_ptr node = NULL)
    {
        if (node == NULL)
            print(this->root);
        else 
            print (node);
    }

    D_type
    getMinData ()
        { return minimum(root)->keys_list.front(); }

    
    D_type
    getMaxData ()
        { return maximum(root)->keys_list.back(); }

    D_type 
    popMinData ()
        { return deleteData (getMinData()); }

    D_type
    popMaxData ()
        {return deleteData (getMaxData); }

private:

    void 
    balance (TN_ptr node)
    {
        if (node->children_list.get_length() > max_degree)
            balanceTooMany (node);
        else if (node->children_list.get_length() <= max_degree/2)
            balanceTooFew(node); 
    }

    void 
    balanceTooFew (TN_ptr node) // while deleting
    {
        if (node == root) {
            fixRootTooFew();
            return;
        }
        auto bro_pair = getBrothers(node);
        auto r_bro = bro_pair.second, l_bro = bro_pair.first;
        if (ableToBorrow(r_bro))
            getFromRightBrother (node, r_bro);
        else if (ableToBorrow(l_bro))
            getFromLeftBrother (node, l_bro);
        else if (r_bro != NULL) // right brother exists
            merge (node, r_bro);
        else if (l_bro != NULL) // left brother exists
            merge (l_bro, node);
        else
            error ("WTH?????");
    }

    void 
    merge (TN_ptr node, TN_ptr r_bro) // expand "node" to "r_bro"
    {
        auto parent_key_node_down_coming = 
                node->parent->keys_list[node->parent->children_list.get_index(node)];
        node->keys_list.push_back(parent_key_node_down_coming->data);
        node->keys_list.concat (r_bro->keys_list);
        node->children_list.concat (r_bro->children_list);
        setParents (node, node->children_list);
        node->parent->children_list.delete_data(r_bro);
        node->parent->keys_list.delete_node(parent_key_node_down_coming);
        delete r_bro;
        balance (node->parent);
    }

    void 
    balanceTooMany (TN_ptr node) // while insering
    {
        if (node == root)
            fixRootTooMany();
        else
        {
            auto upgoing_data = node->keys_list[node->keys_list.get_length()/2]->data;
            auto pair = split(node);
            auto left_node = pair.first;
            auto right_node = pair.second;
            auto parent_node = node->parent;
            auto inserting_spot = getAppropriateSpot(parent_node->keys_list, upgoing_data);
            if (inserting_spot.first == NULL)
                parent_node->keys_list.push_back(upgoing_data);
            else
                parent_node->keys_list.insert_before(inserting_spot.first, upgoing_data);
            auto spliting_children_spot = parent_node->children_list[inserting_spot.second];
            spliting_children_spot->data = left_node;
            parent_node->children_list.insert_next_to(spliting_children_spot, right_node);
            left_node->parent = right_node->parent = parent_node;
            delete node;
            balance (parent_node);
        }
    }

    Pair<TN_ptr, TN_ptr> 
    split (TN_ptr node)
    {
        auto left_node = new TN, right_node = new TN;
        deg_Tp cut_index = node->keys_list.get_length()/2;
        auto middle_node = node->children_list[cut_index];
        node->children_list.insert_next_to(middle_node, NULL);
        auto pair_of_children_lists = node->children_list.cut(middle_node->next, cut_index + 1);
        auto pair_of_keys_lists = node->keys_list.cut(node->keys_list[cut_index], cut_index);
        left_node->keys_list = pair_of_keys_lists.first;
        left_node->children_list = pair_of_children_lists.first;
        right_node->keys_list = pair_of_keys_lists.second;
        right_node->children_list = pair_of_children_lists.second;
        right_node->is_leaf = left_node->is_leaf = node->is_leaf;
        right_node->parent = left_node->parent = node->parent;
        setParents(left_node, left_node->children_list); 
        setParents(right_node, right_node->children_list);
        return Pair<TN_ptr, TN_ptr> (left_node, right_node);
    }

    Pair<TN_ptr, key_LNPtr> 
    whereExactlyToInsert (const D_type& data)
    {
        auto node = findNodeForInsert(root, data); 
        auto list_node = getAppropriateSpot(node->keys_list, data).first;
        return Pair<TN_ptr, key_LNPtr> (node, list_node);
    }

    TN_ptr 
    findNodeForInsert (TN_ptr subtree_root, const D_type& data) 
    {
        if (subtree_root->is_leaf)
            return subtree_root;
        auto spot = getAppropriateSpot (subtree_root->keys_list, data);
        return this->findNodeForInsert(subtree_root->children_list[spot.second]->data, data);
    }    


    Pair<key_LNPtr, deg_Tp> 
    getAppropriateSpot (List<D_type>& list, const D_type& data) 
    {
        int index = 0;
        auto iter = list.begin();
        for ( ; iter != NULL && data > iter->data; iter = iter->next) 
            index++;
        return Pair<key_LNPtr, deg_Tp> (iter, index);
    }

    TN_ptr 
    searchHelper (TN_ptr subtree_root, const D_type& data)
    {
        if (subtree_root == NULL)
            return NULL;
        auto spot = getAppropriateSpot (subtree_root->keys_list, data);
        if (spot.first == NULL)
            return this->searchHelper (subtree_root->children_list.back(), data);
        if (spot.first->data == data)
            return subtree_root;
        return this->searchHelper (subtree_root->children_list[spot.second]->data, data);
    }

    void 
    fixRootTooMany ()
    {
        auto upgoing_data = root->keys_list[
                root->keys_list.get_length()/2]->data;
        auto pair = split(root);
        auto left_node = pair.first;
        auto right_node = pair.second;
        root = new TN;
        root->children_list.push_back(left_node);
        root->children_list.push_back(right_node);
        root->keys_list.push_back(upgoing_data);
        left_node->parent = right_node->parent = root;
        root->is_leaf = false;
    }

    void
    fixRootTooFew ()
    {
        if (root->keys_list.get_length() == 0) 
        { 
            if (root->children_list.front() == NULL)
            {
                delete root;
                root = new TN;
                return;
            }
            root = root->children_list.front();
            delete root->parent; root->parent = NULL;
        }
    }

    void 
    getFromLeftBrother (TN_ptr node, TN_ptr l_bro)
    {
        deg_Tp parent_key_index = node->parent->children_list.get_index(l_bro);
        auto brother_moving_child = l_bro->children_list.pop_back();
        auto brother_moving_key = l_bro->keys_list.pop_back();
        if (brother_moving_child != NULL)
            brother_moving_child->parent = node;
        node->children_list.push_front(brother_moving_child);
        node->keys_list.push_front(node->parent->keys_list[parent_key_index]->data);
        node->parent->keys_list[parent_key_index]->data = brother_moving_key;
    }

    void 
    getFromRightBrother (TN_ptr node, TN_ptr r_bro)
    {
        deg_Tp parent_key_index = node->parent->children_list.get_index(node);
        auto brother_moving_child = r_bro->children_list.pop_front();
        auto brother_moving_key = r_bro->keys_list.pop_front();
        if (brother_moving_child != NULL)
            brother_moving_child->parent = node;
        node->children_list.push_back(brother_moving_child);
        node->keys_list.push_back(node->parent->keys_list[parent_key_index]->data);
        node->parent->keys_list[parent_key_index]->data = brother_moving_key;
    }

    bool 
    ableToBorrow (TN_ptr l_bro)
    {
        return (l_bro != NULL) 
            && (l_bro->children_list.get_length() > (deg_Tp)(max_degree/2 + 1));
    }

    Pair<TN_ptr, TN_ptr> 
    getBrothers (TN_ptr node)
    {
        auto self_loc = node->parent->children_list.find(node);
        return Pair<TN_ptr, TN_ptr> (
                getUsablePtr(self_loc->prev),
                getUsablePtr(self_loc->next)); 
    }

    TN_ptr
    getUsablePtr (ListNode<TN_ptr>* p)
        { return p == NULL ? NULL : p->data; }

    void 
    setParents (TN_ptr parent_node, List<TN_ptr>& children_list) 
    {
        if (children_list.begin()->data != NULL)
            for (auto l_node = children_list.begin(); l_node != NULL; l_node = l_node->next)
                l_node->data->parent = parent_node;
    }

    TN_ptr 
    predecessor (const D_type& data, TN_ptr node = NULL)
    {
        if (node == NULL)
            node = search (data);
        if (node->is_leaf)
            return node;
        deg_Tp index = node->keys_list.get_index(data);
        return maximum (node->children_list[index]->data);
    }

    TN_ptr
    successor (const D_type& data, TN_ptr node = NULL)
    {
        if (node == NULL)
            node = search (data);
        if (node->is_leaf)
            return node;
        deg_Tp index = node->keys_list.get_index(data);
        return minimum (node->children_list[index + 1]->data);
    }

    TN_ptr
    maximum (TN_ptr node)
    {
        if (node->is_leaf)
            return node;
        return maximum (node->children_list.back());
    }

    TN_ptr
    minimum (TN_ptr node)
    {
        if (node->is_leaf)
            return node;
        return minimum (node->children_list.front());
    }

    void 
    print (TN_ptr subtree_root)
    {
        cout << "keys: ";
        subtree_root->keys_list.print_list(); cout << endl;
        if (!subtree_root->is_leaf)
            for (auto child = subtree_root->children_list.begin(); 
                 child != NULL; child = child->next)
            {
                cout << "going to a child: ";
                this->print (child->data);
            }
        cout << "backing..." << endl;
    }

    void 
    error (const string& message)
    {
        cerr << message << endl;
        exit(1);
    }

public:
    bool isEmpty ()
        { return root->keys_list.is_empty(); }

};
