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

bool GuiButton::Draw(Render* render, uint32 id)
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
		switch (id)
		{
		case 1:
		{
			//orange start button coordinates
			button.x = 110;
			button.y = 190;
			button.w = 111;
			button.h = 52;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;
		case 2:
		{
			//orange continue button coordinates
			button.x = 110;
			button.y = 250;
			button.w = 111;
			button.h = 52;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;
		case 3:
		{
			//orange start button coordinates
			button.x = 7;
			button.y = 190;
			button.w = 44;
			button.h = 44;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;
		}
	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		switch (id)
		{
		case 1:
		{
			//white start button coordinates
			button.x = 245;
			button.y = 190;
			button.w = 111;
			button.h = 52;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;
		case 2:
		{
			//white start button coordinates
			button.x = 245;
			button.y = 250;
			button.w = 111;
			button.h = 52;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;
		case 3:
		{
			//orange start button coordinates
			button.x = 7;
			button.y = 190;
			button.w = 44;
			button.h = 44;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;

		}

	} break;
	case GuiControlState::PRESSED:
	{
		switch (id)
		{
		case 1:
		{
			//pressed start button coordinates
			button.x = 375;
			button.y = 190;
			button.w = 111;
			button.h = 52;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;
		case 2:
		{
			//pressed continue button coordinates
			button.x = 375;
			button.y = 250;
			button.w = 111;
			button.h = 52;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;
		case 3:
		{
			//orange start button coordinates
			button.x = 51;
			button.y = 190;
			button.w = 44;
			button.h = 44;

			render->DrawTexture(texture, bounds.x, bounds.y, &button);
		}break;

		}

	} break;

	/******/

	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	return false;
}