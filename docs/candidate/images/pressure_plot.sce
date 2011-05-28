clc();			// clear screen
//xdel();			// delete windows
printf('\n');	// fix first character in output window

G         = 9.8;

//------------------------------------------------------------------------------
//	Fourier :
//------------------------------------------------------------------------------
funcprot(0);

function k = K (w) 
	k=w*w / G; 
endfunction

function w = W (k) 
	w=sqrt(k*G); 
endfunction

N = 32;
Wmax = 1.2;
Wcutoff = Wmax*5;
Kcutoff = K(Wcutoff);
dk      = Kcutoff / N;
Kmax	= K(Wmax);

rand('seed', 0);
phi = rand(N,1) * 2*%pi;

L = 2*%pi / dk;

printf("L = %f\n", L);


//------------------------------------------------------------------------------
//	SpectraPM :
//------------------------------------------------------------------------------

function ea = spectraPM (w)
	ea = 0.0081*G*G * (w ^ -5) * (%e ^ (-1.25 * ((Wmax/w) ^ 4) ));
	return ea;
endfunction

function ea = spectraPM_k (k)
	w = W(k);
	ea = 0.5*spectraPM(w) * G / w;
endfunction

function amp = ampPM_k(k)
	amp = sqrt(2 * spectraPM_k(k) * dk)
endfunction

function h = wave(t,x,z)
	h = z;
	for i=1:N
		k = dk * i;
		fade = exp(k*z);
		h = h + fade * ampPM_k(k) * cos(x*k - W(k)*t + phi(i))
	end
endfunction

function h = wave_approx(t,x,z)
	h = 0;
	for i=1:N
		k = dk * i;
		h = h + ampPM_k(k) * cos(x*k - W(k)*t + phi(i))
	end
	h = h * exp(Kmax*z) + z;
endfunction

function [x,h] = WAVE (z)
	x = linspace(0, L, N*10);
	h = wave(120,x,z);
endfunction

function [x,h] = WAVE_APPROX (z)
	x = linspace(0, L, N*10);
	h = wave_approx(120,x,z);
endfunction

[x,h0] = WAVE(-0);
[x,h0125] = WAVE(-0.125);
[x,h025] = WAVE(-0.25);
[x,h05] = WAVE(-0.5);
[x,h1] = WAVE(-1);
[x,h2] = WAVE(-2);
[x,h4] = WAVE(-4);
[x,h8] = WAVE(-8);
[x,h16] = WAVE(-16);

[x,ah0] = WAVE_APPROX(-0);
[x,ah0125] = WAVE_APPROX(-0.125);
[x,ah025] = WAVE_APPROX(-0.25);
[x,ah05] = WAVE_APPROX(-0.5);
[x,ah1] = WAVE_APPROX(-1);
[x,ah2] = WAVE_APPROX(-2);
[x,ah4] = WAVE_APPROX(-4);
[x,ah8] = WAVE_APPROX(-8);
[x,ah16] = WAVE_APPROX(-16);

hx  = [ h0;   h1;  h2;  h4;  h8 ];
ahx = [ ah0; ah1; ah2; ah4; ah8 ];

//--------------------------------------------------------
//	PLOT :
//--------------------------------------------------------
clf();
subplot(1,3,1);
xtitle("$Поверхности\ равного\ давления\ (точный\ расчет)$", "$\LARGE x, м$", "$\LARGE z, м$");
plot2d(x,h0);
plot2d(x,h0125);
plot2d(x,h025);
plot2d(x,h05);
plot2d(x,h1);
plot2d(x,h2);
plot2d(x,h4);
plot2d(x,h8);
plot2d(x,h16);

subplot(1,3,2);
xtitle("$Поверхности\ равного\ давления\ (приближенный\ расчет)$", "$\LARGE x,\ м$", "$\LARGE z,\ м$");
plot2d(x,ah0);
plot2d(x,ah0125);
plot2d(x,ah025);
plot2d(x,ah05);
plot2d(x,ah1);
plot2d(x,ah2);
plot2d(x,ah4);
plot2d(x,ah8);
plot2d(x,ah16);


xset("wdim", 1100,700);

Wmax = 0.75/1.2;
for j=1:5
	Wmax = Wmax * 1.2;

	Wcutoff = Wmax*5;
	Kcutoff = K(Wcutoff);
	dk      = Kcutoff / N;
	Kmax	= K(Wmax);

	dd = [];
	pp = [];
	stdp = [];
	stdpr = [];
	
	for d=0.00001:0.2:16.1
		dd($+1) = d;// * 9.8 * 1036;
		pp($+1) = d * 9.8 * 1036;
		[dummy, xh] = WAVE(-d);
		[dummy, xah] = WAVE_APPROX(-d);
		stdp($+1) = st_deviation(xh-xah) * 9.8 * 1036;
		stdpr($+1) = stdp($) / pp($);
	end
	
	subplot(2,3,3);
	xtitle("$Стандартное\ отклонения\ ошибки\ расчета\ давления$", "$\LARGE d,\ м$", "$\LARGE \text{stdev}(p_w),\ Па$");
	plot2d(dd,stdp);

	if j<5 then
		xstring(dd(70-j*5), stdp(70-j*5),sprintf("$\Large\omega_{max}=%0.1f$", Wmax));
	else
		xstring(4, 20,sprintf("$\Large\omega_{max}=%0.1f$", Wmax));
	end
	//xpoly([14, dd(40+j*5)], [1200-100*j, stdp(40+j*5)]);

	
	subplot(2,3,6);
	xtitle("$Относительная\ ошибка\ расчета\ давления$", "$\LARGE d,\ м$", "$\LARGE \text{stdev}(p_w) / p_{silent}(d),\ \%$");
	plot2d(dd,stdpr*100);

	xstring(12,8-j,sprintf("$\Large\omega_{max}=%0.1f$", Wmax));
	xpoly([12, dd(20+j*5)], [8-j, stdpr(20+j*5)*100]);
end

xs2pdf(gcf(), "D:/wave_plot_side");
