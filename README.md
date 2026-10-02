# Myne

 2d Engine

 This is realistically intended only for my use during the 2026-2027 BPA V03 event, but anyone is free to use it.
 I at least have *some* ethics, so create an issue or email me if you would like any clarification with this engine. Email will probably be the quickest response, especially for use case specific stuff.

 The general structure of the program is to create a game object, start it, then populate it with processes.

 For example, to have a game that exists:
 
 ```cpp
    game gameplay("Hi I exist", SDL_WINDOW_RESIZABLE, {1500,800});
    set_current_game(&gameplay);
    gameplay.set_physics(false);

    gameplay.set_root(new Process);

    gameplay.start();

    gameplay.get_root()->del();
```

The process root allows any children of it to be part of the game. It is entirely legal to
- Swap out the active root to change the scene.
- Change the class type of the root, eg a score controller.
  
However, you should be hesitant when deleting the root without replacing it. Setting root to nullptr will throw and close the window. Likely, it will also not save the game.

The class diagram is `Process-->Object-->CollObj/DrawObj`  

### Process  
  A Process is the most basic class, and any game object or thing that acts every frame should inherit from it.  ]
  Uses the .process() function, which is called every frame in the order of parent to child.

### Object
  An Object is a Process with position. The main difference is the `pos position` variable.  
  As a bonus, children of the Object will inherit the position. Their position can be viewed as an offset from the parent.

### CollObj
  A Collision Object that allows its position to be controlled by the physics engine. Like Objects, child Objects inherit their position, but the CollObj will move in the world according to the current physics engine. Note: the current game must have an assigned physics engine or the CollObj creation will fail.

### DrawObj
  An Object with a sprite. Acts the exact same as an Object, but the object can have a depth set (unsigned char, 0 is furthest on screen, 255 is closest on screen)
  A DrawObj also has access to the .draw() function, for per frame calls.
  Sorry, but right now only the sprite type is guaranteed to work as a texture. 

  
