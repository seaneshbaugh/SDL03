dofile "scripts/keys.lua"

texts = {}
images = {}
hand = nil
previous_menu_option = 0
menu_option = 0
time = 0
next_state = "main_menu"

-- I probably need to make some sort of proper menu object.
function initialize()
    font_name = "PixChicago"
    font_size = 10

    table.insert(texts, objects.Text.new("New Game", font_name, font_size, 200, 150, 255, 255, 255))
    table.insert(texts, objects.Text.new("Load Game", font_name, font_size, 200, 200, 255, 255, 255))
    table.insert(texts, objects.Text.new("Settings", font_name, font_size, 200, 250, 255, 255, 255))

    hand = objects.Image.new("cursor-right", 175, 150 + (50 * menu_option))
end

function process_input(key_code)
    if key_code == UP_KEY then
        if menu_option > 0 then
            menu_option = menu_option - 1
        else
            menu_option = 2
        end
    end

    if key_code == DOWN_KEY then
        if menu_option < 2 then
            menu_option = menu_option + 1
        else
            menu_option = 0
        end
    end

    if key_code == CONFIRM_KEY then
        if menu_option == 0 then
            next_state = "new_game"
        end
        
        if menu_option == 1 then
            -- Commenting this out for now since it causes a crash.
            -- next_state = "load_game"
        end
        
        if menu_option == 2 then
            -- Commenting this out for now since it causes a crash.
            -- next_state = "settings_menu"
        end
    end

    return ""
end

function update()
    if next_state ~= "main_menu" then
        return next_state
    end

    -- Right now the time counter does nothing.
    time = time + 1

    if time % 60 == 0 then
        time = 0
    end

    if previous_menu_option ~= menu_option then
        hand:setPosition(175, 150 + (50 * menu_option))

        previous_menu_option = menu_option
    end

    return next_state
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
