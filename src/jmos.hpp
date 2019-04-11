#include <string>
#include <sstream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class jmos {
	json gameList;
	enum URLs { mod, image };
	
	json db, mods, categories, categoryFilterList;
	bool categoryFilterOR;
	char categoryColumns, generalColumns;
	std::string game = "null";

public:
	jmos(json&);
	void setGame(std::string&);
	void setGameList(json&);

	static std::string p(json);
	static bool jsonListContains(json, std::string);
	static bool jsonDictContains(json, std::string);
	static std::string linkify(std::string);
	
	bool checkGame() {return jsonDictContains(gameList, game);};
	std::string url(URLs, std::string);
	std::string getGameName() {return gameList[game]["name"];}

	std::stringstream filterCategories(json&);
	std::stringstream modMasterList();
	std::stringstream categoryList();

	struct section{
		char columns;
	}general;

	class filter : public section{
		json list;
	public:
		bool OR;

		void setList(json&);
	}category, author;
};
