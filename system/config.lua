-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.SetVar('d3d_fullscr'                , false);
cfg.SetVar('d3d_mode'                   , 2);
cfg.SetVar('d3d_xpos'                   , 148);
cfg.SetVar('d3d_ypos'                   , 94);


-- key bindings --

unbindAll()
bind ("PGUP", "_DistInc()");
bind ("PGDN", "_DistDec()");
bind ("LEFT", "_YawDec()");
bind ("UP", "_PitchInc()");
bind ("RIGHT", "_YawInc()");
bind ("DOWN", "_PitchDec()");
bind ("A", "_ShipSL()");
bind ("M", "state.sunking 	= not state.sunking");
bind ("N", "state.submersion = not state.submersion");
bind ("S", "_ShipFW()");
bind ("X", "_ShipSR()");
bind ("Z", "_ShipBW()");
bind ("F5", "dofile('run.lua')");
bind ("F6", "SCI_ReloadShaders()");
bind ("F10", "quit()");
