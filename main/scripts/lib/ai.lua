

-- ai funcs
function AI_Spawn( name, pos )
    	Printf("spawning ai: [%s]", name )
	-- create new generic entity and set classname to ai
	ent = entity.Spawn()
	ent:SetClassName("ai")
	ent:SetOrigin(pos)
	
	-- make it think!
	ai.Register(ent:GetNumber(), name)
	ai.Spawn(ent:GetNumber())
	if(ent:GetClassName() == "freed") then -- something went wrong
		return 0;
	end

	return ent
end

