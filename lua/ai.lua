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
    -- Get units owned by opposing player
    opponentUnits = strategy.playerGetUnits(opponentId)
    -- Get units owned by current player
    units = strategy.playerGetUnits(playerId)
    -- Get cities owned by current player
    cities = strategy.playerGetCities(playerId)

    -- Each unit should move towards the opposing player's city, attacking if there is a unit in the way
    for _, unit in ipairs(units) do
        currentTile = strategy.unitGetMaptile(unit)
        targetTile = strategy.cityGetMaptile(opponentCities[1])
        distance, path = strategy.maptilePath(currentTile, targetTile)

        for _, enemy in ipairs(opponentUnits) do
            if (currentTile == strategy.unitGetMaptile(enemy)) then
                strategy.makeMove({2, unit, enemy})
            end
        end

        if (path[1] ~= nil) then
            strategy.makeMove({1, unit, path[1]})
        end
    end

    -- Each city should attempt to produce a unit
    for _, city in ipairs(cities) do
        if (math.random(5) == 1) then
            strategy.makeMove({3, city, 0})
        end
    end

    return
end
