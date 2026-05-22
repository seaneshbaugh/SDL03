function initialize()
  math.randomseed(os.time())
  print("movement/random_walk initialized for " .. actor.name)
end

function update(_delta_time)
  if not actor:isMoving() then
    actor:clearPendingMovement()

    randomDirection = math.random(0, 3)

    actor:queueMovement(randomDirection, 1)
  end
end
