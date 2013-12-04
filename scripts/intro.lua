print("Hello from Lua")

function initialize()

end

function processInput(input)

end

function update()

end

function incrementTime(x)
    print("currentTime is " .. currentTime)
    
    print("Incrementing time by " .. x)

    currentTime = currentTime + x
    
    print("currentTime is now " .. currentTime)

    return currentTime
end
