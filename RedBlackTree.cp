
class TreeNode 
{
public:
    int data;
    enum NodeColor {RED = 'R', BLACK = 'B'};
    NodeColor color = NodeColor::BLACK;
    TreeNode* left_child = NULL;
    TreeNode* right_child = NULL;
    TreeNode* father = NULL;
    bool is_null = true;
    void makeNode (int data)
    {
        this->data = data;
        this->is_null = false;
        left_child = new TreeNode; left_child->father = this;
        right_child = new TreeNode; right_child->father = this;
    }
};

class RedBlackTree
{
public:
    RedBlackTree ()
    {
        root = new TreeNode;
        root->color = TreeNode::BLACK;
    }

    bool isEmpty () 
    {
        return this->root->is_null;
    }

    TreeNode* getRoot()
    {
        return this->root;
    }

    TreeNode* getNodeIncludingKey (const int& num_to_search)
    {
        auto iterator_node = getRoot();
        while (true)
        {
            if (iterator_node->is_null || iterator_node->data == num_to_search)
                return iterator_node;
            else if (iterator_node->data > num_to_search)
                iterator_node = iterator_node->left_child;
            else 
                iterator_node = iterator_node->right_child;
        }
    } 
    
    void insert (int data_to_insert)
    {
        if (isEmpty())
        {
            this->root->makeNode(data_to_insert);
            this->root->color = TreeNode::BLACK;
        }
        else
        {
            auto node_to_insert = getNodeIncludingKey (data_to_insert);
            if (!node_to_insert->is_null)
                return printMessege("repeated data! data already exists");
                
            node_to_insert->makeNode(data_to_insert);
            node_to_insert->color = TreeNode::RED;
            fixInsertedNode(node_to_insert);
        }
    }

    void deleteElement (const int& key)
    {
        auto node_to_delete = getNodeIncludingKey(key);
        if (node_to_delete->is_null)
            return printMessege ("INVALID");
        
        TreeNode* helper;
        auto copy_deleing_node = node_to_delete;
		int copy_original_color = copy_deleing_node->color;
		if (node_to_delete->left_child->is_null) 
        {
			helper = node_to_delete->right_child;
			popNode(node_to_delete, node_to_delete->right_child);
		} 
        else if (node_to_delete->right_child->is_null) 
        {
			helper = node_to_delete->left_child;
			popNode(node_to_delete, node_to_delete->left_child);
		} 
        else 
        {
			copy_deleing_node = getLeastThan (node_to_delete->right_child);
			copy_original_color = copy_deleing_node->color;
			helper = copy_deleing_node->right_child;
			if (copy_deleing_node->father == node_to_delete) 
				helper->father = copy_deleing_node;
            else 
            {
				popNode(copy_deleing_node, copy_deleing_node->right_child);
				copy_deleing_node->right_child = node_to_delete->right_child;
				copy_deleing_node->right_child->father = copy_deleing_node;
			}
			popNode(node_to_delete, copy_deleing_node);
			copy_deleing_node->left_child = node_to_delete->left_child;
			copy_deleing_node->left_child->father = copy_deleing_node;
			copy_deleing_node->color = node_to_delete->color;
		}
		delete node_to_delete;
		if (copy_original_color == TreeNode::BLACK)
			fixDelete(helper);

    }

    void printUngraphicly(TreeNode* subtreeRoot)
    {
        cout << "root: " << subtreeRoot->data << (char)subtreeRoot->color << endl;
        if (subtreeRoot->left_child != NULL)
        {
            cout << "going left...";
            printUngraphicly(subtreeRoot->left_child);
        }
        if (subtreeRoot->right_child != NULL)
        {
            cout << "going right...";
            printUngraphicly(subtreeRoot->right_child); 
        }
        cout << "backing ..." << endl;
        return;
    }

    void printTree ()
    {
        printing_list.addEmptyNode();
        list_iterating_node = printing_list.begin();
        storeTreeShapeInList (this->root);
        for (auto node = printing_list.begin(); node != NULL; node = node->next)
            cout << node->data << endl;

        cout << endl;
        printing_list.clear();
    }

    TreeNode* foundNode = NULL;
private:
    TreeNode* root;
    List<string> printing_list;
    ListNode<string>* list_iterating_node = new ListNode<string>;

    void storeTreeShapeInList  (TreeNode* subtreeRoot)
    {
        if (subtreeRoot->is_null)
        {
            list_iterating_node->data += "N ";
            list_iterating_node = list_iterating_node->prev->prev;
        }
        else 
        {
            list_iterating_node->data += to_string(subtreeRoot->data);
            list_iterating_node->data += (char)(subtreeRoot->color);
            if (subtreeRoot == this->foundNode)
                list_iterating_node->data += '*';
            
            list_iterating_node->data += ' ';
            if (list_iterating_node == printing_list.end())
            {
                printing_list.addEmptyNode(2);
            }
            {// just a block for left child
                list_iterating_node = list_iterating_node->next;
                list_iterating_node->data += "/ ";
                list_iterating_node = list_iterating_node->next;
                storeTreeShapeInList  (subtreeRoot->left_child);
            }
            {// just a block for right child
                list_iterating_node = list_iterating_node->next;
                list_iterating_node->data += "\\ ";
                list_iterating_node = list_iterating_node->next;
                storeTreeShapeInList  (subtreeRoot->right_child);
            }
            if (list_iterating_node != printing_list.begin())
                list_iterating_node = list_iterating_node->prev->prev;
        }
    }

    void fixDelete(TreeNode* helper) 
    {
		TreeNode* sibling;
		while (helper != root && helper->color == TreeNode::BLACK) 
        {
			if (helper == helper->father->left_child) 
            {
				sibling = helper->father->right_child;
				if (sibling->color == TreeNode::RED) 
                {
					sibling->color = TreeNode::BLACK;
					helper->father->color = TreeNode::RED;
					leftRotate(helper->father);
					sibling = helper->father->right_child;
				}
				if (sibling->left_child->color == TreeNode::BLACK && sibling->right_child->color == TreeNode::BLACK) 
                {
					sibling->color = TreeNode::RED;
					helper = helper->father;
				} 
                else 
                {
					if (sibling->right_child->color == TreeNode::BLACK) 
                    {
						sibling->left_child->color = TreeNode::BLACK;
						sibling->color = TreeNode::RED;
						rightRotate(sibling);
						sibling = helper->father->right_child;
					} 
					sibling->color = helper->father->color;
					helper->father->color = TreeNode::BLACK;
					sibling->right_child->color = TreeNode::BLACK;
					leftRotate(helper->father);
					helper = root;
				}
			} 
            else 
            {
				sibling = helper->father->left_child;
				if (sibling->color == TreeNode::RED) 
                {
					sibling->color = TreeNode::BLACK;
					helper->father->color = TreeNode::RED;
					rightRotate(helper->father);
					sibling = helper->father->left_child;
				}

				if (sibling->right_child->color == TreeNode::BLACK && sibling->right_child->color == TreeNode::BLACK) 
                {
					sibling->color = TreeNode::RED;
					helper = helper->father;
				} 
                else 
                {
					if (sibling->left_child->color == TreeNode::BLACK) 
                    {
						sibling->right_child->color = TreeNode::BLACK;
						sibling->color = TreeNode::RED;
						leftRotate(sibling);
						sibling = helper->father->left_child;
					} 

					sibling->color = helper->father->color;
					helper->father->color = TreeNode::BLACK;
					sibling->left_child->color = TreeNode::BLACK;
					rightRotate(helper->father);
					helper = root;
				}
			} 
		}
		helper->color = TreeNode::BLACK;
	}

    void popNode (TreeNode* dad, TreeNode* child)
    {
        if (dad->father == NULL) 
			root = child;
		else if (dad == dad->father->left_child)
			dad->father->left_child = child;
		else 
			dad->father->right_child = child;

		child->father = dad->father;
    }

    void fixInsertedNode (TreeNode* inserted_node)
    {
        while (inserted_node->father->color == TreeNode::RED)
        {
            if (inserted_node->father == inserted_node->father->father->right_child)
                fixFatherInRight(inserted_node);
            else 
                fixFatherInLeft(inserted_node);

            if (inserted_node == this->root)
                break;
        }
        this->root->color = TreeNode::BLACK;
    }

    void fixFatherInRight (TreeNode*& inserted_node)
    {
        auto uncle = inserted_node->father->father->left_child;
        if (uncle->color == TreeNode::RED)
        {
            uncle->color = TreeNode::BLACK;
            inserted_node->father->color = TreeNode::BLACK;
            inserted_node->father->father->color = TreeNode::RED;
            inserted_node = inserted_node->father->father;
        }
        else
        {
            if (inserted_node == inserted_node->father->left_child)
            {
                inserted_node = inserted_node->father;
                rightRotate(inserted_node);
            }
            inserted_node->father->color = TreeNode::BLACK;
            inserted_node->father->father->color = TreeNode::RED;
            leftRotate(inserted_node->father->father);
        }
        
    }
    
    void fixFatherInLeft (TreeNode*& inserted_node)
    {
        auto uncle = inserted_node->father->father->right_child;
        if (uncle->color == TreeNode::RED)
        {
            uncle->color = TreeNode::BLACK;
            inserted_node->father->color = TreeNode::BLACK;
            inserted_node->father->father->color = TreeNode::RED;
            inserted_node = inserted_node->father->father;
        }
        else
        {
            if (inserted_node == inserted_node->father->right_child)
            {
                inserted_node = inserted_node->father;
                leftRotate(inserted_node);
            }
            inserted_node->father->color = TreeNode::BLACK;
            inserted_node->father->father->color = TreeNode::RED;
            rightRotate(inserted_node->father->father);
        }
    }

    void leftRotate (TreeNode* node)
    {
        auto r_hand = node->right_child;
		node->right_child = r_hand->left_child;
		if (!r_hand->left_child->is_null)
			r_hand->left_child->father = node;

		r_hand->father = node->father;
		if (node->father == NULL)
			this->root = r_hand;
        else if (node == node->father->left_child)
			node->father->left_child = r_hand;
		else
			node->father->right_child = r_hand;

		r_hand->left_child = node;
		node->father = r_hand;
    }

    void rightRotate (TreeNode* node)
    {
        auto l_hand = node->left_child;
        node->left_child = l_hand->right_child;
        if (!l_hand->right_child->is_null)
            l_hand->right_child->father = node;

        l_hand->father = node->father;
        if (l_hand->father == NULL)
            this->root = l_hand;
        else if(node == node->father->right_child)
            node->father->right_child = l_hand;
        else
            node->father->left_child = l_hand;

        l_hand->right_child = node;
        node->father = l_hand; 
    }

    TreeNode* getLeastThan (TreeNode* node)
    {
        while (!node->left_child->is_null) 
			node = node->left_child;
		return node;
    }

};
