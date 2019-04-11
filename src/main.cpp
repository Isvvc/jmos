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
	json jsonInput;
	ifstream ifile;
	ofstream ofile;
	rude::Config config;
	stringstream output, ssInput;
	string game;

	auto result = parse(argc, argv);
	auto arguments = result.arguments();

	ifile.open("db.json");
	if(!ifile.is_open()){
		cout <<"Could not load database (db.json).\n";
		exit(1);
	}

	ifile >> jsonInput;
	jmos data(jsonInput);
	
	jsonInput.clear();
	ifile.close();
	ifile.clear();

	config.load("config.ini");

	ifile.open("gamelist.json");
	if(!ifile.is_open()){
		cout <<"Could not load game list (gamelist.json).\n";
		exit(1);
	}

	ifile >> jsonInput;
	data.setGameList(jsonInput);

	jsonInput.clear();
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

	data.general.columns = config.getIntValue("columns");

	if(!data.checkGame()){
		cout	<<"Unrecognized game.\n"
			<<"Make sure the game name is as shown in Nexusmods URLs "
			<<"and is configured in gameList.json\n";
		exit(1);
	}

	config.setSection("category filter");

	if(config.getStringValue("OR")[0] != '\0'){
		data.category.OR = config.getBoolValue("OR");
	}else{
		cout	<<"[category filter] OR not specified in config.ini\n"
			<<"Set it equal to `true` for the list of categories to be OR "
			<<"filtered or set to `false` for the list to be AND\n";
		exit(1);
	}

	if(result.count("category")){
		ssInput << result["category"].as<std::string>();
	}else if(config.getStringValue("category")[0] != '\0'){
		ssInput <<config.getStringValue("category");
	}

	data.category.columns = config.getIntValue("columns");

	cout<<"JMOS - "<< jmos::p(data.getGameName()) <<"\n";

	data.category.setList(ssInput, true);

	output	<<"# Skyrim\n\n"
		<<"## Mods\n\n";
	
	output << data.filterCategories().rdbuf();

	output << data.modMasterList().rdbuf();

	output << data.categoryList().rdbuf();

	ofile << output.rdbuf();

	cout<<"Markdown generation successful.\n";

	return 0;
}
