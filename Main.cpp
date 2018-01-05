#include"RBinarySearchTree.h"

int main()
{
    ///This program will show some examples of how one can use Ranked Binary Search Trees.

    std::cout<<"Welcome to the RBinarySearchTree unit tests! These tests will cover some of the ways that one can use Ranked Binary Search Trees.\n\n";

    std::cout<<"Now, three Ranked Binary Search Trees will be created. They will consist of type character, integer, and string where the tree of type integer will employ AVL rebalancing.\n\n";

    ///A Ranked Binary Search Tree where one can insert elements of type char (character).
    ///It does not employ AVL rebalancing.
    RBinarySearchTree<char> example0;

    ///A Ranked Binary Search Tree where one can insert elements of type int (integer).
    ///It does employ AVL rebalancing which will keep operations running in O(lg(n)) time.
    RBinarySearchTree<int, true> example1;

    ///A Ranked Binary Search Tree where one can insert elements of type string (a sequence of characters).
    ///It does not employ AVL rebalancing.
    RBinarySearchTree<std::string, false> example2;

    ///Now we insert respective elements into each of the examples.

    std::cout<<"Now we will insert appropriate elements into each Ranked Binary Search Tree.\n\n";

    size_t characterRank=1;

    for(short int characterElement=-127; characterElement<=128; ++characterElement)
    {
        example0.insertAtRank(characterRank, static_cast<char>(characterElement));
        ++characterRank;
    }

    size_t integerRank=1;

    for(int integerElement=-4000; integerElement<=4000; ++integerElement)
    {
        example1.insertAtRank(integerRank, integerElement);
        ++integerRank;
    }

    size_t stringRank=1;
    std::string stringElement;

    for(short int characterCounter=-127; characterCounter<=128; ++characterCounter)
    {
        stringElement.push_back(static_cast<char>(characterCounter));
        example2.insertAtRank(stringRank, stringElement);
        ++stringRank;
    }

    ///Now we display certain checks that one can do on RBinarySearchTrees.

    std::cout<<"Now we will display certain checks that one can do on Ranked Binary Search Trees.\n\n";

    std::cout<<"Checking for the size/number of elements in the integer Ranked Binary Search Tree.\n\n";

    std::cout<<"The size/number of elements in the integer Ranked Binary Search Tree is "<<example1.size()<<".\n\n";

    std::cout<<"Checking to see if the character Ranked Binary Search Tree is empty.\n\n";

    if(example0.empty())
    {
        std::cout<<"The character Ranked Binary Search Tree is empty!\n\n";
    }

    else
    {
        std::cout<<"The character Ranked Binary Search Tree is not empty!\n\n";
    }

    std::cout<<"Outputting all the elements in the string Ranked Binary Search Tree in rank order using the array subscript operator.\n\n";

    for(size_t sRank=1; sRank<=example2.size(); ++sRank)
    {
        std::cout<<example2[sRank]<<' ';
    }

    std::cout<<"\n\n";

    std::cout<<"Now we will replace the last element in the string Ranked Binary Search Tree with another element.\n\n";

    std::cout<<"Before replacement the last element is: "<<example2.elementAtRank(example2.size())<<"\n\n";

    example2.replaceAtRank(example2.size(), "The Rolling Stones!");

    std::cout<<"After replacement the last element is: "<<example2.elementAtRank(example2.size())<<"\n\n";

    std::cout<<"Now we will remove many elements from the integer Ranked Binary Search Tree.\n\n";

    std::cout<<"Before removing many elements from the integer Ranked Binary Search Tree its size is: "<<example1.size()<<".\n\n";

    for(size_t iRank=1; iRank<=example1.size(); ++iRank)
    {
        if(iRank%2==0)
        {
            example1.removeAtRank(iRank);
        }
    }

    std::cout<<"After removing many elements from the integer Ranked Binary Search Tree its size is: "<<example1.size()<<".\n\n";

    std::cout<<"Now, two more integer Ranked Binary Search Trees will be created with the contents with the first integer Ranked Binary Search Tree by copy construction and assignment.\n\n";

    ///Second integer Ranked Binary Search Tree constructed via copy constructor.
    RBinarySearchTree<int, true> example3(example1);

    ///Third integer Ranked Binary Search Tree constructed via assignment.
    RBinarySearchTree<int, true> example4;
    example4=example1;

    std::cout<<"checking if all integer ranked Binary SearchTrees are equal.\n\n";

    if(example1==example3&&example1==example4&&example3==example4)
    {
        std::cout<<"The integer ranked Binary search Trees are equal!.\n\n";
    }

    else
    {
        std::cout<<"The integer ranked Binary Search trees are not equal!\n\n";
    }

    std::cout<<"Thank you for running this program on how one could use Ranked binary Search Trees! Hope you found it useful!\n\n";

    return 0;
}
