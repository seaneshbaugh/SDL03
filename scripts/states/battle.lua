dofile "scripts/keys.lua"

-- Yes, I'm aware this is a mess of global state. I'm not sure what I want to do yet, so keeping this
-- so it has no structure is actually easier for now. As soon as I know what I'm doing this will be
-- cleaned up and turned into something a bit more organized. For now I can't assume that any bit of
-- structure will actually be helpful.

time = 0

background = nil

texts = {}

party = {}

monsters = {}

atb = {}

action_queue = {}

current_character = nil

current_action = nil

font_name = "PixChicago"

font_size = 10

character_statuses = {}

cursor_name = "ui.cursor-right"

current_monster_index = nil

hand = nil

menu_background = objects.Image.new("ui.menu.background", 0, 280)

function initialize()
    -- I don't think I like this.
    math.randomseed(os.time())

    background = objects.Image.new("battle.background", 0, 0)

    -- I don't know if I like how there's three tables, one for monsters, one for the party, and an awkard
    -- one that combines them both. But since Lua table values are always references it actually kinda ends
    -- up working just fine. I can use the combined list for all of the ATB stuff and then the separate
    -- lists for when appropriate.

    monsters = battle_state:getMonsters()

    for i, monster in ipairs(objects) do
        table.insert(atb, { character = monster, atb = monster:atbStart(), party = false })
    end

    party = battle_state:getParty()

    y = 300

    for i, character in ipairs(party) do
        table.insert(atb, { character = character, atb = character:atbStart(), party = true })

        status_text = string.format("%s: %d/%d", character:getName(), character:getCurrentHitPoints(), character:getMaxHitPoints())

        character_statuses[character:getName()] = objects.Text.new(status_text, font_name, font_size, 410, y, 255, 255, 255)

        y = y + 40
    end
end

function process_input(key_code)
    if current_character then
        -- This where all of the menu stuff will go. For now we only allow selecting a monster to attack.
        if key_code == CONFIRM_KEY then
            monster = monsters[current_monster_index]

            table.insert(action_queue, { action = "attack", source = current_character["character"], target = monster, message = string.format("%s attacks %s", current_character["character"]:getName(), monster:getName()) })

            current_character["atb"] = 0

            current_character = nil

            hand = nil
        end

        if key_code == LEFT_KEY then
            local length = #monsters

            m = nil

            local i = current_monster_index - 1

            while i >= 1 do
                if monsters[i]:getCurrentHitPoints() > 0 then
                    m = i

                    break
                end

                i = i - 1
            end

            if m == nil then
                i = length

                while i >= current_monster_index do
                    if monsters[i]:getCurrentHitPoints() > 0 then
                        m = i

                        break
                    end

                    i = i - 1
                end
            end

            current_monster_index = m

            hand:setPosition(90 + ((current_monster_index - 1) * 50), 200)
        end

        if key_code == RIGHT_KEY then
            local length = #monsters

            m = nil

            local i = current_monster_index + 1

            while i <= length do
                if monsters[i]:getCurrentHitPoints() > 0 then
                    m = i

                    break
                end

                i = i + 1
            end

            if m == nil then
                i = 1

                while i <= current_monster_index do
                    if monsters[i]:getCurrentHitPoints() > 0 then
                        m = i

                        break
                    end

                    i = i + 1
                end
            end

            current_monster_index = m

            hand:setPosition(90 + ((current_monster_index - 1) * 50), 200)
        end
    end

    return ""
end

function tick()
    for i, v in ipairs(atb) do
        if v["character"]:getCurrentHitPoints() > 0 and v["atb"] < 100 then
            -- TODO: Figure out a better equation here. This will break spectacularly
            -- when dexterity is over 100. Maybe some sort of log function so it
            -- asymptotes.
            v["atb"] = v["atb"] + v["character"]:getDexterity()

            if v["atb"] > 100 then
                v["atb"] = 100
            end
        end
    end
end

function update()
    m = 0

    for i, v in ipairs(monsters) do
        if v:getCurrentHitPoints() > 0 then
            m = m + 1
        end
    end

    if m == 0 then
        table.insert(action_queue, { action = "victory", source = nil, target = nil, message = "You are victorious!" })
    end

    time = time + 1

    if current_character == nil then
        if time % 10 == 0 then
            tick()
        end

        for i, v in ipairs(atb) do
            if v["atb"] >= 100 then
                if v["party"] then
                    if current_character == nil then
                        table.insert(action_queue, { action = "ready", source = v["character"], target = nil, message = string.format("%s is ready to attack.", v["character"]:getName()) })

                        current_character = v

                        x = 90

                        for i, v in ipairs(monsters) do
                            if v:getCurrentHitPoints() > 0 then
                                current_monster_index = i

                                break
                            end

                            x = x + 50
                        end

                        hand = objects.Image.new(cursor_name, x, 200)
                    end
                else
                    n = 0

                    for i, v in ipairs(party) do
                        n = n + 1
                    end

                    target = party[math.random(1, n)]

                    table.insert(action_queue, { action = "attack", source = v["character"], target = target, message = string.format("%s attacks %s.", v["character"]:getName(), target:getName()) })

                    v["atb"] = 0
                end
            end
        end
    end

    process_action_queue()

    for i, v in ipairs(party) do
        character_statuses[v:getName()]:setText(string.format("%s:    %d / %d", v:getName(), v:getCurrentHitPoints(), v:getMaxHitPoints()))
    end

    return ""
end

function process_action_queue()
    if current_action == nil then
        current_action = table.remove(action_queue, 1)

        if current_action then
            current_action["timer"] = 0

            print(current_action["message"])

            texts["current_action_message"] = objects.Text.new(current_action["message"], font_name, font_size, 25, 25, 255, 255, 0)

            -- Currently the only way I can think of making this work is a series of if statements checking the value
            -- of action. Eventually I would like something where the menu is actually labels for references to
            -- functions and that's what's passed along as the action. That way all that this has to do is call the
            -- function. I don't know, this is either gonna be cool or a total mess.
            if current_action["action"] == "attack" then
                current_action["target"]:damage(current_action["source"]:getStrength())

                print(string.format("%s now has %d HP", current_action["target"]:getName(), current_action["target"]:getCurrentHitPoints()))
            end
        end
    else
        current_action["timer"] = current_action["timer"] + 1

        -- 1.5 seconds, maybe make this configurable?
        if current_action["timer"] > 90 then
            -- I REALLY HATE THIS. But I can't think of any other way to force this to happen at the correct time.
            if current_action["action"] == "victory" then
                battle_state:pop()
            else
                texts["current_action_message"] = nil

                current_action = nil
            end
        end
    end
end

function render()
    if background then
        background:render()
    end

    if menu_background then
        menu_background:render()
    end

    -- Eventually I'm going to need to have all characters (in the party and monsters) be wrapped in another table
    -- with position data so that way I can calculate their positions based on their size and stuff during
    -- intitialization and then just loop over it here. As it stands right now all hardcoded and awkward.

    x = 120

    for i, v in ipairs(monsters) do
        if v:getCurrentHitPoints() > 0 then
            v:render(x, 200)
        end

        x = x + 50
    end

    y = 140

    for i, v in ipairs(party) do
        v:render(530, y)

        y = y + 50
    end

    for i, v in ipairs(texts) do
        v:render()
    end

    if texts["current_action_message"] then
        texts["current_action_message"]:render()
    end

    for k, v in pairs(character_statuses) do
        v:render()
    end

    if hand then
        hand:render()
    end
end
