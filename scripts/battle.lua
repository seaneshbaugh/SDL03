dofile "keys.lua"

battle_state = GameState(raw_battle_state)

time = 0

font = nil

background = nil

texts = {}

function initialize()
    font = battle_state:getFont("DroidSans")

    background = GameImage(battle_state:loadTexture("background", "battle-background-001.png"), 0, 0)

    table.insert(texts, GameText("Battle State: 5 seconds remaining...", font, 200, 150, 255, 255, 0))
end

function process_input(key_code)
    return ""
end

function update()
    time = time + 1

    if time % 300 == 0 then
        battle_state:pop()
    end

    texts[1]:setText(string.format("Battle State: %d seconds remaining...", math.ceil((300 - time) / 60)))

    return ""
end

function render()
    if background then
        background:render()
    end

    for i, v in ipairs(texts) do
        v:render()
    end
end
