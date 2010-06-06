
-----------------------------------------------------------------------
--	FRAME :
-----------------------------------------------------------------------

unbindAll()
bind ("F5", 	"dofile('run.lua')");
bind ("F6", 	"SCI_ReloadShaders()");
bind ("F10", 	"quit()");

bind ("PGUP", 	"_DistInc()");
bind ("PGDN", 	"_DistDec()");

bind ("LEFT", 	"_YawDec()");
bind ("RIGHT", 	"_YawInc()");
bind ("UP", 	"_PitchInc()");
bind ("DOWN", 	"_PitchDec()");

state = {
	yaw_inc		=	false;
	yaw_dec		=	false;
	roll_inc	=	false;
	roll_dec	=	false;
	pitch_inc	=	false;
	pitch_dec	=	false;
	dist_inc	=	false;
	dist_dec	=	false;
	yaw		=	90;
	roll	=	0;
	pitch	=	5;
	dist	=	100;
}   

function _YawInc()   state.yaw_inc		=	true; end
function _YawDec()   state.yaw_dec		=	true; end
function _RollInc()  state.roll_inc		=	true; end
function _RollDec()  state.roll_dec		=	true; end
function _PitchInc() state.pitch_inc	=	true; end
function _PitchDec() state.pitch_dec	=	true; end
function _DistInc()  state.dist_inc		=	true; end
function _DistDec()  state.dist_dec		=	true; end
                   
function YawInc()   state.yaw_inc		=	false; end
function YawDec()   state.yaw_dec		=	false; end
function RollInc()  state.roll_inc		=	false; end
function RollDec()  state.roll_dec		=	false; end
function PitchInc() state.pitch_inc		=	false; end
function PitchDec() state.pitch_dec		=	false; end
function DistInc()  state.dist_inc		=	false; end
function DistDec()  state.dist_dec		=	false; end

game_time = 0;
                    
function SciVisFrame(dtime)

	local	rotation = 60;
	game_time = game_time + dtime;

	if state.yaw_inc	then	state.yaw	=	state.yaw 	+ dtime * rotation; end;
	if state.yaw_dec	then	state.yaw	=	state.yaw 	- dtime * rotation; end;
	if state.roll_inc	then	state.roll	=	state.roll 	+ dtime * rotation; end;
	if state.roll_dec	then	state.roll	=	state.roll 	- dtime * rotation; end;
	if state.pitch_inc	then	state.pitch	=	state.pitch	+ dtime * rotation; end;
	if state.pitch_dec	then	state.pitch	=	state.pitch	- dtime * rotation; end;
	if state.dist_inc	then	state.dist	=	state.dist	* 1.03;	end;
	if state.dist_dec	then	state.dist	=	state.dist	/ 1.03; 	end;
                                                    
	SCI_RenderView {
		ship_course		=	45*( math.sin(game_time*0.02) + 0.7*math.sin(game_time*0.03));
		dtime			=	dtime;
		yaw				=	state.yaw;
		roll			=	state.roll;
		pitch			=	state.pitch;
		distance		=	state.dist;
	}

end

