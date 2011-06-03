//PATH = "D:\workspace\Engine\candidate\images\exp_broaching2\";
PATH = "D:\SPACE_MARINES_2_0\sci_docs\candidate\images\exp_broaching2\";


class1 = [5,21,22,36,39,44,46,48,49,52,53,68,73,74,80,81,85,89,94,97,100,101,105,106,110,112,115,118,119,121,128,132,138,139,141,144,145,161,176,179,180,181,184,188,194,195];
class2 = [3,9,13,28,34,35,42,43,56,64,65,83,91,122,127,177,189,191,196,198];
class3 = [2,12,16,20,24,25,30,31,38,45,55,58,63,67,69,87,90,96,104,120,124,129,131,133,134,136,142,146,150,156,157,159,162,164,165,168,170,171,175,182,183,186,187,199];
class4 = [1,4,7,8,10,15,17,18,19,23,26,27,29,32,33,37,40,41,47,50,51,54,57,59,60,61,62,70,71,72,75,77,78,79,82,84,86,88,92,93,95,98,99,102,103,107,108,109,111,113,114,116,117,123,126,130,135,137,140,147,148,149,151,152,153,154,155,158,160,163,166,167,169,173,174,178,185,190,192,193,197,200];
class5 = [6,11,14,66,76,125,143,172];


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

function [T, X, Y, YAW, ROLL, WAVE_HEIGHT, ANGULAR] = ReadLogs()
    T = [];
    X = [];
    Y = [];
    YAW = [];
    ROLL = [];
    WAVE_HEIGHT = [];
    ANGULAR = [];
    num = 0;
    while %t
        num = num + 1;
        
        filename = sprintf("broaching_%i", num);
        [r, t, yaw, pitch, roll, pos_x, pos_y, pos_z, wave, velocity, angular, wave_length_coef, vel_coef, jonswap, narrow] = ReadPos(PATH+filename+".log");
        
        if r < 0 then break end
        
        for i=1:size(t, 1);
            T(num, i) = t(i);
            X(num, i) = pos_x(i);
            Y(num, i) = pos_y(i);
            YAW(num, i) = yaw(i);
            ROLL(num, i) = roll(i);
            WAVE_HEIGHT(num, i) = wave(i);
            ANGULAR(num, i) = angular(i);
        end
    end    
endfunction

function DrawShip(pos_x, pos_y, yaw)
    l = 20;
    w = 6;
    
    value_x = [-l/2 -l/2 -l/2 l/2 l/2 -l/2];
    value_y = [-w/2 w/2 w/2 0 0 -w/2];
    x = value_x * cosd(yaw) - value_y * sind(yaw) + ones(value_x) * pos_x;
    y = value_x * sind(yaw) + value_y * cosd(yaw) + ones(value_y) * pos_y;
    xset("thickness",2);
    xset("clipoff");
    xarrows(x , y, 0, 5);
    xset("thickness",1);
endfunction

function DrawPoint(x, y, size, classnum)
    xset("color", classnum);
    xfarc(x - size / 2, y - size / 2, size, size, 360*64, 360*64);
    xset("color", 1);
    xarc(x - size / 2, y - size / 2, size, size, 360*64, 360*64);
endfunction

function DrawAndSaveGraphics(num, image_filename, T, X, Y, YAW, ROLL, WAVE_HEIGHT, ANGULAR)
	xset("wdim", 800,600)
    clf();
        
    subplot(221);
    xtitle("$\Large Траектория$", "$\Large \xi, м$", "$\Large \eta, м$");
    plot2d(X(num,:), Y(num,:));
    a=get("current_axes");
    a.tight_limits = "on";
    //a.data_bounds = [0, -300; 400, 300];
    a.isoview = "on";
    // a.x_location = "middle";
    for i=1:50:size(YAW(num,:),2) do
        DrawShip(X(num,i), Y(num,i), YAW(num,i));
        xstring(X(num, i),Y(num,i) + 5,sprintf("%.2fs", T(num, i)));
    end
    
    subplot(222);
    xtitle("$\Large Крен\ и\ курс$", "$\Large t, с$", "$\Large \theta^{\circ}, \phi^{\circ}$");
    plot2d( T(num,:), ROLL(num,:), 1);    
    a=gca();
    a.isoview = "off";
    a.tight_limits = "on";
	a.children(1).children.line_style = 2;
    a.data_bounds = [0, -100; 60, 100];
    plot2d( T(num,:), YAW(num,:), 1);    
    a=gca();
    a.isoview = "off";
    a.tight_limits = "on";
	a.children(1).children.line_style = 1;
    a.data_bounds = [0, -100; 60, 100];

    legend(["$\Large \theta$" "$\Large \phi$"], 2);
    
    subplot(223);
    xtitle("$\Large Высота\ волны$", "$\Large t, с$", "$\Large h,м$");
    plot2d(T(num,:), WAVE_HEIGHT(num,:), 1);
    a=gca();
    a.tight_limits = "on";
    a.data_bounds = [0, -1.0; 60, 1.0];
    
    subplot(224);
    xtitle("$\Large Угловая\ скорость$", "$\Large t, с$", "$\Large \dot\phi,рад/c$");
    plot2d(T(num,:), ANGULAR(num,:), 1);
    a=gca();
    a.tight_limits = "on";
    a.data_bounds = [0, -0.25; 60, 0.25]; 
    
    xs2pdf(gcf(), PATH+image_filename);
endfunction

function DrawTrajecoriesForClass(class, id, X, Y)
    for num = class
        
		clsid = ["I", "II", "III", "IV", "V"];
                
        xtitle("$\Large Класс\ траекторий\ "+clsid(id)+"$", "$\Large \xi, м$", "$\Large \eta, м$");
        pos_y = [];
        if Y(num, size(Y(num,:),2)) > 0 then
            pos_y = Y(num,:);
        else
            pos_y = -Y(num,:);
        end
        plot2d(X(num,:), pos_y, 1);
        a=get("current_axes");
        a.tight_limits = "on";
        a.data_bounds = [0, -50; 400, 250];
        a.isoview = "on";
        a.x_location = "origin";
    end
endfunction

// Hardcode for class5

function DrawTrajecoriesForClass5(X, Y)
    for num = class5
        xtitle("$\Large Класс\ траекторий\ V$", "$\Large \xi, м$", "$\Large \eta, м$");
        pos_y = [];
        if Y(num, size(Y(num,:),2)) - Y(num, size(Y(num,:),2) - 1) > 0 then
            pos_y = -Y(num,:);
        else
            pos_y = Y(num,:);
        end
        plot2d(X(num,:), pos_y);
        a=get("current_axes");
        a.tight_limits = "on";
        a.data_bounds = [0, -50; 400, 250];
        a.isoview = "on";
        a.x_location = "origin";
    end
endfunction

function [ANGLE_SUM, DISTANCE, MAXROLL] = CalcParamsForClass(Y, YAW, ROLL, class)
    
    ANGLE_SUM = [];
    DISTANCE = [];
    MAXROLL = [];
    num = 0;
    for i=class
        num = num + 1;
        DISTANCE(num) = abs(max(Y(i,:)) - min(Y(i,:)));
        ANGLE_SUM(num) = 0;
        for j = 2:size(Y,2)
           ANGLE_SUM(num) = ANGLE_SUM(num) + abs(YAW(i,j) - YAW(i,j - 1));
        end
        MAXROLL(num) = max(abs(ROLL(i,:)));
    end
endfunction

function DrawPointsRoll (ANGLE_SUM, DISTANCE, MAXROLL, classnum)
    for i = 1:size(ANGLE_SUM,1)
        DrawPoint(ANGLE_SUM(i), DISTANCE(i), MAXROLL(i) / 90 * 30, classnum);
    end
endfunction

function DrawPointsEnergy (ANGLE_SUM, DISTANCE, MAXROLL, classnum)
    for i = 1:size(ANGLE_SUM,1)
        DrawPoint(ANGLE_SUM(i), DISTANCE(i), (ANGLE_SUM(i) + MAXROLL(i)) / 250 * 15, classnum);
    end
endfunction

function DrawAngleShiftFromRoll(ANGLE_SUM, DISTANCE, MAXROLL)
    plot2d([0 0 45 45], [-250 250 250 -250], 1);
    xpoly([0 45],[0 0]);    
    width = 0.02;
    for i = 1:size(ANGLE_SUM,1)
           
            xset("color", 2);
            xfpoly([MAXROLL(i)-width/2 MAXROLL(i)+width/2 MAXROLL(i)+width/2 MAXROLL(i)-width/2], [ANGLE_SUM(i) ANGLE_SUM(i) 0 0], 0);
            xset("color", 5);
            xfpoly([MAXROLL(i)-width/2 MAXROLL(i)+width/2 MAXROLL(i)+width/2 MAXROLL(i)-width/2], [0 0 -DISTANCE(i) -DISTANCE(i)], 0);
            xset("color", 1);
    end
endfunction

function [major_x, major_y] = CalcTrajectoryVectors (class, T, X, Y)
    major_x = [];
    major_y = [];
    
    timestamps = [0 9 11 19 21 29 31 39 41 49 51 59];//18 22 38 42 60];
    indices = [];
    num = 0;
    for i = timestamps
        num = num + 1;
        indices(num) = find(abs(T(1,:) - i) < 0.2, 1)
    end
    
    for k = 1:size(indices, 1)
       major_x(k) = mean(X(class,indices(k)));
       
       major_y(k) = 0;
       for j = class
           if Y(j, size(Y(j,:),2)) > 0 then
               major_y(k) = major_y(k) + Y(j,indices(k));
           else
               major_y(k) = major_y(k) - Y(j,indices(k));
           end
       end
       major_y(k) = major_y(k) / size(class, 2);
    end
endfunction

//Hardcode for class5

function [major_x, major_y] = CalcTrajectoryVectors5 (T, X, Y)
    major_x = [];
    major_y = [];
    
    timestamps = [0 9 11 19 21 29 31 39 41 49 51 59];//18 22 38 42 60];
    indices = [];
    num = 0;
    for i = timestamps
        num = num + 1;
        indices(num) = find(abs(T(1,:) - i) < 0.2, 1)
    end
    
    for k = 1:size(indices, 1)
       major_x(k) = mean(X(class5,indices(k)));
       
       major_y(k) = 0;
       for j = class5
           if Y(j, size(Y(j,:),2)) - Y(j, size(Y(j,:),2) - 1) > 0 then
               major_y(k) = major_y(k) - Y(j,indices(k));
           else
               major_y(k) = major_y(k) + Y(j,indices(k));
           end
       end
       major_y(k) = major_y(k) / size(class5, 2);
    end
endfunction

function DrawArrow(x,y)
    th = 0.2;
    arrow_x = [-0.5 -0.5 0.1 0.1 0.5 0.1 0.1];
    arrow_y = [-th/2 th/2 th/2 1.1*th 0 -1.1*th -th/2];
    angle = atan(y(2) - y(1), x(2) - x(1));
    c = [x(1) + (x(2)-x(1))/2, y(1) + (y(2)-y(1))/2];
    l = sqrt((x(2)-x(1))*(x(2)-x(1))+(y(2)-y(1))*(y(2)-y(1)));
    arrow_x = arrow_x * l;
    arrow_y = arrow_y * l;
    
    actual_x = arrow_y * sin(angle) + arrow_x * cos(angle) + c(1);
    actual_y = -arrow_y * cos(angle) + arrow_x * sin(angle) + c(2);
    
    xset("color", color(255,255,255));
    xfpoly(actual_x,actual_y);
    xset("color", 1);
    xpoly(actual_x,actual_y, "lines", 1);
endfunction

function DrawArrows(x,y)
    num = 1;
    while num <= size(x,1)
        DrawArrow([x(num), x(num + 1)],[y(num), y(num + 1)]);
        num = num + 2;
    end
endfunction

function [correlation] = CorForVectors(v1, v2)
    m1 = mean(v1);
    m2 = mean(v2);
    
    msum = 0;
    sqsumx = 0;
    sqsumy = 0;
    for i=1:size(v1,2)
        msum = msum + (test1(i) - m1)*(test2(i) - m2);
        sqsumx = sqsumx + (test1(i) - m1)*(test1(i) - m1);
        sqsumy = sqsumy + (test2(i) - m2)*(test2(i) - m2);    
    end

    correlation = msum / sqrt(sqsumx*sqsumy);
endfunction

clc();

[T, X, Y, YAW, ROLL, WAVE_HEIGHT, ANGULAR] = ReadLogs();

// Draw set of trajectories for each class
// Ugly stuff but scilab seems to work in peculiar way in case of vector of vectors with different dimensions
if %t then
	xset("wdim", 800,600)
	clf();
	subplot(2,3,1);
	DrawTrajecoriesForClass(class1, 1, X, Y);
    [major_x major_y] = CalcTrajectoryVectors(class1, T, X, Y);
    DrawArrows(major_x, major_y);
//    xset("thickness",3);
//    xarrows(major_x, major_y, -1, 7);//color(255,255,255));
//    xset("thickness",1);
   xs2pdf(gcf(), PATH+"\class_traj_1");
    subplot(2,3,2);
    DrawTrajecoriesForClass(class2, 2, X, Y);
    [major_x major_y] = CalcTrajectoryVectors(class2, T, X, Y);
    DrawArrows(major_x, major_y);
//    xset("thickness",3);
//    xarrows(major_x, major_y, -1, 5);
//    xset("thickness",1);
    xs2pdf(gcf(), PATH+"\class_traj_2");
    subplot(2,3,3);
    DrawTrajecoriesForClass(class3, 3, X, Y);
    [major_x major_y] = CalcTrajectoryVectors(class3, T, X, Y);
    DrawArrows(major_x, major_y);
//    xset("thickness",3);
//    xarrows(major_x, major_y, -1, 5);
//    xset("thickness",1);
    xs2pdf(gcf(), PATH+"\class_traj_3");
    subplot(2,3,4);
    DrawTrajecoriesForClass(class4, 4, X, Y);
    [major_x major_y] = CalcTrajectoryVectors(class4, T, X, Y);
    DrawArrows(major_x, major_y);
//    xset("thickness",3);
//    xarrows(major_x, major_y, -1, 5);
//    xset("thickness",1);
    xs2pdf(gcf(), PATH+"\class_traj_4");
    subplot(2,3,5);
    DrawTrajecoriesForClass5(X, Y);
    [major_x major_y] = CalcTrajectoryVectors5(T, X, Y);
    DrawArrows(major_x, major_y);
//    xset("thickness",3);
//    xarrows(major_x, major_y, -1, 5);
//    xset("thickness",1);
    xs2pdf(gcf(), PATH+"\class_traj_5");
end



/////////////////////
// Draw all graphics for typical cases
////////
if ~%t then
	typical = [36, 189, 96, 32, 14];
	
	i = 0;
	for num = typical
	    i = i + 1;
	    DrawAndSaveGraphics(num, sprintf("class_typical_%i", i), T, X, Y, YAW, ROLL, WAVE_HEIGHT, ANGULAR);
	end
end

//Draw Sum angle-Shift chart where point size depends on max roll angle
if ~%t then
    clf();
    xtitle("", "Angle sum", "Shift");
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class1);
    plot2d([0 250 250 0], [250 250 0 0], 1);
    DrawPointsRoll(ang, dist, maxroll,1);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class2);
    plot2d([0 250 250 0], [250 250 0 0], 2);
    DrawPointsRoll(ang, dist, maxroll,2);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class3);
    plot2d([0 250 250 0], [250 250 0 0], 3);
    DrawPointsRoll(ang, dist, maxroll, 3);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class4);
    plot2d([0 250 250 0], [250 250 0 0], 4);
    DrawPointsRoll(ang, dist, maxroll,4);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class5);
    plot2d([0 250 250 0], [250 250 0 0], 5);
    DrawPointsRoll(ang, dist, maxroll,5);
    
    legend(["1" "2" "3" "4" "5"]);
    plot2d([0 250 250 0], [250 250 0 0], 1);
    a=get("current_axes");
    a.isoview = "on";
    
    xs2pdf(gcf(), PATH+"\angle_shift_rollsized");
end

//Draw Sum angle-Shift chart where point size depends on "energy"
if ~%t then
    clf();
    xtitle("", "Angle sum", "Shift");
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class1);
    plot2d([0 250 250 0], [250 250 0 0], 1);
    DrawPointsEnergy(ang, dist, maxroll,1);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class2);
    plot2d([0 250 250 0], [250 250 0 0], 2);
    DrawPointsEnergy(ang, dist, maxroll,2);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class3);
    plot2d([0 250 250 0], [250 250 0 0], 3);
    DrawPointsEnergy(ang, dist, maxroll, 3);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class4);
    plot2d([0 250 250 0], [250 250 0 0], 4);
    DrawPointsEnergy(ang, dist, maxroll,4);
    
    [ang, dist, maxroll] = CalcParamsForClass(Y, YAW, ROLL, class5);
    plot2d([0 250 250 0], [250 250 0 0], 5);
    DrawPointsEnergy(ang, dist, maxroll,5);
    
    legend(["1" "2" "3" "4" "5"]);
    plot2d([0 250 250 0], [250 250 0 0], 1);
    a=get("current_axes");
    a.isoview = "on";
    
    xs2pdf(gcf(), PATH+"\angle_shift_energysized");
end


//clf();
////[cov, Mean] = corr(ANGULAR, ROLL, 1);
////
//co = [];
//for j = 1:200
//    co(j) = CorForVectors(YAW(j,:), ROLL(j,:));
//end
//plot2d(1:200,co, 2);
//xtitle("Yaw-roll correlation", "Experiment number", "yaw-roll correlation");
//xs2png(gcf(), PATH+"\yaw_roll_corellation");
//
//clf();
//co = [];
//for j = 1:200
//    co(j) = CorForVectors(ANGULAR(j,:), ROLL(j,:));
//end
//plot2d(1:200,co, 2);
//xtitle("Angular-roll correlation", "Experiment number", "angular-roll correlation");
//xs2png(gcf(), PATH+"\angular_roll_corellation");
//
//clf();
//co = [];
//for j = 1:200
//    co(j) = CorForVectors(ANGULAR(j,:), YAW(j,:));
//end
//plot2d(1:200,co, 2);
//xtitle("Angular-yaw correlation", "Experiment number", "angular-yaw correlation");
//xs2png(gcf(), PATH+"\angular_yaw_corellation");
//
//clf();
//co = [];
//for j = 1:200
//    co(j) = CorForVectors(ROLL(j,:), Y(j,:));
//end
//plot2d(1:200,co, 2);
//xtitle("Roll-Y correlation", "Experiment number", "roll-y correlation");
//xs2png(gcf(), PATH+"\roll_y_corellation");
