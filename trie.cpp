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
    vector<int> pathVector;
    vector<int> lenVector;
    vector< vector<int> > nextVector(height + 1);
    //pathVector.push_back(0);
    
    lenVector.push_back(0);
    
    queue< pair<TrieNode*, int> > *Q = new queue< pair<TrieNode*, int> >[height + 1];
    Q[0].push(make_pair(root, 0));
    
    while(1)
    {
      int currHeight = -1;
      TrieNode* currNode = NULL;
      
      for(int i = 0; i <= height; i++)
      {
	if(Q[i].size() > 0)
	{
	  
	  currHeight = i;
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
  
      if(lenVector.size() == 1 || (lenVector[lenVector.size() - 2] + cnt) != lenVector[lenVector.size() - 1])
      {
    	lenVector.push_back(lenVector[lenVector.size() - 1] + cnt);
      }
      
    }
      //cout << "cnt: " << cnt << endl;
   
    for(int i = 0; i < pathVector.size(); i++) {
	cout << pathVector[i];
    }
    cout << endl;
  
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
    
    //Q[0].push(QueueContainer(NULL, 2));
    //Q[1].push(QueueContainer(NULL, 2));
    //cout << Q[1].size() << endl;
    //vector<bool> pathsAux;
    //vector<bool> pathLensAux;
    //vector<bool> pathNextsAux;
    
    //unsigned long* paths = NULL;
    //uint* pathLens = NULL;
    //uint* pathNexts = NULL;

    //int totalSize = 0;
  
    /*
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
    
    int pathN = pathsAux.size() / 64 + 1;
    paths = new unsigned long[pathN];
    pathLens = new uint[uint_len(pathsAux.size(),1)];
    pathNexts = new uint[uint_len(pathsAux.size(),1)];

    for(int i = 0; i < pathN; i++)
      paths[i] = 0;
    
    int W2 = 64;
    //cout << pathsAux.size() << endl;
    for(int i = 0; i < (int)pathsAux.size(); i++)
    {
	//cout << (pathsAux[i] == ONE) ? 1 : 0;
      
	int idx = i / W2;
      
	paths[idx] <<= 1;

	if(pathsAux[i] == ONE)
	  paths[idx] |= 1;	 
    }
    //cout << endl;
    
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
        
    structure->pathBitmap = new SPBitmap();
    structure->pathBitmap->bitmap = paths;
    structure->pathBitmap->len = pathsAux.size();
    
    structure->pathNextBitmap = new SPBitmap();
    structure->pathNextBitmap->bitSeq = new BitSequenceRRR(pathNexts, pathNextsAux.size());
    
    structure->pathLenBitmap = new SPBitmap();
    structure->pathLenBitmap->bitSeq = new BitSequenceRRR(pathLens, pathLensAux.size());
    */
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