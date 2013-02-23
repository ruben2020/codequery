/**
 * @file: gview_impl.h 
 * Implementational header for QtGUI sub-project
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
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
const int MAX_PRIORITY = 6;

/** Context far border */
const int MAX_PLACE_LEN = 3;

#endif /* GVIEW_IMPL_H */