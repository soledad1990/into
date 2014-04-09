/* This file is part of Into.
 * Copyright (C) Intopii 2014.
 * All rights reserved.
 *
 * Licensees holding a commercial Into license may use this file in
 * accordance with the commercial license agreement. Please see
 * LICENSE.commercial for commercial licensing terms.
 *
 * Alternatively, this file may be used under the terms of the GNU
 * Affero General Public License version 3 as published by the Free
 * Software Foundation. In addition, Intopii gives you special rights
 * to use Into as a part of open source software projects. Please
 * refer to LICENSE.AGPL3 for details.
 */

#ifndef _TESTPIITHREADSAFETIMER_H
#define _TESTPIITHREADSAFETIMER_H

#include <QObject>

class PiiThreadSafeTimer;

class TestPiiThreadSafeTimer : public QObject
{
  Q_OBJECT

public:
  TestPiiThreadSafeTimer();

private slots:
  void oneThread();
  void twoThreads();

protected slots:
  void count();

private:
  void stopTimer(PiiThreadSafeTimer* timer);

  bool _bStopperRunning;
  int _iCount;
  PiiThreadSafeTimer* _pCurrentTimer;
};


#endif //_TESTPIITHREADSAFETIMER_H
