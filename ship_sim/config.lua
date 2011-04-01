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
cfg.setvar('phys_fixed_timestep'           , 0.01);
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
cfg.setvar('gl_xpos'                       , 169);
cfg.setvar('gl_ypos'                       , 131);
cfg.setvar('fr_skip_mlaa'                  , false);
cfg.setvar('fr_skip_hdr'                   , false);
cfg.setvar('fr_skip_fog'                   , false);
cfg.setvar('fr_skip_noise'                 , false);
cfg.setvar('gl_stereo'                     , false);
cfg.setvar('fmod_prof'                     , false);
cfg.setvar('fmod_show_stats'               , true);
cfg.setvar('sound_spread'                  , 45);
cfg.setvar('sound_dopler_level'            , 1);
cfg.setvar('sound_max_distance'            , 10000);
cfg.setvar('sound_min_distance'            , 1);
cfg.setvar('fr_waving_wind'                , 0);
cfg.setvar('fr_waving_max_frequency'       , 6);
cfg.setvar('fr_waving_narrowness'          , 8);
cfg.setvar('fr_waving_num_harmonics'       , 20);
cfg.setvar('fr_waving_num_dirs'            , 12);
cfg.setvar('fr_waving_num_linear_samples'  , 1024);
cfg.setvar('fr_waving_num_time_samples'    , 64);
cfg.setvar('fr_gamma'                      , 0.9);
cfg.setvar('fr_stereo_factor'              , -0.3);
cfg.setvar('fr_waving_omega'               , 0);


-- key bindings --

input.unbindAll()
input.bind ("RBUTTON", "cgame.jump		(true)", "cgame.jump		(false)");
input.bind ("SPACE", "control.state.up = true", "control.state.up = false");
input.bind ("PGDN", "control.state.ship_r = true", "control.state.ship_r = false");
input.bind ("END", "control.state.ship_bw = true", "control.state.ship_bw = false");
input.bind ("HOME", "control.state.ship_fw = true", "control.state.ship_fw = false");
input.bind ("LEFT", "control.state.tl = true", "control.state.tl = false");
input.bind ("UP", "control.state.tu = true", "control.state.tu = false");
input.bind ("RIGHT", "control.state.tr = true", "control.state.tr = false");
input.bind ("DOWN", "control.state.td = true", "control.state.td = false");
input.bind ("DEL", "control.state.ship_l = true", "control.state.ship_l = false");
input.bind ("A", "control.state.sl = true", "control.state.sl = false");
input.bind ("C", "control.state.dn = true", "control.state.dn = false");
input.bind ("H", "game.stopCameraAnim(); control.state.ship_fw = false", "");
input.bind ("J", "game.playCameraAnim('scenes/deckhouse_anim.eax', 20, true)", "");
input.bind ("K", "game.stopCameraAnim(); control.state.ship_fw = false", "");
input.bind ("L", "game.playCameraAnim('scenes/uboat_xxi.eax', 30, false); control.state.ship_fw = true", "");
input.bind ("M", "control.state.ship_fw = true", "");
input.bind ("N", "control.state.ship_fw = false", "");
input.bind ("O", "control.state.id = true", "control.state.id = false");
input.bind ("P", "control.state.iu = true", "control.state.iu = false");
input.bind ("Q", "cgame.restart_log()", "");
input.bind ("R", "cgame.touch_ship_roll()", "");
input.bind ("S", "control.state.fw = true", "control.state.fw = false");
input.bind ("T", "cgame.touch_ship_heave()", "");
input.bind ("W", "cgame.setup_waving()", "");
input.bind ("X", "control.state.sr = true", "control.state.sr = false");
input.bind ("Z", "control.state.bw = true", "control.state.bw = false");
input.bind ("F1", "cfg.load()", "");
input.bind ("F5", "if reloadPackages then reloadPackages() end; game.runGame('scenes/base.esx')", "");
input.bind ("F6", "render.reloadShaders()", "");
input.bind ("F7", "cgame.do_rolling()", "");
input.bind ("F10", "", "core.quit()");
