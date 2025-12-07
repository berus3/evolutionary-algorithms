#include <jni.h>
#include "org_evol_Poc.h"
#include "combat_engine.hpp"

extern "C"
JNIEXPORT jlong JNICALL
Java_org_evol_Poc_nativeLoop(JNIEnv*, jclass, jlong n) {
    return (jlong) runLoop((int64_t)n);
}
