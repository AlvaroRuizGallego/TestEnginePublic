#include "_FontLoader.h"


_FontLoader* _FontLoader::_instance = nullptr;


_FontLoader::_FontLoader()
{
	if (FT_Init_FreeType(&_ftLibrary)) 
	{
		printf("\nWARNING: _FontLoader::_FontLoader -> Could not init Freetype library");
	}
}


_FontLoader::~_FontLoader()
{
	_instance = nullptr;

	for (auto it = _fontMap.begin(); it != _fontMap.end(); it++)
	{
		FT_Done_Face(it->second->_ftFace);
		delete it->second;
	}

	_fontMap.clear();
}

_FontLoader * _FontLoader::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new _FontLoader();
	}

	return _instance;
}

_Font* _FontLoader::LoadFont(std::string file)
{
	auto it = _fontMap.find(file);
	if (it != _fontMap.end())
	{
		return _fontMap.at(file);
	}

	FT_Face face;
	if (FT_New_Face(_ftLibrary, file.c_str(), 0, &face)) 
	{
		printf("\nWARNING: _FontLoader::LoadFont -> Could not open font\n");
		return nullptr;
	}

	FT_Set_Pixel_Sizes(face, _fontSize, _fontSize);

	_Font* font = new _Font(face);
	_fontMap[file] = font;

	return font;
}

void _FontLoader::SetFontSize(int size)
{
	_fontSize = size;
}

void _FontLoader::Clear()
{
	for (auto it = _fontMap.begin(); it != _fontMap.end(); it++)
	{
		FT_Done_Face(it->second->_ftFace);
		delete it->second;
	}

	_fontMap.clear();
}
