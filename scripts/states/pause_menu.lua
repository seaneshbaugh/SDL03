dofile "scripts/keys.lua"

menu_background = nil

function initialize()
    menu_background = objects.Image.new("ui.menu.background", 0, 0)
end

function process_input(key_code)
    if key_code == CANCEL_KEY then
        pause_menu_state:pop()

        return ""
    end

    return "pause_menu"
end

function update()
end

function render()
    if menu_background then
        menu_background:render()
    end
end
