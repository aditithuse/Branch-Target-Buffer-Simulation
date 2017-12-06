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

int ParseInput(string filePath, int &count) {
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




int MapAddress(string address) {
	string str = "";
	str += address[3];
	str += address[4];
	str += address[5];

	int num = stoi(str, 0, 16) / 4;

	return num;

}

void main(int argc, char *argv[])
{
	int BTBSize = 0, count = 0, i = 0, j = 0;
	//cout << "Enter BTB Size:\t";
	//cin >> BTBSize;

	string BTB[1024][4];

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 4; j++)
			BTB[i][j] = "";


	int hit = 0, miss = 0, rightPrediction = 0, wrongPrediction = 0, collisions = 0, NoOfInstructions = 0;

	int counterBTB = 0;

	string line;
	ifstream input;
	string *trace = NULL;
	bool flag = false;

	//input.open("C://Users//Aditi//Documents//Visual Studio 2017//Projects//ConsoleApplication1//ConsoleApplication1//trace_sample.txt");
	input.open(argv[1]);

	if (input.is_open()) {
		while (getline(input, line))
			count++;
	}
	input.close();
	int bufferIndex = 0;
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
	int index = 0;
	for (int i = 0; i < count; i++) {
		
		flag = false;

		//check whether entry is in BTB
		bufferIndex = MapAddress(trace[i]);
		if(trace[i]==BTB[bufferIndex][0])
		{
			flag = true;
			index = bufferIndex;
		}
		

		//if entry not in BTB
		if (flag == false && (i + 1)<count) {
			string result = VerifyNextAddress(trace[i]);

			//if branch not taken
			if (trace[i + 1] == result)
				continue;

			//Branch taken
			else
			{
				miss++;
				bufferIndex = MapAddress(trace[i]);
				if (BTB[bufferIndex][0] != "") {
					collisions++;
				}

				BTB[bufferIndex][0] = trace[i];
				BTB[bufferIndex][1] = trace[i + 1];
				BTB[bufferIndex][2] = "Taken";
				BTB[bufferIndex][3] = "00";
				
			}
		}

		if (flag == true) {
			
			hit++;


			string result = VerifyNextAddress(trace[i]);
			//taken
			if (result != trace[i + 1]) {

				//trace taken BTB-taken 
				//right prediction
				if (BTB[index][2] == "Taken") {

					//right address
					if (trace[i + 1] == BTB[index][1]) {
						rightPrediction++;
						BTB[index][2] = "Taken";
						BTB[index][3] = "00";
					}

					//wrong address
					else
					{
						wrongPrediction++;
						BTB[index][1] = trace[i + 1];
					}
				}
				//trace-taken BTB-NOtTaken
				else {
					wrongPrediction++;
					if (BTB[index][2] == "NotTaken" && BTB[index][3] == "11") {
						BTB[index][2] = "NotTaken";
						BTB[index][3] = "10";
					}
					else if (BTB[index][2] == "NotTaken" && BTB[index][3] == "10") {
							BTB[index][2] = "Taken";
							BTB[index][3] = "01";
					}
				}
			}

			//not taken-trace
			if (result == trace[i + 1]) {

				if (BTB[index][2] == "Taken" && BTB[index][3] == "00") {
					wrongPrediction++;
					BTB[index][2] = "Taken";
					BTB[index][3] = "01";
				}
				else if (BTB[index][2] == "Taken" && BTB[index][3] == "01") {
					wrongPrediction++;
					BTB[index][2] = "NotTaken";
					BTB[index][3] = "10";
				}
				else if (BTB[index][2] == "NotTaken") {
					rightPrediction++;
					BTB[index][2] = "NotTaken";
					BTB[index][3] = "11";
				}

			}
			if (rightPrediction + wrongPrediction != hit)
				cout << "Hello";
		}
	}

	cout << "\nHits: "<<hit;
	cout << "\nMiss: " << miss;
	cout << "\nRight Predictions: " << rightPrediction;
	cout << "\nWrong Predictions: " << wrongPrediction;
	cout << "\nCollisions: " << collisions;

	getchar();
	getchar();
}
