//************************************  PROGRAM IDENTIFICATION  ***************************************
//*                                                                                                   *
//*   PROGRAM FILE NAME: NetworkConnections.cpp           ASSIGNMENT # 3           Grade: _________   *
//*                                                                                                   *
//*   PROGRAM AUTHOR:     _________________________________________                                   *
//*                                    Hein Htet Zaw                                                  *
//*                                                                                                   *
//*   COURSE #:  CSC 40300 11                                           DUE DATE:  December 9, 2016   *
//*                                                                                                   *
//*****************************************************************************************************
//*****************************************************************************************************
//*                                                                                                   *
//* Program description: This program is to determine how the computer nodes in a network should be   *
//*                      connected to MINIMIZE the total amount of cable required to connect the      *
//*                      computer nodes. In the installation being constructed, the cabling will run  *
//*                      beneath the ground, so the amount of cable used to join 2 adjacent nodes on  *
//*                      the network will be equal to the distance between the nodes plus additional  *
//*                      16 feet of cable to provide some slack for ease of installation. The INPUT   *
//*                      FILE for this program consists of a series of "data sets". The total number  *
//*                      of sets is unknown. Each data set begins with a line consisting of a single  *
//*                      number which indicates the total number of computers in the network. Each    *
//*                      network has at least two and at most nine computers. A value below this range*
//*                      signifies the end of the input data. After the initial line in a data set,   *
//*                      each additional line gives the coordinates of each computer in the network.  *
//*                      Each set of coordinates appears on separate lines. These coordinates are     *
//*                      integers in the range 0 to 150. NO two computers are at the same location    *
//*                      and each computer is listed only once.                                       *
//*                                                                                                   *
//*  This program determines the minimum amount of cabling required to connect the computer nodes.    *
//*                                                                                                   *
//*****************************************************************************************************


#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

using namespace std;

		//define maximum numbers for nodes and networks
#define maxNodes 9
#define maxNets 10
#define slack 16

		//setup a structure type Node
struct node {

			//give node latitude, longitude
	int lat, lng;
};

		//setup a structure type Network
struct network {

			//set up variables required for each network
	node nodes[maxNodes];								//actual node information
	int nodeCount = 0;									//number of nodes in network
	int connectionCount = 0;							//number of connections in network
	int connection[maxNodes];							//actual connections from and to nodes
	double shortest = 9999;								//shortest connection
	double totalLength = 0;								//total length of cable required for network
	double connectionLength[maxNodes];					//length for each connection between nodes
	double distanceMatrix[maxNodes][maxNodes];			//distance matrix for the nodes
};

		//declare function names to define later
double getDistance(node, node);
void printConnection(ofstream&, node, node, double);
void addFront(int[maxNodes], int, int, double[maxNodes], double);
void header(ofstream&);
void footer(ofstream&);

int main() {

			//set up input and output files
	ifstream infile("Data3.txt", ios::in);
	ofstream outfile("Output.txt", ios::out);

	network networks[maxNets];
	
			//print header section before anything
	header(outfile);

			//setup number of nodes for the first network and network counter
	int numNodes = 0;
	int networkCount = 0;

			//read in the number of nodes for the first network
	infile >> numNodes;
	while (numNodes > 1) {

				//give the firstwork the number of nodes
		networks[networkCount].nodeCount = numNodes;

				//read in lat and long for each node of the network
		for (int i = 0; i < networks[networkCount].nodeCount; i++) {
			infile >> networks[networkCount].nodes[i].lat;
			infile >> networks[networkCount].nodes[i].lng;
		}

				//increment the network counter
		networkCount++;

				//get number of nodes for the next network
		infile >> numNodes;
	}

			//find the shortest path for each network
	for (int n= 0; n< networkCount; n++) {
		int from, to;

		outfile << "Network # " << n+1 << endl;

				//find the distance matrix and store the shortest connection
		for (int i = 0; i < networks[n].nodeCount; i++) {
			for (int j = i + 1; j < networks[n].nodeCount; j++) {

						//calculate the distance matrix
				networks[n].distanceMatrix[i][j] = 
					getDistance(networks[n].nodes[i], networks[n].nodes[j]);
				networks[n].distanceMatrix[j][i] = 
					getDistance(networks[n].nodes[i], networks[n].nodes[j]);

				if (networks[n].shortest > networks[n].distanceMatrix[i][j]) {

							//store the shortest connection distance
					networks[n].shortest = networks[n].distanceMatrix[i][j];
					from = i;
					to = j;
				}
			}
		}

				//store the nodes that are already connected
		networks[n].connection[networks[n].connectionCount] = from;
		networks[n].connectionCount++;
		networks[n].connection[networks[n].connectionCount] = to;
		networks[n].connectionCount++;

				//remember the shortest distance
		networks[n].connectionLength[1] = networks[n].shortest;

				//look for the rest of the connection path
		for (int i = 0; i < networks[n].nodeCount - 2; i++) {
			double nextShortest = 9999;
			int nextNode;
			bool extendBack;

					//look for which one end to connect next
			for (int j = 0; j < networks[n].nodeCount ; j++) {

						//connect from the front
						//check condition if node is already connected AND new node is the shortest
				if (!(find(begin(networks[n].connection),
					end(networks[n].connection), j) != end(networks[n].connection))
					&& nextShortest > networks[n].distanceMatrix[from][j]) {

							//store the shortest distance
					nextShortest = networks[n].distanceMatrix[from][j];

							//store next node to connect
					nextNode = j;

							//connect from the front
					extendBack = false;
				}

						//connect from the back
						//check condition if node is already connected AND new node is the shortest
				if (!(find(begin(networks[n].connection),
					end(networks[n].connection), j) != end(networks[n].connection))
					&& nextShortest > networks[n].distanceMatrix[to][j]) {

							//store the shortest distance
					nextShortest = networks[n].distanceMatrix[to][j];

							//store next node to connect
					nextNode = j;

							//connect from the back
					extendBack = true;
				}
			}

					//add a new connection that connects at the back
			if (extendBack) {

						//store the new connected node and distance
				networks[n].connection[networks[n].connectionCount] = nextNode;
				networks[n].connectionLength[networks[n].connectionCount] = nextShortest;

						//set new node as terminal node
				to = nextNode;
			}

					//add a new connection that connects at the front
			else {

						//store the new connected node and distance
				addFront(networks[n].connection, nextNode, networks[n].connectionCount, 
					networks[n].connectionLength, nextShortest);

						//set new node as terminal node
				from = nextNode;
			}

					//increment the connected nodes count for the current network
			networks[n].connectionCount++;
		}

				//tranverse the network
		for (int i = 1; i < networks[n].connectionCount; i++) {

					//print out the connections and length
			printConnection(outfile, networks[n].nodes[networks[n].connection[i - 1]], 
				networks[n].nodes[networks[n].connection[i]], networks[n].connectionLength[i]);
			
					//calculate the total distance
			networks[n].totalLength += networks[n].connectionLength[i];
		}

				//print out the total length of cable required
		outfile << "Number of feet of cable required is " << networks[n].totalLength << "." << endl;

				//print out astricks for deviding
		for (int i = 0; i < 70; i++) outfile << "*";
		outfile << endl << endl;
	}

			//print footer after everything
	footer(outfile);
	return 0;
}


//*****************************************************************************************************
double getDistance(node A, node B) {

			// Receives – two nodes
			// Task - find the distance between two nodes + slack
			// Returns - the distance

	return (sqrt(pow(A.lat - B.lat, 2) + pow(A.lng - B.lng, 2)))+slack;

}

//*****************************************************************************************************
void addFront(int nodes[maxNodes], int addNode, int connections, 
				double distances[maxNodes], double distance){

			// Receives – storages for connected nodes and distances, new node, number of connections
			// Task - add the new node to the front of the array
			// Returns - nothing

			//move every node to 1 place to the back
	for (int i = connections; i >= 0; i--) {
		nodes[i] = nodes[i-1];
		distances[i] = distances[i-1];
	}

			//add the new node at the beginning
	nodes[0] = addNode;
	distances[1] = distance;

	return;
}

//*****************************************************************************************************
void printConnection(ofstream &outfile, node A, node B, double distance) {

			// Receives – two nodes and the distance
			// Task - print out the connection between two nodes
			// Returns - nothing

			//print out the formatted output for given connection
	outfile << "Cable required to connect " 
			<< "(" << A.lat << "," << A.lng << ") to "
			<< "(" << B.lat << "," << B.lng << ") is " 
			<< fixed << setprecision(2) << distance << " feet." << endl;

	return;
}

//*****************************************************************************************************
void header(ofstream &outfile) {

			// Receives – the output file
			// Task - Prints the output preamble
			// Returns - Nothing

	outfile << setw(33) << "Hein Htet Zaw";
	outfile << setw(15) << "CSC 40300";
	outfile << setw(15) << "Section 11" << endl;
	outfile << setw(37) << "Fall 2016";
	outfile << setw(20) << "Assignment #3" << endl;
	outfile << setw(38) << "-------------------------------------------";
	outfile << setw(38) << "------------------------------------------\n\n";

	return;
}

//*****************************************************************************************************
void footer(ofstream &outfile) {

			// Receives – the output file
			// Task - Prints the output salutation
			// Returns - Nothing

	outfile << endl;
	outfile << setw(35) << "----------------------------------" << endl;
	outfile << setw(35) << "|      END OF PROGRAM OUTPUT     |" << endl;
	outfile << setw(35) << "----------------------------------" << endl;

	return;
}
//*****************************************************************************************************
