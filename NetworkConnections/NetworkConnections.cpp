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
//*						 signifies the end of the input data. After the initial line in a data set,   *
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

		//setup a structure type Node
struct node {

			//give node latitude, longitude
	int lat, lng;
};

		//setup a structure type Network
struct network {

			//give network node count and actual nodes
	node nodes[maxNodes];
	int nodeCount;
	int connectionCount = 0;
	int connection[maxNodes];
	double shortest = 999999;
	double distanceMatrix[maxNodes][maxNodes];
};

		//declare function names to define later
double getDistance(node, node);
void header(ofstream&);
void footer(ofstream&);

int main() {

			//set up input and output files
	ifstream infile("testData.txt", ios::in);
	ofstream outfile("Output.txt", ios::out);

	network networks[maxNets];
	
			//print header section before anything
	header(outfile);

			//setup number of nodes for the first network and network counter
	int numNodes = 0;
	int networkCount = 0;
	double totalLength = 0;

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

				//find the distance matrix and store the shortest connection
		for (int i = 0; i < networks[n].nodeCount; i++) {
			for (int j = i + 1; j < networks[n].nodeCount; j++) {

						//calculate the distance matrix
				networks[n].distanceMatrix[i][j] = getDistance(networks[n].nodes[i], networks[n].nodes[j]);
				networks[n].distanceMatrix[j][i] = getDistance(networks[n].nodes[i], networks[n].nodes[j]);
				if (networks[n].shortest > networks[n].distanceMatrix[i][j]) {

							//store the shortest connection distance
					networks[n].shortest = networks[n].distanceMatrix[i][j];
					from = i;
					to = j;
				}
				//cout << i << " to " << j << " : " << networks[n].distanceMatrix[i][j] << endl;
			}
		}

				//store the nodes that are already connected
		networks[n].connection[networks[n].connectionCount] = to;
		networks[n].connectionCount++;
		networks[n].connection[networks[n].connectionCount] = from;
		networks[n].connectionCount++;

		totalLength = networks[n].shortest + 16;

		cout << "shortest: " << networks[n].shortest+16 << endl;

		cout << "(" << networks[n].nodes[from].lat << ","
			<< networks[n].nodes[from].lng << ") connects "
			<< "(" << networks[n].nodes[to].lat << ","
			<< networks[n].nodes[to].lng << ")" << endl << endl;

				//look for the rest of the connection path
		for (int i = 0; i < networks[n].nodeCount - 2; i++) {
			double shortest = 9999;
			int nextTo;
			bool extendBack;

			for (int j = 0; j < networks[n].nodeCount ; j++) {

				if (!(find(begin(networks[n].connection), 
					end(networks[n].connection), j) != end(networks[n].connection))
					&& shortest > networks[n].distanceMatrix[from][j]) {

					shortest = networks[n].distanceMatrix[from][j];
					extendBack = false;

					//cout << "Extend From: " << networks[n].distanceMatrix[from][j] << endl;

					nextTo = j;
				}

				if (!(find(begin(networks[n].connection), 
					end(networks[n].connection), j) != end(networks[n].connection))
					&& shortest > networks[n].distanceMatrix[to][j]) {

					shortest = networks[n].distanceMatrix[to][j];
					extendBack = true;

					//cout << "Extend To: " << networks[n].distanceMatrix[to][j] << endl;

					nextTo = j;
				}

			}

			cout << "Next shortest: " << shortest + 16 << endl;

			if (extendBack) {
				cout << "(" << networks[n].nodes[to].lat << ","
					<< networks[n].nodes[to].lng << ") connects "
					<< "(" << networks[n].nodes[nextTo].lat << ","
					<< networks[n].nodes[nextTo].lng << ")" << " Extend Back" << endl;
				to = nextTo;
			}
			else {
				cout << "(" << networks[n].nodes[from].lat << ","
					<< networks[n].nodes[from].lng << ") connects "
					<< "(" << networks[n].nodes[nextTo].lat << ","
					<< networks[n].nodes[nextTo].lng << ")" << " Extend Front" << endl;
				from = nextTo;
			};

			totalLength += shortest + 16;

			networks[n].connection[networks[n].connectionCount] = nextTo;
			networks[n].connectionCount++;

			cout << endl;
		}

		cout << "Total Length: " << totalLength << endl << endl;
		cout << "================================================" << endl;
	}

			//print footer after everything
	footer(outfile);

	cin.get();

	return 0;
}

//*****************************************************************************************************
double getDistance(node A, node B) {

			// Receives – two nodes
			// Task - find the distance between two nodes
			// Returns - the distance

	return sqrt(pow(A.lat - B.lat, 2) + pow(A.lng - B.lng, 2));

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
