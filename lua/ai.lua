Move = 1
Attack = 2
MakeUnit = 3

function think(playerId)
    -- Get opposing players uuid
    players = strategy.getPlayers()
    for _, v in ipairs(players) do
        if (v ~= playerId) then opponentId = v end
    end

    -- Get city locations of opposing player
    opponentCities = strategy.playerGetCities(opponentId)
    -- Get units owned by current player
    units = strategy.playerGetUnits(playerId)
    -- Get cities owned by current player
    cities = strategy.playerGetCities(playerId)

    -- Each unit should move towards the opposing player's city, attacking if there is a unit in the way
    for _, unit in ipairs(units) do
        currentTile = strategy.unitGetMaptile(unit)
        targetTile = strategy.cityGetMaptile(opponentCities[1])
        distance, path = strategy.maptilePath(currentTile, targetTile)
        if (path[1] ~= nil) then
            strategy.makeMove({1, unit, path[1]})
        end
    end

    return
end
