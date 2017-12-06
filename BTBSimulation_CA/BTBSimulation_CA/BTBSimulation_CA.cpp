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



void main(int argc, char *argv[])
{
	int BTBSize = 0, count = 0, i = 0, j = 0;
	//cout << "Enter BTB Size:\t";
	//cin >> BTBSize;

	string BTB[256][4];

	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 4; j++)
			BTB[i][j] = "";


	int hit = 0, miss = 0, rightPrediction = 0, wrongPrediction = 0, collisions = 0, NoOfInstructions = 0;

	int counterBTB = 0;

	string line;
	ifstream input;
	string *trace = NULL;
	bool flag = false;

	//ParseInput(argv[1],count);
	//ifstream input;
	//int count = 0;
	//input.open("C://Users//Aditi//Documents//Visual Studio 2017//Projects//ConsoleApplication1//ConsoleApplication1//trace_sample.txt");
	input.open(argv[1]);
	//string line;

	if (input.is_open()) {
		while (getline(input, line))
			count++;
	}
	input.close();

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
		for (int j = 0; j < counterBTB; j++) {
			if (trace[i] == BTB[j][0]) {
				flag = true;
				index = j;
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

		if (flag == true) {
			hit++;

			string result = VerifyNextAddress(trace[i]);
			//taken
			if (result != trace[i + 1]) {

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
				else
					if (BTB[index][2] == "NotTaken" && BTB[index][3] == "11") {
						wrongPrediction++;
						BTB[index][2] = "NotTaken";
						BTB[index][3] = "10";
					}
					else
						if (BTB[index][2] == "NotTaken" && BTB[index][3] == "10") {
							wrongPrediction++;
							BTB[index][2] = "Taken";
							BTB[index][3] = "01";
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
					BTB[index][2] = " NotTaken";
					BTB[index][3] = "10";
				}
				else if (BTB[index][2] == "NotTaken") {
					rightPrediction++;
					BTB[index][2] = " NotTaken";
					BTB[index][3] = "11";
				}

			}
		}
	}

	getchar();
	getchar();
}
