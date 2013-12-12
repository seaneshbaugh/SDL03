function GameText:show()
    print(self:getText())
end

intro_state = GameState(raw_intro_state)

texts = {}

time = 0

-- For now the velocities will be stored in a separate table.
vx = {}

vy = {}

function initialize()
    font = intro_state:getFont("DroidSans")

    math.randomseed(os.time())

    for i=1, math.random(8, 16) do
        table.insert(texts, GameText("LOL", font, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
    
        table.insert(texts, GameText("ROFL", font, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
    
        table.insert(texts, GameText("LMAO", font, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
    end

    for i, v in ipairs(texts) do
        v:show()
        
        if math.random(0, 1) == 0 then
            table.insert(vx, math.random(-3, -1))
        else
            table.insert(vx, math.random(1, 3))
        end
        
        if math.random(0, 1) == 0 then
            table.insert(vy, math.random(-3, -1))
        else
            table.insert(vy, math.random(1, 3))
        end
    end
end

function process_input()

end

function update()
    -- Right now the time counter does nothing.
    time = time + 1

    if time % 60 == 0 then
        time = 0
    end
    
    for i, v in ipairs(texts) do
        v:setPosition(v:getX() + vx[i], v:getY() + vy[i])

        if vx[i] + v:getX() <= 0 or vx[i] + v:getX() + v:getWidth() >= 640 then
            vx[i] = vx[i] * -1
        end
        
        if vy[i] + v:getY() <= 0 or vy[i] + v:getY() + v:getHeight() >= 480 then
            vy[i] = vy[i] * -1
        end
    end
end

function render()
    for i, v in ipairs(texts) do
        v:render()
    end
end
