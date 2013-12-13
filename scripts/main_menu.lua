dofile "sdl_keys.lua"

main_menu_state = GameState(raw_main_menu_state)

texts = {}

images = {}

hand = nil

previous_menu_option = 0

menu_option = 0

time = 0


-- I probably need to make some sort of proper menu object.
function initialize()
    font = main_menu_state:getFont("DroidSans")

    cursor = main_menu_state:getTexture("cursor-right")

    table.insert(texts, GameText("New Game", font, 200, 150, 255, 255, 255))

    table.insert(texts, GameText("Load Game", font, 200, 200, 255, 255, 255))

    table.insert(texts, GameText("Settings", font, 200, 250, 255, 255, 255))

    hand = GameImage(cursor, 175, 150 + (50 * menu_option))
end

function process_input(key_code)
    if key_code == SDLK_UP then
        if menu_option > 0 then
            menu_option = menu_option - 1
        else
            menu_option = 2
        end
    end

    -- What genius thought ~= would be a good idea for "not equal"?
    if key_code == SDLK_DOWN then
        if menu_option < 2 then
            menu_option = menu_option + 1
        else
            menu_option = 0
        end
    end

    return ""
end

function update()
    -- Right now the time counter does nothing.
    time = time + 1

    if time % 60 == 0 then
        time = 0
    end

    if previous_menu_option ~= menu_option then
        hand:setPosition(175, 150 + (50 * menu_option))

        previous_menu_option = menu_option
    end
end

function render()
    for i, v in ipairs(texts) do
        v:render()
    end

    for i, v in ipairs(images) do
        v:render()
    end

    hand:render()
end
