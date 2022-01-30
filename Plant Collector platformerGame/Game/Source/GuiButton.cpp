#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Textures.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	texture= app->tex->Load("Assets/textures/buttons.png");
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	LOG("GUI update works");
	if (state != GuiControlState::DISABLED)
	{
		LOG("not disabled");
		// L14: TODO 3: Update the state of the GUiButton according to the mouse position
		int mouseX, mouseY;
		mouseX = app->input->mouseX;
		mouseY = app->input->mouseY;

		if ((mouseX > bounds.x ) && (mouseX < (bounds.x + bounds.w )) &&
			(mouseY > bounds.y ) && (mouseY < (bounds.y + bounds.h )))
		{
			state = GuiControlState::FOCUSED;
			LOG("focused");
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{

	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED: 
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL:
	{
		//orange start button coordinates
		button.x = 110;
		button.y = 190;
		button.w = 111;
		button.h = 52;

		render->DrawTexture(texture, bounds.x, bounds.y, &button);

	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		//white start button coordinates
		button.x = 245;
		button.y = 190;
		button.w = 111;
		button.h = 52;

		render->DrawTexture(texture, bounds.x, bounds.y, &button);
	} break;
	case GuiControlState::PRESSED:
	{
		//pressed start button coordinates
		button.x = 375;
		button.y = 190;
		button.w = 111;
		button.h = 52;

		render->DrawTexture(texture, bounds.x, bounds.y, &button);
	} break;

	/******/

	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	return false;
}