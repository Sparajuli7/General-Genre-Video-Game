-- ai.lua
function decide_unit_action(unit, map, resources)
    -- Simple AI: Move unit randomly or collect resources
    local moves = {"up", "down", "left", "right"}
    
    -- If unit is near a resource, collect it
    if resources[unit.x][unit.y] then
        return "collect"
    end

    -- Random move if no resource nearby
    local move = moves[math.random(1, #moves)]
    return move
end
