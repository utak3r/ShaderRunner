#ifndef GLVIEWWIDGET_GLOBAL_H
#define GLVIEWWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GLVIEWWIDGET_LIBRARY)
#  define GLVIEWWIDGETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GLVIEWWIDGETSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GLVIEWWIDGET_GLOBAL_H
