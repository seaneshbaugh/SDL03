function initialize()
  print("interaction/dialogue initialized for " .. actor.name)
end

function on_interact(other_actor)
  print(other_actor.name .. " interacted with " .. actor.name)
end
