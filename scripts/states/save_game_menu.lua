dofile "scripts/keys.lua"

hand = nil

texts = {}

images = {}

save_game_menu = {
    saves = {
        {
            label = "New Save"
        }
    };
    current_option = 0;
    selected_option = nil;
}

top_level_menu_texts = {}

menu_background = nil

top_level_menu_x_offset = 50
top_level_menu_y_offset = 50

current_save = save_game_menu["saves"][0]

function initialize()
    menu_background = objects.Image.new("ui.menu.background", 0, 0)

    font_name = "PixChicago"
    font_size = 10

    for index, save in pairs(save_game_menu["saves"]) do
        x = top_level_menu_x_offset
        y = top_level_menu_y_offset + (35 * (index - 1))

        table.insert(top_level_menu_texts, objects.Text.new(save["label"], font_name, font_size, x, y, 255, 255, 255))
    end

    hand = objects.Image.new("cursor-right", top_level_menu_x_offset - 30, top_level_menu_y_offset + (35 * save_game_menu["current_option"]))

    texts = top_level_menu_texts
end

function process_input(key_code)
    if key_code == CANCEL_KEY then
        save_game_menu_state:pop()

        return ""
    end

    number_of_options = 0

    for k, v in pairs(save_game_menu["saves"]) do
        number_of_options = number_of_options + 1
    end

    if key_code == UP_KEY then
        if save_game_menu["current_option"] > 0 then
            save_game_menu["current_option"] = save_game_menu["current_option"] - 1
        else
            save_game_menu["current_option"] = number_of_options - 1
        end

        hand:setPosition(top_level_menu_x_offset - 30, top_level_menu_y_offset + (35 * save_game_menu["current_option"]))
    end

    if key_code == DOWN_KEY then
        if save_game_menu["current_option"] < number_of_options - 1 then
            save_game_menu["current_option"] = save_game_menu["current_option"] + 1
        else
            save_game_menu["current_option"] = 0
        end

        hand:setPosition(top_level_menu_x_offset - 30, top_level_menu_y_offset + (35 * save_game_menu["current_option"]))
    end

    if key_code == CONFIRM_KEY then
        save_game_menu["selected_option"] = save_game_menu["current_option"]
    end
end

function update()
    if save_game_menu["selected_option"] ~= nil then
        save_game_menu_state:saveGame(save_game_menu["selected_option"])

        save_game_menu["selected_option"] = nil
    end

    return "save_game_menu"
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

    hand:render()
end
