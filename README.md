# Project-Oghma-Markdown

Generating Markdown files for Skyrim mod lists

## Purpose

This application exists for the purpose of helping individuals organize Skyrim mods. This aplication does not handle mod files themsleves like [Mod Organizer](https://github.com/Modorganizer2/modorganizer) does. Instead it allows users to keep track of  mods they might be interested in without necissarily having them installed with their mod manager. This is done by keeping a json database and generating Markdown files for preview.

While features are currently very limited, here are some examples of potential uses I envision:

> You like taking screenshots of Skyrim and use a plethora of different mods. There are tons of these avilable that you like to use: various different grass mods, ENBs, character textures, armors, etc. For conflicting mods, such as different ENBs or character textures, you keep a set of images in your organizer for the ones you use so you can compare them and pick which one you want for the shoot. For modular mods like armors, you have hundreds and don't want all installed in the game at once. You look at your list of armor mods with reference pictures and leveling scales and find appropriate ones to install for the shoot.

> You install various different armor mods, balanced or not. They may or may not be craftable in-game and you want to work toward getting them in a playthrough. You keep a list of armors with image references oranized by level so you can pace your progression through the different armors. For craftbale armors, there is a list of all of the crafting materials required to make it so you can work toward that. For other armors, you keep their location in-game or their item IDs so you can acquire them when you reach the appropriate level.

### Plans

Current work is being put into making the C++ application to generate Markdown files from the json databse. There is no official development on creating the databases. For my personal use and testing I use Shortcuts on iOS to scrape Nexusmods pages, add them to the database, and upload them to my working environment. The regex lines I use for scraping should be easily translatable to other implementations, specifically for desktop.

Other long-term plans include adding further features to a desktop client. One such plan is scanning Mod Organizer installations and automatically adding mods that have been installed there to the database. Another is scanning mod files themselves to extract specifics such as armor ratings or crafting material requirements. These would likely be implemented using [xEdit](https://github.com/TES5Edit/TES5Edit) or [zEdit](https://github.com/z-edit/zedit). I currently have a simple xEdit script for extracting crafting materials based on selected crafting recipes, but further integration would be required.

## Usage

There are no precompiled binaries. Ironically, only macOS and Linux are currently supported. To use, place the `src` folder and     `config.ini` file in a location you want the application to exist in. Install dependencies:

+ [nlohmann/json](https://github.com/nlohmann/json)
    + Place `json.hpp` in the `src` folder
+ [Poordeveloper/ini-parser](https://github.com/Poordeveloper/ini-parser)
    + Place `ini.hpp` in the `src` folder

From the terminal run the command (`generate` may be replaced with whatever name you would like the executable to have):

    g++ -std=c++11 src/main.cpp -o generate

Once compiled, run `generate` when you have a `db.json` to be rendered.

## Database

This project uses json for data storage. There is not yet an official implementation for creating databases. I personally use Shortcuts on iOS with Nexusmods scraping. These shortcuts might be released at a future time.

The json dictionary should be structed as such (will likely change in the future):

    {
      "Mods": {
        "Mod 1 name": {
          "author name": "Author name",
          "id": [Nexusmods ID],
          "author id": [Nexusmods author ID],
          "description": "Description of mod 1",
          "images": [
            "nexusmods image id.extension",
            "nexusmods image id.extension"
          ],
          "main image": "nexusmods image id.extensio",
          "categories": [
            "Category 1",
            "Category 2"
          ]
        }
        ...
      }
    }

**Note on images**: All nexusmods images are hosted after the URL `https://staticdelivery.nexusmods.com/mods/110/images/` and thus only the image name after this needs to be stored. Ex: Image URL `https://staticdelivery.nexusmods.com/mods/110/images/11163-0-1447284824.png` would be stored in the database as `11163-0-1447284824.png`.
