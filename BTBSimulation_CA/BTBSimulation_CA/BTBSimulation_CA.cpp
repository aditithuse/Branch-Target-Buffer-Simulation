// BTBSimulation_CA.cpp : Defines the entry point for the console application.
//Aditi Thuse
//Gayathri Krishnamoorthy

#include "stdafx.h"
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<sstream>

using namespace std;

int ParseInput(string filePath,int &count) {
	ifstream input;
	//int count = 0;
	input.open(filePath);
	string line;

	if (input.is_open()) {
		while (getline(input, line))
			count++;
	}
	input.close();
	return count;
}

string VerifyNextAddress(string trace) {
	int num = stoi(trace, 0, 16) + 4;
	stringstream stream;
	stream << std::hex << num;
	string result(stream.str());
	return result;
}

void main(int argc, char *argv[])
{
	int BTBSize = 0, count = 0, i = 0, j = 0;
	cout << "Enter BTB Size:\t";
	cin >> BTBSize;

	string **BTB = new string*[4];

	for (i = 0; i < BTBSize; i++)
		BTB[i] = new string[4];

	for (int i = 0; i < BTBSize; i++)
		for (int j = 0; j < 4; j++)
			BTB[i][j] = "";


	int hit = 0, miss = 0, rightPrediction = 0, wrongPrediction = 0, collisions = 0, NoOfInstructions = 0;

	int counterBTB = 0;

	string line;
	ifstream input;
	string *trace = NULL;
	bool flag = false;

	 ParseInput(argv[1],count);
	trace = new string[count];
	input.open(argv[1]);

	if (input.is_open()) {
		i = 0;
		while (getline(input, line))
		{
			trace[i] = line;
			i++;
		}
	}

	for (int i = 0; i < count; i++) {

		flag = false;

		//check whether entry is in BTB
		for (int j = 0; j < counterBTB; j++) {
			if (trace[i] == BTB[j][0]) {
				flag = true;
				int index = i;
				break;
			}
		}

		//if entry not in BTB
		if (flag == false) {
			string result = VerifyNextAddress(trace[i]);
			
			//if branch not taken
			if (trace[i + 1] == result)
				continue;

			//Branch taken
			else
			{
				miss++;
				if (counterBTB < BTBSize) {
					BTB[counterBTB][0] = trace[i];
					BTB[counterBTB][1] = trace[i + 1];
					BTB[counterBTB][2] = "Taken";
					BTB[counterBTB][3] = "00";
					counterBTB++;
				}
			}
		}
	}

	getchar();
	getchar();
}

