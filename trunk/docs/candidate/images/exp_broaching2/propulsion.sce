clc();
xdel();
//xset("wdim",400,300)

printf('\n');

//GAME_PATH = "d:\workspace\Engine\game_2048\shipxds\";
GAME_PATH = "d:\SPACE_MARINES_2_0\sci_docs\candidate\images\exp_broaching2\";
EXP_PATH  = GAME_PATH + "propulsion\";

function [result, force, time, velocity, result_v] = ReadVel( path )
    result = -1;
    force = 0;
    time = [];
    velocity = [];
    result_v = 0;
        
    printf("...reading : %s\n", path);
        
    [f, err] = mopen(path, 'r');
    
    if ( err <> 0 ) then
        return;
    else
        result = 1;
    end
    
    num = 0;
    
    force = mfscanf(f, "%f");
    
    while (%t)
		num = num + 1;
		[num_read, t, vel] = mfscanf(f, "%f %f");

        if (num_read<=0) then break end
	
		time(num) = t;
        velocity(num) = vel;
        result_v = vel;
	end

	v = perctl(velocity, 90)
	result_v = v;

    mclose(f);

endfunction

num = 0;
result_v = [];
applied_f = [];

	xset("wdim", 600,700);

output = mopen(EXP_PATH+"prop_table.txt", "w");
subplot(2,1,1);
while (%t)
    num=num+1;
    filename = sprintf("force_%i", num);
    
   // if num > 13 then break end
    
    [r, f, t, vel, r_v] = ReadVel(EXP_PATH+filename+".log");
     
    if r < 0 then break end
    
    applied_f(num) = f;
    result_v(num) = r_v(1);
    
    mfprintf(output,"%.2f\t%.2f\n", f, r_v);
        
    xtitle("$\LARGE Диаграмма\ установившейся\ скорости$", "$\LARGE t, с$", "$\LARGE V,\ м/с$");
    plot2d( t, vel, [1]);
    xtitle("$\LARGE Диаграмма\ установившейся\ скорости$", "$\LARGE t, с$", "$\LARGE V,\ м/с$");
	xstring( 35, num/1.5, sprintf( "$%2.1f кН$", f/1000.0 ));
	xpoly( [30,35], [vel($), num/1.5+0.3]);
    a = gca();
    a.x_location = "origin"
    a.y_location = "origin"
	a.data_bounds = [0,0; 40,15];
    a.tight_limits = "on";
end

mclose(output);

subplot(2,1,2);
xtitle("$\LARGE Диаграмма\ буксировочной\ силы$", "$\LARGE V,\ м/с$", "$\LARGE F_{tug},\ кН$" );
plot2d(result_v, applied_f/1000);
xpoly( [8.17, 8.17], [0, 800] );
xpoly( [0, 12], [305, 305] );
xstring( 8.17, 10, "$\large 8.167\ рад/с$");
xstring( 0.1, 305, "$\large 305\ кН$");
xtitle("$\LARGE Диаграмма\ буксировочной\ силы$", "$\LARGE V,\ м/с$", "$\LARGE F_{tug},\ кН$" );
    a = gca();
    a.tight_limits = "on";
    xs2pdf(gcf(), GAME_PATH+"propulsion");

xs2pdf(gcf(), GAME_PATH+"velocities");




