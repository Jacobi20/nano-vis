-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.setvar('d3d_fullscr'                   , false);
cfg.setvar('d3d_mode'                      , 0);
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


-- key bindings --

input.unbindAll()
input.bind ("RBUTTON", "cgame.jump		(true)", "cgame.jump		(false)");
input.bind ("SPACE", "cgame.moveUp	(true)", "cgame.moveUp		(false)");
input.bind ("LEFT", "cgame.turnLeft	(true)", "cgame.turnLeft	(false)");
input.bind ("UP", "cgame.turnUp	(true)", "cgame.turnUp		(false)");
input.bind ("RIGHT", "cgame.turnRight(true)", "cgame.turnRight	(false)");
input.bind ("DOWN", "cgame.turnDown	(true)", "cgame.turnDown	(false)");
input.bind ("A", "cgame.stepLeft	(true)", "cgame.stepLeft	(false)");
input.bind ("C", "cgame.moveDown	(true)", "cgame.moveDown	(false)");
input.bind ("M", "cgame.orbit_angle = cgame.orbit_angle - 15", "");
input.bind ("N", "cgame.orbit_angle = cgame.orbit_angle + 15", "");
input.bind ("S", "cgame.forward	(true)", "cgame.forward		(false)");
input.bind ("X", "cgame.stepRight(true)", "cgame.stepRight	(false)");
input.bind ("Z", "cgame.backward	(true)", "cgame.backward	(false)");
input.bind ("F1", "cfg.load()", "");
input.bind ("F5", "if reloadPackages then reloadPackages() end; game.runGame('scenes/dm1.esx')", "");
input.bind ("F6", "render.reloadShaders()", "");
input.bind ("F10", "", "core.quit()");
