// Author : Pallavi Meharia
// Solution to The Traveling Salesman (TSP) with Simulated Annealing (SA)
// Goal is to visit every city (node)

#include "iostream"
#include "fstream"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "limits.h"
#include "time.h"
using namespace std;

// Class Declaration
class TravSalesProblem 
{
	// Declaring private variables
	private:
	char distanceCost[30];
	int n;
	double *x, *y;
	double **adjacencyMatrix;
	int *currentPath, *estimatedPath, *minimumPath;
	double temperature;
	clock_t startTime;

	// Declaring public methods
	public:
	 TravSalesProblem();
	 ~TravSalesProblem();
	 void SAnnealing();
	 void inputFile();
	 void initialize();
	 void backtrackMinimumPath(int, int);
	 int randomNumberGenerator();
	 void exchange(int, int, int*);
	 int computeCost(int*);
	 void computeAlternatePath(int, int, int*);
};


// Constructor definition
 TravSalesProblem::TravSalesProblem()
 {
	temperature  =  1000000000000000000;
	startTime  =  clock();
 }

 TravSalesProblem::~TravSalesProblem()
 {
 }
// Initialize paths between source and destination
 void TravSalesProblem::initialize()
 {
	currentPath = new int[n+1];
	currentPath[0] = currentPath[n] = 0;
	for(int i = 1;i<n;i++)
		currentPath[i] = i;

	estimatedPath = new int[n+1];
	estimatedPath[0] = estimatedPath[n] = 0;
	for(int i = 1;i<n;i++)
		estimatedPath[i] = i;

	minimumPath = new int[n+1];
	minimumPath[0] = minimumPath[n] = 0;
	for(int i = 1;i<n;i++)
		minimumPath[i] = i;
 }

// Find path between nodes
 void TravSalesProblem::computeAlternatePath(int nodeOne, int nodeTwo, int *estimatedPath)
{
	int i, j, k, threshold, temp;

	if(nodeOne > nodeTwo)
	{
		temp  =  nodeOne;
		nodeOne  =  nodeTwo;
		nodeTwo  =  temp;
	}
	threshold = floor((nodeTwo-nodeOne+1)/2);
	i = nodeOne, j = nodeTwo;

	// overwrie estimatedPath with flipped values
	for(k = 0;k<threshold+1;k++)
		exchange(i++, j--, estimatedPath);
}

// Method to invoke Simulated Annealing
 void TravSalesProblem::SAnnealing()
 {
 	/* Variable declarations :
 	nodeOne : starting node 
 	nodeTwo : terminating node 

 	gain : Matrix gain
 	randomNumber : stores a random number to serve as logic check
 	acceptancesa : Simulated annealing acceptance function 
 	*/
	int nodeOne, nodeTwo, i;
	double gain, randomNumber, acceptancesa;
	inputFile(); 
	initialize();

	while(10)
	{
		i = 100;
		while(--i)
		{
			for(int i = 1;i<n;i++)    
			estimatedPath[i] = currentPath[i];  

			nodeOne = randomNumberGenerator(); // Assign source
			nodeTwo = randomNumberGenerator(); // Assign destination
			computeAlternatePath(nodeOne, nodeTwo, estimatedPath); // find trial path

			gain = computeCost(estimatedPath) - computeCost(currentPath); // compute delta
			randomNumber = (double) (rand()/ (double) RAND_MAX);

			acceptancesa = 1/(1+ pow(M_E, (gain/temperature)));
			if(acceptancesa > randomNumber)
				// assign instance of minimum path
				for(int i = 1;i<n;i++)    
					currentPath[i] = estimatedPath[i];

			if(computeCost(estimatedPath) < computeCost(minimumPath) )
			{
				// assign instance of minimum path
				for(int i = 1;i<n;i++)    
					minimumPath[i] = estimatedPath[i];
				// print computed path
				backtrackMinimumPath(nodeOne, nodeTwo);
			}
		}
		temperature*=0.999;
	}
}

// Calcuate cost of travelling from source to destination
 int TravSalesProblem::computeCost(int *path)
 {
	int cost = 0;
	for(int i = 0;i<n;i++)
		cost+= adjacencyMatrix[path[i]][path[i+1]];
	return cost;
 }

// Accept node information (coordinates + adjacency matrix) from source file
 void TravSalesProblem::inputFile()
 {
	int i, j;
	
	cin>>distanceCost;  if(strcmp(distanceCost, "non")==0)  cin>>distanceCost;
	cin>>n;
 
	x = new double[n]; y = new double[n];

	adjacencyMatrix = new double*[n];
	for(i = 0;i<n;i++)
		adjacencyMatrix[i] = new double[n]; 

	for(i = 0;i<n;i++)
	{
		cin>>x[i]>>y[i];
	}
 
	for(i = 0;i<n;i++)
	{
		for(j = 0;j<n;j++) {
			cin>>adjacencyMatrix[i][j];
		 }
	}
 }

// Generate a random node (based on the node size defined from the input file)
 int TravSalesProblem::randomNumberGenerator()
 {
	return (rand()%(n-1))+1;
 }

// swap values (nodes)	
 void TravSalesProblem::exchange(int nodeOne, int nodeTwo, int *estimatedPath)
 {
	int temp = estimatedPath[nodeOne];
	estimatedPath[nodeOne] = estimatedPath[nodeTwo];
	estimatedPath[nodeTwo] = temp;
 }

// Display path by backtracking
void TravSalesProblem::backtrackMinimumPath(int nodeOne, int nodeTwo)
 {
 	printf("Paths between node : %d and node : %d \n", nodeOne, nodeTwo);
 	
	printf("Time taken to compute Path: %.2fs\n", (double)(clock() - startTime)/CLOCKS_PER_SEC);
	cout<<"Cost computed in current iteration: "<<computeCost(minimumPath)<<endl;
	for(int i = 0;i<n;i++)
	{
		if ( i < (n-1)) {
			cout<<minimumPath[i]+1<<" -> ";
		}
		else {
			cout<<minimumPath[i]+1<<" ";cout<<endl;
		}

	}
 }

 int main()
 {
	TravSalesProblem t;
	t.SAnnealing();
	return 0;
 }
