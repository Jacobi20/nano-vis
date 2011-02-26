-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.setvar('d3d_fullscr'                   , false);
cfg.setvar('d3d_mode'                      , 5);
cfg.setvar('d3d_xpos'                      , 224);
cfg.setvar('d3d_ypos'                      , 68);
cfg.setvar('d3d_antialiasing'              , 0);
cfg.setvar('d3d_anisotropy_level'          , 16);
cfg.setvar('d3d_mipmap_lod_bias'           , 0);
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
cfg.setvar('phys_max_substeps'             , 10);
cfg.setvar('phys_fixed_timestep'           , 0.0166667);
cfg.setvar('phys_linear_damping'           , 0.01);
cfg.setvar('phys_angular_damping'          , 0.01);
cfg.setvar('phys_linear_sleep_treshold'    , 0.01);
cfg.setvar('phys_angular_sleep_treshold'   , 0.01);
cfg.setvar('phys_contact_proc_treshold'    , 0.01);
cfg.setvar('fr_wireframe'                  , false);
cfg.setvar('ship_show_grid'                , false);
cfg.setvar('ship_disable_simulation'       , false);
cfg.setvar('rs_gl_driver'                  , false);
cfg.setvar('rs_show_gbuffer'               , false);
cfg.setvar('gl_fullscr'                    , false);
cfg.setvar('gl_mode'                       , 0);
cfg.setvar('gl_xpos'                       , 547);
cfg.setvar('gl_ypos'                       , 174);
cfg.setvar('fr_skip_mlaa'                  , true);
cfg.setvar('fr_skip_hdr'                   , false);
cfg.setvar('fr_skip_fog'                   , false);
cfg.setvar('fr_skip_noise'                 , false);
cfg.setvar('gl_stereo'                     , false);
cfg.setvar('fmod_prof'                     , false);
cfg.setvar('fmod_show_stats'               , false);
cfg.setvar('sound_spread'                  , 0);
cfg.setvar('sound_dopler_level'            , 1);
cfg.setvar('sound_max_distance'            , 10000);
cfg.setvar('sound_min_distance'            , 1);


-- key bindings --

input.unbindAll()
input.bind ("RBUTTON", "cgame.jump		(true)", "cgame.jump		(false)");
input.bind ("SPACE", "control.state.up = true", "control.state.up = false");
input.bind ("LEFT", "control.state.tl = true", "control.state.tl = false");
input.bind ("UP", "control.state.tu = true", "control.state.tu = false");
input.bind ("RIGHT", "control.state.tr = true", "control.state.tr = false");
input.bind ("DOWN", "control.state.td = true", "control.state.td = false");
input.bind ("A", "control.state.sl = true", "control.state.sl = false");
input.bind ("C", "control.state.dn = true", "control.state.dn = false");
input.bind ("M", "cgame.orbit_angle = cgame.orbit_angle - 15", "");
input.bind ("N", "cgame.orbit_angle = cgame.orbit_angle + 15", "");
input.bind ("S", "control.state.fw = true", "control.state.fw = false");
input.bind ("X", "control.state.sr = true", "control.state.sr = false");
input.bind ("Z", "control.state.bw = true", "control.state.bw = false");
input.bind ("F1", "cfg.load()", "");
input.bind ("F5", "if reloadPackages then reloadPackages() end; game.runGame('scenes/base.esx')", "");
input.bind ("F6", "render.reloadShaders()", "");
input.bind ("F10", "", "core.quit()");
