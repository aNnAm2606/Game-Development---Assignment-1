#include "GuiManager.h"
#include "App.h"
#include "Textures.h"
#include "Log.h"

#include "GuiButton.h"
//#include "GuiSlideBar.h"
#include "Audio.h"

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// L14: TODO1: Create a GUI control and add it to the list of controls

	GuiControl* control = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, text);
		break;
	
	// More Gui Controls can go here

	default:
		break;
	}

	//Set the observer

	control->SetObserver(observer);
	//control->SetTexture(texture);

	// Created GuiControls are added to the list of controls
	if (control != nullptr) 
	{ 
		LOG("control added to list");
		controls.add(control); 
	}

	return control;
}

bool GuiManager::Update(float dt)
{	
	LOG("GUI MANAGER UPDATE WORKS");
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt,doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	return true;
}

bool GuiManager::UpdateAll(float dt, bool doLogic) 
{

	if (doLogic) {

		ListItem<GuiControl*>* control = controls.start;

		while (control != nullptr)
		{
			control->data->Update(dt);
			control = control->next;
		}

	}
	return true; 

}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		control->data->Draw(app->render,control->data->id);
		control = control->next;
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}



