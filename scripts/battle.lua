dofile "keys.lua"

-- Yes, I'm aware this is a mess of global state. I'm not sure what I want to do yet, so keeping this
-- so it has no structure is actually easier for now. As soon as I know what I'm doing this will be
-- cleaned up and turned into something a bit more organized. For now I can't assume that any bit of
-- structure will actually be helpful.

battle_state = BattleState(raw_battle_state)

time = 0

font = nil

background = nil

texts = {}

party = {}

monsters = {}

atb = {}

action_queue = {}

current_character = nil

current_action = nil

font = battle_state:getFont("DroidSans")

character_statuses = {}

cursor = battle_state:getTexture("cursor-right")

current_monster_index = nil

hand = nil

function initialize()
    background = GameImage(battle_state:loadTexture("background", "battle-background-001.png"), 0, 0)

    -- I don't think I like this.
    math.randomseed(os.time())
end

function after_battle_load()
    -- I don't know if I like how there's three tables, one for monsters, one for the party, and an awkard
    -- one that combines them both. But since Lua table values are always references it actually kinda ends
    -- up working just fine. I can use the combined list for all of the ATB stuff and then the separate
    -- lists for when appropriate.

    objects = battle_state:getMonsters()

    for i, v in ipairs(objects) do
        monster = GameMonster(v)

        table.insert(monsters, monster)

        table.insert(atb, { character = monster, atb = monster:atbStart(), party = false })
    end

    objects = battle_state:getParty()

    y = 400

    for i, v in ipairs(objects) do
        character = GameCharacter(v)

        table.insert(party, character)

        table.insert(atb, { character = character, atb = character:atbStart(), party = true })

        character_statuses[character:getName()] = GameText(string.format("%s: %d/%d", character:getName(), character:getCurrentHitPoints(), character:getMaxHitPoints()), font, 300, y, 255, 255, 255)

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

                        hand = GameImage(cursor, x, 200)
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

            texts["current_action_message"] = GameText(current_action["message"], font, 25, 25, 255, 255, 0)

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

    y = 200

    for i, v in ipairs(party) do
        v:render(400, y)

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
