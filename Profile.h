#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Profile
{
private:


public:
	string name = "";
	int rank, kills, deaths, shots_taken, money, dd, mm, yy, cdd, cmm, cyy;
	float kd, ke;
	double xp, seconds_played;
	bool weapons[12];
	Profile(string N);
	void write();
	string getname();

};

