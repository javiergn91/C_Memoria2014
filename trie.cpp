#include "common.h"
#include "trie.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <queue>
#include "utils.h"
using namespace std;

#define ONE true
#define ZERO false

Trie::Trie()
{
    root = new TrieNode();
    numNodes = 1;
}

Trie::~Trie()
{
    DeleteTrieFromMemory(root);
}

void Trie::DeleteTrieFromMemory(TrieNode* node)
{
    if(node == NULL)
        return;

    if(node->leftChild != NULL)
        DeleteTrieFromMemory(node->leftChild);

    if(node->rightChild != NULL)
        DeleteTrieFromMemory(node->rightChild);

    delete node;
}

void Trie::AddVector(vector<bool> *v)
{
    TrieNode* currNode = root;

    for(int i = 0; i < (int)v->size(); i++)
    {
        if(v->at(i) == false)
        {
            if(currNode->leftChild == NULL)
            {
                CreateSubtree(v, i, currNode);
                return;
            }
            else
            {
                currNode = currNode->leftChild;
            }
        }
        else if(v->at(i) == true)
        {
            if(currNode->rightChild == NULL)
            {
                CreateSubtree(v, i, currNode);
                return;
            }
            else
            {
                currNode = currNode->rightChild;
            }
        }
    }
}

void Trie::BuildPathDecomposition(QuadCodeStructure* structure)
{
    vector<bool> pathsAux;
    vector<bool> pathLensAux;
    vector<bool> pathNextsAux;
    
    uint* paths = NULL;
    uint* pathLens = NULL;
    uint* pathNexts = NULL;

    int totalSize = 0;

    queue< pair<TrieNode*, bool> > Q;
    Q.push(make_pair(root, ZERO));

    while(!Q.empty())
    {
      pair<TrieNode*, bool> curr = Q.front();
      TrieNode* currNode = curr.first;
      Q.pop();

      if(IsLeaf(currNode)) {
	pathsAux.push_back(curr.second);
	pathLensAux.push_back(ONE);
	pathNextsAux.push_back(ZERO);
	totalSize++;
	continue;
      }

      int pathLength = 0;
	
      while(currNode != NULL && !IsLeaf(currNode)) {
	pathLength++;
	pathLensAux.push_back(ZERO);
	
	bool bThereIsNext = false;
	      
	if(currNode->leftChild == NULL || (currNode->rightChild != NULL && currNode->rightChild->numSubtreeLeafs > currNode->leftChild->numSubtreeLeafs)) {
	  pathsAux.push_back(ONE);
	  if(currNode->leftChild != NULL) {
	    bThereIsNext = true;
	    
	    Q.push(make_pair(currNode->leftChild, ZERO));
	  }
	    
	  currNode = currNode->rightChild;
	} else {
	  pathsAux.push_back(ZERO);
	  if(currNode->rightChild != NULL) {
	    bThereIsNext = true;
	    Q.push(make_pair(currNode->rightChild, ONE));
	  }
	  
	  currNode = currNode->leftChild;
	}
	
	pathNextsAux.push_back(bThereIsNext);
      }
      
      totalSize += pathLength;
      pathLensAux[totalSize - 1] = ONE;
    }
    
    paths = new uint[uint_len(pathsAux.size(),1)];
    pathLens = new uint[uint_len(pathsAux.size(),1)];
    pathNexts = new uint[uint_len(pathsAux.size(),1)];

    for(int i = 0; i < (int)pathsAux.size(); i++)
    {
      if(pathsAux[i] == ONE) 
      {
	bitset(paths, i);
      }
      else
      {
	bitclean(paths, i);
      }
    }
    
    for(int i = 0; i < (int)pathLensAux.size(); i++) 
    {
      if(pathLensAux[i] == ONE) 
      {
	bitset(pathLens, i);
      }
      else
      {
	bitclean(pathLens, i);
      }
    }
    
    for(int i = 0; i < (int)pathNextsAux.size(); i++)
    {
      if(pathNextsAux[i] == ONE) 
      {
	bitset(pathNexts, i);
      }
      else
      {
	bitclean(pathNexts, i);
      }
    }
    
    structure->setPathBitmap(new SPBitmap(paths, pathsAux.size(), BITSEQ_NONE));
    structure->setPathLenBitmap(new SPBitmap(pathLens, pathLensAux.size(), BITSEQ_RRR));
    structure->setPathNextBitmap(new SPBitmap(pathNexts, pathNextsAux.size(), BITSEQ_SARRAY));
    
    //cout << "dasdsa" << endl;
    
    //pathBitmap = new SPBitmap(paths, pathsAux.size(), BITSEQ_NONE);
    //pathLenBitmap = new SPBitmap(pathLens, pathLensAux.size(), BITSEQ_RRR);
    //pathNextBitmap = new SPBitmap(pathNexts, pathNextsAux.size(), BITSEQ_SARRAY);
}

void Trie::CreateSubtree(vector<bool>* v, int startIndex, TrieNode* currNode)
{
    if((int)v->size() <= startIndex)
    {
        return;
    }

    if(v->at(startIndex) == false)
    {
        currNode->leftChild = new TrieNode(NULL, NULL);
        currNode = currNode->leftChild;
    }
    else if(v->at(startIndex) == true)
    {
        currNode->rightChild = new TrieNode(NULL, NULL);
        currNode = currNode->rightChild;
    }

    numNodes++;

    CreateSubtree(v, startIndex + 1, currNode);
}

bool Trie::IsInTrie(vector<bool>* v)
{
    TrieNode* currNode = root;

    for(int i = 0; i < (int)v->size(); i++)
    {
        if(currNode == NULL)
        {
            return false;
        }
        else
        {
            if(v->at(i) == false && currNode->leftChild != NULL)
            {
                currNode = currNode->leftChild;
            }
            else if(v->at(i) == true && currNode->rightChild != NULL)
            {
                currNode = currNode->rightChild;
            }
            else
            {
                return false;
            }
        }
    }

    return currNode != NULL && currNode->leftChild == NULL && currNode->rightChild == NULL;
}

//Returns a pointer to the node holding the last bit of the quadcode. Returns NULL if the quadcode
//is fully contained by the trie or is bigger than the height of the trie..
TrieNode* Trie::GetLastBitNode(vector<bool>* v)
{
    TrieNode* currNode = root;

    for(int i = 0; i < (int)v->size(); i++)
    {
        if(currNode == NULL)
        {
            return NULL;
        }
        else
        {
            if(v->at(i) == false && currNode->leftChild != NULL)
            {
                currNode = currNode->leftChild;
            }
            else if(v->at(i) == true && currNode->rightChild != NULL)
            {
                currNode = currNode->rightChild;
            }
            else
            {
                return NULL;
            }
        }
    }

    return currNode;
}

void Trie::CalculateNumberOfLeafsOfEachNode()
{
    root->numSubtreeLeafs = NumLeafs(root->leftChild) + NumLeafs(root->rightChild);
}

int Trie::NumLeafs(TrieNode* node)
{
    if(node == NULL)
    {
        return 0;
    }

    if(node->leftChild == NULL && node->rightChild == NULL)
    {
        return 1;
    }

    int numLeafs = 0;

    if(node->leftChild == NULL)
    {
        numLeafs = NumLeafs(node->rightChild);
    }
    else if(node->rightChild == NULL)
    {
        numLeafs = NumLeafs(node->leftChild);
    }
    else
    {
        numLeafs = NumLeafs(node->leftChild) + NumLeafs(node->rightChild);
    }

    node->numSubtreeLeafs = numLeafs;

    return numLeafs;
}

void Trie::PrintElementsContaining(vector<bool>* v)
{
    TrieNode* lastNode = GetLastBitNode(v);
    PrintElementsDFS(lastNode, v);
}

bool Trie::IsLeaf(TrieNode* node)
{
    return node->leftChild == NULL && node->rightChild == NULL;
}

void Trie::PrintElementsDFS(TrieNode* node, vector<bool>* v)
{
    if(IsLeaf(node))
    {
        PrintBoolVector(v);
        cout << endl;
    }
    else
    {
        if(node->leftChild != NULL)
        {
            v->push_back(false);
            PrintElementsDFS(node->leftChild, v);
            v->erase(v->end());
        }

        if(node->rightChild != NULL)
        {
            v->push_back(true);
            PrintElementsDFS(node->rightChild, v);
            v->erase(v->end());
        }
    }
}

void Trie::PrintBoolVector(vector<bool>* v)
{
    for(int i = 0; i < (int)v->size(); i++)
    {
        if(v->at(i) == true)
        {
            cout << "1 ";
        }
        else
        {
            cout << "0 ";
        }
    }
}