#include <stdio.h>
#include <math.h>

typedef struct {
    double pos[2];
    double charge;
} particle;

double get_coulomb_force(particle p1, particle p2, double k) {
    double dx = p2.pos[0] - p1.pos[0];
    double dy = p2.pos[1] - p1.pos[1];
    double r = sqrt(dx * dx + dy * dy);
    if (r == 0) return 0;
    else return -1 * k * p1.charge * p2.charge / (r * r);           // attracting is positive
}

void get_force_vector(double fvec[2], double force, double pos1[2], double pos2[2]) {
    double dx = pos2[0] - pos1[0];
    double dy = pos2[1] - pos1[1];
    double r = sqrt(dx * dx + dy * dy);
    double nd_vec[2] = {dx/r, dy/r};                            // normalized direction vector
    fvec[0] = force * nd_vec[0];
    fvec[1] = force * nd_vec[1];
}

int main() {
    const double k = 8.99e9;
    particle particles[] = {
        {{3, 3}, -2},
        {{7, 3}, -3},
        {{7, 8}, 5}
    };
    const int amount = sizeof(particles)/sizeof(particles[0]);
    int uniq_pair = amount * (amount-1) / 2;
    double fvecs[uniq_pair][2];
    double forces[uniq_pair];

    int pair_idx = 0;
    for (int i = 0; i < amount; i++) {
        for (int j = i + 1; j < amount; j++) {
            forces[pair_idx] = get_coulomb_force(particles[i], particles[j], k);
            get_force_vector(fvecs[pair_idx], forces[pair_idx], particles[i].pos, particles[j].pos);
            printf("coulomb force for %d to %d: %f\n", i, j, forces[pair_idx]);
            printf("vec forces on %d: (%f, %f)\n", i, fvecs[pair_idx][0], fvecs[pair_idx][1]);
            printf("vec forces on %d: (%f, %f)\n\n", j, -fvecs[pair_idx][0], -fvecs[pair_idx][1]);
            pair_idx++;
        }
    }
    return 0;
}
