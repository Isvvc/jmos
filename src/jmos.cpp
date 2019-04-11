#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "json.hpp"

#include "jmos.hpp"

using namespace std;
using json = nlohmann::json;

string jmos::url(URLs type, string game){
	stringstream s;
	s << gameList[game]["id"];

	switch(type){
		case mod :	return "https://www.nexusmods.com/" + game + "/mods/";
		case image :	return "https://staticdelivery.nexusmods.com/mods/" + s.str() +"/images/";
	}
	return "";
}

// Format a json string for printing
string jmos::p(json input){
	string text = input;
	text.erase(
		remove (text.begin(), text.end(), '\"'),
		text.end());
	return (text=="null")?"":text;
}

// Check if a json list contains a given string
bool jmos::jsonListContains(json list, string value){
	for(json::iterator it = list.begin(); it != list.end(); ++it){
		if(it.value() == value){
			return true;
		}
	}
	return false;
}

// Check if a json dictionary contains a given key
bool jmos::jsonDictContains(json dict, string value){
	for(json::iterator it = dict.begin(); it != dict.end(); ++it){
		if(it.key() == value){
			return true;
		}
	}
	return false;
}

// Replaces every space in a sting with a - so it can be used as a header link in Markdown
string jmos::linkify(std::string text) {
    for(string::iterator it = text.begin(); it != text.end(); ++it) {
        if(*it == ' ') {
            *it = '-';
        }
    }
    return text;
}

jmos::jmos(json& input){
	db = input;
	mods = db["Mods"];
}

void jmos::setGame(std::string& input){
	game = input;
}

void jmos::setGameList(json& input){
	gameList = input;
}

void jmos::filter::setList(std::stringstream& input, bool print){
	string value;

	while(getline(input, value, ',')){
		list.push_back(value);
	}

	if(print){
		if(list.size() == 1){
			cout <<"Sorting by category: "<<list.begin().value();
		}else{
			cout <<"Sorting by categories: ";
			for(json::iterator it = list.begin(); it != list.end(); ++it){
				cout << jmos::p(it.value());
				if (it + 1 != list.end()){
					cout << ", ";
				}
			}
		}
		if(OR){
			cout<<" (OR)\n";
		}else{
			cout<<" (AND)\n";
		}
	}
}

//// Output a filtered list of all the mods from a given category
stringstream jmos::filterCategories(){
	stringstream output, line1, line2;
	char col = 0;

	if(category.list.size() == 1){
		output <<"### Category: "<<category.list.begin().value();
	}else{
		output <<"### Categories: ";
		for(json::iterator it = category.list.begin(); it != category.list.end(); ++it){
			output << p(it.value());
			if (it + 1 != category.list.end()){
				output << ", ";
			}
		}
	}
	output<<"\n\n";

	if(category.OR){
		output <<"All mods that contain any of the listed categories.\n\n";
	}else{
		output <<"All mods that contain all of the listed categories.\n\n";
	}

	line1	<<"|";
	line2	<<"|";
	for(char i = 0; i < category.columns; i++){
		line1 <<"   |";
		line2 <<"---|";
	}
	output	<< line1.rdbuf() << "\n" << line2.rdbuf() << "\n";

	line1.str(string());
	line2.str(string());

	for(json::iterator it = mods.begin(); it != mods.end(); ++it){
		bool valid = category.OR?false:true;

		for(json::iterator jt = category.list.begin(); jt != category.list.end(); jt++){
			if(jsonListContains(it.value()["categories"], jt.value())){
				if(category.OR){
					valid = true;
					break;
				}
			}else{
				if(!category.OR){
					valid = false;
					break;
				}
			}
		}

		if(valid){
			line1<<"| ["<<it.key()<<"](#"<<linkify(it.key())<<") ";
			line2<<"| ![]("<< p(it.value()["main image"]) <<") ";
			col++;
			if(col > category.columns - 1){
				col = 0;
				output	<< line1.rdbuf() << "\n" << line2.rdbuf() << "\n";
				line1.str(string());
				line2.str(string());
			}
		}
	}
	if(col > 0){
		output	<< line1.rdbuf() << "\n" << line2.rdbuf() << "\n";
	}

	return output;
}

//// Output a masterlist of all mods
stringstream jmos::modMasterList(){
	stringstream output;

	output <<"### Mod master list\n\n";
	for(json::iterator it = mods.begin(); it != mods.end(); ++it){
		stringstream line1, line2;
		char col = 0;

		if(!it.value()["id"][game].is_null()){
			output	<<"\n#### "<<it.key()<<"\n\n"
				<< p(it.value()["description"]) <<"\n\n";
			
			for(json::iterator jt = it.value()["id"].begin();
					jt != it.value()["id"].end();
					++jt){
				output	<<"["<< p(gameList[jt.key()]["name"]) <<" Nexus link]"
					<<"(" << p(url(mod, jt.key())) << jt.value() << ")\n\n";
			}

			line1	<<"| Images | ![]("<< p(it.value()["main image"]) <<") |";
			line2	<<"| ------ |:---:|";
			for(char i = 0; i < (general.columns - 2); i++){
				line1 <<"   |";
				line2 <<"---|";
			}

			output	<< line1.rdbuf() << "\n" << line2.rdbuf() << "\n";

			for(json::iterator jt = it.value()["images"].begin();
					jt != it.value()["images"].end();
					++jt){
				for(json::iterator kt = it.value()["images"][jt.key()].begin();
						kt != it.value()["images"][jt.key()].end();
						++kt, col = (col == general.columns - 1) ? 0 : col + 1 ){
					output<<"| ![]("<< p(url(image, jt.key())) << p(kt.value()) <<") ";
					if(col == general.columns - 1){
						output<<" |\n";
					}
				}
			}

			output<<"\n\nCategories:\n\n";
			for(json::iterator jt = it.value()["categories"].begin();
					jt != it.value()["categories"].end();
					++jt){
				output<<"+ "<<p(jt.value())<<"\n";
				if(!jsonListContains(categories, jt.value())){
					categories.push_back(jt.value());
				}
			}
		}
	}

	return output;
}

//// List all the categories
stringstream jmos::categoryList(){
	stringstream output;

	output <<"\n## Categories\n\n";
	for(json::iterator it = categories.begin(); it != categories.end(); ++it){
		output<<"+ "<< p(it.value()) <<"\n";
	}

	return output;
}

