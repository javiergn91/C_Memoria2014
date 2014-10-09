#ifndef _TRIE_H_
#define _TRIE_H_

#include <stdlib.h>
#include <vector>
#include "spbitmap.h"
#include "quadcodestruct.h"
using namespace std;

class TrieNode
{
public:
    TrieNode()
    {
        this->leftChild = NULL;
        this->rightChild = NULL;
        numSubtreeLeafs = 0;
    }

    TrieNode(TrieNode* leftChild, TrieNode* rightChild)
    {
        this->leftChild = leftChild;
        this->rightChild = rightChild;
        numSubtreeLeafs = 0;
    }

    int numSubtreeLeafs;
    TrieNode* leftChild;
    TrieNode* rightChild;
};

class Trie
{
public:
    Trie();
    ~Trie();
    //The storage is not necessarily an array of bool values, but the library implementation may optimize storage so that each value is stored in a single bit.
    void AddVector(vector<bool>* v);
    void BuildPathDecomposition(QuadCodeStructure* structure);
    void CalculateNumberOfLeafsOfEachNode();
    void PrintElementsContaining(vector<bool>* v);
    void PrintBoolVector(vector<bool>* v);
    bool IsInTrie(vector<bool>* v);
    bool IsLeaf(TrieNode* node);
    int getSize() { return numNodes; };

private:
    TrieNode* root;
    void CreateSubtree(vector<bool>* v, int startIndex, TrieNode* currNode);
    int NumLeafs(TrieNode* node);
    void DeleteTrieFromMemory(TrieNode* node);
    TrieNode* GetLastBitNode(vector<bool>* v);
    void PrintElementsDFS(TrieNode* node, vector<bool>* v);

    int numNodes;
};

#endif // _TRIE_H
