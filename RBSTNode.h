#ifndef RBSTNODE_H_INCLUDED
#define RBSTNODE_H_INCLUDED

#include<cstddef>///This is needed to use the NULL macro as well as the size_t integral type.

template<class T> class RBSTNode
{
    private:
        T element;
        size_t leftHeight;///This keeps track of the height level of an RBSTNode's left subtree.
        size_t rightHeight;///This keeps track of the height level of an RBSTNode's right subtree.
        size_t leftSubTreeSize;///This keeps track of how many nodes are in the left subtree of an RBSTNode.
        RBSTNode* leftChild;
        RBSTNode* rightChild;
        RBSTNode();
        RBSTNode(const T&, const size_t, const size_t, const size_t, RBSTNode* const, RBSTNode* const);
        RBSTNode(const RBSTNode&);
        ~RBSTNode();
        template<class R, const bool> friend class RBinarySearchTree;

    public:
        const T& getElement() const;
        const size_t getLeftSubTreeSize() const;
        RBSTNode* const getLeftChild() const;
        RBSTNode* const getRightChild() const;
};

template<typename T> RBSTNode<T>::RBSTNode():leftHeight(0), rightHeight(0), leftSubTreeSize(0), leftChild(NULL), rightChild(NULL){};

template<typename T> RBSTNode<T>::RBSTNode(const T& element, const size_t leftHeight, const size_t rightHeight, const size_t leftSubTreeSize, RBSTNode* const leftChild, RBSTNode* const rightChild):element(element), leftHeight(leftHeight), rightHeight(rightHeight), leftSubTreeSize(leftSubTreeSize), leftChild(leftChild), rightChild(rightChild){};

template<typename T> RBSTNode<T>::RBSTNode(const RBSTNode& argument):element(argument.element), leftHeight(argument.leftHeight), rightHeight(argument.rightHeight), leftSubTreeSize(argument.leftSubTreeSize), leftChild(NULL), rightChild(NULL){};

template<typename T> RBSTNode<T>::~RBSTNode(){};

template<typename T> const T& RBSTNode<T>::getElement() const
{
    return this->element;
}

template<typename T> const size_t RBSTNode<T>::getLeftSubTreeSize() const
{
    return this->leftSubTreeSize;
}

template<typename T> RBSTNode<T>* const RBSTNode<T>::getLeftChild() const
{
    return this->leftChild;
}

template<typename T> RBSTNode<T>* const RBSTNode<T>::getRightChild() const
{
    return this->rightChild;
}

#endif///RBSTNODE_H_INCLUDED
