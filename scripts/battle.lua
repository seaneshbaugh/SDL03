dofile "sdl_keys.lua"

battle_state = GameState(raw_battle_state)

time = 0

font = nil

background = nil

function initialize()
    font = battle_state:getFont("DroidSans")

    background = GameImage(battle_state:loadTexture("background", "battle-background-001.png"), 0, 0)

end

function process_input(key_code)
    return ""
end

function update()
    time = time + 1

    if time % 300 == 0 then
        battle_state:pop()
    end

    return ""
end

function render()
    if background then
        background:render()
    end
end
