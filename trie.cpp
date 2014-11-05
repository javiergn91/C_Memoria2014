#include "common.h"
#include "trie.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <queue>
#include "utils.h"
#include <algorithm>
using namespace std;

#define ONE true
#define ZERO false

#include <BitSequenceRG.h>

class QueueContainer
{
public:
  TrieNode* node;
  int height;
  
  QueueContainer(TrieNode* node, int height)
  {
    this->node = node;
    this->height = height;
  }
};

Trie::Trie()
{
    root = new TrieNode();
    numNodes = 1;
    height = 0;
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
    height = max(height, (int)v->size());
  
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
  //cout << "H: " << height << endl;
  
    vector<int> pathVector;
    vector<int> lenVector(height + 1, 0);
    vector< vector<int> > nextVector(height + 1);
    //pathVector.push_back(0);
    
    lenVector.push_back(0);
    
    queue< pair<TrieNode*, int> > *Q = new queue< pair<TrieNode*, int> >[height + 1];
    Q[0].push(make_pair(root, 0));
    
    //lenVector.push_back(0);
    int lastHRegistered = 0;
    
    while(1)
    {
      int currHeight = -1;
      TrieNode* currNode = NULL;
      
      for(int i = 0; i <= height; i++)
      {
	if(Q[i].size() > 0)
	{
	    
	  
	  currHeight = i;
	  
	  //cout << currHeight << " " << lastHRegistered << endl;
	  
	  //cout << currHeight << " " << pathVector.size() << endl;
	  
	  if(currHeight != lastHRegistered)
	  {
	    //cout << pathVector.size() << " " << height << endl;
	    
	      //lenVector.push_back(pathVector.size());
	    lenVector[currHeight] = pathVector.size();  
	    
	    lastHRegistered = currHeight;
	  }
	  
	  //cout << "currHeight: " << currHeight << endl;
	  pair<TrieNode*, int> p = Q[i].front();
	  currNode = p.first;
	  pathVector.push_back(p.second);
	  Q[i].pop();
	  break;
	}
      }
      //cout << currHeight << endl;
      if(currHeight == -1)
      {
	break;
      }
      
      TrieNode* tmpNode = currNode;
      int tmpHeight = currHeight;
      int cnt = 1;
      while(tmpNode != NULL)
      {	
	/*
	if(tmpNode->leftChild == NULL && tmpNode->rightChild == NULL)
	  break;	
	*/
	if(tmpHeight >= height)
	  break;
	
	cnt++;
	
	bool bHasNext = false;
	
	//cout << "tmpHeight: " << tmpHeight << endl;
	  if(currNode->leftChild == NULL || 
	    (currNode->rightChild != NULL && 
	     currNode->rightChild->numSubtreeLeafs > currNode->leftChild->numSubtreeLeafs)) 
	  {
	    pathVector.push_back(1);
	    
	    if(currNode->leftChild != NULL) {
	      bHasNext = true;
	      nextVector[tmpHeight].push_back(1);
	      Q[tmpHeight + 1].push(make_pair(currNode->leftChild, 0));
	    }
	      
	    currNode = currNode->rightChild;
	  } else {
	    pathVector.push_back(0);
	    
	    if(currNode->rightChild != NULL) {
	      bHasNext = true;
	      nextVector[tmpHeight].push_back(1);
	      Q[tmpHeight + 1].push(make_pair(currNode->rightChild, 1));
	    }
	
	    currNode = currNode->leftChild;
	  }
	  
	  if(!bHasNext)
	  {
	    nextVector[tmpHeight].push_back(0);
	  }
	  
	  tmpHeight++;
      }
  
      //lenVector.push_back(pathVector.size());
  
      /*
      if(lenVector.size() <= 1)
	lenVector.push_back(pathVector.size());
      else
      {
	int newDiff = pathVector.size() - lenVector[lenVector.size() - 1];
	int lastDiff = lenVector[lenVector.size() - 1] - lenVector[lenVector.size() - 2];
	
	if(newDiff != lastDiff)
	  lenVector.push_back(pathVector.size());
      }
      */
      /*
      if(lenVector.size() == 1 || (lenVector[lenVector.size() - 2] + cnt) != lenVector[lenVector.size() - 1])
      {
    	lenVector.push_back(lenVector[lenVector.size() - 1] + cnt);
      }
      */
      
    }
      //cout << "cnt: " << cnt << endl;
   /*
    for(int i = 0; i < pathVector.size(); i++) {
	cout << pathVector[i];
    }
    cout << endl;
    */
  /*
    for(int i = 0; i < lenVector.size(); i++)
    {
	cout << lenVector[i] << " ";
    }
    cout << endl;
    
    for(int i = 0; i < height; i++)
    {
	cout << "Height " << i << ": ";
	for(int j = 0; j < nextVector[i].size(); j++)
	{
	    cout << nextVector[i][j];
	}
	cout << endl;
    }
    */
   /*
    vector<int> tmpVector;
    tmpVector.push_back(0);
    int lastDiff = -1;
    for(int i = 1; i < lenVector.size(); i++)
    {
	int newDiff = lenVector[i] - lenVector[i - 1];
	if(newDiff != lastDiff)
	  tmpVector.push_back(lenVector[i]);
	lastDiff = newDiff;
    }
    
    lenVector.clear();
    for(int i = 0; i < tmpVector.size(); i++)
      lenVector.push_back(tmpVector[i]);
   */
    int pathN = pathVector.size() / 64 + 1;
    unsigned long *paths = new unsigned long[pathN];
    int *lens = new int[lenVector.size()];
    uint **nexts = new uint*[height];
    
    for(int i = 0; i < height; i++)
    {
	nexts[i] = new uint[uint_len(nextVector[i].size(), 1)];
	for(int j = 0; j < nextVector[i].size(); j++)
	{
	    if(nextVector[i][j] == 1)
	      bitset(nexts[i], j);
	    else
	      bitclean(nexts[i], j);
	}
    }
    /*
    for(int i = 0; i < nextVector.size(); i++)
    {
	for(int j = 0; j < nextVector[i].size(); j++)
	{
	    cout << nextVector[i][j];
	}
	cout << endl;
    }
    */
    for(int i = 0; i < pathN; i++)
      paths[i] = 0;
    
    int W2 = 64;
    for(int i = 0; i < (int)pathVector.size(); i++)
    {
	int idx = i / W2;
      
	paths[idx] <<= 1;

	if(pathVector[i] == ONE)
	  paths[idx] |= 1;	 
    }
    
    for(int i = 0; i < lenVector.size(); i++)
    {
	lens[i] = lenVector[i];
	//cout << lens[i] << ", ";
    }
    //cout << endl;
      
    structure->pathVec = new SPBitmap();
    structure->pathVec->bitmap = paths;
    structure->pathVec->len = pathVector.size();
    
    structure->lenVec = lens;
    
    structure->bitSequence = new BitSequence*[height];
    for(int i = 0; i < height; i++)
    {
	structure->bitSequence[i] = new BitSequenceRRR(nexts[i], nextVector[i].size());
    }
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