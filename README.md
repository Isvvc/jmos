# JSON Mod Organization System

JSON and Markdown organization for Skyrim and other game mod lists 

## Purpose

This application exists for the purpose of helping individuals organize Skyrim mods. This aplication does not handle mod files themsleves like [Mod Organizer](https://github.com/Modorganizer2/modorganizer) does. Instead it allows users to keep track of  mods they might be interested in without necissarily having them installed with their mod manager. This is done by keeping a json database and generating Markdown files for preview.

While features are currently very limited, here are some examples of potential uses I envision:

> You like taking screenshots of Skyrim and use a plethora of different mods. There are tons of these avilable that you like to use: various different grass mods, ENBs, character textures, armors, etc. For conflicting mods, such as different ENBs or character textures, you keep a set of images in your organizer for the ones you use or are interested in trying, so you can compare them and pick which one you want for the shoot. For modular mods like armors, you have hundreds and don't want all installed in the game at once. You look at your list of armor mods with reference pictures and leveling scales and find appropriate ones to install for the shoot.

> You install various different armor mods, balanced or not. They may or may not be craftable in-game and you want to work toward getting them in a playthrough. You keep a list of armors with image references oranized by level so you can pace your progression through the different armors. For craftbale armors, there is a list of all of the crafting materials required to make it so you can work toward that. For other armors, you keep their location in-game or their item IDs so you can acquire them when you reach the appropriate level.

### Plans

Current work is being put into making the C++ application to generate Markdown files from the json databse. There is no official development on creating the databases. For my personal use and testing I use Shortcuts on iOS to scrape Nexusmods pages, add them to the database, and upload them to my working environment. The regex lines I use for scraping should be easily translatable to other implementations, specifically for desktop.

Other long-term plans include adding further features to a desktop client. One such plan is scanning Mod Organizer installations and automatically adding mods that have been installed there to the database. Another is scanning mod files themselves to extract specifics such as armor ratings or crafting material requirements. These would likely be implemented using [xEdit](https://github.com/TES5Edit/TES5Edit) or [zEdit](https://github.com/z-edit/zedit). I currently have a simple xEdit script for extracting crafting materials based on selected crafting recipes, but further integration would be required.

## Usage

There are currently no precompiled binaries. Ironically, only macOS and Linux are currently supported. To use, place the `src` folder, `gamelist.json` and `config.ini` file in a location you want the application to exist in. Install dependencies:

+ [nlohmann/json](https://github.com/nlohmann/json)
    + Place `json.hpp` in the `src` folder
+ [RudeConfig](http://rudeserver.com/config/download.html)
    + Place `config.h` and `librudeconfig.a` in the `src` folder
    + **How to obtain librudeconfig.a**: download the latest source code package and unzip. Run `./configure` in the extracted directory then `make`. Copy the `librudeconfig.a` from the `.libs` folder (will be hidden) to this program's `src` folder.
+ [jarro2783/cxxopts](https://github.com/jarro2783/cxxopts)
    + Place `cxxopts.hpp` in the `src` folder

From the terminal run the command (`generate` may be replaced with whatever name you would like the executable to have):

    g++ -std=c++11 src/main.cpp src/jmos.cpp src/librudeconfig.a -o generate

Once compiled, run `generate` when you have a `db.json` to be rendered. This will generate a `mods.md` which can be viewed in a markdown viewer.

### Configuration

JMOS can be configured through the `config.ini` file and sorting can be done at runtime. The `config.ini` settings are explained below:

+ `game=` Specify the name of the game based on how it appears in nexusmods URLs.
+ `columns=` Specify the number of columns when listing content in a grid.
+ `category=` Specify a category to filter by. Use commas, but not spaces, to filter by multiple categories. Is overridden by the runtime argument.
+ `categoryFilterOR=` Specify if mods should be filtered using OR or AND. OR means mods will be shown if they contains _any_ of the listed categories. AND means mods will only be shown if they contain _all_ of the listed categories.

At runtime, the command-line argument `-c` or `--category` may be used to specify a category to filter by. As with the `config.ini` option, multiple categories may be specified using commas but not spaces. Example:

    ./generate -c Armor,Realistic

This will generate the document filtering by both the _Armor_ and _Realistic_ category above the mod list. This will override the category specified in the `config.ini`.

### Game List

For a game to be used, it must be in the gamelist.json file. Each game should be a dictionry with the key being the name of the game as seen in nexusmods URLs (the name after `.com/` in a nexusmods URL). The dictionary should have a `"id"` key with the value equal to the Nexusmods ID as seen in image URLs (the number after `/mods/` in a Nexusmods image URL) and a `"name"` key with the value of a display name for the game with proper capitalization and spacing. The current included games list has support for Skyrim, Skyrim Special Edition, and Oblivion.

The easiest way to add a new game manually would be by using a JSON validator such as [DuckDuckGo's](https://duckduckgo.com/?q=json+validator&t=ffab&ia=answer). Paste in your current games list, copy one of the games and paste it below the list (but above the last `}`, and change the values to match the game you want to add.

## Database

This project uses json for data storage. There is not yet an official implementation for creating databases. I personally use Shortcuts on iOS with Nexusmods scraping. These shortcuts might be released at a future time.

The json dictionary should be structed as shown below. `"id"` and `"author id"` should be the Nexusmods IDs. Structure likely to change as program develops.

```json
{
  "Mods": {
    "Mod 1 name": {
      "author name": "Author name",
      "id": {
        "game1": 55555,
        "game2": 5555
      },
      "author id": 5555555,
      "description": "Description of mod 1",
      "main image": "full image URL",
      "images": {
        "game1": [
          "nexusmods image",
          "nexusmods image"
        ],
        "game2": [
          "nexusmods image"
        ]
      },
      "categories": [
        "Category 1",
        "Category 2"
      ]
    }
  }
}
```

**Notes**: Games (listed as `"game1"` and `"game2"` in the example) should be the name of the game as seen in Nexusmods URLs. For example, `"skyrim"` or `"skyrimspecialedition"`. All nexusmods images are hosted after the URL `https://staticdelivery.nexusmods.com/mods/`_gameID_`/images/` and thus only the image name after this needs to be stored. Ex: Image URL `https://staticdelivery.nexusmods.com/mods/110/images/11163-0-1447284824.png` would be stored in the database as `11163-0-1447284824.png`. The main image should have its full URL stored and should not be included redundantly in the images list.
