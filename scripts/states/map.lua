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

function update(delta_time)
    if pause_menu_opened then
        pause_menu_opened = false

        return "pause_menu"
    end

    return "map"
end
