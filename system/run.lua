-----------------------------------------------------------------------
--	INIT :
-----------------------------------------------------------------------

function NVisInit()
end


-----------------------------------------------------------------------
--	FRAME :
-----------------------------------------------------------------------

bind ("LEFT",  	"_YawInc()");
bind ("RIGHT",	"_YawDec()");
bind ("UP",  	"_RollInc()");
bind ("DOWN",	"_RollDec()");

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
	pitch	=	0;
	dist	=	10;
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
                    
function NVisFrame(dtime)

	local	rotation = 100;
	print(dtime);
	dtime = 4;

	if state.yaw_inc	then	state.yaw	=	state.yaw 	+ dtime * rotation; end;
	if state.yaw_dec	then	state.yaw	=	state.yaw 	- dtime * rotation; end;
	if state.roll_inc	then	state.roll	=	state.roll 	+ dtime * rotation; end;
	if state.roll_dec	then	state.roll	=	state.roll 	- dtime * rotation; end;
	if state.pitch_inc	then	state.pitch	=	state.pitch	+ dtime * rotation; end;
	if state.pitch_dec	then	state.pitch	=	state.pitch	- dtime * rotation; end;
	if state.dist_inc	then	state.dist	=	state.dist	* 1.1;	end;
	if state.dist_dec	then	state.dist	=	state.dist	/ 1.1; 	end;
                                                    
	NVisSnapshot {
		-- data stuff --
		--path			=	"download/O8mgpc40a2.cube";
		path			=	"download/vitaminc.cube";
		skip_shot		=	true;
		
		-- position --
		yaw				=	state.yaw;
		roll			=	state.roll;
		pitch			=	state.pitch;
		distance		=	state.dist;
		
		-- ball&stick stuff
		atom_scale		=	0.1;
		bond_radius		=	0.03;
		use_vdw_radius	=	true;
		
		-- volume stuff --
		slice_num		=	100;
		intens_scale	=	200;
		
		value_low		=	0.0;	
		value_high		=	1.0;
	}

end
