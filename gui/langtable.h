
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef LANGTABLE_H_CQ
#define LANGTABLE_H_CQ

// Forward declarations
class QString;
//class QStringList;

class langtable
{

public:
static QStringList getLangNameList(void);
static QString getLangFilePath(const QString& lang);

};

#endif // LANGTABLE_H_CQ

