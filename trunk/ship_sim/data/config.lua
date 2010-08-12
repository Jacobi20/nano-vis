-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.setvar('d3d_wireframe'              , false);
cfg.setvar('d3d_fullscr'                , false);
cfg.setvar('d3d_mode'                   , 0);
cfg.setvar('d3d_xpos'                   , 369);
cfg.setvar('d3d_ypos'                   , 203);
cfg.setvar('d3d_anisotropy_level'       , 16);
cfg.setvar('d3d_mipmap_lod_bias'        , 0);
cfg.setvar('ship_show_hull'             , true);
cfg.setvar('ship_show_voxels'           , false);
cfg.setvar('ship_show_submerge'         , false);
cfg.setvar('ship_hsf_method'            , 'surface');


-- key bindings --

input.unbindall()
input.bind ("PGUP", "_DistInc()");
input.bind ("PGDN", "_DistDec()");
input.bind ("LEFT", "_YawInc()");
input.bind ("UP", "_PitchInc()");
input.bind ("RIGHT", "_YawDec()");
input.bind ("DOWN", "_PitchDec()");
input.bind ("A", "_ShipSL()");
input.bind ("M", "state.sunking 		= not state.sunking");
input.bind ("N", "state.submersion 		= not state.submersion");
input.bind ("S", "_ShipFW()");
input.bind ("X", "_ShipSR()");
input.bind ("Z", "_ShipBW()");
input.bind ("F1", "show_info()");
input.bind ("F2", "ship_show_hull      = not ship_show_hull");
input.bind ("F3", "ship_show_voxels    = not ship_show_voxels");
input.bind ("F4", "ship_show_submerge  = not ship_show_submerge");
input.bind ("F5", "dofile('run.lua')");
input.bind ("F6", "naval.reload_shaders()");
input.bind ("F10", "core.quit()");
