
require "lib/helper"

-- main funcs
function InitGame( levelTime, randomSeed, restart )
    Printf("InitGame [%d] [%d] [%d]", levelTime, randomSeed, restart )
end
function RunFrame(levelTime) 
       --if math.mod( levelTime, 10000 ) == 0 then
             --game.Print("RunFrame ", levelTime, "")
       --end
end
function ShutdownGame( restart )
    game.Print("ShutdownGame of global game.lua") 
end
function Quit()
    game.Print("Quit of global game.lua") 
end
