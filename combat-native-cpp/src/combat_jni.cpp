#include <jni.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include "org_evol_RPGNativeBridge.h"

#include "combat_engine.hpp"
#include "team.hpp"
#include "player.hpp"
#include "anchors.hpp"
#include "genome_decode.hpp"

/* ============================================================
   Helpers JNI
   ============================================================ */
static void throwIAE(JNIEnv* env, const char* msg) {
    jclass ex = env->FindClass("java/lang/IllegalArgumentException");
    if (ex) env->ThrowNew(ex, msg);
}

static void throwISE(JNIEnv* env, const char* msg) {
    jclass ex = env->FindClass("java/lang/IllegalStateException");
    if (ex) env->ThrowNew(ex, msg);
}

/* ============================================================
   ANCHORS (hardcodeados)
   ============================================================ */

static std::vector<Team*> g_anchors;

static void initAnchorsOnce() {
    if (!g_anchors.empty()) return;

    g_anchors.reserve(ANCHOR_COUNT);
    for (int a = 0; a < ANCHOR_COUNT; a++) {
        Team* t = buildTeamFromGenome(ANCHOR_GENOMES[a], 10000 + a);
        g_anchors.push_back(t);
    }
}


/* ============================================================
   JNI evaluatePopulation — usa combat_engine::winrate_* con anchors
   ============================================================ */
extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_org_evol_RPGNativeBridge_evaluatePopulation(
    JNIEnv* env,
    jclass,
    jintArray flatPopulation,
    jint populationSize
) {
    if (flatPopulation == nullptr) {
        throwIAE(env, "flatPopulation is null");
        return nullptr;
    }
    if (populationSize <= 0) {
        throwIAE(env, "populationSize must be > 0");
        return nullptr;
    }

    // init anchors 1 vez
    initAnchorsOnce();

    jsize totalLen = env->GetArrayLength(flatPopulation);
    const jsize expected = (jsize)populationSize * (jsize)GENOME_SIZE;

    static bool printedOnce = false;
    if (!printedOnce) {
        printedOnce = true;
        std::cerr << "[JNI] popSize=" << (int)populationSize
                  << " totalLen=" << (int)totalLen
                  << " expected=" << (int)expected
                  << " GENOME_SIZE=" << GENOME_SIZE
                  << " anchors=" << (int)g_anchors.size()
                  << std::endl;
    }

    if (totalLen != expected) {
        throwIAE(env, "flatPopulation length mismatch (expected popSize*500)");
        return nullptr;
    }

    jint* data = env->GetIntArrayElements(flatPopulation, nullptr);
    if (data == nullptr) {
        throwISE(env, "GetIntArrayElements returned null");
        return nullptr;
    }

    static bool printedIntsOnce = false;
    if (!printedIntsOnce) {
        printedIntsOnce = true;
        std::cerr << "[JNI] first 20 genes: ";
        for (int i = 0; i < 20; i++) std::cerr << data[i] << " ";
        std::cerr << std::endl;
    }

    // Construcción de equipos
    std::vector<Team*> teams;
    teams.reserve((size_t)populationSize);

    for (int i = 0; i < populationSize; i++) {
        const int* genome_i = reinterpret_cast<const int*>(data + (i * GENOME_SIZE));
        Team* t = buildTeamFromGenome(genome_i, i);
        teams.push_back(t);
    }

    // Calcular winrate incluyendo anchors:
    // - Random-k dentro de población + todos los anchors
    std::vector<double> wr = winrate_anchor_random_k(teams, g_anchors, 5);

    // (alternativa: round-robin completo + anchors)
    // std::vector<double> wr = winrate_anchor(teams, g_anchors);

    env->ReleaseIntArrayElements(flatPopulation, data, 0);

    if ((int)wr.size() != populationSize) {
        for (Team* t : teams) delete t;
        throwISE(env, "winrate_anchor_* returned vector size != populationSize");
        return nullptr;
    }

    jdoubleArray out = env->NewDoubleArray(populationSize);
    if (out == nullptr) {
        for (Team* t : teams) delete t;
        throwISE(env, "NewDoubleArray failed");
        return nullptr;
    }

    std::vector<jdouble> jwr(populationSize);
    for (int i = 0; i < populationSize; i++) {
        jwr[i] = (jdouble)wr[i];
    }
    env->SetDoubleArrayRegion(out, 0, populationSize, jwr.data());

    for (Team* t : teams) delete t;

    return out;
}
