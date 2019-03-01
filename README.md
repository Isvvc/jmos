# Project-Oghma-Markdown

Generating Markdown files for Skyrim mod lists

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
