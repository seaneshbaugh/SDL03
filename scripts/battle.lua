dofile "keys.lua"

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

function initialize()
    background = GameImage(battle_state:loadTexture("background", "battle-background-001.png"), 0, 0)

    --table.insert(texts, GameText("Battle State: 5 seconds remaining...", font, 200, 150, 255, 255, 0))

    -- I don't think I like this
    math.randomseed(os.time())
end

function after_battle_load()
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
        -- This where all of the menu stuff will go.
        if key_code == CONFIRM_KEY then
            monster = nil

            for i, v in ipairs(monsters) do
                if v:getCurrentHitPoints() > 0 then
                    monster = v
                end
            end

            table.insert(action_queue, { action = "attack", source = current_character["character"], target = monster, message = string.format("%s attacks %s", current_character["character"]:getName(), monster:getName()) })

            current_character["atb"] = 0

            current_character = nil
        end
    end

    return ""
end

function tick()
    for i, v in ipairs(atb) do
        if v["character"]:getCurrentHitPoints() > 0 && v["atb"] < 100 then
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
                    end
                else
                    -- This is where enemy actions would be added to the action queue.
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

            -- This is where the action would actually happen.
            if current_action["action"] == "attack" then
                current_action["target"]:damage(current_action["source"]:getStrength())

                print(string.format("%s now has %d HP", current_action["target"]:getName(), current_action["target"]:getCurrentHitPoints()))
            end
        end
    else
        current_action["timer"] = current_action["timer"] + 1

        -- 1.5 seconds, maybe make this configurable?
        if current_action["timer"] > 90 then
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
end
