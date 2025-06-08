#include <iostream>
#include <cmath>

struct two_arr {
    double one[2];
    double two[2];
};

two_arr calculate(const double c_poss[2][2], const double c_vels[2][2]) {
    two_arr result;
    double vec_12[2] = {
        c_poss[1][0] - c_poss[0][0], 
        c_poss[1][1] - c_poss[0][1]
    };

    double rel_vel_12[2] = {
        c_vels[1][0] - c_vels[0][0], 
        c_vels[1][1] - c_vels[0][1]
    };

    double len_vec_12 = std::sqrt(
        vec_12[0] * vec_12[0] + 
        vec_12[1] * vec_12[1]
    );

    double norm_vec_12[2] = {
        vec_12[0] / len_vec_12,
        vec_12[1] / len_vec_12
    };
    double perp_norm_vec_12[2] = {
        norm_vec_12[1] * -1,
        norm_vec_12[0]
    };


    double proj_rel_vel_12 = rel_vel_12[0] * norm_vec_12[0] + rel_vel_12[1] * norm_vec_12[1];
    if (proj_rel_vel_12 > 0) {
        for (int x = 0; x <= 1; x++) {
            result.one[x] = c_vels[0][x];
            result.two[x] = c_vels[1][x];
        }
        return result;
    }

    // one for each circle
    // after collision so it's reversed
    double proj_vel[2] = {
        c_vels[1][0] * norm_vec_12[0] + c_vels[1][1] * norm_vec_12[1],
        c_vels[0][0] * norm_vec_12[0] + c_vels[0][1] * norm_vec_12[1]
    };

    double tangent_vels[2] = {
        c_vels[0][0] * perp_norm_vec_12[0] + c_vels[0][1] * perp_norm_vec_12[1],
        c_vels[1][0] * perp_norm_vec_12[0] + c_vels[1][1] * perp_norm_vec_12[1]
    };

    for (int x = 0; x <= 1; x++) {
        result.one[x] = proj_vel[0] * norm_vec_12[x] + tangent_vels[0] * perp_norm_vec_12[x];
        result.two[x] = proj_vel[1] * norm_vec_12[x] + tangent_vels[1] * perp_norm_vec_12[x];
    }
    return result;
}

int main() {
    double c1_pos[2] = {2.5, 4.5};
    double c1_vel[2] = {-2.5, -4.5};
    double c2_pos[2] = {-2.5, 4.5};
    double c2_vel[2] = {2.5, -4.5};
    double c_poss[2][2];
    double c_vels[2][2];

    for (int x = 0; x <= 1; x++) {
        c_poss[0][x] = c1_pos[x];
        c_poss[1][x] = c2_pos[x];
        c_vels[0][x] = c1_vel[x];
        c_vels[1][x] = c2_vel[x];
    }
    two_arr result = calculate(c_poss, c_vels);
    std::cout << "first circle vel:" << '\n';
    for (int i = 0; i <= 1; i++) {
        std::cout << result.one[i] << '\n';
    }

    std::cout << "second circle vel:" << '\n';
    for (int j = 0; j <= 1; j++) {
        std::cout << result.two[j] << '\n';
    }
    return 0;
}
