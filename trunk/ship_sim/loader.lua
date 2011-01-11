-- loader.lua --
-- loads packages and performs initial setup :

local package_list = {}
table.insert( package_list, "cgame");
table.insert( package_list, "shaders");

function reloadPackages()
	for i=1, #package_list do	package.loaded[ package_list[i] ] = nil;	end
	for i=1, #package_list do	require( package_list[i] );					end
end

reloadPackages()

--	initial setup :
input.setKeyboardScan	( true );
input.setMouseClip		( true );
input.setMouseRelative	( true );
input.setMouseHide		( true );