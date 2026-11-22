#ifndef QUASARAAPP_GLOBAL_H
#define QUASARAAPP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QUASARAPP_LIBRARY)
  /// @brief QUASARAPPSHARED_EXPORT This is the export/import macro of the QuasarApp library.
  /// It expands to Q_DECL_EXPORT when the library itself is being built and to Q_DECL_IMPORT otherwise.
#  define QUASARAPPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QUASARAPPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QUASARAAPP_GLOBAL_H
