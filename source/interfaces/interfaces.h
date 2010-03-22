/*
    The MIT License

    Copyright (c) 2010 IFMO/GameDev Studio

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
//  interfaces.h    -   list of all interfaces


#pragma once


//	Base classes :
#include "i_disposable.h"
#include "i_locatable.h"

//	Core systems :
#include "i_atoms.h"
#include "i_core.h"
#include "i_log.h"
#include "i_system.h"

//	Engine systems :
#include "i_config.h"
#include "i_shell.h"
#include "i_filesys.h"
#include "i_geometry.h"

//	Driver and game systems :
#include "i_input.h"
#include "i_rendersystem.h"
#include "i_physics.h"
#include "i_game.h"
#include "i_sound_system.h"
#include "i_netsys.h"
#include "i_ui.h"

//	Scientific stuff :
#include "i_nanovis.h"