/* This file is part of Into.
 * Copyright (C) Intopii 2013.
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

#include "PiiQtTypeWrapper.h"
#include "PiiScript.h"

#include <QSize>
#include <QSizeF>
#include <QRect>
#include <QRectF>
#include <QColor>

namespace PiiQtTypeWrapper
{
  PII_STATIC_TR_FUNC(PiiQtTypeWrapper)

#define PII_SCRIPT_CLASS QPoint
#define PII_SCRIPT_PROPERTIES (Int32, x, ), (Int32, y, )
#define PII_SCRIPT_PROPERTY_CNT 2
#include "PiiScriptClassWrapper.h"

#define PII_SCRIPT_CLASS QPointF
#define PII_SCRIPT_PROPERTIES (Number, x, ), (Number, y, )
#define PII_SCRIPT_PROPERTY_CNT 2
#include "PiiScriptClassWrapper.h"

#define PII_SCRIPT_CLASS QSize
#define PII_SCRIPT_PROPERTIES (Int32, w, idth), (Int32, h, eight)
#define PII_SCRIPT_PROPERTY_CNT 2
#include "PiiScriptClassWrapper.h"

#define PII_SCRIPT_CLASS QSizeF
#define PII_SCRIPT_PROPERTIES (Number, w, idth), (Number, h, eight)
#define PII_SCRIPT_PROPERTY_CNT 2
#include "PiiScriptClassWrapper.h"

#define PII_SCRIPT_CLASS QRect
#define PII_SCRIPT_PROPERTIES (Int32, x, ), (Int32, y, ), (Int32, w, idth), (Int32, h, eight)
#define PII_SCRIPT_PROPERTY_CNT 4
#include "PiiScriptClassWrapper.h"

#define PII_SCRIPT_CLASS QRectF
#define PII_SCRIPT_PROPERTIES (Number, x, ), (Number, y, ), (Number, w, idth), (Number, h, eight)
#define PII_SCRIPT_PROPERTY_CNT 4
#include "PiiScriptClassWrapper.h"

  static QScriptValue createQColor(QScriptContext *context, QScriptEngine *engine)
  {
    switch (context->argumentCount())
      {
      case 1:
        {
          QColor clr(context->argument(0).toString());
          if (!clr.isValid())
            return context->throwError(tr("Invalid color specification."));
          context->thisObject().setProperty("red", clr.red());
          context->thisObject().setProperty("green", clr.green());
          context->thisObject().setProperty("blue", clr.blue());
        }
        break;
      case 4:
        context->thisObject().setProperty("alpha", context->argument(3).toInt32());
      case 3:
        context->thisObject().setProperty("red", context->argument(0).toInt32());
        context->thisObject().setProperty("green", context->argument(1).toInt32());
        context->thisObject().setProperty("blue", context->argument(2).toInt32());
        break;
      default:
        return context->throwError(tr(PiiScript::pTakesNArguments).arg("QColor").arg("{1,3,4}"));
      }
    return engine->undefinedValue();
  }

#define PII_SCRIPT_CLASS QColor
#define PII_SCRIPT_PROPERTIES (Int32, r, ed), (Int32, g, reen), (Int32, b, lue), (Int32, a, lpha)
#define PII_SCRIPT_PROPERTY_CNT 4
#define PII_SCRIPT_CUSTOM_CONSTRUCTOR
#include "PiiScriptClassWrapper.h"
}

#define PII_REGISTER_CUSTOM_SCRIPT_CLASS(CLASS) \
  PII_REGISTER_SCRIPT_TYPE_CONVERSION(PiiQtTypeWrapper, CLASS);         \
  engine->globalObject().setProperty(PII_STRINGIZE(CLASS), engine->newFunction(PII_JOIN(PiiQtTypeWrapper::create, CLASS)))

void initQtTypes(QScriptEngine* engine)
{
  PII_REGISTER_CUSTOM_SCRIPT_CLASS(QPoint);
  PII_REGISTER_CUSTOM_SCRIPT_CLASS(QPointF);
  PII_REGISTER_CUSTOM_SCRIPT_CLASS(QSize);
  PII_REGISTER_CUSTOM_SCRIPT_CLASS(QSizeF);
  PII_REGISTER_CUSTOM_SCRIPT_CLASS(QRect);
  PII_REGISTER_CUSTOM_SCRIPT_CLASS(QRectF);
  PII_REGISTER_CUSTOM_SCRIPT_CLASS(QColor);
}
