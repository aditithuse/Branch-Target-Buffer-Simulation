//Project_ComputerArchitecture.cpp : Defines the entry point for the console application.


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

	int num = stoi(str, 0, 16) / 8;

	return num;

}

void ZeroOne(string* trace, int count) {
	bool flag = false;
	string BTB[1024][5];

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 4; j++)
			BTB[i][j] = "";

	int hit = 0, miss = 0, rightPrediction = 0, wrongPrediction = 0, collisions = 0, NoOfInstructions = 0;

	int index = 0;
	int bufferIndex = 0;
	for (int i = 0; i < count; i++) {
		flag = false;

		//check whether entry is in BTB
		bufferIndex = MapAddress(trace[i]);
		if (trace[i] == BTB[bufferIndex][0])
		{
			flag = true;
			index = bufferIndex;
		}

		//if entry not in BTB
		if (flag == false && (i + 1) < count) {
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
			}
		}

		//entry is in BTB
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
					}

					//wrong address
					else
					{
						wrongPrediction++;
						BTB[index][1] = trace[i + 1];
					}
				}
				//trace-taken BTB-NotTaken
				else {
					wrongPrediction++;
					if (BTB[index][2] == "NotTaken") {
						BTB[index][2] = "Taken";
					}
				}
			}

			//not taken-trace BTB-Taken
			if (result == trace[i + 1]) {

				if (BTB[index][2] == "Taken") {
					wrongPrediction++;
					BTB[index][2] = "NotTaken";
				}
				else if (BTB[index][2] == "NotTaken") {
					rightPrediction++;
					BTB[index][2] = "NotTaken";
				}

			}
		}
	}

	cout << "\n\nZero_One(0,1):\nHits: " << hit;
	cout << "\nMiss: " << miss;
	cout << "\nRight Predictions: " << rightPrediction;
	cout << "\nWrong Predictions: " << wrongPrediction;
	cout << "\nCollisions: " << collisions;
	cout << "Addition: " << rightPrediction + wrongPrediction;

}

void ZeroTwoClassStateMachine(string* trace, int count) {
	bool flag = false;
	string BTB[1024][5];

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 4; j++)
			BTB[i][j] = "";

	int hit = 0, miss = 0, rightPrediction = 0, wrongPrediction = 0, collisions = 0, NoOfInstructions = 0;

	int index = 0;
	int bufferIndex = 0;
	for (int i = 0; i < count; i++) {

		flag = false;

		//check whether entry is in BTB
		bufferIndex = MapAddress(trace[i]);
		if (trace[i] == BTB[bufferIndex][0])
		{
			flag = true;
			index = bufferIndex;
		}


		//if entry not in BTB
		if (flag == false && (i + 1) < count) {
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
		}
	}

	cout << "\n\nZero Two Class State Machine:\nHits: " << hit;
	cout << "\nMiss: " << miss;
	cout << "\nRight Predictions: " << rightPrediction;
	cout << "\nWrong Predictions: " << wrongPrediction;
	cout << "\nCollisions: " << collisions;

}

void ZeroTwoSM_B(string* trace, int count) {
	bool flag = false;
	string BTB[1024][5];

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 4; j++)
			BTB[i][j] = "";

	int hit = 0, miss = 0, rightPrediction = 0, wrongPrediction = 0, collisions = 0, NoOfInstructions = 0;

	int index = 0;
	int bufferIndex = 0;
	for (int i = 0; i < count; i++) {

		flag = false;

		//check whether entry is in BTB
		bufferIndex = MapAddress(trace[i]);
		if (trace[i] == BTB[bufferIndex][0])
		{
			flag = true;
			index = bufferIndex;
		}

		//if entry not in BTB
		if (flag == false && (i + 1) < count) {
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
				BTB[bufferIndex][3] = "01";

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
						BTB[index][3] = "00";
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
		}
	}

	cout << "\n\nZeroTwo(0,2) for state machine B:\nHits: " << hit;
	cout << "\nMiss: " << miss;
	cout << "\nRight Predictions: " << rightPrediction;
	cout << "\nWrong Predictions: " << wrongPrediction;
	cout << "\nCollisions: " << collisions;
	cout << "\nAddition: " << rightPrediction + wrongPrediction;

}

void TwoWayZeroTwoClassSM(string* trace, int count) {
	bool flag = false;
	string BTB[1024][5];

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 5; j++)
			BTB[i][j] = "";

	int hit = 0, miss = 0, rightPrediction = 0, wrongPrediction = 0, collisions = 0, NoOfInstructions = 0;

	int counterBTB = 0;
	int index = 0;
	int bufferIndex = 0;
	for (int i = 0; i < count; i++) {

		flag = false;

		//check whether entry is in BTB
		bufferIndex = MapAddress(trace[i]);
		if (trace[i] == BTB[bufferIndex][0])
		{
			flag = true;
			index = bufferIndex;
		}
		if (trace[i] == BTB[bufferIndex + 512][0])
		{
			flag = true;
			index = bufferIndex + 512;
		}
		//if entry not in BTB
		if (flag == false && (i + 1) < count) {
			string result = VerifyNextAddress(trace[i]);

			//if branch not taken
			if (trace[i + 1] == result)
				continue;

			//Branch taken
			else
			{
				int ind = 0;
				miss++;
				bufferIndex = MapAddress(trace[i]);
				if (BTB[bufferIndex][0] != "" && BTB[bufferIndex + 512][0] != "") {
					collisions++;
					if (BTB[bufferIndex][4] == "0")
						ind = bufferIndex;
					if (BTB[bufferIndex + 512][4] == "0")
						ind = bufferIndex + 512;
				}
				if (BTB[bufferIndex][0] == "") {
					ind = bufferIndex;
					BTB[ind][4] = "1";
					BTB[ind + 512][4] = "0";
				}
				if (BTB[bufferIndex][0] != "" && BTB[bufferIndex + 512][0] == "") {
					ind = bufferIndex + 512;
					BTB[ind][4] = "1";
					BTB[bufferIndex][4] = "0";
				}
				BTB[ind][0] = trace[i];
				BTB[ind][1] = trace[i + 1];
				BTB[ind][2] = "Taken";
				BTB[ind][3] = "00";
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
						BTB[index][4] = "1";
						if (index < 512)
							BTB[index + 512][4] = "0";
						else
							BTB[index - 512][4] = "0";
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
					BTB[index][4] = "1";
					if (index < 512)
						BTB[index + 512][4] = "0";
					else
						BTB[index - 512][4] = "0";

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
				BTB[index][4] = "1";
				if (index < 512)
					BTB[index + 512][4] = "0";
				else
					BTB[index - 512][4] = "0";
			}
			//if (rightPrediction + wrongPrediction != hit)
			//cout << "Hello";
		}
	}

	cout << "\n\nTwo Way Zero Two(0,2) Class State Machine:\nHits: " << hit;
	cout << "\nMiss: " << miss;
	cout << "\nRight Predictions: " << rightPrediction;
	cout << "\nWrong Predictions: " << wrongPrediction;
	cout << "\nCollisions: " << collisions;
}

void main(int argc, char *argv[])
{
	int count = 0;
	string line;
	ifstream input;
	string *trace = NULL;

	input.open(argv[1]);

	if (input.is_open()) {
		while (getline(input, line))
			count++;
	}
	input.close();

	trace = new string[count];
	input.open(argv[1]);
	int i = 0;
	if (input.is_open()) {
		i = 0;
		while (getline(input, line))
		{
			trace[i] = line;
			i++;
		}
	}

	//zero One (0,1)
	//ZeroOne(trace, count);

	//zero Two class State Machine(0,2)
	//ZeroTwoClassStateMachine(trace, count);
	
	//zero Two State Machine B(0,2)
	//ZeroTwoSM_B(trace, count);

	//2 way class state machine(0,2)
	TwoWayZeroTwoClassSM(trace,count);

	getchar();
}
