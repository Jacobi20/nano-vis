
-----------------------------------------------------------------------
--	setup and control :
-----------------------------------------------------------------------

input.unbindall()
input.bind ("F7", 	"do_rolling()");
input.bind ("F5", 	"dofile('run.lua')");
input.bind ("F6", 	"rs.reload_shaders()");
input.bind ("F10", 	"core.quit()");
input.bind ("PGUP", 	"_DistInc()");
input.bind ("PGDN", 	"_DistDec()");
input.bind ("RIGHT", 	"_YawDec()");
input.bind ("LEFT", 	"_YawInc()");
input.bind ("UP", 		"_PitchInc()");
input.bind ("DOWN", 	"_PitchDec()");

input.bind ("S",	"_ShipFW()");
input.bind ("Z",  "_ShipBW()");
input.bind ("A",	"_ShipSL()");
input.bind ("X",  "_ShipSR()");

input.bind ("F1", "show_info()"  	);
input.bind ("F2", "setup_rolling_on_silent_water()"  	);
input.bind ("F3", "setup_rolling_on_sin_wave()"  		);
input.bind ("F4", "setup_rolling_on_wind_wave()"  		);

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
	yaw		=	-125;
	roll	=	0;
	pitch	=	20;
	dist	=	50;
	
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
                                          
-------------------------------------------------------------------------------
--	create ship :
-------------------------------------------------------------------------------

game_time = 0;

user.ship_hsf_method	=	"hxfse";

naval.remove_all_ships();
naval.set_wind(0);

function create_cutter(roll, dens)
	print("");
	print("---- creating Coast Guard Ship ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 1.0 );
	
	ship:set_vis_mesh	( "boat.esx|boat1"			);
	ship:set_hdf_mesh	( "boat.esx|flow" 			);
	ship:set_hsf_mesh	( "boat.esx|flow" 			);
	ship:make_rigidbody	( "boat.esx|stat", 400000	);
	
	ship:set_position	( 0, 0, 0.101+0.0 );	
	ship:set_angles		( 90, -0.869, 0);
	ship:set_cmass		( 0, 0, 0 );
	
	ship:build_voxels	( "boat.esx|flow", 1	);
	ship:build_surf_dxdy( "boat.esx|flow", dens, 0.1	);
	
	print("---- done ----");
	print("");
	return ship;
end

ship = create_cutter(0,10);

local rolling_log = io.open("rolling1.log", "w");


-------------------------------------------------------------------------------
--	frame :
-------------------------------------------------------------------------------

function setup_rolling_on_silent_water()
	print("---------------------------------------");
	print("  Rolling on silent water");
	print("---------------------------------------");
	
	rolling_log = io.open("silent_water.log", "w");
	
	naval.set_wave(0,0);
	naval.set_wind(0);
	
	ship:dispose();
	ship	=	create_cutter(5,10);
end


function setup_rolling_on_sin_wave()
	print("---------------------------------------");
	print("  Rolling on sin wave");
	print("---------------------------------------");
	
	rolling_log = io.open("sin_wave.log", "w");

	naval.set_wave(1.5	,0.15);
	
	ship:dispose();
	ship	=	create_cutter(0,3);
end


function setup_rolling_on_wind_wave()
	print("---------------------------------------");
	print("  Rolling on sin wave");
	print("---------------------------------------");

	rolling_log = io.open("wind_wave.log", "w");
	
	naval.set_wind(10);
	
	ship:dispose();
	ship	=	create_cutter(0,20);
	ship:set_angles(45,0,0);
end


--setup_rolling_on_silent_water()
--setup_rolling_on_sin_wave()
setup_rolling_on_wind_wave()


function sim_ship(ship, logfile, dtime)
	if ship  then 
		ship:simulate(dtime);  
		

		local yaw, pitch, roll 	= ship:get_angles();
		local x, y, z 			= ship:get_position();

		-- yaw = 90;
		-- --pitch = -0.869;
		-- x   = 0;
		-- y   = 0;
		ship:set_angles(yaw, pitch, roll);
		ship:set_position(x,y,z);

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

grid_size = 0;
local cpu_log = io.open("cpu.log", "w");


function sci_frame(dtime)

	--print(grid_size);
	local cpu = {
		[0] = 0;
		[1] = 0;
		[2] = 0;
		[3] = 0;
	};
	
	chunk = math.floor(grid_size/4);
	
	for i=0, grid_size do
		if 		i<(chunk*1) then		cpu[0] = cpu[0] + grid_submerging[i];
		elseif 	i<(chunk*2) then		cpu[1] = cpu[1] + grid_submerging[i];
		elseif 	i<(chunk*3) then		cpu[2] = cpu[2] + grid_submerging[i];
		elseif 	i<(chunk*4) then		cpu[3] = cpu[3] + grid_submerging[i];
		end
	end
	
	local out = string.format("%f %f %f %f", cpu[0], cpu[1], cpu[2], cpu[3]);
	cpu_log:write(out.."\n");
	cpu_log:flush();

	local	rotation = 60;
	game_time = game_time + dtime;
	
	dtime = 1 / 30;
	
	sim_ship(ship,  rolling_log, dtime);
	

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
	local yaw	= 	math.rad(state.yaw	);
	local pitch	= 	math.rad(state.pitch	);
	local roll	= 	math.rad(state.roll	);
	local x	 	=	-state.dist * math.cos(yaw) * math.cos(-pitch);
	local y		=	-state.dist * math.sin(yaw) * math.cos(-pitch);
	local z		=	-state.dist * math.sin(-pitch);
	naval.set_view(90, vmath.vec4(x,y,z,1), state.yaw, state.pitch, state.roll);--state.pitch, state.roll);

end

