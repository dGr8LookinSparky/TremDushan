function ClientBegin(clientNum) 
    game.Print("ClientBegin of global game.lua")    
end 
function ClientConnect(clientNum, firstTime, isBot) 
    game.Print("ClientConnect of global game.lua") 
end
function ClientDisconnect(clientNum) 
    game.Print("ClientDisconnect of global game.lua") 
end
function ClientUserinfoChanged(clientNum) 
    game.Print("ClientUserinfoChanged of global game.lua") 
end
function ClientSpawn(clientNum)
    game.Print("ClientSpawn of global game.lua")  
end
function ClientCommand(clientNum, command)
    game.Print("ClientCommand of global game.lua")  
end







