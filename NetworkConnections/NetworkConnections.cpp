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

using namespace std;

#define maxNodes 9
#define maxSets 10

void header(ofstream&);
void footer(ofstream&);

		//setup a structure type Node
struct node {

			//give node latitude, longitude
	int lat, lng;
};

		//setup a structure type Network
struct network {

			//give network node count and actual nodes
	int nodeCount;
	node nodes[maxNodes];
};

int main() {

			//set up input and output files
	ifstream infile("Data3.txt", ios::in);
	ofstream outfile("Output.txt", ios::out);

	network networks[maxSets];
	
			//print header section before anything
	header(outfile);

			//setup number of nodes for the first network and network counter
	int numNodes = 0;
	int counter = 0;

			//read in the number of nodes for the first network
	infile >> numNodes;
	while (numNodes > 1) {

				//give the firstwork the number of nodes
		networks[counter].nodeCount = numNodes;

		cout << networks[counter].nodeCount << endl;

				//read in lat and long for each node of the network
		for (int i = 0; i < networks[counter].nodeCount; i++) {
			infile >> networks[counter].nodes[i].lat;
			infile >> networks[counter].nodes[i].lng;

			cout << networks[counter].nodes[i].lat << " | " << networks[counter].nodes[i].lng << endl;
		}

				//increment the network counter
		counter++;

				//get number of nodes for the next network
		infile >> numNodes;
	}

			//print footer after everything
	footer(outfile);

	cin.get();

	return 0;
}

//*****************************************************************************************************
void header(ofstream &outfile) {

			// Receives � the output file
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

			// Receives � the output file
			// Task - Prints the output salutation
			// Returns - Nothing

	outfile << endl;
	outfile << setw(35) << "----------------------------------" << endl;
	outfile << setw(35) << "|      END OF PROGRAM OUTPUT     |" << endl;
	outfile << setw(35) << "----------------------------------" << endl;

	return;
}
//*****************************************************************************************************
