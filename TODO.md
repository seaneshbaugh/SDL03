# TODO

- Make a namespace for Actor, ActorAppearance, and Camera.
- Move resources directory to data.
- Add collision detection with NPCs. Something like:
  ```
  bool Map::IsTileOccupied(int x, int y, Actor* ignore)
  {
      for (auto& actor : actors) {
          if (actor.get() == ignore) {
              continue;
          }

          if (actor->tileX == x && actor->tileY == y) {
              return true;
          }
      }

      return false;
  }
  ```
  I don't think this necessarily belongs in the States::Map class though.
- Find a better name for the Game::Objects namespace.
- Implement MovmentCommand for Actor.
- Add a way to keep track of whether an Actor has finished moving entirely (MoveCommand queue is empty maybe?).
- Implement a way to interrupt an Actor's movement (clear MoveCommand queue?).
- Turn moving boolean into checker for whether MoveCommand queue is empty or not?
- Consider implementing an ICameraTarget so things that aren't Actors can be followed by the camera.
- Add sort ordering for Actors.
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
- Initialize all member variables!
- Use in-class initializers where possible.
