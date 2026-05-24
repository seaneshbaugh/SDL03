# TODO

- Clean up States::Map::Update function, break out different substate updates into their own functions.
- Make it so going to dialogue substate doesn't necessarily freeze all other NPCs. We might want to be able to have NPCs moving while dialogue is happening.
- Organize declarations and definitions for States::Map according to functionality. Need to do this with all classes but this one is especially bad right now.
- Add NPC spawn points to map data.
- Add spawn scripts that actually run the NPC spawns and tie dialogue to them.
- Tie to dialogue could be as simple as having different interaction scripts for each NPC. Something that looks like:
  ```
    function interact(self, initiator)
      if world.flags.cows_stolen then
          game:startDialogue("village/cattle_rancher_after_theft")
      else
          game:startDialogue("village/cattle_rancher_before_theft")
      end
  end
  ```
  Not sure If I'm a huge fan of that though because it means *every* NPC needs a custom interaction script rather than just sharing a handful that say "start whatever dialogue this NPC should start".
- Add Actor::Wait function that makes the Actor wait and do nothing for a specified duration.
- Add Actor::IsBusy function that checks if the Actor is currently moving or waiting. Maybe checks MoveCommand queue?
- Add Actor::MoveTo function that takes a target position and adds the necessary MoveCommands to the queue to move the Actor there. This will involve pathfinding to determine the correct sequence of MoveCommands to get to the target position.
- Add Actor::PlayAnimation function that takes an animation name and plays the corresponding animation on the Actor's sprite.
- Add Actor::FaceActor function that takes another Actor as a parameter and makes the first Actor face towards the second Actor. This will involve calculating the direction from the first Actor to the second Actor and setting first Actor's direction accordingly.
- Maybe add Actor as a parameter to States::Map::StartDialogue so it's possible to display different names and portraits in the dialogue box depending on which Actor is speaking. Otherwise the dialogue data will need to keep track of the Actor's name and portrait, which seems less flexible.
- Right now a dialogue graph looks like
  ```
    {
    "start": "intro",

    "nodes": {
      "intro": {
        "type": "text",
        "text": "hello",
        "next": "question"
      },

      "question": {
        "type": "choice",
        "text": "ask_did_see",

        "choices": [
          {
            "text": "yes",
            "next": "did_see"
          },
          {
            "text": "no",
            "next": "did_not_see"
          }
        ]
      },

      "did_see": {
        "type": "text",
        "text": "did_see",
        "next": null
      },

      "did_not_see": {
        "type": "text",
        "text": "did_not_see",
        "next": null
      }
    }
  }
  ```
  I will eventually want nodes that look like:
  ```
  {
    "type": "conditional",
    "condition": "world.flags.cows_stolen",
    "true": "thieves_dialogue",
    "false": "peaceful_dialogue"
  }
  ```
  or
  ```
  {
    "type": "event",
    "event": "give_item",
    "item": "potion",
    "next": "after_item"
  }
  ```
  I don't want to encode logic into the dialogue graph itself, but I do want to be able to trigger events and have conditional branches based on game state. So maybe the dialogue graph just specifies what events to trigger and what conditions to check, and the actual logic for those events and conditions lives in the game code somewhere.
- Add a way to differentiate Player Actors and NPC Actors. Not sure if I want to go with inheritance here.
- Figure out what to do about the "asset lists". I never liked that design. If I keep them around they might just live in the assets directory the same way spritesheets do. They're all that's left in the resources directory and I'm just about done removing the concept of resources from this.
- Find a better name for the Game::Objects namespace.
- Implement MovmentCommand for Actor.
- Add a way to keep track of whether an Actor has finished moving entirely (MoveCommand queue is empty maybe?).
- Implement a way to interrupt an Actor's movement (clear MoveCommand queue?).
- Turn moving boolean into checker for whether MoveCommand queue is empty or not?
- Maybe MoveCommand queue is a generic Command queue.
- Add more basic movement AI scripts. Random walk gets me a pretty long way towards making NPCs that wander around, but it would be nice to have some more complex behaviors as well. Maybe a "patrol" script that makes an NPC walk back and forth between two points, or a "follow" script that makes an NPC follow the player around. I will also want some sort of random walk with bounding coordinates so NPCs stay in their particular zones if they have one.
- Consider implementing an ICameraTarget so things that aren't Actors can be followed by the camera.
- Serialize World object for saving/loading.
- Add some sort of MapStateData container for the World. Currently we just have the player's X and Y positions on the map (and a reference to the map itself). But eventually we may want:
  - A list of NPCs and their positions.
  - A list of items on the ground and their positions.
  - A list of doors and whether they are open or closed.
  - A list of traps and whether they are active or not.
  An object heirarchy for this might be:
  ```
  World
  -> MapStateData
      -> ActorStates
      -> EventFlags
      -> ObjectStates
  ```
  Where ActorStates is a list of ActorState objects, each containing the Actor's position, EventFlags is a list of flags that can be set by events (e.g. "door1_opened"), and ObjectStates is a list of objects on the map and their states (e.g. "chest1_opened", "trap1_active").
  We'll likely need this on a per-map basis, so there might actually be an array of MapStateData objects in the World, each corresponding to a map.
- Figure out how and where to store game state flags that persist across sessions.
- Initialize all member variables!
- Use in-class initializers where possible.
- Async scripting?
- Sounds and music.
- Add a way to play sound from a script. Probably will live in States::Map at first.
- Actually implement saving and loading.
