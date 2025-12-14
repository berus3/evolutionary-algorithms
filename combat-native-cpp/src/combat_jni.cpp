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
#include "instance.hpp"
#include "rng_context.hpp"   // IMPORTANTE: GLOBAL_SEED

// helpers JNI
static void throwIAE(JNIEnv* env, const char* msg) {
    jclass ex = env->FindClass("java/lang/IllegalArgumentException");
    if (ex) env->ThrowNew(ex, msg);
}

static void throwISE(JNIEnv* env, const char* msg) {
    jclass ex = env->FindClass("java/lang/IllegalStateException");
    if (ex) env->ThrowNew(ex, msg);
}

// RNG seed (STATELESS RNG) Java es la fuente de verdad de la seed
extern "C"
JNIEXPORT void JNICALL
Java_org_evol_RPGNativeBridge_setSeed(
    JNIEnv*,
    jclass,
    jint seed
) {
    GLOBAL_SEED = static_cast<uint64_t>(seed);
    std::cerr << "[JNI] GLOBAL_SEED set to " << GLOBAL_SEED << std::endl;
}

// instance selection
extern "C"
JNIEXPORT void JNICALL
Java_org_evol_RPGNativeBridge_setInstance(
    JNIEnv*,
    jclass,
    jint instanceId
) {
    if (instanceId < 0 || instanceId > 2) {
        std::cerr << "[JNI] Invalid instance id: "
                  << instanceId << std::endl;
        return;
    }

    chooseInstance(static_cast<Instance>(instanceId));
    std::cerr << "[JNI] Instance set to " << instanceId << std::endl;
}

// hardcoded anchors
static std::vector<Team*> g_anchors;

static void initAnchorsOnce() {
    if (!g_anchors.empty()) return;

    g_anchors.reserve(ANCHOR_COUNT);
    for (int a = 0; a < ANCHOR_COUNT; a++) {
        Team* t = buildTeamFromGenome(ANCHOR_GENOMES[a], 10000 + a); //TODO revisar
        g_anchors.push_back(t);
    }
}

// JNI evaluatePopulation

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

    initAnchorsOnce();

    jsize totalLen = env->GetArrayLength(flatPopulation);
    const jsize expected = (jsize)populationSize * (jsize)GENOME_SIZE;

    if (totalLen != expected) {
        throwIAE(env, "flatPopulation length mismatch (expected popSize*GENOME_SIZE)");
        return nullptr;
    }

    jint* data = env->GetIntArrayElements(flatPopulation, nullptr);
    if (data == nullptr) {
        throwISE(env, "GetIntArrayElements returned null");
        return nullptr;
    }

    std::vector<Team*> teams;
    teams.reserve((size_t)populationSize);

    for (int i = 0; i < populationSize; i++) {
        const int* genome_i = data + (i * GENOME_SIZE);
        teams.push_back(buildTeamFromGenome(genome_i, i));
    }

    static const std::vector<Team*> empty_hof;
	std::vector<double> wr = winrate_anchor_random_k(teams, g_anchors, empty_hof, 10);

    env->ReleaseIntArrayElements(flatPopulation, data, 0);

    if ((int)wr.size() != populationSize) {
        for (Team* t : teams) delete t;
        throwISE(env, "winrate returned wrong size");
        return nullptr;
    }

    jdoubleArray out = env->NewDoubleArray(populationSize);
    if (!out) {
        for (Team* t : teams) delete t;
        throwISE(env, "NewDoubleArray failed");
        return nullptr;
    }

    env->SetDoubleArrayRegion(out, 0, populationSize, wr.data());

    for (Team* t : teams) delete t;
    return out;
}
