
-----------------------------------------------------------------------
--	setup and control :
-----------------------------------------------------------------------

input.unbindall()
input.bind ("F7", 	"do_rolling()");
input.bind ("F5", 	"dofile('run.lua')");
input.bind ("F6", 	"rs.reloadShaders()");
input.bind ("F10", 	"core.quit()");
input.bind ("PGUP", 	"_DistInc()"	,"DistInc()"	);
input.bind ("PGDN", 	"_DistDec()"	,"DistDec()"	);
input.bind ("RIGHT", 	"_YawDec()"		,"YawDec()"		);
input.bind ("LEFT", 	"_YawInc()"		,"YawInc()"		);
input.bind ("UP", 		"_PitchInc()"	,"PitchInc()"	);
input.bind ("DOWN", 	"_PitchDec()"	,"PitchDec()"	);

input.bind ("S",	"_ShipFW()",	"ShipFW()");
input.bind ("Z",  	"_ShipBW()",	"ShipBW()");
input.bind ("A",	"_ShipSL()",	"ShipSL()");
input.bind ("X",  	"_ShipSR()",	"ShipSR()");

function toggle_wireframe()
	user.fr_wireframe	=	not user.fr_wireframe;
	print( user.fr_wireframe );
end

input.bind ("F1", "show_info()"  		);
input.bind ("F2", "toggle_wireframe()"  );

input.bind ("N", "state.submersion 		= not state.submersion" );
input.bind ("M", "state.sunking 		= not state.sunking" );

input.bind("1", "dofile('exp01/exp01.lua')");
input.bind("2", "dofile('exp02/exp02.lua')");
input.bind("3", "dofile('exp03/exp03.lua')");
input.bind("4", "dofile('exp04/exp04.lua')");

-- local avc = vmath.vec4(1,2,3,1);
-- local x = avc:x();

state = {
	yaw_inc		=	false;
	yaw_dec		=	false;
	roll_inc	=	false;
	roll_dec	=	false;
	pitch_inc	=	false;
	pitch_dec	=	false;
	dist_inc	=	false;
	dist_dec	=	false;
	yaw		=	-180;
	roll	=	0;
	pitch	=	20;
	dist	=	20;
	
	submersion	=	false;
	sunking		=	false;
	
	ship_fw	=	false;	--	forward
	ship_bw	=	false;	--	backward
	ship_sl	=	false;	--	steer left
	ship_sr	=	false;	--	steer right
}   

function _YawInc()   state.yaw_inc		=	true; end
function _YawDec()   state.yaw_dec		=	true; end
function _RollInc()  state.roll_inc		=	true; end
function _RollDec()  state.roll_dec		=	true; end
function _PitchInc() state.pitch_inc	=	true; end
function _PitchDec() state.pitch_dec	=	true; end
function _DistInc()  state.dist_inc		=	true; end
function _DistDec()  state.dist_dec		=	true; end

function _ShipFW()	state.ship_fw		=	true; end;
function _ShipBW()	state.ship_bw		=	true; end;
function _ShipSL()	state.ship_sl		=	true; end;
function _ShipSR()	state.ship_sr		=	true; end;
                   
function YawInc()   state.yaw_inc		=	false; end
function YawDec()   state.yaw_dec		=	false; end
function RollInc()  state.roll_inc		=	false; end
function RollDec()  state.roll_dec		=	false; end
function PitchInc() state.pitch_inc		=	false; end
function PitchDec() state.pitch_dec		=	false; end
function DistInc()  state.dist_inc		=	false; end
function DistDec()  state.dist_dec		=	false; end

function ShipFW()	state.ship_fw		=	false; end;
function ShipBW()	state.ship_bw		=	false; end;
function ShipSL()	state.ship_sl		=	false; end;
function ShipSR()	state.ship_sr		=	false; end;
                                          

game_time = 0;

-------------------------------------------------------------------------------
--	create ship :
-------------------------------------------------------------------------------

naval.remove_all_ships();
naval.set_wind(15);

function show_info()
	local x,y,z;
	local yaw, pitch, roll;
	x,y,z 			=	uboat:get_position();
	yaw,pitch,roll	=	uboat:get_angles()
	
	print("---- ship info ----");
	print("position : ", x, y, z);
	print("yaw      : ", yaw 	);
	print("pitch    : ", pitch 	);
	print("roll     : ", roll 	);
	print("");
end

function create_uboat()
	print("");
	print("---- creating U-boat ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 3 );
	
	ship:set_vis_mesh	( "uboat.esx|boat1"			);
	ship:set_hdf_mesh	( "uboat.esx|flowsurf2" 		);
	ship:set_hsf_mesh	( "uboat.esx|flowsurf2" 		);
	ship:make_rigidbody	( "uboat.esx|stat", 1805000	);
	
	ship:set_position	( 20, 20, 0 );	
	ship:set_angles		( 0, 0, 0 );
	ship:set_cmass		( 0,0,-0.5 );
	
	ship:build_voxels	( "uboat.esx|flowsurf2", 1	);
	ship:build_surf_dxdy( "uboat.esx|flowsurf2", 3, 0.1	);
	
	print("---- done ----");
	print("");
	return ship;
end


function create_cutter()
	print("");
	print("---- creating Coast Guard Ship ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 1.0 );
	
	ship:set_vis_mesh	( "boat.esx|boat1"			);
	ship:set_hdf_mesh	( "boat.esx|flow" 			);
	ship:set_hsf_mesh	( "boat.esx|flow" 			);
	ship:make_rigidbody	( "boat.esx|stat", 400000	);
	
	ship:set_position	( -100, 0, 0 );	
	ship:set_angles		( 30, 0, 5);
	ship:set_cmass		( 0, 0, 0 );
	
	ship:build_voxels	( "boat.esx|flow", 1	);
	ship:build_surf_dxdy( "boat.esx|flow", 0.1, 0.1	);
	
	print("---- done ----");
	print("");
	return ship;
end

function create_ssn668()
	print("");
	print("---- creating SSN-668 'Los Angeles' ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 0.2 );
	
	ship:set_vis_mesh	( "ssn668.esx|vismesh"			);
	ship:set_hdf_mesh	( "ssn668.esx|hydromesh" 		);
	ship:set_hsf_mesh	( "ssn668.esx|hydromesh" 		);
	ship:make_rigidbody	( "ssn668.esx|physmesh", 6000000	);
	
	ship:set_position	( -30, -30, -3.5 );	
	ship:set_angles		( 90, 0, 5 );
	ship:set_cmass		( 0, 0, 0 );
	
	ship:build_voxels	( "ssn668.esx|hydromesh", 2	);
	ship:build_surf_dxdy( "ssn668.esx|hydromesh", 3, 0.1	);
	
	print("---- done ----");
	print("");
	return ship;
end

function create_box()
	print("");
	print("---- creating Box ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 5 );
	
	ship:set_vis_mesh	( "box.esx|box"				);
	ship:set_hdf_mesh	( "box.esx|box" 			);
	ship:set_hsf_mesh	( "box.esx|box" 			);
	ship:make_rigidbody	( "box.esx|box",  8000000	);
	
	ship:set_position	( 0, 0, 0 );	
	ship:set_angles		( 0, 0, 90 );
	ship:set_cmass		( 0, 0, 0 );
	
	ship:build_voxels	( "box.esx|box", 2	);
	ship:build_surf_dxdy( "box.esx|box", 1, 2 );
	
	print("---- done ----");
	print("");
	return ship;
end


local rolling_log1 = io.open("rolling1.log", "w");
local rolling_log2 = io.open("rolling2.log", "w");
--local rolling_log = io.open("cutter_calibration_roll_5_deg_kappa.log", "w");


--uboat	=	create_ssn668();
--uboat	=	create_cutter();
uboat2	=	create_uboat();
--uboat	=	create_box();

uboat	=	create_cutter();

user.ship_hsf_method	=	"hxfse";
--ship_hsf_method	=	"surface";


-------------------------------------------------------------------------------
--	frame :
-------------------------------------------------------------------------------

function DriveShip()
	local yaw, pitch, roll = uboat:get_angles();

	local force = 1600000;
	
	if (state.ship_sl) then yaw = yaw - 10; end
	if (state.ship_sr) then yaw = yaw + 10; end
	
	local fx	=	force * math.cos(math.rad(yaw)) * 1;--math.cos(-pitch);
	local fy	=	force * math.sin(math.rad(yaw)) * 1;--math.cos(-pitch);
	local fz	=	force * 0;--math.sin(-pitch);
	
	--local x,y,z	=	uboat:get_position(-50,0,-1);
	local x,y,z	=	-50, 0, -1.5;

	if state.ship_fw then uboat:add_force( vmath.vec4( fx, fy, fz, 0), vmath.vec4(x,y,z,1), true); end;
	if state.ship_bw then uboat:add_force( vmath.vec4(-fx,-fy,-fz, 0), vmath.vec4(x,y,z,1), true); end;
	
	if state.submersion then
		uboat:add_force( vmath.vec4(0,0,-16537500,0), vmath.vec4(0,0,0,1), true);
	end
	if state.sunking then
		uboat:add_force( vmath.vec4(0,0,-2000000,0), vmath.vec4(0,0,0,1), true);
	end
end



--------------------------------------------------
wind = 0;

function sim_ship(uboat, logfile, dtime)
	if uboat  then 
		uboat:simulate(dtime);  
		

		local yaw, pitch, roll 	= uboat:get_angles();
		local x, y, z 			= uboat:get_position();

		uboat:set_angles(yaw, pitch, roll);
		uboat:set_position(x,y,z);

		yaw		=	math.rad(yaw);
		pitch	=	math.rad(pitch);
		roll	=	math.rad(roll);
		
		-- local wave_offset0		= naval.get_wave_offset(x,y,0);
		-- local wave_offset1		= naval.get_wave_offset(x+1/16,y,0);
		local step              = 0.001
		local wave_offset0		= naval.get_wave_offset(-step,0,0);
		local wave_offset1		= naval.get_wave_offset( step,0,0);
		local wave_offset		= (wave_offset0 + wave_offset1)/2;
		local wave_slope		= math.atan((wave_offset1 - wave_offset0)/(2*step));
		local out				= string.format("%f %f %f %f %f %f %f %f", yaw, pitch, roll, x, y, z, wave_offset, wave_slope);
		logfile:write(out.."\n");
		logfile:flush();

	end;
end


local filtered_view = {
		x = 40;
		y = 40;
		z = 40;
	}
	
function filter(a, b, f) 
	return a * (1-f) + b * f;
end	

function sci_frame(dtime)

--	SCI_ShipForce( vmath.vec4(0, 0,-5000000,0), vmath.vec4(50,0,0,1));
	core.debug_string("FPS", 1 / dtime);

	local	rotation = 60;
	game_time = game_time + dtime;
	
	dtime = 0.03;
	
	DriveShip();

	sim_ship(uboat,  rolling_log1, dtime);
	sim_ship(uboat2, rolling_log2, dtime);
	
	if cutter then 
		cutter:simulate(dtime); 
	end
	
	--uboat:add_force( vmath.vec4(0,0,-2000000,0), vmath.vec4(0,0,0,1), true);
	

	if state.yaw_inc	then	state.yaw	=	state.yaw 	+ dtime * rotation; end;
	if state.yaw_dec	then	state.yaw	=	state.yaw 	- dtime * rotation; end;
	if state.roll_inc	then	state.roll	=	state.roll 	+ dtime * rotation; end;
	if state.roll_dec	then	state.roll	=	state.roll 	- dtime * rotation; end;
	if state.pitch_inc	then	state.pitch	=	state.pitch	+ dtime * rotation; end;
	if state.pitch_dec	then	state.pitch	=	state.pitch	- dtime * rotation; end;
	if state.dist_inc	then	state.dist	=	state.dist	* 1.03;	end;
	if state.dist_dec	then	state.dist	=	state.dist	/ 1.03; 	end;
	
	--
	--	view stuff
	--
	if uboat and false then
	
		local yaw, pitch, roll 	= uboat:get_angles();
		
		--local x, y, z 			= uboat:get_position(1.5,0.5,8.8);
		local x, y, z 			= uboat:get_position(8.0,-3.5,7.8);
		filtered_view.x = filter(filtered_view.x, x, 0.1);
		filtered_view.y = filter(filtered_view.y, y, 0.1);
		filtered_view.z = filter(filtered_view.z, z, 0.1);
		local x2 = filtered_view.x;
		local y2 = filtered_view.y;
		local z2 = filtered_view.z;
		naval.set_view(120, vmath.vec4(x2,y2,z2,1), yaw + state.yaw, pitch + state.pitch, roll + state.roll);
	else
		local yaw	= 	math.rad(state.yaw	);
		local pitch	= 	math.rad(state.pitch	);
		local roll	= 	math.rad(state.roll	);
		local x	 	=	-state.dist * math.cos(yaw) * math.cos(-pitch);
		local y		=	-state.dist * math.sin(yaw) * math.cos(-pitch);
		local z		=	-state.dist * math.sin(-pitch);
		naval.set_view(120, vmath.vec4(x,y,z,1), state.yaw, state.pitch, state.roll);
	end

end

