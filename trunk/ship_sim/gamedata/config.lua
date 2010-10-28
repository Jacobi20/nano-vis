-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.setvar('d3d_wireframe'                 , nil);
cfg.setvar('d3d_fullscr'                   , false);
cfg.setvar('d3d_mode'                      , 2);
cfg.setvar('d3d_xpos'                      , 190);
cfg.setvar('d3d_ypos'                      , 37);
cfg.setvar('d3d_anisotropy_level'          , 16);
cfg.setvar('d3d_mipmap_lod_bias'           , 0);
cfg.setvar('ship_show_hull'                , true);
cfg.setvar('ship_show_voxels'              , false);
cfg.setvar('ship_show_submerge'            , false);
cfg.setvar('ship_hsf_method'               , 'hxfse');
cfg.setvar('d3d_antialiasing'              , 1);
cfg.setvar('phys_debug'                    , false);
cfg.setvar('phys_draw_wireframe'           , false);
cfg.setvar('phys_draw_aabb'                , false);
cfg.setvar('phys_draw_features_text'       , false);
cfg.setvar('phys_draw_contact_points'      , false);
cfg.setvar('phys_no_deactivation'          , false);
cfg.setvar('phys_no_help_text'             , false);
cfg.setvar('phys_draw_text'                , false);
cfg.setvar('phys_profile_timings'          , false);
cfg.setvar('phys_enable_sat_comparison'    , false);
cfg.setvar('phys_disable_bullet_lcp'       , false);
cfg.setvar('phys_enable_ccd'               , false);
cfg.setvar('phys_draw_constraints'         , false);
cfg.setvar('phys_draw_constraint_limits'   , false);
cfg.setvar('phys_fast_wireframe'           , false);


-- key bindings --

input.unbindall()
input.bind ("PGUP", "_DistInc()");
input.bind ("PGDN", "_DistDec()");
input.bind ("LEFT", "_YawInc()");
input.bind ("UP", "_PitchInc()");
input.bind ("RIGHT", "_YawDec()");
input.bind ("DOWN", "_PitchDec()");
input.bind ("1", "dofile('exp01/exp01.lua')");
input.bind ("2", "dofile('exp02/exp02.lua')");
input.bind ("3", "dofile('exp03/exp03.lua')");
input.bind ("4", "dofile('exp04/exp04.lua')");
input.bind ("A", "_ShipSL()");
input.bind ("M", "state.sunking 		= not state.sunking");
input.bind ("N", "state.submersion 		= not state.submersion");
input.bind ("S", "_ShipFW()");
input.bind ("X", "_ShipSR()");
input.bind ("Z", "_ShipBW()");
input.bind ("F1", "show_info()");
input.bind ("F2", "setup_rolling_on_silent_water()");
input.bind ("F3", "setup_rolling_on_sin_wave()");
input.bind ("F4", "setup_rolling_on_wind_wave()");
input.bind ("F5", "dofile('run.lua')");
input.bind ("F6", "rs.reload_shaders()");
input.bind ("F7", "do_rolling()");
input.bind ("F10", "core.quit()");
