#include "SpriteSheet.h"

#include <fstream>
#include <iostream>
#include <algorithm>


void SpriteSheet::parseSpriteSheetData(std::string fileName)
{
	std::string line;
	std::ifstream dataFile(fileName);

	if (!dataFile.good())
	{
		printf("\nWARNING: SpriteSheet::parseSpriteSheetData -> Can't load sprite sheet data");
		return;
	}

	printf("\n\nSprite sheet data loaded correctly\n-----------------------------------");

	float spriteSheetWidth, spriteSheetHeight;

	getline(dataFile, line);

	while (!dataFile.eof())
	{
		if (line[0] != '}' && line[0] != '{' && line.size())
		{
			line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
			line.erase(std::remove(line.begin(), line.end(), '\"'), line.end());

			int pos = (int)line.find(":", 0);
			std::string variable = line.substr(0, pos);

			if (variable == "image")//_Sprite sheet image name
			{
				std::string value = line.substr(pos + 1, line.size() - pos - 2);

				_spriteSheetImageName = value;

				printf("\nSprite sheet name: %s", _spriteSheetImageName.c_str());
			}
			else if (variable == "size") //Size of the whole sprite sheet
			{
				std::string value = line.substr(pos + 1, line.size() - pos - 2);

				int pos1, pos2;

				//width
				pos1 = (int)line.find("w", 0);
				pos2 = (int)line.find(",", pos1 + 1);

				spriteSheetWidth = (float)atof(line.substr(pos1 + 2, pos2).c_str());

				//height
				pos1 = (int)line.find("h", 0);
				pos2 = (int)line.find("}", pos1 + 1);

				spriteSheetHeight = (float)atof(line.substr(pos1 + 2, pos2).c_str());
				printf("\nSprite sheet size: width: %g, height: %g", spriteSheetWidth, spriteSheetHeight);
			}
		}
		getline(dataFile, line);
	}


	dataFile.clear();
	dataFile.seekg(0);
	
	std::string name;

	getline(dataFile, line);

	while (!dataFile.eof())
	{
		if (line[0] != '}' && line[0] != '{' && line.size())
		{
			line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
			line.erase(std::remove(line.begin(), line.end(), '\"'), line.end());

			int pos = (int)line.find(":", 0);
			std::string variable = line.substr(0, pos);

			if (variable == "filename")//_Sprite name
			{
				std::string value = line.substr(pos + 1, line.size() - pos - 2);

				name = value.substr(0, value.find(".", 0));

				printf("\n\nSprite name: %s", name.c_str());
			}
			else if (variable == "frame")//UVs
			{
				std::string value = line.substr(pos + 1, line.size() - pos - 2);

				int pos1, pos2;
				float x, y, width, height;

				//x
				pos1 = (int)line.find("x", 0);
				pos2 = (int)line.find(",", pos1 + 1);

				x = (float)atof(line.substr(pos1 + 2, pos2).c_str());

				//y
				pos1 = (int)line.find("y", 0);
				pos2 = (int)line.find(",", pos1 + 1);

				y = (float)atof(line.substr(pos1 + 2, pos2).c_str());

				//width
				pos1 = (int)line.find("w", 0);
				pos2 = (int)line.find(",", pos1 + 1);

				width = (float)atof(line.substr(pos1 + 2, pos2).c_str());

				//height
				pos1 = (int)line.find("h", 0);
				pos2 = (int)line.find("}", pos1 + 1);

				height = (float)atof(line.substr(pos1 + 2, pos2).c_str());

				printf("\nFrame: x: %g, y: %g, width: %g, height: %g", x, y, width, height);

				Rectangle rect = Rectangle(x / spriteSheetWidth, (spriteSheetHeight - (y + height)) / spriteSheetHeight, (x + width) / spriteSheetWidth, (spriteSheetHeight - y) / spriteSheetHeight);
				_spriteMap.insert(std::pair<std::string, Rectangle>(name, rect));
			
				printf("\nUV rectangle: minX: %g, minY: %g, maxX: %g, maxY: %g", rect.min.x, rect.min.y, rect.max.x, rect.max.y);
			}
		}
		getline(dataFile, line);
	}

	printf("\n-----------------------------------");

	dataFile.close();



	int pos = (int)fileName.find_last_of(".");
	std::string path = fileName.substr(0, pos + 1);

	pos = (int)_spriteSheetImageName.find_last_of(".");
	std::string fileFormat = _spriteSheetImageName.substr(pos + 1, _spriteSheetImageName.size() - (pos + 1));

	_spriteSheetImagePath = path + fileFormat;
}

SpriteSheet::SpriteSheet(std::string spriteSheetFile)
{
	parseSpriteSheetData(spriteSheetFile);
}

SpriteSheet::~SpriteSheet()
{
}

std::string SpriteSheet::GetSpriteSheetImageName()
{
	return _spriteSheetImageName;
}

std::string SpriteSheet::GetSpriteSheetImagePath()
{
	return _spriteSheetImagePath;
}

Rectangle SpriteSheet::GetUVs(std::string spriteName)
{
	auto it = _spriteMap.find(spriteName);
	if (it != _spriteMap.end())
	{
		return it->second;
	}
	
	printf("\nWARNING: SpriteSheet::GetUVs -> There is no sprite with name %s in sprite sheet %s", spriteName, _spriteSheetImageName.c_str());

	return Rectangle(0,0,0,0);
}

std::vector<Rectangle> SpriteSheet::GetAnimationUVs(std::string baseAnimationName)
{
	std::vector<Rectangle> animationUVs;

	for (auto it = _spriteMap.begin(); it != _spriteMap.end(); ++it) 
	{
		std::string spriteName = it->first;

		if (spriteName.find(baseAnimationName) != std::string::npos) 
		{
			animationUVs.push_back(it->second);
		}
	}

	return animationUVs;
}
