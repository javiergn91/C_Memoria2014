#include "relation2D.h"
#include "math.h"
#include "quadcode.h"
#include "utils.h"

Relation2D::Relation2D()
{
    elementList.clear();
    logStream = NULL;
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

	if(logStream != NULL)
	{
	  *logStream << "Latitude X degree: (min, max) - (" << minDegreeX << ", " << maxDegreeX << ")" << endl;
	  *logStream << "Longitude Y degree: (min, max) - (" << minDegreeY << ", " << maxDegreeY << ")" << endl;
	  *logStream << "Each matrix position represents " << xCellSize << " degree latitude and " << yCellSize << " degree longitude" << endl;
	  *logStream << "Matrix size: x-dimension = " << xArraySize << ", y-dimension = " << yArraySize << endl;
	}
    }
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
      cout << rand() % xArraySize << " " << rand() % yArraySize << endl;
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
