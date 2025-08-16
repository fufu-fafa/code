#include <stdio.h>
#include <math.h>

typedef struct {
    double pos[2];
    double charge;
} particle;

double get_coulomb_force(const particle *p1, const particle *p2, double k) {
    double dx = p2->pos[0] - p1->pos[0];
    double dy = p2->pos[1] - p1->pos[1];
    double r = sqrt(dx * dx + dy * dy);
    if (r == 0) return 0;
    else return -1.0 * k * p1->charge * p2->charge / (r * r);           // attracting is positive
}

void get_force_vector(double fvec[2], double force, double pos1[2], double pos2[2]) {
    double dx = pos2[0] - pos1[0];
    double dy = pos2[1] - pos1[1];
    double r = sqrt(dx * dx + dy * dy);
    if (r == 0) {
        fvec[0] = fvec[1] = 0;
        return;
    }
    double nd_vec[2] = {dx/r, dy/r};                            // normalized direction vector
    fvec[0] = force * nd_vec[0];
    fvec[1] = force * nd_vec[1];
}

int main() {
    const double k = 8.99e9;
    particle particles[] = {
        {{3.0, 3.0}, -2.0e-6},
        {{3.0, 8.0}, 4.0e-6},
        {{7.0, 3.0}, -3.0e-6},
        {{7.0, 8.0}, 5.0e-6}
    };
    const int amount = sizeof(particles)/sizeof(particles[0]);
    int uniq_pair = amount * (amount-1) / 2;
    double f_on_pair[uniq_pair];
    double net_vecs[amount][2];
    double temp_vecs[2];
    for (int n = 0; n < amount; n++) net_vecs[n][0] = net_vecs[n][1] = 0.0;

    int pair_idx = 0;
    for (int i = 0; i < amount; i++) {
        for (int j = i + 1; j < amount; j++) {
            f_on_pair[pair_idx] = get_coulomb_force(&particles[i], &particles[j], k);
            get_force_vector(temp_vecs, f_on_pair[pair_idx], particles[i].pos, particles[j].pos);
            printf("coulomb force on pair %d (particle %d and %d): %g\n", pair_idx+1, i+1, j+1, f_on_pair[pair_idx]);
            printf("vec forces on %d: (%g, %g)\n", i+1, temp_vecs[0], temp_vecs[1]);
            printf("vec forces on %d: (%g, %g)\n\n", j+1, -temp_vecs[0], -temp_vecs[1]);
            pair_idx++;

            
            for (int p = 0; p < 2; p++) {              // calculate net force on a particle
                net_vecs[i][p] += temp_vecs[p];
                net_vecs[j][p] -= temp_vecs[p];
            }
        }
    }
    for (int n = 0; n < amount; n++) {
        printf("net force on %d: (%e, %e)\n", n+1, net_vecs[n][0], net_vecs[n][1]);
    }
    return 0;
}
