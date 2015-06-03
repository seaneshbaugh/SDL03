dofile "keys.lua"

settings_menu_state = GameState(raw_settings_menu_state)

video_settings_menu = {
    label = "Video Settings";
    options = {};
    current_option = 0;
}

audio_settings_menu = {
    label = "Audio Settings";
    options = {};
    current_option = 0;
}

input_settings_menu = {
    label = "Input Settings";
    options = {};
    current_option = 0;
}

main_settings_menu = {
label = "Settings";
    options = {
        video_settings_menu,
        audio_settings_menu,
        input_settings_menu
    };
    current_option = 0;
}

current_menu = main_settings_menu

texts = {}

images = {}

hand = nil

function initialize()
    font = settings_menu_state:getFont("DroidSans")

    cursor = settings_menu_state:getTexture("cursor-right")

    for index, menu in pairs(current_menu["options"]) do
        table.insert(texts, GameText(menu["label"], font, 200, 100 + (50 * index - 1), 255, 255, 255))
    end

    hand = GameImage(cursor, 175, 150 + (50 * current_menu["current_option"]))
end

function process_input(key_code)
    number_of_options = 0

    for k, v in pairs(current_menu["options"]) do
        number_of_options = number_of_options + 1
    end

    if key_code == UP_KEY then
        if current_menu["current_option"] > 0 then
            current_menu["current_option"] = current_menu["current_option"] - 1
        else
            current_menu["current_option"] = number_of_options - 1
        end
    end

    if key_code == DOWN_KEY then
        if current_menu["current_option"] < number_of_options - 1 then
            current_menu["current_option"] = current_menu["current_option"] + 1
        else
            current_menu["current_option"] = 0
        end
    end

    hand:setPosition(175, 150 + (50 * current_menu["current_option"]))

    if key_code == CANCEL_KEY then
        settings_menu_state:pop()
    end

    return ""
end

function update()

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
