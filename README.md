# YouTube Demonstration
https://youtu.be/E3F-iaHaCd0

# The Project
This project is created for learning the fundamentals of Unreal Engine with C++ and game desing. I wanted to experience lots of processes about game making as much as I can from model animation to enemy AI. Guns, enemies and props are all modeled and animated by myself with using Autodesk MAYA3D and Adobe Substance Painter (Combine and Zombie are modeled by myself but for rigging and several animations, Mixamo is used). Since the project is all about learning, almost all materials are inspired from Valve's Half-Life game. 

# The Work Done
Project is started with learning enhanced input system of the engine for the movement of my main character. 
Game Mode is created for handling the runtime game events like "Game Start" or "Actor Died".
Trace & tagging is used for object grabbing and triggering events.
Object-oriented programming fundamentals are used for the weapon system. Mutual systems like attacking, reloading etc. are inherited. Systems are seperated according to being melee or changing reloading system like reloading whole magazine or inserting bullets one by one.
Health component is created for both main character and enemies.
Perceive & stimulus systems are implemented for enemies to detect the main character.
Patrol Path system is created for enemies to walk around.
For the enemy Combine, AI is done with the engine's built-in behaviour tree & blackboard systems.
For remaining enemies, AI is implemented in C++ with using state machines.
For the simple hud, Unreal Engine's widget system is used

# Most Challenging Part

## Animation Precision
- Melee weapon hit presicion
- Shotgun reloding and shooting precision (interruption of the reloading with firing)

## AI
- Implementation of persive system for enemies on C++ side
- Correct animation transition between enemy behaviours

## Modeling 
- Smooth weight painting while rigging
- Character movement animation


