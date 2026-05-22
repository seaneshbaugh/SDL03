function initialize()
  math.randomseed(os.time())
  print("Random walk initialized for " .. actor.name)
end

function update(_delta_time)
  if not actor:isMoving() then
    actor:clearPendingMovement()

    randomDirection = math.random(0, 3)

    actor:queueMovement(randomDirection, 1)
  end
end

function on_interact(other_actor)
  print(other_actor.name .. " interacted with " .. actor.name)
end
