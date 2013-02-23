/**
 * @file: utils_iface.h 
 * Interface of Utils library in ShowGraph
 * @defgroup Utils Utils
 * @ingroup Core
 * @brief Low level utils
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef UTILS_IFACE_H
#define UTILS_IFACE_H
#include <stdio.h>
#include <QTextStream>
#include <QtGlobal>
#include "misc.h"
#include "asrt.h"
#include "print.h"
#include "list.h"
#include "singleton.h"
#include "mem.h"
#include "conf.h"


namespace Utils
{
    using namespace PrintUtils;
};

/**
 * Test list classes
 */
bool uTestList();

/**
 * Test memory manager
 */
bool uTestMem();

/**
 * Test configuration-related functionality
 */
bool uTestConf();

#endif