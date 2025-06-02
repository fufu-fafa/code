import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation
import numpy as np
import random
import itertools

# config
circle_speed = 0.8

def meth(pos1, pos2, vel1, vel2):
    # vector12 which is circle 1 -> circle 2 and also the collision axis
    vector_12 = [pos2[0] - pos1[0], pos2[1] - pos1[1]]
    vec_length = ((pos2[0] - pos1[0])**2 + (pos2[1] - pos1[1])**2)**0.5

    # normalize vector12 (make the length = 1)
    normalized_vector = [vector_12[0] / vec_length, vector_12[1] / vec_length]

    # projects circles vector to collision point 
    v1_proj = vel1[0] * normalized_vector[0] + vel1[1] * normalized_vector[1]
    v2_proj = vel2[0] * normalized_vector[0] + vel2[1] * normalized_vector[1]

    # transfer vector after elastic collision
    v1_proj_after = v2_proj
    v2_proj_after = v1_proj

    # pnv = perpendicular normalized vector
    pnv = [-normalized_vector[1], normalized_vector[0]]

    # how much vector/energy doesn't affect and affected by collision
    v1t = vel1[0] * pnv[0] + vel1[1] * pnv[1]
    v2t = vel2[0] * pnv[0] + vel2[1] * pnv[1]
    v1_final = [v1_proj_after * normalized_vector[0] + v1t * pnv[0], 
                v1_proj_after * normalized_vector[1] + v1t * pnv[1]]
    v2_final = [v2_proj_after * normalized_vector[0] + v2t * pnv[0], 
                v2_proj_after * normalized_vector[1] + v2t * pnv[1]]
    return v1_final, v2_final

def check_circle_collision(pos1, pos2, rad):
    distance = (pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2
    return distance <= (2 * (rad + 0.3))**2      # 0.3 is margin for error

def circle_collision_handler(pos1, pos2, vel1, vel2, rad=2.0):
    if check_circle_collision(pos1, pos2, rad):
        return meth(pos1, pos2, vel1, vel2)
    return vel1, vel2

def check_wall_collision(pos, bounds):
    temp = []
    for n in range(2):
        temp.append(pos[n] <= -bounds or pos[n] >= bounds)
    return temp

def wall_collision_handler(pos, vel, bounds=17.8):
    temp_vel = vel[:]
    collision_list = check_wall_collision(pos, bounds)
    for n in range(2):
        if collision_list[n]:
            temp_vel[n] = -vel[n]
    return temp_vel

# randomize starting velocities
def random_vel_pair(speed):
    vx = speed - random.random() * speed
    vy = speed - vx
    return [vx, vy]

fig, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_xlim(-20, 20)
ax.set_ylim(-20, 20)
ax.axis("on")

poses = [
    [random.randint(0, 17),  random.randint(0, 17)],   # c1_pos
    [random.randint(-17, 0), random.randint(0, 17)],   # c2_pos
    [random.randint(-17, 0), random.randint(-17, 0)],  # c3_pos
    [random.randint(0, 17),  random.randint(-17, 0)],  # c4_pos
]
vels = [random_vel_pair(circle_speed) for _ in range(4)]
radii = [2.0] * 4

patches_list = []
colors = ["red", "green", "blue", "black"]
for pos, color in zip(poses, colors):
    patch = patches.Circle((pos[0], pos[1]), radii[0], color=color, alpha=0.6)
    ax.add_patch(patch)
    patches_list.append(patch)

def update(frame):
    global poses, vels

    for i, j in itertools.combinations(range(len(poses)), 2):
        v_i_new, v_j_new = circle_collision_handler(
            poses[i], poses[j], vels[i], vels[j], rad=radii[i]
        )
        vels[i], vels[j] = v_i_new, v_j_new

    for k in range(len(poses)):
        vels[k] = wall_collision_handler(poses[k], vels[k])

    for k in range(len(poses)):
        poses[k][0] += vels[k][0]
        poses[k][1] += vels[k][1]

    for patch, pos in zip(patches_list, poses):
        patch.set_center(pos)

    return patches_list

ani = FuncAnimation(fig, update, frames=np.arange(0, 200), interval=20, blit=True)
plt.show()

