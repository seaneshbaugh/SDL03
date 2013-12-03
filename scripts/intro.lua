print("Hello from Lua")

function incrementTime(x)
    print("currentTime is " .. currentTime)
    
    print("Incrementing time by " .. x)

    currentTime = currentTime + x
    
    print("currentTime is now " .. currentTime)

    return currentTime
end
