clc();
xdel();

PATH = "d:\workspace\Engine\game_2048\shipxds\test03_br\";

function [result, time, yaw, pitch, roll, pos_x, pos_y, pos_z, wave, velocity, angular, wave_length_coef, vel_coef, jonswap, narrow] = ReadPos( path )
    result = -1;
    time = [];
    yaw = [];
    pitch = [];
    roll = [];
    pos_x = [];
    pos_y = [];
    pos_z = [];
    wave = [];
    velocity = [];
    angular = [];
    wave_length_coef = 0;
    vel_coef = 0;
    jonswap = 0;
    narrow = 0;
    
    printf("...reading : %s\n", path);
        
    [f, err] = mopen(path, 'r');
    
    if ( err <> 0 ) then
        return;
    else
        result = 1;
    end
    
    num = 0;
    
    [num_read, wave_length_coef, vel_coef, jonswap, narrow] = mfscanf(f, "%f %f %i %i");
    
    while (%t)
		num = num + 1;
		[num_read, ct, cyaw, cpitch, croll, cpos_x, cpos_y, cpos_z, cwave, cvelocity, cangular] = mfscanf(f, "%f %f %f %f %f %f %f %f %f %f");

        if (num_read<=0) then break end
	
		time(num) = ct;
        yaw(num) = cyaw;
        pitch(num) = cpitch;
        roll(num) = croll;
        pos_x(num) = cpos_x;
        pos_y(num) = cpos_y;
        pos_z(num) = cpos_z;
        wave(num) = cwave;
        velocity(num) = cvelocity;
        angular(num) = cangular;
	end

    mclose(f);

endfunction

function DrawShip(pos_x, pos_y, yaw)
    value_x = [-10 -10 -10 10 10 -10];
    value_y = [-3 3 3 0 0 -3]
    x = value_x * cosd(yaw) - value_y * sind(yaw) + [pos_x pos_x pos_x pos_x pos_x pos_x];
    y = value_x * sind(yaw) + value_y * cosd(yaw) + [pos_y pos_y pos_y pos_y pos_y pos_y];
    xset("thickness",2);
    xset("clipoff");
    xarrows(x , y, 0, 5);
    xset("thickness",1);
endfunction

function DrawAndSaveGraphics(num, image_filename)
    clf();
    filename = sprintf("broaching_%i", num);
    
    [r, t, yaw, pitch, roll, pos_x, pos_y, pos_z, wave, velocity, angular, wave_length_coef, vel_coef, jonswap, narrow] = ReadPos(PATH+filename+".log");
    if r < 0 then break end
    
    subplot(221);
    xtitle("Trajectory", "X (m)", "Y (m)");    
    plot2d(pos_x, pos_y);
    a=get("current_axes");
    a.tight_limits = "on";
    //a.data_bounds = [0, -300; 400, 300];
    a.isoview = "on";
   // a.x_location = "middle";
    for i=1:50:size(yaw,1) do
        DrawShip(pos_x(i), pos_y(i), yaw(i));
        xstring(pos_x(i),pos_y(i) + 5,sprintf("%.2fs", t(i)));
    end
    
    subplot(222);
    xtitle("Roll and Yaw", "Time (s)", "Roll, Yaw (degrees)");
    plot2d( t, [roll yaw], [2 3]);    
    a=gca();
    a.isoview = "off";
    a.tight_limits = "on";
    a.data_bounds = [0, -100; 60, 100];
    legend(["Roll" "Yaw"], 3, %f);
    
    subplot(223);
    xtitle("Wave height in (0;0)", "Time (s)", "Height (m)");
    plot2d(t, wave, 2);
    a=gca();
    a.tight_limits = "on";
    a.data_bounds = [0, -1.0; 60, 1.0];
    
    subplot(224);
    xtitle("Angular velocity", "Time (s)", "Angular velocity (rad/s)");
    plot2d(t, angular, 2);
    a=gca();
    a.tight_limits = "on";
    a.data_bounds = [0, -0.25; 60, 0.25]; 
    
    xs2pdf(gcf(), PATH+"\result\"+image_filename);
endfunction

function DrawTrajecoriesForClass(class)
    for num = class
        filename = sprintf("broaching_%i", num);
        [r, t, yaw, pitch, roll, pos_x, pos_y, pos_z, wave, velocity, angular, wave_length_coef, vel_coef, jonswap, narrow] = ReadPos(PATH+filename+".log");
        
        if r < 0 then break end
                
        xtitle("Trajectory", "X (m)", "Y (m)");    
        plot2d(pos_x, pos_y);
        a=get("current_axes");
        a.tight_limits = "on";
        a.data_bounds = [0, -250; 400, 250];
        a.isoview = "on";
        a.x_location = "middle";
    end
endfunction


clf();

class1 = [5,21,22,36,39,44,46,48,49,52,53,68,73,74,80,81,85,89,94,97,100,101,105,106,110,112,115,118,119,121,128,132,138,139,141,144,145,161,176,179,180,181,184,188,194,195];
class2 = [3,9,13,28,34,35,42,43,56,64,65,83,91,122,127,177,189,191,196,198];
class3 = [2,12,16,20,24,25,30,31,38,45,55,58,63,67,69,87,90,96,104,120,124,129,131,133,134,136,142,146,150,156,157,159,162,164,165,168,170,171,175,182,183,186,187,199];
class4 = [1,4,7,8,10,15,17,18,19,23,26,27,29,32,33,37,40,41,47,50,51,54,57,59,60,61,62,70,71,72,75,77,78,79,82,84,86,88,92,93,95,98,99,102,103,107,108,109,111,113,114,116,117,123,126,130,135,137,140,147,148,149,151,152,153,154,155,158,160,163,166,167,169,173,174,178,185,190,192,193,197,200];
class5 = [6,11,14,66,76,125,143,172];

// Draw set of trajectories for each class
// Ugly stuff but scilab seems to work in peculiar way in case of vector of vectors with different dimensions
clf();
DrawTrajecoriesForClass(class1);
xs2pdf(gcf(), PATH+"\result\class_traj_1");
clf();
DrawTrajecoriesForClass(class2);
xs2pdf(gcf(), PATH+"\result\class_traj_2");
clf();
DrawTrajecoriesForClass(class3);
xs2pdf(gcf(), PATH+"\result\class_traj_3");
clf();
DrawTrajecoriesForClass(class4);
xs2pdf(gcf(), PATH+"\result\class_traj_4");
clf();
DrawTrajecoriesForClass(class5);
xs2pdf(gcf(), PATH+"\result\class_traj_5");



/////////////////////
// Draw all graphics for typical cases
////////
typical = [36, 189, 96, 32, 14];

i = 0;
for num = typical
    i = i + 1;
    DrawAndSaveGraphics(num, sprintf("class_typical_%i", i));
end