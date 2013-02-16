--------------------------------------------------------------------------
--	Legends Of Drakan 2013
--	Created by Jannik Mewes
--------------------------------------------------------------------------
--	$Id$
--	$DateTime$
--	Description: This node interpolates between dayValue and nightValue (both need to be the same var type) based on TimeOfDay.
--  
--------------------------------------------------------------------------
--  History:
--  - 14/02/2013   : Created by Jannik Mewes
--
--------------------------------------------------------------------------

ScriptCommand = {
	Category = "customTools",
	Inputs = {"dayValue","nightValue",{"nightBegin","float"},{"nightEnd","float"},{"fadeDuration","float"}},
	Outputs = {{"Done","bool"},{"result","bool"}},
	Implementation=function(bActivate,sCommand)
		--if(type(vec3DayValue) != "number" type(bDayValue) != type(bNightValue)) then return 0;
		--local t = type(bDayValue);
		
		if()
		
		if(type(bDayValue) != type(bNightValue)) then return 0;
		if(sCommand~="")then
			local f=loadstring(sCommand);
			if (f~=nil) then
				f();
				return 1;
			else
				return 0;
			end;
		else
			return 0;
		end;
	end;
} 


