time = 0
texts = {}
vx = {}
vy = {}
next_state = "intro"

function initialize()
    font_name = "DroidSans"
    font_size = 16

    math.randomseed(os.time())

    for i = 1, math.random(8, 16) do
        table.insert(texts, objects.Text.new("LOL", font_name, font_size, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
        table.insert(texts, objects.Text.new("ROFL", font_name, font_size, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
        table.insert(texts, objects.Text.new("LMAO", font_name, font_size, math.random(0, 600), math.random(0, 400), math.random(100, 255), math.random(100, 255), math.random(100, 255)))
    end

    for i, v in ipairs(texts) do
        if math.random(0, 1) == 0 then
            table.insert(vx, math.random(-180, -60))
        else
            table.insert(vx, math.random(60, 180))
        end
        
        if math.random(0, 1) == 0 then
            table.insert(vy, math.random(-180, -60))
        else
            table.insert(vy, math.random(60, 180))
        end
    end
end

function process_input(key_code)
    next_state = "main_menu"

    return next_state
end

function update(delta_time)
    if next_state ~= "intro" then
        return next_state
    end

    -- Right now the time counter does nothing.
    time = time + 1

    if time % 60 == 0 then
        time = 0
    end

    -- Leaving this here in case I need to do some more debugging as I work on timing.
    -- Right now this only works because of the call to SDL_Delay in the engine loop.
    -- This causes delta_time to be ~0.16 which is large enough to increase the position
    -- by an amount that does not get truncated when the position is converted back to
    -- to integers. I need to update pretty much everything for positions to use
    -- SDL_FRect or floats. I'm pretty sure SDL handles sub pixel rendering fine.
    -- local x_1 = texts[1]:getX()
    -- local y_1 = texts[1]:getY()
    -- local vx_1 = vx[1]
    -- local vy_1 = vy[1]
    -- local new_x_1 = (vx[1] * delta_time) + x_1
    -- local new_y_1 = (vy[1] * delta_time) + y_1
    -- print("x: " .. x_1 .. " y: " .. y_1 .. " vx: " .. vx_1 .. " vy: " .. vy_1 .. " new_x: " .. new_x_1 .. " new_y: " .. new_y_1 .. " delta_time: " .. delta_time)

    for i, v in ipairs(texts) do
        v:setPosition(v:getX() + (vx[i] * delta_time), v:getY() + (vy[i] * delta_time))

        local next_x = (vx[i] * delta_time) + v:getX()

        if next_x <= 0 or next_x + v:getWidth() >= 640 then
            vx[i] = vx[i] * -1
        end

        local next_y = (vy[i] * delta_time) + v:getY()

        if next_y <= 0 or next_y + v:getHeight() >= 480 then
            vy[i] = vy[i] * -1
        end
    end

    return next_state
end

function render()
    for i, v in ipairs(texts) do
        v:render()
    end
end
