dofile "scripts/keys.lua"

DIRECTION_DOWN = 0
DIRECTION_LEFT = 1
DIRECTION_RIGHT = 2
DIRECTION_UP = 3

texts = {}

current_map = map_state:getCurrentMap()

player = nil

pause_menu_opened = false

function initialize()
    math.randomseed(os.time())

    -- map_state:step(player_current_x, player_current_y)
end

function after_map_load()
    current_map = map_state:getCurrentMap()
end

function process_input(key_code)
    if key_code == MENU_KEY then
        pause_menu_opened = true
    end

    return ""
end

function step(x, y)
    objects = current_map:getObjects(x, y)

    map_state:step(x, y)

    for i, object in ipairs(objects) do
        object_type = object:getType()

        if object_type == "map" then
            map = object:getProperty("map")

            map_state:loadMap("resources/maps/" .. map .. ".json")

            current_map = map_state:getCurrentMap()

            -- map_state:step(player_current_x, player_current_y)
        end

        if object_type == "encounter_area" then
            map_state:setCurrentMapEncounterArea(object)

            -- This should be determined by a method on the encounter area. Each one should define its own
            -- probability for a random encounter.
            if math.random(0, 10) == 0 then
                return "battle"
            end
        end
    end

    return "map"
end

function update(delta_time)
    if pause_menu_opened then
        pause_menu_opened = false

        return "pause_menu"
    end

    return "map"
end

function render()
    for i, v in ipairs(texts) do
        v:render()
    end
end
