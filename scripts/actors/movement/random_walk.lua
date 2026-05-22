pausing = false
pause_for_time = 0
paused_time = 0

function initialize()
  math.randomseed(os.time())
  print("movement/random_walk initialized for " .. actor.name)
end

function update(delta_time)
  if not actor:isMoving() then
    if pausing then
      paused_time = paused_time + delta_time

      if paused_time > pause_for_time then
        pausing = false
        pause_for_time = 0
        paused_time = 0
      end
    else
      actor:clearPendingMovement()

      if math.random(0, 5) == 0 then
        pausing = true
        pause_for_time = math.random(1, 3)
        paused_time = 0
      else
        randomDirection = math.random(0, 3)

        actor:queueMovement(randomDirection, 1)      
      end
    end
  end
end
