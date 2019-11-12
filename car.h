#pragma once
#include "pch.h"
#include <string>
#include <fstream>
using namespace std;

class car
{
public:
	car(int nr, string brand, string model, float engine, double avg_cons);
	car();
	void display();
	void load(ifstream &carFile);
	float consumption();
	float consumptionWithAC(double distance);
	float totalAverageConsumption(double distance);
private:
	int Number;
	string Brand;
	string Model;
	float EngineSize;
	double AverageConsumption;
	float consumptionForDistance;
	int consumptionAC;
	int consumptionSumForAC;
};
