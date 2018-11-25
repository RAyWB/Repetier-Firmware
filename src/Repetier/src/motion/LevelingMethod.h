#ifndef _LEVELING_METHOD_H
#define _LEVELING_METHOD_H

#ifndef LEVELING_METHOD
#define LEVELING_METHOD 0
#endif

#ifndef LEVELING_CORRECTOR
#define LEVELING_CORRECTOR 0
#endif

#if ENABLE_BUMP_CORRECTION && LEVELING_METHOD != 1
#undef ENABLE_BUMP_CORRECTION
#define ENABLE_BUMP_CORRECTION 0 // Disable if not supported
#endif

class Plane;

#if LEVELING_CORRECTOR == 0 // software correction

class LevelingCorrector {
public:
    inline static void init() {}
    inline static void handleEeprom() {}
    inline static void resetEeprom() {}
    static void correct(Plane* plane);
};

#elif LEVELING_CORRECTOR == 1 // Motorized correction

class LevelingCorrector {
public:
    static void init();
    static void handleEeprom();
    static void resetEeprom();
    static void correct(Plane* plane);
};

#endif

#if LEVELING_METHOD == 0 // No leveling

class Leveling {
public:
    inline static void addDistortion(float* pos) {}
    inline static void subDistortion(float* pos) {}
    inline static void setDistortionEnabled(bool newState) {}
    inline static bool isDistortionEnabled() { return false; }
    inline static float distortionAt(float xp, float yp) { return 0; }
    inline static void measure() {}
    inline static void init() {}
    inline static void handleEeprom() {}
    inline static void resetEeprom() {}
    inline static void execute_G32(GCode* com) {}
    inline static void execute_G33(GCode* com) {}
    inline static void execute_M323(GCode* com) {}
};

#elif LEVELING_METHOD == 1 // Grid leveling

#ifndef GRID_SIZE
#define GRID_SIZE 3
#endif

class Leveling {
    static float grid[GRID_SIZE][GRID_SIZE]; // Bumps up have negative values!
    static float xMin, xMax, yMin, yMax;
    static float dx, dy, invDx, invDy;
    static float startDegrade, endDegrade, diffDegrade;
    static uint16_t eprStart;
    static uint8_t distortionEnabled;
    inline static float xPosFor(fast8_t index) {
        return xMin + dx * index;
    }
    inline static float yPosFor(fast8_t index) {
        return yMin + dy * index;
    }
    static void extrapolateGrid();
    static bool extrapolateableNeighbours(int x, int y);
    static float extrapolateNeighbours(int x, int y);
    inline static bool validGridIndex(int x, int y) {
        return x >= 0 && y >= 0 && x < GRID_SIZE && y < GRID_SIZE;
    }
    static bool gridIndexForDir(int dir, int dist, int& x, int& y);
#if ENABLE_BUMP_CORRECTION
    static void showMatrix();
    static void set(float x, float y, float z);
#endif
public:
    static void setDistortionEnabled(bool newState);
#if ENABLE_BUMP_CORRECTION
    static void addDistortion(float* pos); // ads bumps so you get required z position, printer coordinates
    static void subDistortion(float* pos); // printer coordinates
    inline static bool isDistortionEnabled() { return distortionEnabled; }
    static float distortionAt(float xp, float yp); // printer coordinates
    static void execute_M323(GCode* com);
#else
    inline static void addDistortion(float* pos) {}
    inline static void subDistortion(float* pos) {}
    inline static bool isDistortionEnabled() { return false; }
    inline static void execute_M323(GCode* com) {}
    inline static float distortionAt(float xp, float yp) { return 0; }
#endif
    static void reportDistortionStatus();
    static void measure();
    static void init();
    static void handleEeprom();
    static void resetEeprom();
    static void updateDerived();
    static void execute_G32(GCode* com);
    static void execute_G33(GCode* com);
};

#elif LEVELING_METHOD == 2 // 4 point symmetric

class Leveling {
public:
    inline static void addDistortion(float* pos) {}
    inline static void subDistortion(float* pos) {}
    inline static void setDistortionEnabled(bool newState) {}
    inline static bool isDistortionEnabled() { return false; }
    inline static float distortionAt(float xp, float yp) { return 0; }
    static void measure();
    inline static void init() {}
    inline static void handleEeprom() {}
    inline static void resetEeprom() {}
    static void execute_G32(GCode* com);
    inline static void execute_G33(GCode* com) {}
    inline static void execute_M323(GCode* com) {}
};

#elif LEVELING_METHOD == 3 // 3 points

class Leveling {
public:
    inline static void addDistortion(float* pos) {}
    inline static void subDistortion(float* pos) {}
    inline static void setDistortionEnabled(bool newState) {}
    inline static bool isDistortionEnabled() { return false; }
    inline static float distortionAt(float xp, float yp) { return 0; }
    static void measure();
    inline static void init() {}
    inline static void handleEeprom() {}
    inline static void resetEeprom() {}
    static void execute_G32(GCode* com);
    inline static void execute_G33(GCode* com) {}
    inline static void execute_M323(GCode* com) {}
};

#else
#error Unknown value for LEVELING_METHOD set!
#endif

#endif