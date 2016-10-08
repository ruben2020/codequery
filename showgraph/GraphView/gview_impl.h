/**
 * @file: gview_impl.h 
 * Implementational header for QtGUI sub-project
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef GVIEW_IMPL_H
#define GVIEW_IMPL_H

#include "gview_iface.h"
#include <math.h>
/** Pi definition */
static const double Pi = 3.14159265358979323846264338327950288419717;
/**  2 * Pi constant */
static double TwoPi = 2.0 * Pi;

/** Arrow size constant */
const qreal arrowSize = 10;

/** Self edge vertical margin*/
const qreal SE_VERT_MARGIN = 20;

/** Self edge horizontal margin*/
const qreal SE_HOR_MARGIN = 20;

/** Node speed */
const qreal NODE_SPEED = 4;

/** Opacity step */
const qreal OPACITY_STEP = 0.1;

/** Context visible border */
const int MAX_VISIBLE_LEN = 3;

/** Context visible border */
const int GVIEW_MAX_PRIORITY = 6;

/** Context far border */
const int MAX_PLACE_LEN = 3;

#endif /* GVIEW_IMPL_H */
