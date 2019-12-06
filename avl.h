#ifndef AVL_H
#define AVL_H
#include "bst_functions.h"

template <typename T>
class AVL
{
    //preconditions: none
    //postconditions: call tree_print to print the tree from right to left.
    friend ostream& operator<<(ostream& outs, const AVL<T>& tree)
    {
        //outs << "Calling tree_print from AVL" << endl;
        tree_print(tree.root,0,outs);
        outs << endl;
        return outs;
    }

public:
    AVL();
    AVL(const T* sorted_list, int size=-1); //construct an avl from the sorted list using tree_from_sorted_List.
    AVL(const AVL<T>& copy_me);             //construct an avl that contains the same values as copy_me.
    ~AVL();

    AVL<T>& operator =(const AVL<T>& rhs);  //assign this avl the contents of rhs.
    AVL<T>& operator +=(const AVL<T>& rhs); //add each node from rhs to this avl.

    bool insert(const T& insert_me);        //insert the value into this avl.
    bool erase(const T& target);            //remove the value into this avl.
    bool search(const T& target, tree_node<T>* & found_ptr); //find the value in this avl.

    bool isBalanced(); //non-recursive caller for verifyBalance.

    size_t size() const
    {
        return (root) ? root->_size : 0;
    }

private:
     //traverse the tree and verify the balance factor is < 2 and > -2 at each node.
    void verifyBalance(tree_node<T>* root, bool &balance);
    tree_node<T>* root;
};

//preconditions: none.
//postconditions: a new, empty AVL object is constructed.
// root is initialized to null.
template <typename T>
AVL<T>::AVL()
{
    root = nullptr;
}

//preconditions: size >= 0, list must be sorted.
//postconditions: a new, AVL object is constructed, root will point to a tree containing the values
// of the recieved array. The tree_from_sorted_list function is called, which will recursively split
// the array into left and right sub arrays, (the current node's value will be set to the middle of the array.)
// the right subarray will be distributed to the right subtree, and the left subarray to the left subarray by
// making recursive calls that reduce the size and change the starting position of the arary.
template <typename T>
AVL<T>::AVL(const T* sorted_list, int size)
{
    assert(size >=0);
    root = tree_from_sorted_list(sorted_list, size);
}

//preconditions: none.
//postconditions: a new AVL object is constructed with its
// contents equal to those of the recieved AVL
template <typename T>
AVL<T>::AVL(const AVL<T>& copy_me)
{
    root = tree_copy(copy_me.root);
}

//preconditions: self-assignment is not allowed.
//postconditions: clear the current tree pointed to by root,
// then assign root to be a copy of rhs.
template <typename T>
AVL<T>& AVL<T>::operator =(const AVL<T>& rhs)
{
    assert(&rhs != this);
    tree_clear(root);
    root = tree_copy(rhs.root);
    return *this;
}

//preconditions: none
//postconditions: call tree_clear to traverse the tree and deallocate all nodes,
// starting from the leftmost leaves.
template <typename T>
AVL<T>::~AVL()
{
    tree_clear(root);
}

//preconditions: none
//postconditions: insert the item into the tree by calling tree_insert,
//  return true if it was inserted, otherwise false.
template <typename T>
bool AVL<T>::insert(const T& insert_me)
{
    return tree_insert(root,insert_me,true);
}

//preconditions: none
//postconditions: call tree_erase to find the item in the this AVL and remove it.
// return true if the node was removed, otherwise return false.
template <typename T>
bool AVL<T>::erase(const T& target)
{
    return tree_erase(root,target,true);
}

//preconditions: none
//postconditions: return true if the node was found in the tree, use tree_search to
// conduct a binary search of the tree pointed to by root.
template <typename T>
bool AVL<T>::search(const T& target, tree_node<T>* & found_ptr)
{
    return tree_search(root,target,found_ptr);
}

//preconditions: self-assignment is not allowed.
//postconditions: add the rhs to this tree, using tree_insert,
// return the AVL pointed to by this.
template <typename T>
AVL<T>& AVL<T>::operator +=(const AVL<T>& rhs)
{
    assert(&rhs != this);
    tree_add(this->root,rhs.root,true);
    return *this;
}

//preconditions: none
//postconditions: traverse the tree recursively and if the balance
// factor of any node is found to be >= 2 or <= -2, set balance to false.
template <typename T>
void AVL<T>::verifyBalance(tree_node<T>* root, bool &balance)
{
    if(root)
    {
        verifyBalance(root->_left,balance);
        if(abs(root->balance_factor()) >= 2) balance = false;
        verifyBalance(root->_right,balance);
    }
}

//preconditions: none
//postconditions: wrapper function for recursive funnction: verifyBalance.
// return the reference parameter.
template <typename T>
bool AVL<T>::isBalanced()
{
    bool balanced = true;
    verifyBalance(root, balanced);
    return balanced;
}

#endif // AVL_H
