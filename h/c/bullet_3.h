//
// 1-Jan-1999
// Bullet 3 master include
//

#ifndef BULLET_3_H
#define BULLET_3_H

#if defined(BLT_WIN32)
 #include "b3win32.h"
#elif defined(BLT_WIN16)
 #include "b3win16.h"
#elif defined(BLT_OS232)
 #include "b3os232.h"
#elif defined(BLT_DOS32)
 #include "b3dos32.h"
#elif defined(BLT_DOS16)
 #include "b3dos16.h"
#else
 #error [Bullet] Platform (BLT_WIN32, etc.) define was not specified
#endif

#endif
