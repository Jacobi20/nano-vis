

funcprot(0);


//function r=R(w,W,A,Q)
//	dw = W/Q;
//	r = A * (dw*dw) / sqrt( ((W-w)^2) + (dw^2) )
//endfunction


function rrr=I(w,W,A,Q)
	dw = W/Q;
	aaa = A * dw * dw;
	bbb = (w-W) * (w-W) + dw*dw;
	rrr = aaa / bbb;
	return;
endfunction

clf();

xx=[];
yy=[];
for x=0:0.01:10
	xx(1,$+1) = x;
	yy(1,$+1) = I(x, 1.2, 1, 2);
	yy(2,$)   = I(x, 3.0, 2, 4);
	yy(3,$)   = I(x, 4.5, 0.5, 2);
end

for i=1:3
plot2d(xx(1,:), yy(i,:));
	a=gca();
	//a.children.children.mark_mode = "off";
	a.children(1).children.line_style = i;
	a.x_location = "origin";
	a.y_location = "origin";
	a.data_bounds = [0,0; 10,2.5];
end

xtitle("$\LARGE Резонансные\ кривые$", "$\LARGE \omega$", "$\LARGE R(\omega)$")

legend(["$\LARGE \Omega=1.2,\ A=1.0,\ Q=2.0$" 
		"$\LARGE \Omega=3.0,\ A=1.0,\ Q=4.0$" 
		"$\LARGE \Omega=4.5,\ A=0.5,\ Q=2.0$"]);

xs2pdf(gcf(), "E:\resonance_curve")