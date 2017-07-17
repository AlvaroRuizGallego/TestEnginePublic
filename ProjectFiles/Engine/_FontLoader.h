#ifndef _FONT_LOADER_H
#define _FONT_LOADER_H

#include "_Font.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <string>
#include <map>


class _FontLoader
{
	private:

		static _FontLoader* _instance;

		FT_Library _ftLibrary;

		int _fontSize = 100;

		std::map<std::string, _Font*> _fontMap;

		_FontLoader();

	public:
		~_FontLoader();

		static _FontLoader* GetInstance();

		_Font* LoadFont(std::string file);
		void SetFontSize(int size);
		void Clear();
};

#endif
