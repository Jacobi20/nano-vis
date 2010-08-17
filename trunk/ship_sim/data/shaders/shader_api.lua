-------------------------------------------------------------------------------
-- The MIT License
--
-- Copyright (c) 2010 IFMO/GameDev Studio
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.
-------------------------------------------------------------------------------


-------------------------------------------------------------------------------
--	API for shader creation :
-------------------------------------------------------------------------------


-------------------------------------------------------------------------------
--	API for shader creation :
-------------------------------------------------------------------------------

function init_shader_lib ()
	global_effect_list = { num = 0; }
end
	

function term_shader_lib ()
	global_effect_list = nil;
end


function postfix(path, postfix)
	local ext	=	string.sub(path, -3, -1);
	local name	=	string.sub(path,  0, -5);
	return name .. postfix .. "." .. ext;
end


function inject_surface_fx(injection)

	local effects = global_effect_list;

	if effects[injection] then
		return effects[injection];
	end
	
	effects[injection]  = string.format("shaders/surface%04i.fx", effects.num);
	effects.num = effects.num + 1;
	
	local text = "";
	
	for line in io.lines("../../engine/shaders/surface.fx") do 
		text = text..line.."\n";
	end
	
	text = string.gsub (text, '#pragma surface shader', injection);
	
	fout = io.open(effects[injection], "w");
	fout:write(text);
	fout:close();
	
	return effects[injection];
end

