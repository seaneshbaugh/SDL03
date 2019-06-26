dofile "scripts/keys.lua"

DIRECTION_DOWN = 0
DIRECTION_LEFT = 1
DIRECTION_RIGHT = 2
DIRECTION_UP = 3

texts = {}

-- This should be rolled into some sort of table. LOL global variables.
player_current_x = 15

player_current_y = 10

screen_moving = false

screen_x_velocity = 0

screen_x_movement_offset = 0

screen_y_velocity = 0

screen_y_movement_offset = 0

player_moving = false

player_x_velocity = 0

player_x_movement_offset = 0

player_y_velocity = 0

player_y_movement_offset = 0

player_screen_x_position = 320

player_screen_y_position = 208

player_direction = DIRECTION_DOWN

current_map = map_state:getCurrentMap()

player = nil

function initialize()
    player_sprite_name = map_state:getPlayerSpriteName()
    
    player = objects.Image(player_sprite_name, 320, 224)
    
    math.randomseed(os.time())
end

function after_map_load()
    current_map = map_state:getCurrentMap()
end

function process_input(key_code)
    if screen_moving == false and player_moving == false then
        if key_code == UP_KEY then
            player_direction = DIRECTION_UP

            -- use map height here, for now assume 100 (with screen height of 15 tiles)
            if current_map:getWalkability(player_current_x, player_current_y - 1) then
                if player_current_y > 7 and player_current_y < 93 then
                    screen_y_velocity = 4
                    
                    screen_y_movement_offset = 0
                    
                    screen_moving = true
                else
                    player_y_velocity = -4
                    
                    player_y_movement_offset = 0
                    
                    player_moving = true
                end
            end
        end
        
        if key_code == DOWN_KEY then
            player_direction = DIRECTION_DOWN

            -- use map height here, for now assume 100 (with screen height of 15 tiles)
            if current_map:getWalkability(player_current_x, player_current_y + 1) then
                if player_current_y > 6 and player_current_y < 92 then
                    screen_y_velocity = -4
                    
                    screen_y_movement_offset = 0
                    
                    screen_moving = true
                else
                    player_y_velocity = 4
                    
                    player_y_movement_offset = 0
                    
                    player_moving = true
                end
            end
        end
        
        if key_code == LEFT_KEY then
            player_direction = DIRECTION_LEFT

            -- use map height here, for now assume 100 (with screen width of 20 tiles)
            if current_map:getWalkability(player_current_x - 1, player_current_y) then
                if player_current_x > 10 and player_current_x <= 90 then
                    screen_x_velocity = 4
                    
                    screen_x_movement_offset = 0
                    
                    screen_moving = true
                else
                    player_x_velocity = -4
                    
                    player_x_movement_offset = 0
                    
                    player_moving = true
                end
            end
        end
        
        if key_code == RIGHT_KEY then
            player_direction = DIRECTION_RIGHT

            -- use map height here, for now assume 100 (with screen width of 20 tiles)
            if current_map:getWalkability(player_current_x + 1, player_current_y) then
                if player_current_x >= 10 and player_current_x < 90 then
                    screen_x_velocity = -4
                    
                    screen_x_movement_offset = 0
                    
                    screen_moving = true
                else
                    player_x_velocity = 4
                    
                    player_x_movement_offset = 0
                    
                    player_moving = true
                end
            end
        end
    end

    return "map"
end

function step(x, y)
    objects = current_map:getObjects(x, y)

    for i, object in ipairs(objects) do
        object_type = object:type()

        if object_type == "map" then
            map = object:getProperty("map")

            map_state:loadMap("resources/maps/" .. map .. ".json")

            current_map = map_state:getCurrentMap()

            player_current_x = tonumber(object:getProperty("x"))

            player_current_y = tonumber(object:getProperty("y"))

            player_screen_x_position = 320

            player_screen_y_position = 224

            if player_current_x < 7 then
                player_screen_x_position = player_current_x * 32
            end

            if player_current_x > 92 then
                player_screen_x_position = (100 - player_current_x) * 32
            end

            if player_current_y < 7 then
                player_screen_y_position = player_current_y * 32
            end

            if player_current_y > 92 then
                player_screen_y_position = (100 - player_current_y) * 32
            end

            player:setPosition(player_screen_x_position, player_screen_y_position)
        end
        
        if object_type == "encounter_area" then
            map_state:setCurrentEncounterArea(object)

            -- This should be determined by a method on the encounter area. Each one should define its own
            -- probability for a random encounter.
            if math.random(0, 10) == 0 then
                return "battle"
            end
        end
    end

    return "map"
end

function update()
    done_moving = false

    screen_x_movement_offset = screen_x_movement_offset + screen_x_velocity
    
    screen_y_movement_offset = screen_y_movement_offset + screen_y_velocity

    if screen_x_velocity < 0 and screen_x_movement_offset <= -32 then
        screen_x_velocity = 0
        
        screen_x_movement_offset = 0
        
        player_current_x = player_current_x + 1
        
        screen_moving = false
        
        done_moving = true
    end

    if screen_x_velocity > 0 and screen_x_movement_offset >= 32 then
        screen_x_velocity = 0
        
        screen_x_movement_offset = 0
        
        player_current_x = player_current_x - 1
        
        screen_moving = false
        
        done_moving = true
    end
    
    if screen_y_velocity < 0 and screen_y_movement_offset <= -32 then
        screen_y_velocity = 0
        
        screen_y_movement_offset = 0
        
        player_current_y = player_current_y + 1
        
        screen_moving = false
        
        done_moving = true
    end

    if screen_y_velocity > 0 and screen_y_movement_offset >= 32 then
        screen_y_velocity = 0
        
        screen_y_movement_offset = 0
        
        player_current_y = player_current_y - 1
        
        screen_moving = false

        done_moving = true
    end

    player_screen_x_position = player_screen_x_position + player_x_velocity

    player_x_movement_offset = player_x_movement_offset + player_x_velocity

    player_screen_y_position = player_screen_y_position + player_y_velocity

    player_y_movement_offset = player_y_movement_offset + player_y_velocity

    player:setPosition(player_screen_x_position, player_screen_y_position)

    if player_x_velocity < 0 and player_x_movement_offset <= -32 then
        player_x_velocity = 0
        
        player_x_movement_offset = 0
        
        player_current_x = player_current_x - 1
        
        player_moving = false
        
        done_moving = true
    end
    
    if player_x_velocity > 0 and player_x_movement_offset >= 32 then
        player_x_velocity = 0
        
        player_x_movement_offset = 0
        
        player_current_x = player_current_x + 1
        
        player_moving = false
        
        done_moving = true
    end

    if player_y_velocity < 0 and player_y_movement_offset <= -32 then
        player_y_velocity = 0
        
        player_y_movement_offset = 0
        
        player_current_y = player_current_y - 1
        
        player_moving = false
        
        done_moving = true
    end
    
    if player_y_velocity > 0 and player_y_movement_offset >= 32 then
        player_y_velocity = 0
        
        player_y_movement_offset = 0
        
        player_current_y = player_current_y + 1
        
        player_moving = false
        
        done_moving = true
    end

    if done_moving then
        return step(player_current_x, player_current_y)
    else
        return "map"
    end
end

function render()
    if player_current_x - 10 < 0 then
        top_x = 0
    else
        top_x = player_current_x - 10
    end
    
    if player_current_y - 7 < 0 then
        top_y = 0
    else
        top_y = player_current_y - 7
    end

    current_map:render(top_x, top_y, screen_x_movement_offset, screen_y_movement_offset)

    player_crop_x = 0

    if screen_moving or player_moving then
        sx = math.abs(player_x_movement_offset) > 0 and math.abs(player_x_movement_offset) % 8 == 0
        sy = math.abs(player_y_movement_offset) > 0 and math.abs(player_y_movement_offset) % 8 == 0

        if sx or sy then
            player_crop_x = 32
        else
            player_crop_x = 64
        end
    end

    player:renderWithClip(player_crop_x, 48 * player_direction, 32, 48)

    for i, v in ipairs(texts) do
        v:render()
    end
end
