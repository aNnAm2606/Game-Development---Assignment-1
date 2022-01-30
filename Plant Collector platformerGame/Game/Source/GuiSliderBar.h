##ifndef __GUISLIDEBAR_H__
#define __GUISLIDEBAR_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlideBar : public GuiSlideBar
{
public:

	GuiSlideBar(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiSlideBar();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUISLIDEBAR_H__pragma once
