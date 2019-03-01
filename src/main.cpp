#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

string p(json input){
	string text = input;
	text.erase(
		remove (text.begin(), text.end(), '\"'),
		text.end());
	return (text=="null")?"":text;
}

int main(){
	json db, mods, categories;
	int unsigned jsize;
	ifstream ifile;
	ofstream ofile;
	json tmp;

	ifile.open("db.json");
	if(!ifile.is_open()){
		cout <<"Could not load database.\n";
		exit(1);
	}


	ofile.open("mods.md");
	if(!ofile.is_open()){
		cout <<"Could not load Markdown file.\n";
		exit(1);
	}

	ifile>>db;
	mods		= db["Mods"];
	categories	= db["Categories"];

	ofile	<<"# Skyrim\n\n"
		<<"### Mod list\n\n";

	for(json::iterator it = mods.begin(); it != mods.end(); ++it){
		ofile	<<"\n#### "<<it.key()<<"\n\n"
			<< p(it.value()["description"]) <<"\n\n"
			<<"[Nexus link](https://www.nexusmods.com/skyrim/mods/" << it.value()["id"] << ")\n\n"
			<<"| Images | ![]("<< "https://staticdelivery.nexusmods.com/mods/110/images/" << p(it.value()["main image"]) <<") |\n"
			<<"| --- |:---:|\n";
		{	bool right = false;
			for(json::iterator jt = it.value()["images"].begin();
				jt != it.value()["images"].end();
				++jt, right=!right){
				ofile<<"| ![]("<< "https://staticdelivery.nexusmods.com/mods/110/images/" << p(jt.value()) <<")";
				if(right){
					ofile<<" |\n";
				}
			}
		}
		ofile<<"\n\nCategories:\n\n";
		for(json::iterator jt = it.value()["categories"].begin();
			jt != it.value()["categories"].end();
			++jt){
			ofile<<"+ "<<p(jt.value())<<"\n";
		}
	}

	cout<<"Markdown generation successful.\n";

	return 0;
}
