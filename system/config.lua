-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.setvar('d3d_fullscr'                , false);
cfg.setvar('d3d_mode'                   , 2);
cfg.setvar('d3d_xpos'                   , 229);
cfg.setvar('d3d_ypos'                   , 139);


-- key bindings --

input.unbindall()
input.bind ("PGUP", "_DistInc()");
input.bind ("PGDN", "_DistDec()");
input.bind ("LEFT", "_YawDec()");
input.bind ("UP", "_PitchInc()");
input.bind ("RIGHT", "_YawInc()");
input.bind ("DOWN", "_PitchDec()");
input.bind ("A", "_ShipSL()");
input.bind ("M", "state.sunking 	= not state.sunking");
input.bind ("N", "state.submersion = not state.submersion");
input.bind ("S", "_ShipFW()");
input.bind ("X", "_ShipSR()");
input.bind ("Z", "_ShipBW()");
input.bind ("F5", "dofile('run.lua')");
input.bind ("F6", "SCI_ReloadShaders()");
input.bind ("F10", "core.quit()");
