-----------------------------------------------------------------------
--	Experiment #1
-----------------------------------------------------------------------

print ("Experiment #1");

--	world properties :
local	gravity	=	9.8;				--	m/s^2
local	g		=	gravity;
local	gamma	=	gravity * 1000;		--	water weight density

local function sqr(a) return a*a; end;

-----------------------------------------------------------------------
--	SUBMARINE  --
-----------------------------------------------------------------------
--	attributes :
-----------------------------------------------------------------------

local function S(t,r)	--	(1.2) 
	return (t-r)*math.sqrt(t*(2*r-t)) + r*r*(math.asin((t-r)/r) + math.pi/2 );	
end

local	T			=	8;		--	submarine's initial draft - equlibrium keel depth
local	R			=	5;		--	submarine's radius, m
local	L			=	100;    --	submarine's length, m
local	Zg			=	2;		--	center of mass vertical offset
local	Zp			=	T - R;	--	distance between center and waterline
local 	B0			=	2*math.sqrt(R*R - Zp*Zp);	--	initial waterline width 	(1.1)
local	D			=	S(T,R) * L * gamma;			--	weight of the submarine		(1.2)
local	M			=	D/g;						--	mass of the submarine		
local	Ixx			=	M/12 * (B0*B0 + 4*Zg*Zg);	--	ineria						(1.8)
	
local 	lambda33	=	0;		--	attached water mass
local 	lambda44	=	0;		--	attached water inertia
local 	mu33		=	-3000000;	--	linear damping coeeficient
local 	mu44		=	-1000000;	--	angular damping coeeficient
	
--	derived properties :
local 	zeta		=	1;	--	linear offset
local 	zeta_d		=	0;	--	linear velocity
local 	zeta_dd		=	0;	--	linear acceleration
local 	theta		=	0;	--	angle (roll)
local 	theta_d		=	0;	--	angular velocity
local 	theta_dd	=	0;	--	angular acceleration
	
local	M_l33		=	M   + lambda33;
local	I_l44		=	Ixx + lambda44;

-----------------------------------------------------------------------
--	forces :
-----------------------------------------------------------------------

--	(1.9)	--
local function Fd(zeta_d)
	return mu33 * zeta_d;
end

--	(1.9)	--	
local function Md(theta_d)
	return mu44 * theta_d;
end

print(D);

--	(1.10)	-- 
local function Fr(zeta)
	if zeta>(2*R-T) then 
		return  - ( gamma * math.pi * R*R * L - D );
	end
	
	if zeta<-T then
		return D;
	end
	
	local A, B, C, D;
	A 	= R*R * math.asin((zeta+Zp)/R);
	B 	= R*R * math.asin(Zp/R);
	C	= (zeta+Zp) * math.sqrt(R*R - sqr(Zp+zeta));
	D	= Zp * math.sqrt(R*R - Zp*Zp);
	return - gamma * L * (A - B + C - D);
end

--	(1.12)	--	
local function Mr(theta)
	return D * (R-Zg) * math.sin(theta);
end

--local function

local function update_forces(dt)
	
end


local function integrate(dt)
	zeta_d = zeta_d + zeta_dd * dt;
	zeta   = zeta + zeta_d * dt;

	local Ftotal = Fr(zeta) + Fd(zeta_d);
	zeta_dd = Ftotal / M;
	
end

-----------------------------------------------------------------------
--	WAVING  --
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
--	EXPERIMENT  --
-----------------------------------------------------------------------

local function plot(F, x0, x1, step)
	local f = io.open("exp01/plot.txt", "w");
	for x=x0, x1, step do
		local y = F(x);
		local out	= string.format("%g %g", x, y);
		f:write(out.."\n");
	end
	io.close(f);
end

plot(Fr, -20, 7, 0.1);


local function z()
	integrate(0.1);
	return zeta;
end

plot(z, 0, 300, 1);

----------------------

local function do_experiment()
	local f = io.open("exp01/waving.txt", "w");
	for t=0, 100, 0.125 do
		local offset, angle = waving:wave(t, 0);
		local out	= string.format("%g %g %g", t, offset, angle);
		f:write(out.."\n");
	end
	io.close(f);

	local f = io.open("exp01/fr.txt", "w");
	
	for x=0, 20, 0.2 do
		force = Fr(x);
		local out	= string.format("%g %g", x, force);
		f:write(out.."\n");
	end
	io.close(f);
end

do_experiment();














