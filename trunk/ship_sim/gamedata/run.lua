
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
input.bind ("F2", "cfg.vars.ship_show_hull      = not cfg.vars.ship_show_hull"  	);
input.bind ("F3", "cfg.vars.ship_show_voxels    = not cfg.vars.ship_show_voxels" 	);
input.bind ("F4", "cfg.vars.ship_show_submerge  = not cfg.vars.hip_show_submerge");

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
	yaw		=	-90;
	roll	=	0;
	pitch	=	5;
	dist	=	110;
	
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

	ship:set_resistance	( 5 );
	
	ship:set_vis_mesh	( "uboat.esx|boat1"			);
	ship:set_hdf_mesh	( "uboat.esx|flowsurf2" 		);
	ship:set_hsf_mesh	( "uboat.esx|flowsurf2" 		);
	ship:make_rigidbody	( "uboat.esx|stat", 1805000	);
	
	ship:set_position	( 0, 0, 0 );	
	ship:set_angles		( 90, 0, 0 );
	ship:set_cmass		( 0,0,-0.5 );
	
	ship:build_voxels	( "uboat.esx|flowsurf2", 1	);
	ship:build_surf_dxdy( "uboat.esx|flowsurf2", 1, 0.1	);
	
	print("---- done ----");
	print("");
	return ship;
end


function create_cutter()
	print("");
	print("---- creating Coast Guard Ship ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 5 );
	
	ship:set_vis_mesh	( "boat.esx|boat1"			);
	ship:set_hdf_mesh	( "boat.esx|flow" 			);
	ship:set_hsf_mesh	( "boat.esx|flow" 			);
	ship:make_rigidbody	( "boat.esx|stat", 400000	);
	
	ship:set_position	( 0, 0, 0 );	
	ship:set_angles		( 90, 0, 10);
	ship:set_cmass		( 0, 0, 0 );
	
	ship:build_voxels	( "boat.esx|flow", 1	);
	ship:build_surf_dxdy( "boat.esx|flow", 1, 0.1	);
	
	print("---- done ----");
	print("");
	return ship;
end

function create_ssn668()
	print("");
	print("---- creating SSN-668 'Los Angeles' ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 5 );
	
	ship:set_vis_mesh	( "ssn668.esx|vismesh"			);
	ship:set_hdf_mesh	( "ssn668.esx|hydromesh" 		);
	ship:set_hsf_mesh	( "ssn668.esx|hydromesh" 		);
	ship:make_rigidbody	( "ssn668.esx|physmesh", 6000000	);
	
	ship:set_position	( 0, 0, 0 );	
	ship:set_angles		( 90, 0, 0 );
	ship:set_cmass		( 3.8, 0, -3 );
	
	ship:build_voxels	( "ssn668.esx|hydromesh", 2	);
	ship:build_surf_dxdy( "ssn668.esx|hydromesh", 1, 0.1	);
	
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


local rolling_log = io.open("rolling.log", "w");


--uboat	=	create_ssn668();
uboat	=	create_cutter();
--uboat	=	create_uboat();
--uboat	=	create_box();

cfg.vars.ship_hsf_method	=	"hxfse";
--ship_hsf_method	=	"surface";


-------------------------------------------------------------------------------
--	frame :
-------------------------------------------------------------------------------

function DriveShip()
	if state.ship_fw then uboat:add_force( vmath.vec4( 20000000,0,0,0), vmath.vec4(-50,0,-0.4,1), true); end;
	if state.ship_bw then uboat:add_force( vmath.vec4(-20000000,0,0,0), vmath.vec4(-50,0,-0.4,1), true); end;
	
	if state.ship_sl then uboat:add_force( vmath.vec4(0,  10000000,0,0), vmath.vec4(-50,0,-0.4,1), true); end;
	if state.ship_sr then uboat:add_force( vmath.vec4(0, -10000000,0,0), vmath.vec4(-50,0,-0.4,1), true); end;
	
	if state.submersion then
		uboat:add_force( vmath.vec4(0,0,-16537500,0), vmath.vec4(0,0,0,1), true);
	end
	if state.sunking then
		uboat:add_force( vmath.vec4(0,0,-2000000,0), vmath.vec4(0,0,0,1), true);
	end
end



--------------------------------------------------

function do_rolling()
	
--	ship_hsf_method	=	"voxel";
	ship_hsf_method	=	"surface";

	print("");
	print("Rolling experiment");
	local f = io.open("rolling.txt", "w");
	
	for roll=0, 180, 1 do
		
		uboat:set_position	( 0, 0, -0.5 );	
		uboat:set_angles	( 90, 0, roll );
		uboat:simulate		( 0.001 );
		
		local ra 	= uboat:get_right_arm();
		local hsf 	= uboat:get_hsf_force()/1000;
		
		local out	= string.format("%8.4f %7.4f %7.4f", roll, ra, hsf);
		print(out);
		f:write(out.."\n");
		f:flush();
	end
	
	uboat:set_position	( 0, 0, -0.5 );	
	uboat:set_angles	( 90, 0, nil );
	io.close(f);

	print("Done.");
end


function sci_frame(dtime)

--	SCI_ShipForce( vmath.vec4(0, 0,-5000000,0), vmath.vec4(50,0,0,1));

	local	rotation = 60;
	game_time = game_time + dtime;
	
	dtime = 0.03;
	
	DriveShip();
	
	if uboat  then 
		uboat:simulate(dtime);  
		

		local yaw, pitch, roll 	= uboat:get_angles();
		local x, y, z 			= uboat:get_position();

		yaw = 90;
		x   = 0;
		y   = 0;
		uboat:set_angles(yaw, pitch, roll);
		uboat:set_position(x,y,z);

		yaw		=	math.rad(yaw);
		pitch	=	math.rad(pitch);
		roll	=	math.rad(roll);
		
		-- local wave_offset0		= naval.get_wave_offset(x,y,0);
		-- local wave_offset1		= naval.get_wave_offset(x+1/16,y,0);
		local wave_offset0		= naval.get_wave_offset(0,0,0);
		local wave_offset1		= naval.get_wave_offset(0.001,0,0);
		local wave_offset		= (wave_offset0 + wave_offset1)/2;
		local wave_slope		= math.atan(16*(wave_offset1 - wave_offset0));
		local out				= string.format("%f %f %f %f %f %f %f %f", yaw, pitch, roll, x, y, z, wave_offset, wave_slope);
		rolling_log:write(out.."\n");
		rolling_log:flush();

	end;
	
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
	local yaw	= 	math.rad(state.yaw	);
	local pitch	= 	math.rad(state.pitch	);
	local roll	= 	math.rad(state.roll	);
	local x	 	=	-state.dist * math.cos(yaw) * math.cos(-pitch);
	local y		=	-state.dist * math.sin(yaw) * math.cos(-pitch);
	local z		=	-state.dist * math.sin(-pitch);
	naval.set_view(90, vmath.vec4(x,y,z,1), state.yaw, state.pitch, state.roll);--state.pitch, state.roll);
	--naval.set_view(90, vmath.vec4(30,130,1,1), -90,1,0);

end

