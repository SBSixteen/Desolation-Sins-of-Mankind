#include "Profile.h"
#define CRT_SECURE_NO_WARNINGS

Profile::Profile(string N){

	//new profile
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	name = N;
	rank = 0;
	kills = 0;
	deaths = 0;
	shots_taken = 0;
	money = 0;
	cyy = 1900 + newtime.tm_year;
	cmm = 1+ newtime.tm_mon;
	cdd = newtime.tm_mday;

	kd = 0.0f;
	ke = 0.0f;

	xp = 0;
	seconds_played = 0;

	}

void Profile::write() {

	string format = ".txt";
	string fn = name + format;
	
	ofstream file1{fn};
	file1.open(fn, ios::app);
	file1.write((char*)(this), sizeof(this));
	file1.close();
}

string Profile::getname() {

	return name;

}