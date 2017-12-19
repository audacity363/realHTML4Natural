#ifndef RH4N_JSONHANDLERS
#define RH4N_JSONHANDLERS

int (*getHandlerFuncton(jint vartype))(JNIEnv*, HandlerArgs);

#include "plain_handlers.h"
#include "1d_handlers.h"
#include "2d_handlers.h"
#include "3d_handlers.h"

#endif
