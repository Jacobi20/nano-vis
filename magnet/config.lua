-- engine settings --
-- warning: syntax errors will cause configuration resetting --

cfg.setvar('rs_gl_driver'                  , false);
cfg.setvar('rs_show_gbuffer'               , false);
cfg.setvar('gl_fullscr'                    , false);
cfg.setvar('gl_mode'                       , 0);
cfg.setvar('gl_xpos'                       , 263);
cfg.setvar('gl_ypos'                       , 194);
cfg.setvar('gl_stereo'                     , false);
cfg.setvar('fmod_prof'                     , false);
cfg.setvar('fmod_show_stats'               , false);
cfg.setvar('sound_spread'                  , 0);
cfg.setvar('sound_dopler_level'            , 1);
cfg.setvar('sound_max_distance'            , 10000);
cfg.setvar('sound_min_distance'            , 1);
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
cfg.setvar('phys_linear_damping'           , 0.1);
cfg.setvar('phys_angular_damping'          , 0.1);
cfg.setvar('phys_linear_sleep_treshold'    , 0.01);
cfg.setvar('phys_angular_sleep_treshold'   , 0.01);
cfg.setvar('phys_contact_proc_treshold'    , 0.01);
cfg.setvar('fr_wireframe'                  , false);
cfg.setvar('fr_skip_mlaa'                  , false);
cfg.setvar('fr_skip_hdr'                   , false);
cfg.setvar('fr_skip_fog'                   , false);
cfg.setvar('fr_skip_noise'                 , false);
cfg.setvar('fr_gamma'                      , 1);
cfg.setvar('fr_stereo_factor'              , 1);


-- key bindings --

input.unbindAll()
input.bind ("F5", "engine.setGame(nil)", "");
