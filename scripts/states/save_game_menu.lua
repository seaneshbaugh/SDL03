dofile "scripts/keys.lua"

hand = nil

texts = {}

images = {}

menu_background = nil

function initialize()
    menu_background = objects.Image.new("ui.menu.background", 0, 0)
end

function process_input(key_code)
    if key_code == CANCEL_KEY then
        save_game_menu_state:pop()

        return ""
    end

    number_of_options = 0

    for k, v in pairs(current_menu["options"]) do
        number_of_options = number_of_options + 1
    end
end

function update()
end

function render()
    if menu_background then
        menu_background:render()
    end

    for i, v in ipairs(texts) do
        v:render()
    end

    for i, v in ipairs(images) do
        v:render()
    end

    -- hand:render()
end
