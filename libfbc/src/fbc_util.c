#include <stdlib.h>
#include "fbc_util.h"

#define INCREMENT 5
#define LOOP_DELTA 20
#define MAX_PARTICLES 30

#define DIVISOR 10000

typedef struct particle {
  double pos, vel, best;
} particle_t;

typedef struct set {
  particle_t kP, kI, kD;
  double best_err;
} set_t;

// Declared at the file scope to prevent a stack overflow
static set_t p[MAX_PARTICLES];

// Returns a random double between 0 and 1
static inline double rand_num() {
  return (rand() / (double)RAND_MAX);
}

// Binds an input to a set of boundaries
static double lock(double in, double lo, double hi) {
  return ((in < lo) ? lo : ((in > hi) ? hi : in));
}


void pidAutoTune(fbc_t* fbc, int num_iterations, int num_particles, int timeout, int goal,
		 FILE* lcd, double kP_min, double kP_max, double kI_min, double kI_max,
		 double kD_min, double kD_max, double k_settle, double k_itae) {
  if (num_particles > MAX_PARTICLES) {
    puts("ERROR: can't have more than 30 particles");
    return;
  }
  set_t p_global;

  // Initialize the particles
  for (int i = 0; i < num_particles; i++) {
    p[i].kP.pos = kP_min + (kP_max - kP_min) * rand_num();
    p[i].kP.vel = p[i].kP.pos / INCREMENT;
    p[i].kP.best = p[i].kP.pos;
    p[i].kI.pos = kI_min + (kI_max - kI_min) * rand_num();
    p[i].kI.vel = p[i].kI.pos / INCREMENT;
    p[i].kI.best = p[i].kI.pos;
    p[i].kD.pos = kD_min + (kD_max - kD_min) * rand_num();
    p[i].kD.vel = p[i].kD.pos / INCREMENT;
    p[i].kD.best = p[i].kD.pos;
    p[i].best_err = INT64_MAX;
  }
  p_global.best_err = INT64_MAX;
  p_global.kP.best = 0;
  p_global.kI.best = 0;
  p_global.kD.best = 0;

  fbc->acceptableConfidence *= 2; //double the confidence for extra accuracy

  // Run the optimization
  for (int j = 0; j < num_iterations; j++) {
    //test constants then calculate fitness function
    bool inv_goal = false;
    for (int i = 0; i < num_particles; i++) {
      //set the new constants
      fbc_pid_t* data = (fbc_pid_t*)fbc->_controllerData;
      data->kP = p[i].kP.pos;
      data->kI = p[i].kI.pos;
      data->kD = p[i].kD.pos;

      // Reverse the goal every other time so the robot doesn't drive all over everywhere
      if (inv_goal)
	     fbcSetGoal(fbc, -goal);
      else
	     fbcSetGoal(fbc, goal);
      inv_goal = !inv_goal;

      int settle_time = 0, itae = 0;
      while (!fbcIsConfident(fbc)) {
        settle_time += LOOP_DELTA;
        if (settle_time > timeout) break;

	      int error = fbc->sense() - fbc->goal;
	      itae += ((settle_time * abs(error)) / DIVISOR); //sum of the error emphasizing later error

        fbcRunContinuous(fbc);
        delay(LOOP_DELTA);
      }

      double err = k_settle * settle_time + k_itae * itae;
      if (err < p[i].best_err) {
        p[i].kP.best = p[i].kP.pos;
        p[i].kI.best = p[i].kI.pos;
        p[i].kD.best = p[i].kD.pos;
        p[i].best_err = err;
        if (err < p_global.best_err) {
          p_global.kP.best = p[i].kP.pos;
          p_global.kI.best = p[i].kI.pos;
          p_global.kD.best = p[i].kD.pos;
          p_global.best_err = err;
          puts("new best");
        }
      }

      printf("best_err: %lf, kp: %lf\n", p_global.best_err, p_global.kP.best);

      fbcReset(fbc);
      fbc->move(0);
      delay(1000); //stop for a second to allow the bot to settle
    }

    // Update particle trajectories
    for (int i = 0; i < num_particles; i++) {
      // Factor in the particles intertia to keep on same trajectory
      p[i].kP.vel = INERTIA * p[i].kP.vel;
      // Move towards particle's best
      p[i].kP.vel += CONF_SELF * ((p[i].kP.best - p[i].kP.pos) / INCREMENT) * rand_num();
      // Move towards swarm's best
      p[i].kP.vel += CONF_SWARM * ((p_global.kP.best - p[i].kP.pos) / INCREMENT) * rand_num();
      // Kinematics
      p[i].kP.pos += p[i].kP.vel * INCREMENT;

      p[i].kI.vel = INERTIA * p[i].kI.vel;
      p[i].kI.vel += CONF_SELF * ((p[i].kI.best - p[i].kI.pos) / INCREMENT) * rand_num();
      p[i].kI.vel += CONF_SWARM * ((p_global.kI.best - p[i].kI.pos) / INCREMENT) * rand_num();
      p[i].kI.pos += p[i].kI.vel * INCREMENT;

      p[i].kD.vel = INERTIA * p[i].kD.vel;
      p[i].kD.vel += CONF_SELF * ((p[i].kD.best - p[i].kD.pos) / INCREMENT) * rand_num();
      p[i].kD.vel += CONF_SWARM * ((p_global.kD.best - p[i].kD.pos) / INCREMENT) * rand_num();
      p[i].kD.pos += p[i].kD.vel * INCREMENT;

      p[i].kP.pos = lock(p[i].kP.pos, kP_min, kP_max);
      p[i].kI.pos = lock(p[i].kI.pos, kI_min, kI_max);
      p[i].kD.pos = lock(p[i].kD.pos, kD_min, kD_max);
    }
    puts("next iteration");
  }

  fbc->move(0); //stop the motors, keeps it from running off when using the killswitch
  printf("\n\nFinal Constants: \n");
  printf("kP: %lf\n", p_global.kP.best);
  printf("kI: %lf\n", p_global.kI.best);
  printf("kD: %lf\n", p_global.kD.best);

  if (lcd != NULL) {
    lcdPrint(lcd, 1, "Const P:%1.4lf", p_global.kP.best);
    lcdPrint(lcd, 2, "I:%1.4lf D:%1.4lf", p_global.kI.best, p_global.kD.best);
  }
}


void fbcFindDeadband(fbc_t* fbc, int delta_sense, FILE* lcd) {
  int pos = 0, neg = 0;

  // Find positive deadband
  for (int i = 0; i < 128; i++) {
    int sense = fbc->sense();
    fbc->move(i);
    delay(500);
    fbc->move(0);
    if (abs(fbc->sense() - sense) > delta_sense) {
      pos = i;
      break;
    }
  }

  delay(2000);

  // Find negative deadband
  for (int i = 0; i > -128; i--) {
    int sense = fbc->sense();
    fbc->move(i);
    delay(500);
    fbc->move(0);
    if (abs(fbc->sense() - sense) > delta_sense) {
      neg = i;
      break;
    }
  }

  printf("Pos_db: %d, Neg_db: %d\n", pos, neg);
  if (lcd != NULL) {
    lcdPrint(lcd, 1, "Pos: %d", pos);
    lcdPrint(lcd, 2, "Neg: %d", neg);
  }
}
