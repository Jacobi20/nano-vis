
-----------------------------------------------------------------------
--	setup and control :
-----------------------------------------------------------------------

input.unbindall()
input.bind ("F5", 	"dofile('run.lua')");
input.bind ("F6", 	"naval.reload_shaders()");
input.bind ("F10", 	"core.quit()");
input.bind ("PGUP", 	"_DistInc()");
input.bind ("PGDN", 	"_DistDec()");
input.bind ("LEFT", 	"_YawDec()");
input.bind ("RIGHT", 	"_YawInc()");
input.bind ("UP", 		"_PitchInc()");
input.bind ("DOWN", 	"_PitchDec()");

input.bind ("S",	"_ShipFW()");
input.bind ("Z",  "_ShipBW()");
input.bind ("A",	"_ShipSL()");
input.bind ("X",  "_ShipSR()");

input.bind ("F1", "print('Help!!! :)'"  	);
input.bind ("F2", "ship_show_hull      = not ship_show_hull"  	);
input.bind ("F3", "ship_show_voxels    = not ship_show_voxels" 	);
input.bind ("F4", "ship_show_submerge  = not ship_show_submerge");

input.bind ("N", "state.submersion 		= not state.submersion" );
input.bind ("M", "state.sunking 		= not state.sunking" );

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
	yaw		=	0;
	roll	=	0;
	pitch	=	5;
	dist	=	100;
	
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

function create_uboat()
	print("---- creating U-boat ----");
	local ship = naval.create_ship();

	ship:set_resistance	( 200 );
	
	ship:set_vis_mesh	( "../scidata/uboat.esx|boat1"			);
	ship:set_hdf_mesh	( "../scidata/uboat.esx|flowsurf2" 		);
	ship:set_hsf_mesh	( "../scidata/uboat.esx|flowsurf2" 		);
	ship:make_rigidbody	( "../scidata/uboat.esx|stat", 2705000	);
	
	ship:set_position	( 0, 0, -1 );	
	ship:set_angles		( 90, 0, 100 );
	
	ship:build_voxels	( "../scidata/uboat.esx|flowsurf2", 1	);
	
	print("---- done ----");
	return ship;
end

uboat	=	create_uboat();


-------------------------------------------------------------------------------
--	frame :
-------------------------------------------------------------------------------

function DriveShip()
	if state.ship_fw then SCI_ShipForce( vmath.vec4( 20000000,0,0,0), vmath.vec4(-50,0,-0.4,1)); end;
	if state.ship_bw then SCI_ShipForce( vmath.vec4(-20000000,0,0,0), vmath.vec4(-50,0,-0.4,1)); end;
	
	if state.ship_sl then SCI_ShipForce( vmath.vec4(0,  10000000,0,0), vmath.vec4(-50,0,-0.4,1)); end;
	if state.ship_sr then SCI_ShipForce( vmath.vec4(0, -10000000,0,0), vmath.vec4(-50,0,-0.4,1)); end;
	
	if state.submersion then
		SCI_ShipForce( vmath.vec4(0,0,-16537500,0), vmath.vec4(0,0,0,1));
	end
	if state.sunking then
		SCI_ShipForce( vmath.vec4(0,0,-2000000,0), vmath.vec4(0,0,0,1));
	end
end

function SciVisFrame(dtime)

--	SCI_ShipForce( vmath.vec4(0, 0,-5000000,0), vmath.vec4(50,0,0,1));

	local	rotation = 60;
	game_time = game_time + dtime;
	
	if dtime>0.100 then
		--print ("dtime > 0.060  (", dtime, ")");
		dtime = 0.100;
	end
	
	dtime = 0.016;
	
	DriveShip();

	if state.yaw_inc	then	state.yaw	=	state.yaw 	+ dtime * rotation; end;
	if state.yaw_dec	then	state.yaw	=	state.yaw 	- dtime * rotation; end;
	if state.roll_inc	then	state.roll	=	state.roll 	+ dtime * rotation; end;
	if state.roll_dec	then	state.roll	=	state.roll 	- dtime * rotation; end;
	if state.pitch_inc	then	state.pitch	=	state.pitch	+ dtime * rotation; end;
	if state.pitch_dec	then	state.pitch	=	state.pitch	- dtime * rotation; end;
	if state.dist_inc	then	state.dist	=	state.dist	* 1.03;	end;
	if state.dist_dec	then	state.dist	=	state.dist	/ 1.03; 	end;
	
	--SCI_ShipForce( vmath.vec4(10000000,0,0,0), vmath.vec4(-20,-1,0,1));
                                                    
	naval.render_view {
		ship_course		=	45*( math.sin(game_time*0.02) + 0.7*math.sin(game_time*0.03));
		dtime			=	dtime;
		yaw				=	state.yaw;
		roll			=	state.roll;
		pitch			=	state.pitch;
		distance		=	state.dist;
	}

end

