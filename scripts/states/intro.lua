time = 0
texts = {}
vx = {}
vy = {}

function initialize()
    font_name = "DroidSans"
    font_size = 16

    math.randomseed(os.time())

    for i = 1, math.random(8, 16) do
        table.insert(texts, objects.Text("LOL", font_name, font_size, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
        table.insert(texts, objects.Text("ROFL", font_name, font_size, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
        table.insert(texts, objects.Text("LMAO", font_name, font_size, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
    end

    for i, v in ipairs(texts) do
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

function process_input(key_code)
    return "main_menu"
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
