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
--	WAVING  --
-----------------------------------------------------------------------

local  waving = {
	length	=	100;	--	lambda, m
	height	=	0.5;	--	h, 		m
	period	=	2.1;	--	tau, 	s
	
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

local function a_d(t)
	local dt = 1/16;
	local offs0, angle0 = waving:wave(t,0);
	local offs1, angle1 = waving:wave(t+dt,0);
	return (angle1 - angle0) / dt;
end

local function a_dd(t)
	local dt = 1/16;
	local a_d0 = a_d(t);
	local a_d1 = a_d(t+dt);
	return (a_d1 - a_d0) / dt;
end

local k = 2 * math.pi / waving.length;

-----------------------------------------------------------------------
--	SUBMARINE  --
-----------------------------------------------------------------------
--	attributes :
-----------------------------------------------------------------------

--	(1.2)	--
local function S(t,r)	
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
local 	mu33		=	-1000000;	--	linear damping coeeficient
local 	mu44		=	-1000000;	--	angular damping coeeficient
	
--	derived properties :
local 	zeta		=	0;	--	linear offset
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
	return -D * (Zg) * math.sin(theta);
end


--	(1.35)	--
function S2(z)
	if z>=R  then return 0; end;
	if z<=-R then return 0; end;
	return math.sqrt(R*R - z*z);
end

function kappa()
	local S0 = S2(T-R+zeta);

	local dz = 1/16;
	
	local function dS(z) 
		return (S2(z+dz) - S2(z))/dz;
	end
	
	local I  = 0;
	for z=0, T+zeta, dz do
		I = I + math.exp(-k * z) * dS(z) * dz;
	end
	
	local k = - 1 / S0 * I;
	return k;
end



--  [ integration ]  --
local function integrate(t, dt)

	zeta_w, theta_w = waving:wave(t, 0);

	--	integrate forces :
	local k = kappa();
	--k = 1;

	local Ftotal = k * Fr(zeta + zeta_w) + Fd(zeta_d);
	
	zeta_dd = 	Ftotal / M;
	zeta_d 	= 	zeta_d + zeta_dd * dt;
	zeta   	= 	zeta + zeta_d * dt;
	
	--	integrate torques :
	local Zpp = Zp + zeta;
	local A0 = gamma / g * (
			math.pi*R*R*R*R/8 + 
			R*R*R*R*math.asin((Zpp)/R) +
			Zpp/12 * math.sqrt(R*R-Zpp*Zpp) * (5*R*R - 2*Zpp*Zpp)
		);
	
	local Mtotal 	= Mr(theta - theta_w) + Md(theta_d)
					- gamma * (Zp+zeta) * L * S(T+zeta, R) * (theta - theta_w) - (Ixx - A0) * a_dd(t);
	theta_dd = 	Mtotal / Ixx;
	theta_d  =  theta_d + theta_dd * dt;
	theta  	 =  theta + theta_d * dt;
end


-----------------------------------------------------------------------
--	EXPERIMENT  --
-----------------------------------------------------------------------

local function do_experiment()
	local f = io.open("exp01/exp01.txt", "w");
	
	local dt = 1/16;
	
	for t=0, 100, dt do
		local zeta_w, theta_w = waving:wave(t, 0);

		integrate(t, dt);
		
		local out	= string.format("%g %g %g %g %g", t, zeta_w, zeta, theta_w, theta);
		f:write(out.."\n");
	end
	io.close(f);

end

do_experiment();














