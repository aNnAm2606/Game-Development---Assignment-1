# Game Development - Assignment 1
 
This is a platformer game created by Anna, Miguel for Game Development. This project was created using C++, XML and box2D library.

Author's github links:

Anna https://github.com/aNnAm2606

Miguel https://github.com/migon25

Controls:

Space to start the game.
Space to jump/ twice to double jump.
A to move right, D to move left.
Left shift to sprint.
Hold space when in the ladder to climb up.



Lives:


Maximum 3.
Can be restored when a heart is collected, as long as player doesn't have the maximum amount already.



Collectables:


Coins can be collected to gain points.


Checkpoints:


Save the state of the game when player passes the flag.
When player passes the first time there is an animation, and sound.


Enemies: 

3 types of enemies: dog, cat and crow.
All enemies have pathfinding implemented when the player is within their radius, they will follow the player, 
within their radius. 
If player jumps on top of any enemy, the enemy dies.
If enemy hits the player, player loses a life and returns to the start.

GUI and Entity systems:

Entity System implemented: Player, Enemies, Coins and Hearts) from a base Entity class and an EntityManager
class manages them (Initializes, includes in a list, Updates, Draws, etc…)

GUI: Title Screen Main Menu: Includes the following GuiButtons:
○ [PLAY]
○ [CONTINUE]
○ [SETTINGS]
■ Adjust music volume, with a GuiSlider.
■ Adjust fx volume, with a GuiSlider.
■ Toggle fullscreen mode, with a GuiCheckBox.
■ Toggle VSync, with a GuiCheckBox.
○ [CREDITS]
○ [EXIT]
○ Buttons are responsive and include:
■ Visible state change on mouse-hover (FOCUSED)
■ Visible state change on mouse-pressed (PRESSED)
■ Generate an OnMouseClick event to be processed
■ Audio feedback on mouse-hover and mouse-click.

GUI: Gameplay Screen HUD: In-game GUI included the following elements:
○ Player lives
○ Coins
○ Timer:

● GUI: Gameplay Screen Pause Menu: Pressing ESCAPE triggers an in-game Pause Menu
with the following options GuiButtons:
○ [RESUME]
○ [SETTINGS]
○ [BACK to TITLE]
○ [EXIT]


Debug Features:

F1 for starting the game.
F2 for instant death.
F3 for restart level.
F4 for instant win.
F5 for saving current state.
F6 for loading state.
F7 for tutorials.
F9 for drawing colliders.
F10 for God mode.
Arrow keys for screen navigation in collider mode.

Github Link to the project can be found here:https://github.com/aNnAm2606/Game-Development---Assignment-1

Special features: 
