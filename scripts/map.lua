dofile "sdl_keys.lua"

map_state = MapState(raw_map_state)

texts = {}

current_x = 0

current_y = 0

moving = false

x_velocity = 0

x_movement_offset = 0

y_velocity = 0

y_movement_offset = 0

current_map = nil

function initialize()
    font = map_state:getFont("DroidSans")
end

function after_map_load()
    current_map = GameMap(map_state:getCurrentMap())
end

function process_input(key_code)
    if moving == false then
        if key_code == SDLK_UP then
            if current_y > 0 then
                --current_y = current_y - 1
                y_velocity = 4

                y_movement_offset = 0
                
                moving = true
            end
        end
        
        if key_code == SDLK_DOWN then
            -- load map height here, for now assume 100 (with screen height of 15 tiles)
            if current_y < 85 then
                --current_y = current_y + 1
                y_velocity = -4
                
                y_movement_offset = 0
                
                moving = true
            end
        end
        
        if key_code == SDLK_LEFT then
            if current_x > 0 then
                --current_x = current_x - 1
                x_velocity = 4
                
                x_movement_offset = 0
                
                moving = true
            end
        end
        
        if key_code == SDLK_RIGHT then
            -- load height here, for now assume 100 (with screen width of 20 tiles)
            if current_x < 80 then
                --current_x = current_x + 1
                x_velocity = -4
                
                x_movement_offset = 0
                
                moving = true
            end
        end
    end

    return ""
end

function update()
    x_movement_offset = x_movement_offset + x_velocity
    
    y_movement_offset = y_movement_offset + y_velocity

    if x_velocity < 0 and x_movement_offset <= -32 then
        x_velocity = 0
        
        x_movement_offset = 0
        
        current_x = current_x + 1
        
        moving = false
    end

    if x_velocity > 0 and x_movement_offset >= 32 then
        x_velocity = 0
        
        x_movement_offset = 0
        
        current_x = current_x - 1
        
        moving = false
    end
    
    if y_velocity < 0 and y_movement_offset <= -32 then
        y_velocity = 0
        
        y_movement_offset = 0
        
        current_y = current_y + 1
        
        moving = false
    end

    if y_velocity > 0 and y_movement_offset >= 32 then
        y_velocity = 0
        
        y_movement_offset = 0
        
        current_y = current_y - 1
        
        moving = false
    end
end

function render()
    current_map:render(current_x, current_y, x_movement_offset, y_movement_offset)

    for i, v in ipairs(texts) do
        v:render()
    end
end
