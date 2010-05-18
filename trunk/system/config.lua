-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.SetVar('d3d_fullscr'                , false);
cfg.SetVar('d3d_mode'                   , 0);
cfg.SetVar('d3d_xpos'                   , 1500);
cfg.SetVar('d3d_ypos'                   , 100);


-- key bindings --

unbindAll()
bind ("LEFT", "_YawInc()");
bind ("UP", "_RollInc()");
bind ("RIGHT", "_YawDec()");
bind ("DOWN", "_RollDec()");
bind ("F5", "dofile('run.lua')");
bind ("F10", "quit()");
