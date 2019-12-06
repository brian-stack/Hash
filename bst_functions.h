#ifndef BST_FUNCTIONS_H
#define BST_FUNCTIONS_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cassert>

using namespace std;

template <typename T>
struct tree_node
{
    T _item;
    tree_node<T>* _left;
    tree_node<T>* _right;
    int _height;
    size_t _size;

    int balance_factor()
    {
        return (((_right) ? _right->_height : -1) - ((_left)? _left->_height : -1));
    }

    // Height of a node is 1 + height of the "taller" child
    //A leaf node has a height of zero: 1 + max(-1,-1)
    int height()
    {
        return (1 + std::max((_left) ? _left->_height : -1,(_right) ? _right->_height : -1));
    }

    //Set the _height member variable (call height();)
    int update_height()
    {
        return _height = height();
    }

    //Add the _size of the left and right sub trees
    //  (if they are not null, the size of a null child is 0) to the _size of this node.
    size_t update_size()
    {
        return _size = 1 + ((_left) ? _left->_size : 0) + ((_right) ? _right->_size : 0 );
    }

    tree_node(T item=T(), tree_node* left=NULL, tree_node* right=NULL): _item(item), _left(left), _right(right)
    {
        update_height();
        update_size();
    }

    friend ostream& operator <<(ostream& outs,const tree_node<T>& t_node)
    {
        //outs << "Calling record<< from tree_node<<" << endl;
        outs <<"|"<<t_node._item<<"|";
        return outs;
    }
};

//preconditions: none
//postconditions: A new node is created with the value: insert_me, and added to the tree when root is null.
// Recursive calls are made to this function with root->_left or root->_right depending on if our value to
// insert is less than or greater than the value of the node pointed to by the current root.
// If insertion is successful, return true, otherwise return false. Insertion can fail if a duplicate is found.
// When returning, update the height and size of all nodes that the newly inserted node is a decendent of,
//   if the AVL flag is true, call rotate also.
template <typename T>
bool tree_insert(tree_node<T>* &root, const T& insert_me, bool avl = false);

//preconditions: none
//postconditions: a recursive binary search is conducted until the target or null is encountered.
// recursively call tree_search with our left / right child, if root's value is greater / less than the target, respectively.
// return a pointer to the node with the target value or null if it was not found.
template <typename T>
tree_node<T>* tree_search(tree_node<T>* root, const T& target);

//preconditions: none
//postconditions: a recursive binary search is conducted until the target or null is encountered.
// If null is found, return false as the target does not exist in the tree. Otherwise, return true,
// return a pointer to the node by reference.
template <typename T>
bool tree_search(tree_node<T>* root, const T& target, tree_node<T>* &found_ptr);

//preconditions: none
//postconditions: the contents of the tree are printed to the
//   provided output stream from right to left.
template<typename T>
void tree_print(tree_node<T>* root, int level=0, ostream& outs=cout);

//preconditions: none
//postconditions: the contents of the tree are printed to the
//   provided output stream from right to left.
template<typename T>
void tree_print_debug(tree_node<T>* root, int level=0, ostream& outs=cout);

//preconditions: none
//postconditions: The memory reserved by the tree poined to by root is deallocated.
// Use recursion to delete leftmost node first, then that node's sibling and finally its parent.
template <typename T>
void tree_clear(tree_node<T>* &root);

template <typename T>
bool tree_erase(tree_node<T>*& root, const T& target, bool avl = false);

//preconditions: none
//postconditions: erase rightmost node from the tree,  store the item in max_value
template <typename T>
void tree_remove_max(tree_node<T>* &root, T& max_value, bool avl = false);

//preconditions: none
//postconditions: wrapper function for remove_max, returns the value removed by remove max
// that is returned by reference. If this is called on an empty tree, returns
// whatever is provided by the default constructor for type T.
template <typename T>
T tree_remove_max(tree_node<T>* &root, bool avl = false);

//preconditions: none
//postconditions: a new tree is constructed with the contents of the tree pointed to by root.
// Recursive calls are made to tree_copy, and the leftmost leaf nodes are constructed first.
template <typename T>
tree_node<T>* tree_copy(tree_node<T>* root);

//preconditions: none
//postconditions: the contents of src are copied to dest using recursive calls
// to traverse src and tree_insert to add items to dest.
template <typename T>
void tree_add(tree_node<T>* & dest, const tree_node<T>* src, bool avl = true);

//preconditions: a must be sorted.
//postconditions: a BST is constructed with the contents of a, the root is returned.
template <typename T>
tree_node<T>* tree_from_sorted_list(const T* a, int size);

//preconditions: none
//postconditions: if a is less than b, return b, otherwise return a.
template <typename T>
T max(const T & a, const T & b);

//preconditions: none
//postconditions: calls update_height() and update_size() on the node if it is not null.
template<typename T>
void updateNode(tree_node<T>* & root);


//----------------      AVL       ----------------
// ---------------- ROTATIONS --------------------------

//preconditions: root != null, root->_left != null,
//postconditions: Use temp pointers to save root->_left right child and the current root.
// Now set root to point to root's left subtree, set the new root's right to point to the previous root.
// Then set root->_right left to point to the previous root->_left right child.
template <typename T>
tree_node<T>* rotate_right(tree_node<T>* &root);

//preconditions: root != null, root->_right != null,
//postconditions: Use temp pointers to save root->_right left child and the current root.
// Now set root to point to root's right subtree, set the new root's left to point to the previous root.
// Then set root->_left right to point to the previous root->_right left child.
template <typename T>
tree_node<T>* rotate_left(tree_node<T>* &root);

//preconditions: none
//postconditions: decide which rotate is needed based on balance factor
// Case 1) Rotate left, then right: If the balance factor is -2 and our left child's balance factor is 1
// Case 2) Rotate right, then left: If the balance factor is 2 and our right child's balance factor is -1
// Case 3) Rotate right: If the balance factor is 2 and our right child's balance factor is NOT -1
// Case 4) Rotate left: If the balance factor is -2 and our left child's balance factor is NOT 1
template <typename T>
tree_node<T>* rotate(tree_node<T>* & root);


//preconditions: none
//postconditions: A new node is created with the value: insert_me, and added to the tree when root is null.
// Recursive calls are made to this function with root->_left or root->_right depending on if our value to
// insert is less than or greater than the value of the node pointed to by the current root.
// If insertion is successful, return true, otherwise return false. Insertion can fail if a duplicate is found.
// When returning, update the height and size of all nodes that the newly inserted node is a decendent of,
//   if the AVL flag is true, call rotate also.
template <typename T>
bool tree_insert(tree_node<T>* &root, const T& insert_me, bool avl)
{
    bool itemInserted = false;
    if(!root)
    {
        root = new tree_node<T>(insert_me);
        return true;
    }
    else if(root->_item < insert_me)
    {
        itemInserted = tree_insert(root->_right,insert_me,avl);
    }
    else if(root->_item > insert_me)
    {
        itemInserted = tree_insert(root->_left,insert_me,avl);
    }
    else
    {
        return false;
    }

    if(root && itemInserted)
    {
        root->update_height();
        root->update_size();
    }

    if(avl)
        root = rotate(root);

    return itemInserted;
}

//preconditions: none
//postconditions: a recursive binary search is conducted until the target or null is encountered.
// recursively call tree_search with our left / right child, if root's value is greater / less than the target, respectively.
// return a pointer to the node with the target value or null if it was not found.
template <typename T>
tree_node<T>* tree_search(tree_node<T>* root, const T& target)
{
    if(root->_item == target || !root)
        return root;
    else if(root->_item < target)
        return tree_search(root->_right, target);
    else
        return tree_search(root->_left, target);
}

//preconditions: none
//postconditions: a recursive binary search is conducted until the target or null is encountered.
// If null is found, return false as the target does not exist in the tree. Otherwise, return true,
// return a pointer to the node by reference.
template <typename T>
bool tree_search(tree_node<T>* root, const T& target, tree_node<T>* &found_ptr)
{
    if(!root)
    {
        found_ptr = nullptr; //if our target does not exist in the bst.
        return false;
    }
    else if(root->_item == target)
    {
        found_ptr = root; //if we have found our target.
        return true;
    }
    else if(root->_item < target)
    {
        return tree_search(root->_right, target,found_ptr); //if our target is more than the current root, search the right subtree
    }
    else
    {
        return tree_search(root->_left, target,found_ptr); //if our target is less than the current root, search the left subtree.
    }
}

//preconditions: none
//postconditions: the contents of the tree are printed to the
//   provided output stream from right to left.
template<typename T>
void tree_print(tree_node<T>* root, int level, ostream& outs)
{
    if(root)
    {
        tree_print(root->_right,level+1,outs);
        //outs << "Calling tree_node<< from tree_print" << endl;
        outs << setw(level*5) << *root << endl;
        tree_print(root->_left,level+1,outs);
    }
}

//preconditions: none
//postconditions: the contents of the tree are printed to the
//   provided output stream from right to left.
template<typename T>
void tree_print_debug(tree_node<T>* root, int level, ostream& outs)
{
    if(root)
    {
        tree_print_debug(root->_right,level+1,outs);
        outs << setw(level*5) << *root << " b=" << root->balance_factor() << " h=" << root->_height << " s=" << root->_size << endl;
        tree_print_debug(root->_left,level+1,outs);
    }
}

//preconditions: none
//postconditions: The memory reserved by the tree poined to by root is deallocated.
// Use recursion to delete leftmost node first, then that node's sibling and finally its parent.
template <typename T>
void tree_clear(tree_node<T>* &root)
{
    if(root)
    {
        tree_clear(root->_left);
        tree_clear(root->_right);
        delete root;
    }
}

//preconditions: none
//postconditions: removes the target from the tree (if it exists) and returns true, otherwise returns false.
// case 1) We know that the item is not in the tree if this function is called with root = null.
// case 2) If the value of the node pointed to by root is larger than our target, make a recursive call with root = root->_left
// case 3) If the value of the node pointed to by root is smaller than our target, make a recursive call with root = root->_right
// case 4) If root points to the node containing the target, we have found the node to remove, consider 2 different subcases:
//          a) We don't have a left subtree, simply bypass the node and delete it.
//          b) We do have a left subtree, replace the target node with the largest value in its left subtree
//            (calling remove_max, which will delete the largest valued node)
template <typename T>
bool tree_erase(tree_node<T>*& root, const T& target, bool avl)
{
    bool itemRemoved = false;

    //case 1: item is not in the tree.
    if(!root)
    {
        itemRemoved = false;
    }
    //case 4: we have found the item.
    else if (root->_item == target)
    {
        //case 4a: no left subtree.
        if(!root->_left)
        {
            tree_node<T> * temp = root->_right;
            delete root;
            root = temp;
        }
        //case 4b: left subtree.
        else
        {
            //find the largest node in the left subtree.
            root->_item = tree_remove_max(root->_left,avl);
        }
        itemRemoved = true;
    }
    //case 3: target is larger than current root
    else if (root->_item < target)
    {
        itemRemoved = tree_erase(root->_right, target,avl);
    }
    //case 2: target is smaller than current root
    else
    {
        itemRemoved = tree_erase(root->_left, target,avl);
    }

    if(root && itemRemoved)
        updateNode(root);

    if(avl)
        root = rotate(root);

    return itemRemoved;
}

//preconditions: none
//postconditions: erase rightmost node from the tree,  store the item in max_value
template <typename T>
void tree_remove_max(tree_node<T>* &root, T& max_value, bool avl)
{
    if(!root)
    {
        return;
    }
    else if(root->_right)
    {
        tree_remove_max(root->_right,max_value,avl);
        //decrement the _size member and update the height of the current root when returning.
        root->update_height();
        root->_size--;
    }
    else
    {
        max_value = root->_item;
        if(root->_left)
        {
            tree_node<T>* temp = root->_left;
            delete root;
            root = temp;
        }
        else
        {
            delete root;
            root = nullptr;
        }
    }
    if(avl)
        root = rotate(root);
}

//preconditions: none
//postconditions: wrapper function for remove_max, returns the value removed by remove max
// that is returned by reference. If this is called on an empty tree, returns
// whatever is provided by the default constructor for type T.
template <typename T>
T tree_remove_max(tree_node<T>* &root,bool avl)
{
    T itemRemoved = T();
    tree_remove_max(root,itemRemoved,avl);
    return itemRemoved;
}

//preconditions: none
//postconditions: a new tree is constructed with the contents of the tree pointed to by root.
// Recursive calls are made to tree_copy, and the leftmost leaf nodes are constructed first.
template <typename T>
tree_node<T>* tree_copy(tree_node<T>* root)
{
    return (root) ? new tree_node<T>(root->_item, tree_copy(root->_left), tree_copy(root->_right)) : nullptr;
}

//preconditions: none
//postconditions: the contents of src are copied to dest using recursive calls
// to traverse src and tree_insert to add items to dest.
template <typename T>
void tree_add(tree_node<T>* & dest, const tree_node<T>* src, bool avl)
{
    if(src)
    {
        tree_add(dest,src->_left,avl);
        tree_insert(dest, src->_item,avl);
        tree_add(dest,src->_right,avl);
    }
}

//preconditions: a must be sorted.
//postconditions: returns a tree_node pointer to a tree containing the values of
// of the recieved array. The tree_from_sorted_list function is called, which will recursively split
// the array into left and right sub arrays, (the current node's value will be set to the middle of the array.)
// the right subarray will be distributed to the right subtree, and the left subarray to the left subarray by
// making recursive calls that reduce the size and change the starting position of the arary.
template <typename T>
tree_node<T>* tree_from_sorted_list(const T* a, int size)
{
    if(size < 1)
        return nullptr;

    return new tree_node<T>(a[size/2], tree_from_sorted_list(a,size/2), tree_from_sorted_list(a+(size/2)+1,(size-1)/2));
}

//preconditions: root != null, root->_left != null,
//postconditions: Use temp pointers to save root->_left right child and the current root.
// Now set root to point to root's left subtree, set the new root's right to point to the previous root.
// Then set root->_right left to point to the previous root->_left right child.
template <typename T>
tree_node<T>* rotate_right(tree_node<T>* &root)
{
    tree_node<T>* leftChildsRight = root->_left->_right;
    tree_node<T>* oldRoot = root;
    root = root->_left;
    root->_right = oldRoot;
    root->_right->_left = leftChildsRight;
    return root;
}

//preconditions: root != null, root->_right != null,
//postconditions: Use temp pointers to save root->_right left child and the current root.
// Now set root to point to root's right subtree, set the new root's left to point to the previous root.
// Then set root->_left right to point to the previous root->_right left child.
template <typename T>
tree_node<T>* rotate_left(tree_node<T>* &root)
{
    tree_node<T>* rightChildsLeft = root->_right->_left;
    tree_node<T>* oldRoot = root;
    root = root->_right;
    root->_left = oldRoot;
    root->_left->_right = rightChildsLeft;
    return root;
}

//preconditions: none
//postconditions: decide which rotate is needed based on balance factor
// Case 1) Rotate left, then right: If the balance factor is -2 and our left child's balance factor is 1
// Case 2) Rotate right, then left: If the balance factor is 2 and our right child's balance factor is -1
// Case 3) Rotate right: If the balance factor is 2 and our right child's balance factor is NOT -1
// Case 4) Rotate left: If the balance factor is -2 and our left child's balance factor is NOT 1
template <typename T>
tree_node<T>* rotate(tree_node<T>* & root)
{
    if(root && root->_left && root->balance_factor() == -2)
    {
        //Rotate left on left child
        if(root->_left->balance_factor() == 1)
            root->_left = rotate_left(root->_left);

        //Rotate right on root only.
        root = rotate_right(root);
    }
    else if(root && root->_right&& root->balance_factor() == 2)
    {
        //Rotate right on right child
        if(root->_right->balance_factor() == -1)
            root->_right = rotate_right(root->_right);

        //Rotate left on root only.
        root = rotate_left(root);
    }

    //update the size and height of affected nodes.
    if(root)
    {
        updateNode(root->_right);
        updateNode(root->_left);
        updateNode(root);
    }

    return root;
}

//preconditions: none
//postconditions: calls update_height() and update_size() on the node if it is not null.
template <typename T>
void updateNode(tree_node<T>* & root)
{
    if(root)
    {
        root->update_height();
        root->update_size();
    }
}

#endif // BST_FUNCTIONS_H
