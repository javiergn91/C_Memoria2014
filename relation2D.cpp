#include "relation2D.h"
#include "math.h"
#include "quadcode.h"
#include "utils.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

bool orderPoints(Point p1, Point p2)
{
  if(p1.y == p2.y)
  {
    return p1.x < p2.x;
  }
  
  return p1.y < p2.y;
}

Relation2D::Relation2D()
{
    elementList.clear();
    logStream = NULL;
    N = 1;
}

void Relation2D::AddPair(float element1, float element2)
{
    elementList.push_back(make_pair(element1, element2));
}

void Relation2D::SetCellSize(float x, float y)
{
    xCellSize = x;
    yCellSize = y;
}

void Relation2D::PreprocessPointListUniqueValue()
{
  vector<Point> newPointList;
  
  map< pair<int, int> , bool> M;
  for(int i = 0; i < (int)points.size(); i++)
  {
    if(!M[make_pair(points[i].x, points[i].y)])
    {
      newPointList.push_back(points[i]);
      M[make_pair(points[i].x, points[i].y)] = true;
    }
  }
  
  points.clear();
  for(int i = 0; i < (int)newPointList.size(); i++)
  {
    points.push_back(newPointList[i]);
  }
}

void Relation2D::DetermineArrayLimits()
{
    //cout << "Calculing array limits" << endl;

    if(elementList.size() > 0)
    {
        minDegreeX = elementList[0].first;
        minDegreeY = elementList[0].second;
        maxDegreeX = elementList[0].first;
        maxDegreeY = elementList[0].second;

        for(int i = 1; i < (int)elementList.size(); i++)
        {
            float currX = elementList[i].first;
            float currY = elementList[i].second;

            minDegreeX = min(minDegreeX, currX);
            minDegreeY = min(minDegreeY, currY);
            maxDegreeX = max(maxDegreeX, currX);
            maxDegreeY = max(maxDegreeY, currY);
        }

        xArraySize = (int)ceil((fabs(maxDegreeX - minDegreeX)) / xCellSize);
        yArraySize = (int)ceil((fabs(maxDegreeY - minDegreeY)) / yCellSize);

	int maxSize = max(xArraySize, yArraySize);
	numBits = bits(maxSize) * 2;
	
	//cout << numBits << " " << maxSize << endl;
	
	if(logStream != NULL)
	{
	  *logStream << "Latitude X degree: (min, max) - (" << minDegreeX << ", " << maxDegreeX << ")" << endl;
	  *logStream << "Longitude Y degree: (min, max) - (" << minDegreeY << ", " << maxDegreeY << ")" << endl;
	  *logStream << "Each matrix position represents " << xCellSize << " degree latitude and " << yCellSize << " degree longitude" << endl;
	  *logStream << "Matrix size: x-dimension = " << xArraySize << ", y-dimension = " << yArraySize << endl;
	}
    }
}

void Relation2D::ReadBinaryFile(const char* filename)
{
  points.clear();
  
  //cout << filename << endl;
  
  ifstream file(filename, ios::in | ios::binary);
  
  int intAux;
  long longAux;
  
  file.read((char*)&intAux, sizeof(int));
  file.read((char*)&longAux, sizeof(long int));
  
  //cout << intAux << endl;
  //cout << longAux << endl;
  
  this->N = intAux;
  
  numBits = bits(this->N) * 2;
  
  int N = intAux;
  
  file.read((char*)&intAux, sizeof(int));
  
  intAux = 0;
  for(int i = 0; i < N; i++)
  {
    while(1)
    {
      if(file.eof())
      {
	break;
      }
      
      file.read((char*)&intAux, sizeof(int));
      
      if(intAux >= 0)
      {
	points.push_back(Point(intAux - 1, i));
      }
      else
      {
	//cout << intAux << endl;
	
	break;
      }
    }
  }
  
  file.close();
}

void Relation2D::WriteBinaryFile(const char* filename)
{ 
  sort(points.begin(), points.end(), orderPoints);
  
  ofstream myFile(filename, ios::out | ios::binary);
  
  //cout << numBits << endl;
  
  int N = 1 << numBits;
 
  long numPoints = points.size();
  
  //cout << N << endl;
  
  myFile.write((char*)&N, sizeof(int));
  myFile.write((char*)&numPoints, sizeof(long int));
  
  int cnt = 0;
  int currIdx = 0;
  
  for(int i = 1; i <= N; i++)
  {
    int newList = i * (-1);
    /*
    cnt++;
    if(cnt >= 100000)
    {
      cout << newList << endl;
      cnt = 0;
    }
    */
    cout << newList << endl;
    bool bFound = false;
    myFile.write((char*)&newList, sizeof(int));
    for(int j = 0; j < points.size(); j++)
    {
      if(points[j].y == (i - 1))
      {
	//bFound = true;
	int p = points[j].x + 1;
	myFile.write((char*)&p, sizeof(int));
	//currIdx = j + 1;
      }
      else
      {
	/*
	if(bFound)
	{
	  break;
	}
	*/
      }
    }
  }
  
  //cout << "OK" << endl;
  
  myFile.close();
}

bool Relation2D::IsInRelation(int x, int y)
{
  for(int i = 0; i < (int)points.size(); i++) 
  {
    if(points[i].x == x && points[i].y == y)
    {
      return true;
    }
  }
  
  return false;
}

void Relation2D::PrintRandomTestCaseCheckPoint(int n)
{
  srand(time(NULL));
  
  while(n--)
  {
    int r = rand() % 2;
    if(r == 0)
    {
      int idx = rand() % (int)points.size();
      cout << points[idx].x << " " << points[idx].y << endl;
    }
    else
    {
      cout << rand() % N << " " << rand() % N << endl;
    }
  }
}

void Relation2D::PrintPointList()
{
  for(int i = 0; i < points.size(); i++)
  {
    cout << points[i].x << " " << points[i].y << endl;
  }
}

void Relation2D::FillTriePointsDefined(Trie* trie)
{
    numBits = bits(N) * 2;
    vector<bool> quadcode;
    
    for(int i = 0; i < (int)points.size(); i++)
    {
	BitmapWrapper bitmapWrapper;
	Utils::CreateQuadCode(points[i].x, points[i].y, &bitmapWrapper, numBits);
	
	quadcode.clear();
	for(int i = 0; i < numBits; i++)
	{
	  quadcode.push_back((bitget(bitmapWrapper.bitmap, i) == 1) ? true : false);
	}
	
	trie->AddVector(&quadcode);	
    }

    trie->CalculateNumberOfLeafsOfEachNode();
}

void Relation2D::FillTrie(Trie* trie)
{
    vector<bool> quadcode;

    numBits = 0;
    int maxSize = max(xArraySize, yArraySize);

    while(maxSize)
    {
        numBits++;
        maxSize /= 2;
    }

    if(logStream != NULL)
    {
      *logStream << "Quadcode size: " << numBits << " bits." << endl;
    }
    
    points.clear();
    
    for(int i = 0; i < (int)elementList.size(); i++)
    {
        int xPos = (int)ceil((elementList[i].first - minDegreeX) / xCellSize);
        int yPos = (int)ceil((elementList[i].second - minDegreeY) / yCellSize);;
	
	//if(xPos == 4584 && yPos == 6667)
	//{
	points.push_back(Point(xPos, yPos));
	
	//cout << "Added: " << xPos << " " << yPos << " ==> ";
	
	BitmapWrapper bitmapWrapper;
	Utils::CreateQuadCode(xPos, yPos, &bitmapWrapper, numBits * 2);
	
	quadcode.clear();
	for(int i = 0; i < numBits * 2; i++)
	{
	  quadcode.push_back((bitget(bitmapWrapper.bitmap, i) == 1) ? true : false);
	  //cout << bitget(bitmapWrapper.bitmap, i);
	}
	//cout << endl;
	
	//int a;
	//cin >> a;
	
	//cout << " (" << xPos << ", " << yPos << ")" << endl;
	
	
	//cout << xPos << " " << yPos << endl;
	
	trie->AddVector(&quadcode);
	//}	
    }

    trie->CalculateNumberOfLeafsOfEachNode();
}

void Relation2D::SetLogStream(ofstream* newStream)
{
    logStream = newStream;
}
