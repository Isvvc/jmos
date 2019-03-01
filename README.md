# Project-Oghma-Markdown

Generating Markdown files for Skyrim mod lists

## Purpose

This application exists for the purpose of helping individuals organize Skyrim mods. This aplication does not handle mod files themsleves like [Mod Organizer](https://github.com/Modorganizer2/modorganizer) does. Instead it allows users to keep track of  mods they might be interested in without necissarily having them installed with their mod manager.

While features are currently very limited, here are some examples of potential uses I envision:

> You like taking screenshots of Skyrim and use a plethora of different mods. There are tons of these avilable that you like to use: various different grass mods, ENBs, character textures, armors, etc. For conflicting mods, such as different ENBs or character textures, you keep a set of images in your organizer for the ones you use so you can compare them and pick which one you want for the shoot. For modular mods like armors, you have hundreds and don't want all installed in the game at once. You look at your list of armor mods with reference pictures and leveling scales and find appropriate ones to install for the shoot.

> You install various different armor mods, balanced or not. They may or may not be craftable in-game and you want to work toward getting them in a playthrough. You keep a list of armors with image references oranized by level so you can pace your progression through the different armors. For craftbale armors, there is a list of all of the crafting materials required to make it so you can work toward that. For other armors, you keep their location in-game or their item IDs so you can acquire them when you reach the appropriate level.

## Usage

There are no precompiled binaries. Ironically, only macOS and Linux are currently supported. To use, place the `src` folder in a location you want the application to exist in. Download `json.hpp` from [nlohmann/json](https://github.com/nlohmann/json/releases) and place it in the src folder. From the terminal run the command (`generate` may be replaced with whatever name you would like the executable to have):

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
