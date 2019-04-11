#include <string>
#include <sstream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class jmos {
	json gameList;
	enum URLs { mod, image };
	
	json db, mods, categoryMasterList, categoryFilterList;
	bool categoryFilterOR;
	char categoryColumns, generalColumns;
	std::string category = "null", game = "null";

public:
	jmos(json&);
	void setGame(std::string&);
	void setGameList(json&);
	static std::string p(json);
	static bool jsonListContains(json, std::string);
	static bool jsonDictContains(json, std::string);
	static std::string linkify(std::string);
	std::string url(URLs, std::string);

	std::stringstream filterCategories(json&, bool, char);
	std::stringstream modMasterList(json&, char);
};
