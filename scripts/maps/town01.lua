function spawn_npcs()
  print("Spawning NPCs for town01")
  map_state:add_actor_at_spawn_point("casie", "Casie", "characters/casie", "casie", "casie", Direction.Down, "random_walk", "dialogue")
  map_state:add_actor_at_spawn_point("kyle", "Kyle", "characters/kyle", "kyle", "kyle", Direction.Left, "random_walk", "dialogue")
end
