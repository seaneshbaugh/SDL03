local inspect = require "scripts/inspect"

dofile "scripts/keys.lua"

items_menu = {
    label = "Items";
    options = {};
    current_option = 0;
}

magic_menu = {
    label = "Magic";
    options = {};
    current_option = 0;
}

equipment_menu = {
    label = "Equipment";
    options = {};
    current_option = 0;
}

status_menu = {
    label = "Status";
    options = {};
    current_option = 0;
}

order_menu = {
    label = "Order";
    options = {};
    current_option = 0;
}

config_menu = {
    label = "Config";
    options = {};
    current_option = 0;
}

journal_menu = {
    label = "Journal";
    options = {};
    current_option = 0;
}

save_menu = {
    label = "Save";
    options = {};
    current_option = 0;
}

pause_menu = {
    label = "Pause";
    options = {
        items_menu,
        magic_menu,
        equipment_menu,
        status_menu,
        order_menu,
        config_menu,
        journal_menu,
        save_menu
    };
    current_option = 0;
    selected_option = nil;
}

current_menu = pause_menu

menu_background = nil

hand = nil

texts = {}

images = {}

top_level_menu_texts = {}

top_level_menu_x_offset = 540
top_level_menu_y_offset = 20

time_text = nil

-- TODO: THIS IS A TEST
items = {}

characters = {}

character_statuses = {}

function initialize()
    menu_background = objects.Image.new("ui.menu.background", 0, 0)

    font_name = "PixChicago"
    font_size = 10

    for index, menu in pairs(current_menu["options"]) do
        x = top_level_menu_x_offset
        y = top_level_menu_y_offset + (35 * (index - 1))

        table.insert(top_level_menu_texts, objects.Text.new(menu["label"], font_name, font_size, x, y, 255, 255, 255))
    end

    hand = objects.Image.new("cursor-right", top_level_menu_x_offset - 30, top_level_menu_y_offset + (35 * current_menu["current_option"]))

    texts = top_level_menu_texts

    time_text = objects.Text.new("", font_name, font_size, top_level_menu_x_offset, 440, 255, 255, 255)

    inventory = pause_menu_state:getParty():getInventory()

    inventory_items = inventory:getConsumables()

    print(inspect(inventory_items))

    for item, quantity in pairs(inventory_items) do
        print(item:getName())
        print(quantity)
    end

    characters = pause_menu_state:getPartyCharacters()

    y = 20

    for i, character in ipairs(characters) do
        name = character:getName()
        level = string.format("Level %9d", character:getLevel())
        hp = string.format("HP %4d/%4d", character:getCurrentHitPoints(), character:getMaxHitPoints())
        mp = string.format("MP   %3d/%3d", character:getCurrentMagicPoints(), character:getMaxMagicPoints())

        character_statuses[string.format("%s_name", character:getName())] = objects.Text.new(name, font_name, font_size, 30, y, 255, 255, 255)
        character_statuses[string.format("%s_level", character:getName())] = objects.Text.new(level, font_name, font_size, 80, y + 20, 255, 255, 255)
        character_statuses[string.format("%s_hp", character:getName())] = objects.Text.new(hp, font_name, font_size, 80, y + 40, 255, 255, 255)
        character_statuses[string.format("%s_mp", character:getName())] = objects.Text.new(mp, font_name, font_size, 80, y + 60, 255, 255, 255)

        y = y + 100
    end
end

function process_input(key_code)
    if key_code == CANCEL_KEY then
        pause_menu_state:pop()
    end

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

        hand:setPosition(top_level_menu_x_offset - 30, top_level_menu_y_offset + (35 * current_menu["current_option"]))
    end

    if key_code == DOWN_KEY then
        if current_menu["current_option"] < number_of_options - 1 then
            current_menu["current_option"] = current_menu["current_option"] + 1
        else
            current_menu["current_option"] = 0
        end

        hand:setPosition(top_level_menu_x_offset - 30, top_level_menu_y_offset + (35 * current_menu["current_option"]))
    end

    if key_code == CONFIRM_KEY then
        current_menu["selected_option"] = current_menu["current_option"]
    end
end

function update()
    if current_menu["selected_option"] == 7 then
        current_menu["selected_option"] = nil

        return "save_game_menu"
    end

    return "pause_menu"
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

    for k, v in pairs(character_statuses) do
        v:render()
    end

    hours, minutes, seconds = pause_menu_state:getClockTime()

    time = string.format("%02d:%02d:%02d", hours, minutes, seconds)

    time_text:setText(time)

    time_text:render()
end
