template <typename _Dt>
class MinHeap
{
public:
    
    _Dt* arrayOfHeap;
    MinHeap (const int& size)
    {
        arrayOfHeap = new _Dt[size + 1];
        lastNodeIndex = 0;
    }

    void enqueue (_Dt value)
    {
        arrayOfHeap[++lastNodeIndex] = value;
        fix_increased_node();
    }

    _Dt dequeue ()
    {
        auto rootValue = arrayOfHeap[1];
        swap (arrayOfHeap[1], arrayOfHeap[lastNodeIndex]);
        lastNodeIndex--;
        fix_subtree_root ();
        return rootValue;
    }

    void fix_subtree_root ()
    {
        int movingNodeIndex = 1; // root index
        auto leftChildIndex = movingNodeIndex*2, rightChildIndex = movingNodeIndex*2 + 1; 
        while (leftChildIndex <= lastNodeIndex && rightChildIndex <= lastNodeIndex) // while right child and left child exist
        {
            if (is_in_right_place(movingNodeIndex)) // if movingNode is in the place it must be
            {
                return;
            }
            else if (arrayOfHeap[leftChildIndex] < arrayOfHeap[rightChildIndex])
            {
                swap (arrayOfHeap[leftChildIndex], arrayOfHeap[movingNodeIndex]);
                movingNodeIndex = leftChildIndex;
            }
            else
            {
                swap (arrayOfHeap[rightChildIndex], arrayOfHeap[movingNodeIndex]);
                movingNodeIndex = rightChildIndex;
            }
            leftChildIndex = movingNodeIndex*2; rightChildIndex = movingNodeIndex*2 + 1;
        }

        if (movingNodeIndex*2 == lastNodeIndex && (arrayOfHeap[movingNodeIndex*2] < arrayOfHeap[movingNodeIndex]))
        {
            swap (arrayOfHeap[movingNodeIndex], arrayOfHeap[movingNodeIndex*2]);
        }
    };

    _Dt get_root ()
    {
        return arrayOfHeap[1];
    };

private:
    int lastNodeIndex;

    void fix_increased_node()
    {
        auto movingNodeIndex = lastNodeIndex;
        while (movingNodeIndex != 1 && (arrayOfHeap[movingNodeIndex] < arrayOfHeap[movingNodeIndex/2]))
        {
            swap (arrayOfHeap[movingNodeIndex], arrayOfHeap[movingNodeIndex/2]);
            movingNodeIndex = movingNodeIndex/2;
        }
    }

    bool is_in_right_place(const int& index)
    {
        return (arrayOfHeap[index] < arrayOfHeap[index*2])
            && (arrayOfHeap[index] < arrayOfHeap[index*2 + 1]);
    }
};
