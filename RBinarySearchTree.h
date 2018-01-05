#ifndef RBINARYSEARCHTREE_H_INCLUDED
#define RBINARYSEARCHTREE_H_INCLUDED

#include<algorithm>///This is needed to use the max function.
#include<deque>///This is needed for the double-ended queue (deque) data structure.
#include<iostream>///This is needed to use input and output streams.
#include<new>///This is needed to use the new and delete operators for dynamic memory allocation and deallocation respectively.
#include<queue>///This is needed to use the FIFO (First-In-First-Out) queue data structure.
#include<sstream>///This is needed to use string streams.
#include<stack>///This is needed to use the LIFO (Last-In-First-Out) stack data structure.
#include<stdexcept>///This is needed to use predefined exception classes.
#include<string>///This is needed to use string objects.
#include<utility>///This is needed to use the pair structure.
#include<vector>///This is needed to use vector objects.
#include"RBSTNode.h"///This is needed to use RBSTNode objects.

template<class T, const bool AVLTree=false> class RBinarySearchTree///The AVLTree flag is used to disable/enable AVL rebalancing if set to false/true.
{
    private:
        size_t nodeCount;///This keeps track of the number of RBSTNodes in the RBinarySearchTree.
        RBSTNode<T>* root;
        void AVL(std::stack<RBSTNode<T>*>&);

    public:
        RBinarySearchTree();
        RBinarySearchTree(const RBinarySearchTree&);
        ~RBinarySearchTree();
        const size_t size() const;
        const bool empty() const;
        void insertAtRank(const size_t, const T&);
        const T elementAtRank(const size_t) const;
        const T replaceAtRank(const size_t, const T&);///This returns the element at the specified rank BEFORE replacement.
        const T removeAtRank(const size_t);///This returns the element at the specified rank that was removed.
        const std::vector<T> getElements() const;///This returns all the elements in the RBinarySearchTree in rank-order.
        const std::vector<T> clear();///This removes all the elements in the RBinarySearchTree and returns them.
        const std::pair<const size_t, const T> minimumRank() const;///This returns the lowest rank and the element at that rank.
        const std::pair<const size_t, const T> maximumRank() const;///This returns the highest rank and the element at that rank.
        const std::pair<const size_t, const T> predecessorRank(const size_t) const;///This returns the previous rank and the element at the previous rank before the specified rank.
        const std::pair<const size_t, const T> successorRank(const size_t) const;///This returns the next rank and the element at the next rank after the specified rank.
        T& operator[](const size_t);///This is used to access an element at a rank, without bounds checking, for both reading and writing.
        const T& operator[](const size_t) const;///This is used to access an element at a rank, without bounds checking, for reading only.
        T& at(const size_t);///This is used to access an element at a rank, with bounds checking, for both reading and writing.
        const T& at(const size_t) const;///This is used to access an element at a rank, with bounds checking, for reading only.
        template<typename R, const bool AVLT> friend std::istream& operator>>(std::istream&, RBinarySearchTree<R, AVLT>&);
        template<typename R, const bool AVLT> friend void outputPreOrder(std::ostream&, const RBinarySearchTree<R, AVLT>&);
        template<typename R, const bool AVLT> friend void outputInOrder(std::ostream&, const RBinarySearchTree<R, AVLT>&);
        template<typename R, const bool AVLT> friend void outputPostOrder(std::ostream&, const RBinarySearchTree<R, AVLT>&);
        template<typename R, const bool AVLT> friend void outputLevelOrder(std::ostream&, const RBinarySearchTree<R, AVLT>&);
        const bool operator<(const RBinarySearchTree&) const;
        const bool operator>(const RBinarySearchTree&) const;
        const bool operator<=(const RBinarySearchTree&) const;
        const bool operator>=(const RBinarySearchTree&) const;
        const bool operator==(const RBinarySearchTree&) const;
        const bool operator!=(const RBinarySearchTree&) const;
        const RBinarySearchTree& operator=(const RBinarySearchTree&);
};

template<typename T, const bool AVLTree> RBinarySearchTree<T, AVLTree>::RBinarySearchTree():nodeCount(0), root(NULL){};///This creates an empty RBinarySearchTree.

template<typename T, const bool AVLTree> RBinarySearchTree<T, AVLTree>::RBinarySearchTree(const RBinarySearchTree& argument):nodeCount(argument.nodeCount), root(NULL)///This creates an exact copy of the RBinarySearchTree argument by copying the data of the argument's RBSTNodes level by level.
{
    RBSTNode<T>* firstArgumentNode=argument.root;///This helps with locating the current RBSTNode whose data is to be copied into this RBinarySearchTree.
    RBSTNode<T>* secondArgumentNode=NULL;///This helps with calculating the rank of the current RBSTNode whose data is to be copied into this RBinarySearchTree.
    size_t argumentRankAccumulator=0;///This helps with calculating the rank of the potentially next RBSTNode whose data is to be copied into this RBinarySearchTree.
    std::queue<RBSTNode<T>*> argumentLevelQueue;///This helps with keeping track of the argument's RBSTNodes in level order.
    std::queue<size_t> argumentRankQueue;///This helps with keeping track of the ranks of the argument's RBSTNodes.

    RBSTNode<T>* firstCopyNode=NULL;///This helps with locating the RBSTNode which will copy the data of the RBSTNode whose data is to be copied from the argument RBinarySearchTree.
    RBSTNode<T>* secondCopyNode=NULL;///This also helps with locating the RBSTNode which will copy the data of the RBSTNode whose data is to be copied from the argument RBinarySearchTree.
    size_t copyRankAccumulator=0;///This helps with keeping track of ranks of the RBSTNodes in this RBinarySearchTree so the right RBSTNode copy can be made from the data of the RBSTNode whose data is to be copied.
    size_t copyRank=0;///This also helps with keeping track of ranks of the RBSTNodes in this RBinarySearchTree so the right RBSTNode copy can be made from the data of the RBSTNode whose data is to be copied.

    while(firstArgumentNode)///This loop will run until all levels of the argument RBinarySearchTree have been traversed.
    {
        if(!argumentRankQueue.empty())///This if statement executes to find the rank of the RBSTNode whose data is to be copied as well as the position of where to insert the next RBSTNode into this RBinarySearchTree.
        {
            secondArgumentNode=argument.root;///This will help in finding the RBSTNode whose data is to be copied into this RBinarySearchTree.
            argumentRankAccumulator=0;///This will help with finding the rank of the current RBSTNode whose data is to be copied into this RBinarySearchTree.

            secondCopyNode=this->root;///This will help with finding the next RBSTNode in this RBinarySearchTree which will copy the data of the RBSTNode whose data is to be copied from the argument RBinarySearchTree.
            copyRankAccumulator=0;///This will help with placing the next RBSTNode to be inserted in this RBinarySearchTree.

            while(argumentRankQueue.front()!=argumentRankAccumulator+secondArgumentNode->leftSubTreeSize+1)///We are searching for the rank and data of the RBSTNode whose data is to be copied as well as the position of the next RBSTNode to be inserted into this RBinarySearchTree.
            {
                firstCopyNode=secondCopyNode;

                if(argumentRankQueue.front()<argumentRankAccumulator+secondArgumentNode->leftSubTreeSize+1)///If the rank of the RBSTNode whose data is to be copied is less than the rank of the current RBSTNode being traversed, we go to the left subtrees of this RBinarySearchTree and the argument RBinarySearchTree.
                {
                    secondArgumentNode=secondArgumentNode->leftChild;
                    secondCopyNode=secondCopyNode->leftChild;
                }

                else///If the rank of the RBSTNode whose data is to be copied is less than the rank of the current RBSTNode being traversed, we go to the right subtrees of the this RBinarySearchTree and the argument RBinarySearchTree.
                {
                    argumentRankAccumulator+=secondArgumentNode->leftSubTreeSize+1;///We get the current rank of the currently traversed RBSTNode in the argument RBinarySearchTree.
                    secondArgumentNode=secondArgumentNode->rightChild;

                    if(secondCopyNode->rightChild)
                    {
                        copyRankAccumulator+=secondCopyNode->leftSubTreeSize+1;///We get the current rank of the currently traversed RBSTNode in this RBinarySearchTree.
                    }

                    secondCopyNode=secondCopyNode->rightChild;
                }
            }

            copyRank=argumentRankQueue.front();
            argumentRankQueue.pop();
        }

        if(!this->root)///These if/else statements help in adding the next RBSTNode into this RBinarySearchTree in level order.
        {
            this->root=new RBSTNode<T>(*firstArgumentNode);
        }

        else if(copyRank<copyRankAccumulator+firstCopyNode->leftSubTreeSize+1)
        {
            firstCopyNode->leftChild=new RBSTNode<T>(*firstArgumentNode);
        }

        else
        {
            firstCopyNode->rightChild=new RBSTNode<T>(*firstArgumentNode);
        }

        if(firstArgumentNode->leftChild)///These if/else statements help in setting up variables for the next iteration of traversing the argument RBinarySearchTree.
        {
            argumentRankQueue.push(argumentRankAccumulator+firstArgumentNode->leftChild->leftSubTreeSize+1);
            argumentLevelQueue.push(firstArgumentNode->leftChild);
        }

        if(firstArgumentNode->getRightChild())
        {
            argumentRankAccumulator+=firstArgumentNode->leftSubTreeSize+1;///We add the current rank of the currently traversed RBSTNode in the argument RBinarySearchTree in order to calculate the rank of the RBSTNode in the argument RBinarySearchTree that could be traversed next.
            argumentRankQueue.push(argumentRankAccumulator+firstArgumentNode->rightChild->leftSubTreeSize+1);
            argumentLevelQueue.push(firstArgumentNode->rightChild);
        }

        if(!argumentLevelQueue.empty())
        {
            firstArgumentNode=argumentLevelQueue.front();
            argumentLevelQueue.pop();
        }

        else
        {
            firstArgumentNode=NULL;
        }
    }
}

template<typename T, const bool AVLTree> RBinarySearchTree<T, AVLTree>::~RBinarySearchTree()
{
    RBSTNode<T>* destructorNode=this->root;///This will help with traversing all the RBSTNodes of this RBinarySearchTree.
    std::stack<RBSTNode<T>*> destructorStack;///This will help with removing all the RBSTNodes of this RBinarySearchTree.

    while(destructorNode)///This loop will run until all the RBSTNodes have been traversed and removed from this RBinarySearchTree. The RBSTNodes get removed in a post-order traversal-like fashion..
    {
        if(destructorNode->leftChild)///These if/else statements help with traversing the RBSTNodes in this RBinarySearchTree.
        {
            destructorStack.push(destructorNode);
            destructorNode=destructorNode->leftChild;
        }

        else if(destructorNode->rightChild)
        {
            destructorStack.push(destructorNode);
            destructorNode=destructorNode->rightChild;
        }

        if(!destructorStack.empty())///These if/else statements helps with removing all the RBSTNodes from this RBinarySearchTree.
        {
            if(destructorStack.top()->leftChild==destructorNode)
            {
                destructorStack.top()->leftChild=NULL;
            }

            else
            {
                destructorStack.top()->rightChild=NULL;
            }

            delete destructorNode;
            destructorNode=destructorStack.top();
            destructorStack.pop();
        }

        else
        {
            delete destructorNode;
            destructorNode=NULL;
        }
    }
}

template<typename T, const bool AVLTree> const size_t RBinarySearchTree<T, AVLTree>::size() const///This function simply returns the number of RBSTNodes/elements in the RBinarySearchTree.
{
    return this->nodeCount;
}

template<typename T, const bool AVLTree> const bool RBinarySearchTree<T, AVLTree>::empty() const///This function simply returns whether there are no RBSTNodes/elements in the RBinarySearchTree. Another way to check for an empty RBinarySearchTree is to check whether the pointer to the root RBSTNode is null or not.
{
    return !this->nodeCount;
}

template<typename T, const bool AVLTree> void RBinarySearchTree<T, AVLTree>::insertAtRank(const size_t elementRank, const T& element)///The first argument is the rank of the element to be inserted and the second argument is the element to be inserted.
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: Cannot insert element at zeroth rank.");
    }

    else if(elementRank>this->nodeCount+1)
    {
        throw std::out_of_range("Error: Cannot insert element at rank without previous rank.");
    }

    if(!this->root)///If the root is null then we create the first RBSTNode of this RBinarySearchTree with the root pointing to it.
    {
        this->root=new RBSTNode<T>(element, 0, 0, 0, NULL, NULL);
    }

    else///If the root is not null then we traverse to the appropriate location to insert the new RBSTNode and add it to this RBinarySearchTree.
    {
        RBSTNode<T>* firstInsertNode=NULL;
        RBSTNode<T>* secondInsertNode=this->root;
        size_t rankAccumulator=0;
        std::stack<RBSTNode<T>*> AVLStack;

        while(secondInsertNode)
        {
            firstInsertNode=secondInsertNode;
            AVLStack.push(secondInsertNode);

            if(elementRank<=rankAccumulator+secondInsertNode->leftSubTreeSize+1)
            {
                ++secondInsertNode->leftSubTreeSize;
                secondInsertNode=secondInsertNode->leftChild;
            }

            else
            {
                if(secondInsertNode->rightChild)
                {
                    rankAccumulator+=secondInsertNode->leftSubTreeSize+1;
                }

                secondInsertNode=secondInsertNode->rightChild;
            }
        }

        if(elementRank<rankAccumulator+firstInsertNode->leftSubTreeSize+1)
        {
            firstInsertNode->leftChild=new RBSTNode<T>(element, 0, 0, 0, NULL, NULL);
        }

        else
        {
            firstInsertNode->rightChild=new RBSTNode<T>(element, 0, 0, 0, NULL, NULL);
        }

        std::stack<RBSTNode<T>*> heightStack(AVLStack);

        while(!heightStack.empty())///We now update the heights of the RBSTNodes that were traversed.
        {
            if(heightStack.top()->leftChild)
            {
                heightStack.top()->leftHeight=std::max(heightStack.top()->leftChild->leftHeight, heightStack.top()->leftChild->rightHeight)+1;
            }

            else
            {
                heightStack.top()->leftHeight=0;
            }

            if(heightStack.top()->rightChild)
            {
                heightStack.top()->rightHeight=std::max(heightStack.top()->rightChild->leftHeight, heightStack.top()->rightChild->rightHeight)+1;
            }

            else
            {
                heightStack.top()->rightHeight=0;
            }

            heightStack.pop();
        }

        if(AVLTree)
        {
            this->AVL(AVLStack);
        }
    }

    ++this->nodeCount;///After inserting a new RBSTNode, we increase the number of RBSTNodes/elements in this RBinarySearchTree.
}

template<typename T, const bool AVLTree> const T RBinarySearchTree<T, AVLTree>::elementAtRank(const size_t elementRank) const///This function returns the element for the given rank argument. It throws an exception if the rank argument provided is out of bounds.
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: No element exists at zeroth rank.");
    }

    else if(elementRank>this->nodeCount)
    {
        throw std::out_of_range("Error: No element exists at rank greater than current max rank.");
    }

    RBSTNode<T>* searchNode=this->root;
    size_t rankAccumulator=0;

    while(elementRank!=rankAccumulator+searchNode->leftSubTreeSize+1)///We search for the element at the rank argument.
    {
        if(elementRank<rankAccumulator+searchNode->leftSubTreeSize+1)
        {
            searchNode=searchNode->leftChild;
        }

        else
        {
            rankAccumulator+=searchNode->leftSubTreeSize+1;
            searchNode=searchNode->rightChild;
        }
    }

    return searchNode->element;///We return the element at the rank argument.
}

template<typename T, const bool AVLTree> const T RBinarySearchTree<T, AVLTree>::replaceAtRank(const size_t elementRank, const T& element)///This function finds the current element at the first rank argument and replaces the element at that rank with the second element argument.
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: No element exists at zeroth rank to replace.");
    }

    else if(elementRank>this->nodeCount)
    {
        throw std::out_of_range("Error: No element exists to replace at rank greater than max rank.");
    }

    RBSTNode<T>* replaceNode=this->root;
    size_t rankAccumulator=0;

    while(elementRank!=rankAccumulator+replaceNode->leftSubTreeSize+1)
    {
        if(elementRank<rankAccumulator+replaceNode->leftSubTreeSize+1)
        {
            replaceNode=replaceNode->leftChild;
        }

        else
        {
            rankAccumulator+=replaceNode->leftSubTreeSize+1;
            replaceNode=replaceNode->rightChild;
        }
    }

    const T elementReplaced(replaceNode->element);
    replaceNode->element=element;

    return elementReplaced;
}

template<typename T, const bool AVLTree> const T RBinarySearchTree<T, AVLTree>::removeAtRank(const size_t elementRank)///This function simply removes the RBSTNode with the element at the argument rank from this RBinarySearchTree. An exception is thrown if the argument rank provided is out of bounds.
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: No element exists at zeroth rank to remove.");
    }

    else if(elementRank>this->nodeCount)
    {
        throw std::out_of_range("Error: No element exists to remove at rank greater than max rank.");
    }

    RBSTNode<T>* removeNode=this->root;
    size_t rankAccumulator=0;
    std::stack<RBSTNode<T>*> AVLStack;

    while(elementRank!=rankAccumulator+removeNode->leftSubTreeSize+1)///We now search for the RBSTNode to be removed with the given argument rank.
    {
        AVLStack.push(removeNode);

        if(elementRank<rankAccumulator+removeNode->leftSubTreeSize+1)
        {
            --removeNode->leftSubTreeSize;
            removeNode=removeNode->leftChild;
        }

        else
        {
            rankAccumulator+=removeNode->leftSubTreeSize+1;
            removeNode=removeNode->rightChild;
        }
    }

    if(!removeNode->leftChild)///These if/else statements cover cases that cover the actions that should be taken to appropriately remove the RBSTNode to be deleted from this RBinarySearchTree whether it has only a right child, only a left child, or has both a left and a right child.
    {
        if(removeNode==this->root)
        {
            this->root=this->root->rightChild;
        }

        else if(AVLStack.top()->leftChild==removeNode)
        {
            AVLStack.top()->leftChild=removeNode->rightChild;
        }

        else
        {
            AVLStack.top()->rightChild=removeNode->rightChild;
        }
    }

    else if(!removeNode->rightChild)
    {
        if(removeNode==this->root)
        {
            this->root=this->root->leftChild;
        }

        else if(AVLStack.top()->leftChild==removeNode)
        {
            AVLStack.top()->leftChild=removeNode->leftChild;
        }

        else
        {
            AVLStack.top()->rightChild=removeNode->leftChild;
        }
    }

    else///If the RBSTNode to be removed has two children, then we must find its successor which will take its place.
    {
        RBSTNode<T>* removeParent=NULL;
        const size_t removeLeftSubTreeSize(removeNode->leftSubTreeSize);
        std::deque<RBSTNode<T>*>removeDeque;

        if(!AVLStack.empty())
        {
            removeParent=AVLStack.top();
        }

        RBSTNode<T>* successorNode=removeNode->rightChild;

        while(successorNode->leftChild)
        {
            removeDeque.push_back(successorNode);
            --successorNode->leftSubTreeSize;
            successorNode=successorNode->leftChild;
        }

        if(successorNode!=removeNode->rightChild)
        {
            removeDeque.back()->leftChild=successorNode->rightChild;
            successorNode->rightChild=removeNode->rightChild;
        }

        if(removeNode==this->root)
        {
            this->root=successorNode;
        }

        else if(removeParent->leftChild==removeNode)
        {
            removeParent->leftChild=successorNode;
        }

        else
        {
            removeParent->rightChild=successorNode;
        }

        successorNode->leftChild=removeNode->leftChild;
        successorNode->leftSubTreeSize=removeLeftSubTreeSize;

        AVLStack.push(successorNode);

        while(!removeDeque.empty())
        {
            AVLStack.push(removeDeque.front());
            removeDeque.pop_front();
        }
    }

    const T elementRemoved(removeNode->element);
    delete removeNode;

    std::stack<RBSTNode<T>*> heightStack(AVLStack);

    while(!heightStack.empty())///We update the heights of the RBSTNodes traversed after the removal procedure.
    {
        if(heightStack.top()->leftChild)
        {
            heightStack.top()->leftHeight=std::max(heightStack.top()->leftChild->leftHeight, heightStack.top()->leftChild->rightHeight)+1;
        }

        else
        {
            heightStack.top()->leftHeight=0;
        }

        if(heightStack.top()->rightChild)
        {
            heightStack.top()->rightHeight=std::max(heightStack.top()->rightChild->leftHeight, heightStack.top()->rightChild->rightHeight)+1;
        }

        else
        {
            heightStack.top()->rightHeight=0;
        }

        heightStack.pop();
    }

    if(AVLTree)
    {
        this->AVL(AVLStack);
    }

    --this->nodeCount;

    return elementRemoved;
}

template<typename T, const bool AVLTree> const std::vector<T> RBinarySearchTree<T, AVLTree>::getElements() const///This function returns a vector of all the elements of this RBinarySearchTree in rank order.
{
    RBSTNode<T>* inOrderNode=this->root;
    std::stack<RBSTNode<T>*> inOrderStack;
    std::vector<T> elementVector;

    while(inOrderNode)///This loop will run until all the RBSTNodes of this RBinarySearchTree have been traversed in rank order.
    {
        if(inOrderNode->leftChild)
        {
            inOrderStack.push(inOrderNode);
            inOrderNode=inOrderNode->leftChild;
        }

        else
        {
            elementVector.push_back(inOrderNode->element);

            if(inOrderNode->rightChild)
            {
                inOrderStack.push(inOrderNode);
                inOrderNode=inOrderNode->rightChild;
            }

            else
            {
                while(!inOrderStack.empty())
                {
                    if(inOrderNode==inOrderStack.top()->leftChild)
                    {
                        elementVector.push_back(inOrderStack.top()->element);

                        if(inOrderStack.top()->rightChild)
                        {
                            inOrderNode=inOrderStack.top()->rightChild;

                            break;
                        }

                        else
                        {
                            inOrderNode=inOrderStack.top();
                            inOrderStack.pop();
                        }
                    }

                    else
                    {
                        inOrderNode=inOrderStack.top();
                        inOrderStack.pop();
                    }
                }

                if(inOrderStack.empty())
                {
                    inOrderNode=NULL;
                }
            }
        }
    }

    return elementVector;
}

template<typename T, const bool AVLTree> const std::vector<T> RBinarySearchTree<T, AVLTree>::clear()///This function removes all the RBSTNodes from this RBinarySearchTree and returns a vector of all the removed elements.
{
    RBSTNode<T>* clearNode=this->root;
    std::stack<RBSTNode<T>*> clearStack;
    std::vector<T> allElements;

    while(clearNode)///This loop will run until all the RBSTNodes have been traversed and removed from this RBinarySearchTree.
    {
        if(clearNode->leftChild)
        {
            clearStack.push(clearNode);
            clearNode=clearNode->leftChild;
        }

        else if(clearNode->rightChild)
        {
            clearStack.push(clearNode);
            clearNode=clearNode->rightChild;
        }

        else if(!clearStack.empty())
        {
            if(clearStack.top()->leftChild==clearNode)
            {
                clearStack.top()->leftChild=NULL;
            }

            else
            {
                clearStack.top()->rightChild=NULL;
            }

            allElements.push_back(clearNode->element);
            delete clearNode;
            clearNode=clearStack.top();
            clearStack.pop();
        }

        else
        {
            allElements.push_back(clearNode->element);
            delete clearNode;
            clearNode=NULL;
        }
    }

    this->nodeCount=0;
    this->root=NULL;

    return allElements;
}

template<typename T, const bool AVLTree> const std::pair<const size_t, const T> RBinarySearchTree<T, AVLTree>::minimumRank() const///This function returns the RBSTNode at the lowest rank's element in this RBinarySearchTree.
{
    if(!this->root)
    {
        throw std::runtime_error("Error: No element exists at minimum rank on an empty Ranked Binary Search Tree.");
    }

    RBSTNode<T>* minimumNode=this->root;

    while(minimumNode->leftChild)///We go to the left most RBSTNode in the RBinarySearchTree. It will be at rank 1 due to not having any RBSTNodes in its left sub tree.
    {
        minimumNode=minimumNode->leftChild;
    }

    return std::pair<const size_t, const T>(1, minimumNode->element);
}

template<typename T, const bool AVLTree> const std::pair<const size_t, const T> RBinarySearchTree<T, AVLTree>::maximumRank() const///This function returns the RBSTNode with the highest rank's element in this RBinarySearchTree.
{
    if(!this->root)
    {
        throw std::runtime_error("Error: No element exists at maximum rank on an empty Ranked Binary Search Tree.");
    }

    RBSTNode<T>* maximumNode=this->root;

    while(maximumNode->rightChild)///We go to the right most RBSTNode in the RBinarySearchTree. It will be at the rank equal to the size of the RBinarySearchTree since it is the last RBSTNode by the binary search property.
    {
        maximumNode=maximumNode->rightChild;
    }

    return std::pair<const size_t, const T>(this->nodeCount, maximumNode->element);
}

template<typename T, const bool AVLTree> const std::pair<const size_t, const T> RBinarySearchTree<T, AVLTree>::predecessorRank(const size_t elementRank) const///This function returns the RBSTNode's element with the rank that precedes the rank argument.
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: No element exists at zeroth rank to have a predecessor.");
    }

    else if(elementRank>this->nodeCount)
    {
        throw std::out_of_range("Error: No element exists to have a predecessor at rank greater than max rank.");
    }

    RBSTNode<T>* firstPredecessorNode=this->root;
    size_t rankAccumulator=0;
    std::stack<RBSTNode<T>*> predecessorStack;

    while(elementRank!=rankAccumulator+firstPredecessorNode->leftSubTreeSize+1)///We search for the RBSTNode at the argument rank.
    {
        predecessorStack.push(firstPredecessorNode);

        if(elementRank<rankAccumulator+firstPredecessorNode->leftSubTreeSize+1)
        {
            firstPredecessorNode=firstPredecessorNode->leftChild;
        }

        else
        {
            rankAccumulator+=firstPredecessorNode->leftSubTreeSize+1;
            firstPredecessorNode=firstPredecessorNode->rightChild;
        }
    }

    RBSTNode<T>* secondPredecessorNode=firstPredecessorNode->leftChild;

    if(secondPredecessorNode)///These if/else statements help us find the predecessor RBSTNode appropriately.
    {
        while(secondPredecessorNode->rightChild)
        {
            rankAccumulator+=secondPredecessorNode->leftSubTreeSize+1;
            secondPredecessorNode=secondPredecessorNode->rightChild;
        }
    }

    else
    {
        RBSTNode<T>* thirdPredecessorNode=firstPredecessorNode;

        while(!predecessorStack.empty()&&firstPredecessorNode==predecessorStack.top()->leftChild)
        {
            firstPredecessorNode=predecessorStack.top();
            predecessorStack.pop();
        }

        if(!predecessorStack.empty())
        {
            secondPredecessorNode=predecessorStack.top();
            rankAccumulator-=secondPredecessorNode->leftSubTreeSize+1;
        }

        else
        {
            secondPredecessorNode=thirdPredecessorNode;
        }
    }

    return std::pair<const size_t, const T>(rankAccumulator+secondPredecessorNode->leftSubTreeSize+1, secondPredecessorNode->element);
}

template<typename T, const bool AVLTree> const std::pair<const size_t, const T> RBinarySearchTree<T, AVLTree>::successorRank(const size_t elementRank) const///This function returns the RBSTNode's element with the rank that follows the rank argument.
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: No element exists at zeroth rank to have a successor.");
    }

    else if(elementRank>this->nodeCount)
    {
        throw std::out_of_range("Error: No element exists to have a successor at rank greater than max rank.");
    }

    RBSTNode<T>* firstSuccessorNode=this->root;
    size_t rankAccumulator=0;
    std::stack<RBSTNode<T>*> successorStack;

    while(elementRank!=rankAccumulator+firstSuccessorNode->leftSubTreeSize+1)///We search for the RBSTNode at the argument rank.
    {
        successorStack.push(firstSuccessorNode);

        if(elementRank<rankAccumulator+firstSuccessorNode->leftSubTreeSize+1)
        {
            firstSuccessorNode=firstSuccessorNode->leftChild;
        }

        else
        {
            rankAccumulator+=firstSuccessorNode->leftSubTreeSize+1;
            firstSuccessorNode=firstSuccessorNode->rightChild;
        }
    }

    RBSTNode<T>* secondSuccessorNode=firstSuccessorNode->rightChild;

    if(secondSuccessorNode)///These if/else statements help us find the successor RBSTNode appropriately.
    {
        rankAccumulator+=firstSuccessorNode->leftSubTreeSize+1;

        while(secondSuccessorNode->leftChild)
        {
            secondSuccessorNode=secondSuccessorNode->leftChild;
        }
    }

    else
    {
        RBSTNode<T>* thirdSuccessorNode=firstSuccessorNode;

        while(!successorStack.empty()&&firstSuccessorNode==successorStack.top()->rightChild)
        {
            firstSuccessorNode=successorStack.top();
            rankAccumulator-=firstSuccessorNode->leftSubTreeSize+1;
            successorStack.pop();
        }

        if(!successorStack.empty())
        {
            secondSuccessorNode=successorStack.top();
        }

        else
        {
            secondSuccessorNode=thirdSuccessorNode;

            while(firstSuccessorNode!=secondSuccessorNode)
            {
                rankAccumulator+=firstSuccessorNode->leftSubTreeSize+1;
                firstSuccessorNode=firstSuccessorNode->rightChild;
            }
        }
    }

    return std::pair<const size_t, const T>(rankAccumulator+secondSuccessorNode->leftSubTreeSize+1, secondSuccessorNode->element);
}

template<typename T, const bool AVLTree> T& RBinarySearchTree<T, AVLTree>::operator[](const size_t elementRank)
{
    RBSTNode<T>* searchNode=this->root;
    size_t rankAccumulator=0;

    while(elementRank!=rankAccumulator+searchNode->leftSubTreeSize+1)
    {
        if(elementRank<rankAccumulator+searchNode->leftSubTreeSize+1)
        {
            searchNode=searchNode->leftChild;
        }

        else
        {
            rankAccumulator+=searchNode->leftSubTreeSize+1;
            searchNode=searchNode->rightChild;
        }
    }

    return searchNode->element;
}

template<typename T, const bool AVLTree> const T& RBinarySearchTree<T, AVLTree>::operator[](const size_t elementRank) const
{
    RBSTNode<T>* searchNode=this->root;
    size_t rankAccumulator=0;

    while(elementRank!=rankAccumulator+searchNode->leftSubTreeSize+1)
    {
        if(elementRank<rankAccumulator+searchNode->leftSubTreeSize+1)
        {
            searchNode=searchNode->leftChild;
        }

        else
        {
            rankAccumulator+=searchNode->leftSubTreeSize+1;
            searchNode=searchNode->rightChild;
        }
    }

    return searchNode->element;
}

template<typename T, const bool AVLTree> T& RBinarySearchTree<T, AVLTree>::at(const size_t elementRank)
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: No element exists at zeroth rank.");
    }

    else if(elementRank>this->nodeCount)
    {
        throw std::out_of_range("Error: No element exists at rank greater than current max rank.");
    }

    RBSTNode<T>* searchNode=this->root;
    size_t rankAccumulator=0;

    while(elementRank!=rankAccumulator+searchNode->leftSubTreeSize+1)
    {
        if(elementRank<rankAccumulator+searchNode->leftSubTreeSize+1)
        {
            searchNode=searchNode->leftChild;
        }

        else
        {
            rankAccumulator+=searchNode->leftSubTreeSize+1;
            searchNode=searchNode->rightChild;
        }
    }

    return searchNode->element;
}

template<typename T, const bool AVLTree> const T& RBinarySearchTree<T, AVLTree>::at(const size_t elementRank) const
{
    if(!elementRank)
    {
        throw std::out_of_range("Error: No element exists at zeroth rank.");
    }

    else if(elementRank>this->nodeCount)
    {
        throw std::out_of_range("Error: No element exists at rank greater than current max rank.");
    }

    RBSTNode<T>* searchNode=this->root;
    size_t rankAccumulator=0;

    while(elementRank!=rankAccumulator+searchNode->leftSubTreeSize+1)
    {
        if(elementRank<rankAccumulator+searchNode->leftSubTreeSize+1)
        {
            searchNode=searchNode->leftChild;
        }

        else
        {
            rankAccumulator+=searchNode->leftSubTreeSize+1;
            searchNode=searchNode->rightChild;
        }
    }

    return searchNode->element;
}

template<typename R, const bool AVLT> std::istream& operator>>(std::istream& inputStream, RBinarySearchTree<R, AVLT>& RBST)///This function allows one to input [rank,value] pairs from an input stream.
{
    unsigned char token='\0';
    std::string inputStreamTokenBuffer;
    std::stringstream tokenBuffer;
    std::stringstream elementRankBuffer;
    std::stringstream elementBuffer;
    size_t elementRank=0;
    R element;

    inputStream>>inputStreamTokenBuffer;
    tokenBuffer<<inputStreamTokenBuffer;

    while(tokenBuffer>>token)
    {
        while(tokenBuffer>>token&&token!=',')
        {
            elementRankBuffer<<token;
        }

        elementRankBuffer>>elementRank;
        elementRankBuffer.str(std::string());
        elementRankBuffer.clear();

        while(tokenBuffer>>token&&token!=']')
        {
            elementBuffer<<token;
        }

        elementBuffer>>element;
        elementBuffer.str(std::string());
        elementBuffer.clear();

        RBST.insertAtRank(elementRank, element);
    }

    return inputStream;
}

template<typename R, const bool AVLT> void outputPreOrder(std::ostream& outputStream, const RBinarySearchTree<R, AVLT>& RBST)
{
    RBSTNode<R>* preOrderNode=RBST.root;
    size_t rankAccumulator=0;
    std::stack<RBSTNode<R>*> preOrderStack;

    while(preOrderNode)
    {
        outputStream<<'['<<rankAccumulator+preOrderNode->getLeftSubTreeSize()+1<<','<<preOrderNode->getElement()<<']';

        if(preOrderNode->getLeftChild())
        {
            preOrderStack.push(preOrderNode);
            preOrderNode=preOrderNode->getLeftChild();
        }

        else if(preOrderNode->getRightChild())
        {
            rankAccumulator+=preOrderNode->getLeftSubTreeSize()+1;
            preOrderStack.push(preOrderNode);
            preOrderNode=preOrderNode->getRightChild();
        }

        else
        {
            while(!preOrderStack.empty())
            {
                if(preOrderNode==preOrderStack.top()->getLeftChild())
                {
                    if(preOrderStack.top()->getRightChild())
                    {
                        rankAccumulator+=preOrderStack.top()->getLeftSubTreeSize()+1;
                        preOrderNode=preOrderStack.top()->getRightChild();

                        break;
                    }

                    else
                    {
                        preOrderNode=preOrderStack.top();
                        preOrderStack.pop();
                    }
                }

                else
                {
                    preOrderNode=preOrderStack.top();
                    rankAccumulator-=preOrderNode->getLeftSubTreeSize()+1;
                    preOrderStack.pop();
                }
            }

            if(preOrderStack.empty())
            {
                preOrderNode=NULL;
            }
        }
    }
}

template<typename R, const bool AVLT> void outputInOrder(std::ostream& outputStream, const RBinarySearchTree<R, AVLT>& RBST)
{
    RBSTNode<R>* inOrderNode=RBST.root;
    size_t rankAccumulator=0;
    std::stack<RBSTNode<R>*> inOrderStack;

    while(inOrderNode)
    {
        if(inOrderNode->getLeftChild())
        {
            inOrderStack.push(inOrderNode);
            inOrderNode=inOrderNode->getLeftChild();
        }

        else
        {
            outputStream<<'['<<rankAccumulator+inOrderNode->getLeftSubTreeSize()+1<<','<<inOrderNode->getElement()<<']';

            if(inOrderNode->getRightChild())
            {
                rankAccumulator+=inOrderNode->getLeftSubTreeSize()+1;
                inOrderStack.push(inOrderNode);
                inOrderNode=inOrderNode->getRightChild();
            }

            else
            {
                while(!inOrderStack.empty())
                {
                    if(inOrderNode==inOrderStack.top()->getLeftChild())
                    {
                        outputStream<<'['<<rankAccumulator+inOrderStack.top()->getLeftSubTreeSize()+1<<','<<inOrderStack.top()->getElement()<<']';

                        if(inOrderStack.top()->getRightChild())
                        {
                            rankAccumulator+=inOrderStack.top()->getLeftSubTreeSize()+1;
                            inOrderNode=inOrderStack.top()->getRightChild();

                            break;
                        }

                        else
                        {
                            inOrderNode=inOrderStack.top();
                            inOrderStack.pop();
                        }
                    }

                    else
                    {
                        inOrderNode=inOrderStack.top();
                        rankAccumulator-=inOrderNode->getLeftSubTreeSize()+1;
                        inOrderStack.pop();
                    }
                }

                if(inOrderStack.empty())
                {
                    inOrderNode=NULL;
                }
            }
        }
    }
}

template<typename R, const bool AVLT> void outputPostOrder(std::ostream& outputStream, const RBinarySearchTree<R, AVLT>& RBST)
{
    RBSTNode<R>* postOrderNode=RBST.root;
    size_t rankAccumulator=0;
    std::stack<RBSTNode<R>*> postOrderStack;

    while(postOrderNode)
    {
        if(postOrderNode->getLeftChild())
        {
            postOrderStack.push(postOrderNode);
            postOrderNode=postOrderNode->getLeftChild();
        }

        else if(postOrderNode->getRightChild())
        {
            rankAccumulator+=postOrderNode->getLeftSubTreeSize()+1;
            postOrderStack.push(postOrderNode);
            postOrderNode=postOrderNode->getRightChild();
        }

        else
        {
            outputStream<<'['<<rankAccumulator+postOrderNode->getLeftSubTreeSize()+1<<','<<postOrderNode->getElement()<<']';

            while(!postOrderStack.empty())
            {
                if(postOrderNode==postOrderStack.top()->getLeftChild())
                {
                    if(postOrderStack.top()->getRightChild())
                    {
                        rankAccumulator+=postOrderStack.top()->getLeftSubTreeSize()+1;
                        postOrderNode=postOrderStack.top()->getRightChild();

                        break;
                    }

                    else
                    {
                        outputStream<<'['<<rankAccumulator+postOrderStack.top()->getLeftSubTreeSize()+1<<','<<postOrderStack.top()->getElement()<<']';
                        postOrderNode=postOrderStack.top();
                        postOrderStack.pop();
                    }
                }

                else
                {
                    rankAccumulator-=postOrderStack.top()->getLeftSubTreeSize()+1;
                    outputStream<<'['<<rankAccumulator+postOrderStack.top()->getLeftSubTreeSize()+1<<','<<postOrderStack.top()->getElement()<<']';
                    postOrderNode=postOrderStack.top();
                    postOrderStack.pop();
                }
            }

            if(postOrderStack.empty())
            {
                postOrderNode=NULL;
            }
        }
    }
}

template<typename R, const bool AVLT> void outputLevelOrder(std::ostream& outputStream, const RBinarySearchTree<R, AVLT>& RBST)
{
    RBSTNode<R>* levelOrderNode=RBST.root;
    RBSTNode<R>* rankNode=NULL;
    size_t rankAccumulator=0;
    std::queue<RBSTNode<R>*> levelOrderQueue;
    std::queue<size_t> rankQueue;

    while(levelOrderNode)
    {
        if(!rankQueue.empty())
        {
            rankNode=RBST.root;
            rankAccumulator=0;

            while(rankQueue.front()!=rankAccumulator+rankNode->getLeftSubTreeSize()+1)
            {
                if(rankQueue.front()<rankAccumulator+rankNode->getLeftSubTreeSize()+1)
                {
                    rankNode=rankNode->getLeftChild();
                }

                else
                {
                    rankAccumulator+=rankNode->getLeftSubTreeSize()+1;
                    rankNode=rankNode->getRightChild();
                }
            }

            rankQueue.pop();
        }

        outputStream<<'['<<rankAccumulator+levelOrderNode->getLeftSubTreeSize()+1<<','<<levelOrderNode->getElement()<<']';

        if(levelOrderNode->getLeftChild())
        {
            rankQueue.push(rankAccumulator+levelOrderNode->getLeftChild()->getLeftSubTreeSize()+1);
            levelOrderQueue.push(levelOrderNode->getLeftChild());
        }

        if(levelOrderNode->getRightChild())
        {
            rankAccumulator+=levelOrderNode->getLeftSubTreeSize()+1;
            rankQueue.push(rankAccumulator+levelOrderNode->getRightChild()->getLeftSubTreeSize()+1);
            levelOrderQueue.push(levelOrderNode->getRightChild());
        }

        if(!levelOrderQueue.empty())
        {
            levelOrderNode=levelOrderQueue.front();
            levelOrderQueue.pop();
        }

        else
        {
            levelOrderNode=NULL;
        }
    }
}

template<typename T, const bool AVLTree> const bool RBinarySearchTree<T, AVLTree>::operator<(const RBinarySearchTree& secondArgument) const
{
    return this->getElements()<secondArgument.getElements();
}

template<typename T, const bool AVLTree> const bool RBinarySearchTree<T, AVLTree>::operator>(const RBinarySearchTree& secondArgument) const
{
    return this->getElements()>secondArgument.getElements();
}

template<typename T, const bool AVLTree> const bool RBinarySearchTree<T, AVLTree>::operator<=(const RBinarySearchTree& secondArgument) const
{
    return this->getElements()<=secondArgument.getElements();
}

template<typename T, const bool AVLTree> const bool RBinarySearchTree<T, AVLTree>::operator>=(const RBinarySearchTree& secondArgument) const
{
    return this->getElements()>=secondArgument.getElements();
}

template<typename T, const bool AVLTree> const bool RBinarySearchTree<T, AVLTree>::operator==(const RBinarySearchTree& secondArgument) const
{
    return this->getElements()==secondArgument.getElements();
}

template<typename T, const bool AVLTree> const bool RBinarySearchTree<T, AVLTree>::operator!=(const RBinarySearchTree& secondArgument) const
{
    return this->getElements()!=secondArgument.getElements();
}

template<typename T, const bool AVLTree> const RBinarySearchTree<T, AVLTree>& RBinarySearchTree<T, AVLTree>::operator=(const RBinarySearchTree& secondArgument)
{
    if(this!=&secondArgument)
    {
        this->clear();///We clear the contents of this RBinarySearchTree before assignment.

        this->nodeCount=secondArgument.nodeCount;///We are setting this RBinarySearchTree to be the same size as the right hand side RBinarySearchTree.

        RBSTNode<T>* firstArgumentNode=secondArgument.root;///This helps with locating the current RBSTNode whose data is to be copied into this RBinarySearchTree.
        RBSTNode<T>* secondArgumentNode=NULL;///This helps with calculating the rank of the current RBSTNode whose data is to be copied into this RBinarySearchTree.
        size_t secondArgumentRankAccumulator=0;///This helps with calculating the rank of the potentially next RBSTNode whose data is to be copied into this RBinarySearchTree.
        std::queue<RBSTNode<T>*> secondArgumentLevelQueue;///This helps with keeping track of the secondArgument's RBSTNodes in level order.
        std::queue<size_t> secondArgumentRankQueue;///This helps with keeping track of the ranks of the secondArgument's RBSTNodes.

        RBSTNode<T>* firstCopyNode=NULL;///This helps with locating the RBSTNode which will copy the data of the RBSTNode whose data is to be copied from the secondArgument RBinarySearchTree.
        RBSTNode<T>* secondCopyNode=NULL;///This also helps with locating the RBSTNode which will copy the data of the RBSTNode whose data is to be copied from the secondArgument RBinarySearchTree.
        size_t copyRankAccumulator=0;///This helps with keeping track of ranks of the RBSTNodes in this RBinarySearchTree so the right RBSTNode copy can be made from the data of the RBSTNode whose data is to be copied.
        size_t copyRank=0;///This also helps with keeping track of ranks of the RBSTNodes in this RBinarySearchTree so the right RBSTNode copy can be made from the data of the RBSTNode whose data is to be copied.

        while(firstArgumentNode)///This loop will run until all levels of the secondArgument RBinarySearchTree have been traversed.
        {
            if(!secondArgumentRankQueue.empty())///This if statement executes to find the rank of the RBSTNode whose data is to be copied as well as the position of where to insert the next RBSTNode into this RBinarySearchTree.
            {
                secondArgumentNode=secondArgument.root;///This will help in finding the RBSTNode whose data is to be copied into this RBinarySearchTree.
                secondArgumentRankAccumulator=0;///This will help with finding the rank of the current RBSTNode whose data is to be copied into this RBinarySearchTree.

                secondCopyNode=this->root;///This will help with finding the next RBSTNode in this RBinarySearchTree which will copy the data of the RBSTNode whose data is to be copied from the secondArgument RBinarySearchTree.
                copyRankAccumulator=0;///This will help with placing the next RBSTNode to be inserted in this RBinarySearchTree.

                while(secondArgumentRankQueue.front()!=secondArgumentRankAccumulator+secondArgumentNode->leftSubTreeSize+1)///We are searching for the rank and data of the RBSTNode whose data is to be copied as well as the position of the next RBSTNode to be inserted into this RBinarySearchTree.
                {
                    firstCopyNode=secondCopyNode;

                    if(secondArgumentRankQueue.front()<secondArgumentRankAccumulator+secondArgumentNode->leftSubTreeSize+1)///If the rank of the RBSTNode whose data is to be copied is less than the rank of the current RBSTNode being traversed, we go to the left subtrees of this RBinarySearchTree and the secondArgument RBinarySearchTree.
                    {
                        secondArgumentNode=secondArgumentNode->leftChild;
                        secondCopyNode=secondCopyNode->leftChild;
                    }

                    else///If the rank of the RBSTNode whose data is to be copied is less than the rank of the current RBSTNode being traversed, we go to the right subtrees of the this RBinarySearchTree and the secondArgument RBinarySearchTree.
                    {
                        secondArgumentRankAccumulator+=secondArgumentNode->leftSubTreeSize+1;///We get the current rank of the currently traversed RBSTNode in the secondArgument RBinarySearchTree.
                        secondArgumentNode=secondArgumentNode->rightChild;

                        if(secondCopyNode->rightChild)
                        {
                            copyRankAccumulator+=secondCopyNode->leftSubTreeSize+1;///We get the current rank of the currently traversed RBSTNode in this RBinarySearchTree.
                        }

                        secondCopyNode=secondCopyNode->rightChild;
                    }
                }

                copyRank=secondArgumentRankQueue.front();
                secondArgumentRankQueue.pop();
            }

            if(!this->root)///These if/else statements help in adding the next RBSTNode into this RBinarySearchTree in level order.
            {
                this->root=new RBSTNode<T>(*firstArgumentNode);
            }

            else if(copyRank<copyRankAccumulator+firstCopyNode->leftSubTreeSize+1)
            {
                firstCopyNode->leftChild=new RBSTNode<T>(*firstArgumentNode);
            }

            else
            {
                firstCopyNode->rightChild=new RBSTNode<T>(*firstArgumentNode);
            }

            if(firstArgumentNode->leftChild)///These if/else statements help in setting up variables for the next iteration of traversing the secondArgument RBinarySearchTree.
            {
                secondArgumentRankQueue.push(secondArgumentRankAccumulator+firstArgumentNode->leftChild->leftSubTreeSize+1);
                secondArgumentLevelQueue.push(firstArgumentNode->leftChild);
            }

            if(firstArgumentNode->getRightChild())
            {
                secondArgumentRankAccumulator+=firstArgumentNode->leftSubTreeSize+1;///We add the current rank of the currently traversed RBSTNode in the secondArgument RBinarySearchTree in order to calculate the rank of the RBSTNode in the secondArgument RBinarySearchTree that could be traversed next.
                secondArgumentRankQueue.push(secondArgumentRankAccumulator+firstArgumentNode->rightChild->leftSubTreeSize+1);
                secondArgumentLevelQueue.push(firstArgumentNode->rightChild);
            }

            if(!secondArgumentLevelQueue.empty())
            {
                firstArgumentNode=secondArgumentLevelQueue.front();
                secondArgumentLevelQueue.pop();
            }

            else
            {
                firstArgumentNode=NULL;
            }
        }
    }

    return *this;
}

template<typename T, const bool AVLTree> void RBinarySearchTree<T, AVLTree>::AVL(std::stack<RBSTNode<T>*>& AVLStack)
{
    RBSTNode<T>* firstAVLNode=NULL;
    std::stack<RBSTNode<T>*> heightStack;

    while(!AVLStack.empty())
    {
        firstAVLNode=AVLStack.top();
        AVLStack.pop();

        if(firstAVLNode->leftHeight>firstAVLNode->rightHeight&&firstAVLNode->leftHeight-firstAVLNode->rightHeight>1)
        {
            if(firstAVLNode->leftChild->leftHeight>=firstAVLNode->leftChild->rightHeight)
            {
                RBSTNode<T>* secondAVLNode=firstAVLNode->leftChild;

                firstAVLNode->leftChild=secondAVLNode->rightChild;
                secondAVLNode->rightChild=firstAVLNode;

                if(firstAVLNode->leftChild)
                {
                    firstAVLNode->leftHeight=std::max(firstAVLNode->leftChild->leftHeight, firstAVLNode->leftChild->rightHeight)+1;
                    firstAVLNode->leftSubTreeSize-=secondAVLNode->leftSubTreeSize+1;
                }

                else
                {
                    firstAVLNode->leftHeight=0;
                    firstAVLNode->leftSubTreeSize=0;
                }

                secondAVLNode->rightHeight=std::max(secondAVLNode->rightChild->leftHeight, secondAVLNode->rightChild->rightHeight)+1;

                if(firstAVLNode==this->root)
                {
                    this->root=secondAVLNode;
                }

                else if(AVLStack.top()->leftChild==firstAVLNode)
                {
                    AVLStack.top()->leftChild=secondAVLNode;
                }

                else
                {
                    AVLStack.top()->rightChild=secondAVLNode;
                }
            }

            else
            {
                RBSTNode<T>* secondAVLNode=firstAVLNode->leftChild;
                RBSTNode<T>* thirdAVLNode=secondAVLNode->rightChild;

                firstAVLNode->leftChild=thirdAVLNode->rightChild;
                thirdAVLNode->rightChild=firstAVLNode;
                secondAVLNode->rightChild=thirdAVLNode->leftChild;
                thirdAVLNode->leftChild=secondAVLNode;

                if(firstAVLNode->leftChild)
                {
                    firstAVLNode->leftHeight=std::max(firstAVLNode->leftChild->leftHeight, firstAVLNode->leftChild->rightHeight)+1;
                    firstAVLNode->leftSubTreeSize-=secondAVLNode->leftSubTreeSize+thirdAVLNode->leftSubTreeSize+2;
                }

                else
                {
                    firstAVLNode->leftHeight=0;
                    firstAVLNode->leftSubTreeSize=0;
                }

                if(secondAVLNode->rightChild)
                {
                    secondAVLNode->rightHeight=std::max(secondAVLNode->rightChild->leftHeight, secondAVLNode->rightChild->rightHeight)+1;
                }

                else
                {
                    secondAVLNode->rightHeight=0;
                }

                thirdAVLNode->leftHeight=std::max(thirdAVLNode->leftChild->leftHeight, thirdAVLNode->leftChild->rightHeight)+1;
                thirdAVLNode->leftSubTreeSize+=thirdAVLNode->leftChild->leftSubTreeSize+1;
                thirdAVLNode->rightHeight=std::max(thirdAVLNode->rightChild->leftHeight, thirdAVLNode->rightChild->rightHeight)+1;

                if(firstAVLNode==this->root)
                {
                    this->root=thirdAVLNode;
                }

                else if(AVLStack.top()->leftChild==firstAVLNode)
                {
                    AVLStack.top()->leftChild=thirdAVLNode;
                }

                else
                {
                    AVLStack.top()->rightChild=thirdAVLNode;
                }
            }
        }

        else if(firstAVLNode->rightHeight>firstAVLNode->leftHeight&&firstAVLNode->rightHeight-firstAVLNode->leftHeight>1)
        {
            if(firstAVLNode->rightChild->rightHeight>=firstAVLNode->rightChild->leftHeight)
            {
                RBSTNode<T>* secondAVLNode=firstAVLNode->rightChild;

                firstAVLNode->rightChild=secondAVLNode->leftChild;
                secondAVLNode->leftChild=firstAVLNode;

                if(firstAVLNode->rightChild)
                {
                    firstAVLNode->rightHeight=std::max(firstAVLNode->rightChild->leftHeight, firstAVLNode->rightChild->rightHeight)+1;
                }

                else
                {
                    firstAVLNode->rightHeight=0;
                }

                secondAVLNode->leftHeight=std::max(secondAVLNode->leftChild->leftHeight, secondAVLNode->leftChild->rightHeight)+1;
                secondAVLNode->leftSubTreeSize+=secondAVLNode->leftChild->leftSubTreeSize+1;

                if(firstAVLNode==this->root)
                {
                    this->root=secondAVLNode;
                }

                else if(AVLStack.top()->leftChild==firstAVLNode)
                {
                    AVLStack.top()->leftChild=secondAVLNode;
                }

                else
                {
                    AVLStack.top()->rightChild=secondAVLNode;
                }
            }

            else
            {
                RBSTNode<T>* secondAVLNode=firstAVLNode->rightChild;
                RBSTNode<T>* thirdAVLNode=secondAVLNode->leftChild;

                firstAVLNode->rightChild=thirdAVLNode->leftChild;
                thirdAVLNode->leftChild=firstAVLNode;
                secondAVLNode->leftChild=thirdAVLNode->rightChild;
                thirdAVLNode->rightChild=secondAVLNode;

                if(firstAVLNode->rightChild)
                {
                    firstAVLNode->rightHeight=std::max(firstAVLNode->rightChild->leftHeight, firstAVLNode->rightChild->rightHeight)+1;
                }

                else
                {
                    firstAVLNode->rightHeight=0;
                }

                if(secondAVLNode->leftChild)
                {
                    secondAVLNode->leftHeight=std::max(secondAVLNode->leftChild->leftHeight, secondAVLNode->leftChild->rightHeight)+1;
                    secondAVLNode->leftSubTreeSize-=thirdAVLNode->leftSubTreeSize+1;
                }

                else
                {
                    secondAVLNode->leftHeight=0;
                    secondAVLNode->leftSubTreeSize=0;
                }

                thirdAVLNode->leftHeight=std::max(thirdAVLNode->leftChild->leftHeight, thirdAVLNode->leftChild->rightHeight)+1;
                thirdAVLNode->leftSubTreeSize+=thirdAVLNode->leftChild->leftSubTreeSize+1;
                thirdAVLNode->rightHeight=std::max(thirdAVLNode->rightChild->leftHeight, thirdAVLNode->rightChild->rightHeight)+1;

                if(firstAVLNode==this->root)
                {
                    this->root=thirdAVLNode;
                }

                else if(AVLStack.top()->leftChild==firstAVLNode)
                {
                    AVLStack.top()->leftChild=thirdAVLNode;
                }

                else
                {
                    AVLStack.top()->rightChild=thirdAVLNode;
                }
            }
        }

        heightStack=AVLStack;

        while(!heightStack.empty())
        {
            if(heightStack.top()->leftChild)
            {
                heightStack.top()->leftHeight=std::max(heightStack.top()->leftChild->leftHeight, heightStack.top()->leftChild->rightHeight)+1;
            }

            else
            {
                heightStack.top()->leftHeight=0;
            }

            if(heightStack.top()->rightChild)
            {
                heightStack.top()->rightHeight=std::max(heightStack.top()->rightChild->leftHeight, heightStack.top()->rightChild->rightHeight)+1;
            }

            else
            {
                heightStack.top()->rightHeight=0;
            }

            heightStack.pop();
        }
    }
}

#endif///RBINARYSEARCHTREE_H_INCLUDED
