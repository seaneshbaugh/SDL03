function spawn_npcs()
  print("Spawning NPCs for town01")
  map_state:addActorAtSpawnPoint("casie", "Casie", "characters/casie", "hello_world", "casie", Direction.Down, "random_walk", "dialogue")
  map_state:addActorAtSpawnPoint("kyle", "Kyle", "characters/kyle", "help", "kyle", Direction.Left, "random_walk", "dialogue")
end
