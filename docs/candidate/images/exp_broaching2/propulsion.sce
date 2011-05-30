clc();
xdel();
//xset("wdim",400,300)

printf('\n');

GAME_PATH = "d:\workspace\Engine\game_2048\shipxds\";
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

    mclose(f);

endfunction

num = 0;
result_v = [];
applied_f = [];

output = mopen(EXP_PATH+"prop_table.txt", "w");

while (%t)
    num=num+1;
    filename = sprintf("force_%i", num);
    
   // if num > 13 then break end
    
    [r, f, t, vel, r_v] = ReadVel(EXP_PATH+filename+".log");
     
    if r < 0 then break end
    
    applied_f(num) = f;
    result_v(num) = r_v;
    
    mfprintf(output,"%.2f\t%.2f\n", f, r_v);
        
    xtitle("$\LARGE Time-Velocity\ Diagram$", "$\LARGE t\ (s)$", "$\LARGE V\ (ms^{-1})$");
    plot2d( t, vel, [modulo(num,10)]);
    xtitle("$\LARGE Time-Velocity\ Diagram$", "$\LARGE t\ (s)$", "$\LARGE V\ (ms^{-1})$");
    a = gca();
    a.x_location = "origin"
    a.y_location = "origin"
    a.tight_limits = "on";
end
xs2pdf(gcf(), EXP_PATH+"velocities");

mclose(output);

clf();
xtitle("$\LARGE Force-Velocity\ Diagram$", "$\LARGE V\ (ms^{-1})$", "$\LARGE F_{tug} (kN)$" );
plot2d(result_v, applied_f);
xtitle("$\LARGE Force-Velocity\ Diagram$", "$\LARGE V\ (ms^{-1})$", "$\LARGE F_{tug} (kN)$" );
    a = gca();
    a.tight_limits = "on";
    xs2pdf(gcf(), EXP_PATH+"propulsion");