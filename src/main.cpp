#include <iostream>
#include <fstream>
#include <ios>
#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>
#include "json.hpp"
#include "cxxopts.hpp"
#include "config.h"

#include "jmos.hpp"

using namespace std;
using json = nlohmann::json;

//// List all the categories
stringstream categoryList(json& categories){
	stringstream output;

	output <<"\n## Categories\n\n";
	for(json::iterator it = categories.begin(); it != categories.end(); ++it){
		output<<"+ "<< jmos::p(it.value()) <<"\n";
	}

	return output;
}

// cxxopts command line argument parsing
cxxopts::ParseResult parse(int argc, char* argv[]){
	try{
		cxxopts::Options options(argv[0], " - example command line options");
		options.add_options()
			("c,category", "Category", cxxopts::value<std::string>())
			("g,game", "Game", cxxopts::value<std::string>())
			("h,help", "Print help")
		;

		options.parse_positional({"input", "output", "positional"});
		auto result = options.parse(argc, argv);

		if (result.count("help")){
			std::cout << options.help({""}) << std::endl;
			exit(0);
		}

		return result;
	}catch (const cxxopts::OptionException& e){
		std::cout << "error parsing options: " << e.what() << std::endl;
		exit(1);
	}
}

int main(int argc, char* argv[]){
	json db, /*mods,*/ categoryMasterList, categoryFilterList, gameList;
	//int unsigned jsize;
	bool categoryFilterOR;
	char categoryColumns, generalColumns;
	ifstream ifile;
	ofstream ofile;
	rude::Config config;
	stringstream output, csv;
	string category = "null", game = "null";

	auto result = parse(argc, argv);
	auto arguments = result.arguments();

	ifile.open("db.json");
	if(!ifile.is_open()){
		cout <<"Could not load database (db.json).\n";
		exit(1);
	}

	ifile >> db;
	jmos data(db);

	ifile.close();
	ifile.clear();

	config.load("config.ini");

	ifile.open("gamelist.json");
	if(!ifile.is_open()){
		cout <<"Could not load game list (gamelist.json).\n";
		exit(1);
	}

	ifile >> gameList;
	data.setGameList(gameList);

	ifile.close();
	ifile.clear();

	ofile.open("mods.md");
	if(!ofile.is_open()){
		cout <<"Could not load Markdown file.\n";
		exit(1);
	}

	config.setSection("general");

	if(result.count("game")){
		game = result["game"].as<string>();
	}else if(config.getStringValue("game")[0] != '\0'){
		game = config.getStringValue("game");
	}else{
		cout <<"A game needs to be specified in config.ini\n";
		exit(1);
	}
	data.setGame(game);

	generalColumns = config.getIntValue("columns");

	if(!jmos::jsonDictContains(gameList, game)){
		cout	<<"Unrecognized game.\n"
			<<"Make sure the game name is as shown in Nexusmods URLs "
			<<"and is configured in gameList.json\n";
		exit(1);
	}

	config.setSection("category filter");

	if(config.getStringValue("OR")[0] != '\0'){
		categoryFilterOR = config.getBoolValue("OR");
	}else{
		cout	<<"[category filter] OR not specified in config.ini\n"
			<<"Set it equal to `true` for the list of categories to be OR "
			<<"filtered or set to `false` for the list to be AND\n";
		exit(1);
	}

	if(result.count("category")){
		csv << result["category"].as<std::string>();
	}else if(config.getStringValue("category")[0] != '\0'){
		csv <<config.getStringValue("category");
	}

	while(getline(csv, category, ',')){
		categoryFilterList.push_back(category);
	}

	categoryColumns = config.getIntValue("columns");

	cout<<"JMOS - "<< jmos::p(gameList[game]["name"]) <<"\n";
	
	if(categoryFilterList.size() == 1){
		cout <<"Sorting by category: "<<categoryFilterList.begin().value();
	}else{
		cout <<"Sorting by categories: ";
		for(json::iterator it = categoryFilterList.begin(); it != categoryFilterList.end(); ++it){
			cout << jmos::p(it.value());
			if (it + 1 != categoryFilterList.end()){
				cout << ", ";
			}
		}
	}
	if(categoryFilterOR){
		cout<<" (OR)\n";
	}else{
		cout<<" (AND)\n";
	}

	//mods = data.db["Mods"];
	
	output	<<"# Skyrim\n\n"
		<<"## Mods\n\n";
	
	//output << filterCategories(data.mods, game, categoryFilterList, categoryFilterOR, categoryColumns).rdbuf();
	
	output << data.filterCategories(categoryFilterList, categoryFilterOR, categoryColumns).rdbuf();

	//output << modMasterList(data.mods, game, categoryMasterList, generalColumns).rdbuf();
	
	output << data.modMasterList(categoryMasterList, generalColumns).rdbuf();

	output << categoryList(categoryMasterList).rdbuf();

	ofile << output.rdbuf();

	cout<<"Markdown generation successful.\n";

	return 0;
}
