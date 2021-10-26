
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

void Map::Draw()
{
    if(mapLoaded == false)
        return;

    // L03: TODO 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

    pugi::xml_document mapFile;
    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFile);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFile);
    }

    if(ret == true)
    {
        //L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
    }

    if(mapFile) mapFile.reset();

    mapLoaded = ret;

    return ret;
}

// L03: TODO 3: Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;

    return ret;
}

// L03: TODO 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    return ret;
}


