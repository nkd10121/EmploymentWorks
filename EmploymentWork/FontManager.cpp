#include "FontManager.h"
#include "DxLib.h"

#include <cassert>

FontManager* FontManager::m_instance = nullptr;

namespace
{
	const char* const kFontPath = "data/font/YasashisaGothicBold-V2.otf";
	const char* const kFontName = "やさしさゴシックボールドV2";

	const int kFontSize[] =
	{
		8,16,24,32,40,48,56,64,72,80,88
	};
}

FontManager::~FontManager()
{
	for (const auto& f : m_fonts)
	{
		DeleteFontToHandle(f.handle);
	}
	m_fonts.clear();
}

void FontManager::LoadFont()
{
	// 読み込むフォントファイルのパス
	if (AddFontResourceEx(kFontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}

	//とりあえず使いそうなサイズのフォントを作っておく
	for (auto& size : kFontSize)
	{
		Font add;
		add.fontName = kFontName;
		add.size = size;
		add.handle = CreateFontToHandle(kFontName, size, -1);
		m_fonts.push_back(add);
	}
}

void FontManager::DrawCenteredText(int x, int y, std::string text, unsigned int color,unsigned int edgeColor,std::string fontName, int size)
{
	Font drawFont;
	bool isFound = false;

	for (auto& font : m_fonts)
	{
		if (font.fontName == fontName && font.size == size)
		{
			isFound = true;
			drawFont = font;
		}
	}

	if (!isFound)
	{
#ifdef _DEBUG
		assert(0 && "フォントデータが見つかりませんでした");
#endif
		return;
	}

	int textWidth = GetDrawStringWidthToHandle(text.c_str(), text.length(), drawFont.handle);
	int textHeight = GetFontSizeToHandle(drawFont.handle);
	DrawStringToHandle(x - textWidth / 2, y - textHeight / 2, text.c_str(), color, drawFont.handle, edgeColor);
}

void FontManager::DrawBottomRightText(int x, int y, std::string text, unsigned int color, unsigned int edgeColor, std::string fontName, int size)
{
	Font drawFont;
	bool isFound = false;

	for (auto& font : m_fonts)
	{
		if (font.fontName == fontName && font.size == size)
		{
			isFound = true;
			drawFont = font;
		}
	}

	if (!isFound)
	{
#ifdef _DEBUG
		assert(0 && "フォントデータが見つかりませんでした");
#endif
		return;
	}

	int textWidth = GetDrawStringWidthToHandle(text.c_str(), text.length(), drawFont.handle);
	int textHeight = GetFontSizeToHandle(drawFont.handle);
	DrawStringToHandle(x - textWidth, y - textHeight, text.c_str(), color, drawFont.handle);
}
