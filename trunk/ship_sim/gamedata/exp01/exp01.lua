-----------------------------------------------------------------------
--	Experiment #1
-----------------------------------------------------------------------

print ("Experiment #1");

--	world properties :
local	gravity	=	9.8;				--	m/s^2
local	gamma	=	gravity * 1000;		--	water weight density

-----------------------------------------------------------------------
--	submarine :
-----------------------------------------------------------------------

local submarine = {
	--	origin properties :
	radius		=	5;		--	radius, m
	length		=	100;    --	length, m
	draft		=	7;		--	initial equlibrium keel depth
	lambda33	=	0;		--	attached water mass
	lambda44	=	0;		--	attached water inertia
	mju33		=	0.8;	--	linear damping coeeficient
	mju44		=	0.8;	--	angular damping coeeficient
	
	--	derived properties :
	x	=	0;	--	linear offset
	dx	=	0;	--	linear velocity
	ddx	=	0;	--	linear acceleration
	a	=	0;	--	angle (roll)
	da	=	0;	--	angular velocity
	dda	=	0;	--	angular acceleration
	
	--	temporary :
	force	=	0;
	torque	=	0;
};                    


local function calculate_submerged_volume()
	local R = submarine.radius;
	local T = submarine.draft;
	local L = submarine.length;
	local S = (T-R)*math.sqrt(T*(2*R-T)) + R*R*(math.asin((T-R)/R) + math.pi/2 );
end


local function update_forces(dt)
	
end


local function integrate_velocities(dt)
	
end

-----------------------------------------------------------------------
--	waving
-----------------------------------------------------------------------

local  waving = {
	length	=	90;	--	lambda, m
	height	=	5;	--	h, 		m
	period	=	7;	--	tau, 	s
	
	wave = function(this, t, x)
		local offset, angle, k, r0, a0;
		local sgm; -- sigma
		
		k 	= 	2 * math.pi / this.length;
		r0	=	this.height / 2;
		sgm	=	math.sqrt(k*gravity);
		a0	=	k * r0;
		
		offset	=	 r0 * math.cos(k*x - sgm*t);
		angle	=	-a0 * math.sin(k*x - sgm*t);
		
		return offset, angle;
	end
};


-----------------------------------------------------------------------
--	waving
-----------------------------------------------------------------------

local function do_experiment()
	local f = io.open("exp01/waving.txt", "w");
	
	for t=0, 100, 0.125 do
		local offset, angle = waving:wave(t, 0);
		local out	= string.format("%g %g %g", t, offset, angle);
		f:write(out.."\n");
	end

	io.close(f);
end

do_experiment();