import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation
import numpy as np
import random

# adjustment
circle_speed = 0.8
c1_x_speed = circle_speed - random.random() * circle_speed
c1_y_speed = circle_speed - c1_x_speed
c2_x_speed = circle_speed - random.random() * circle_speed
c2_y_speed = circle_speed - c2_x_speed
c3_x_speed = circle_speed - random.random() * circle_speed
c3_y_speed = circle_speed - c3_x_speed
c4_x_speed = circle_speed - random.random() * circle_speed
c4_y_speed = circle_speed - c4_x_speed

c_vels = [[c1_x_speed, c1_y_speed],
         [c2_x_speed, c2_y_speed],
         [c3_x_speed, c3_y_speed],
         [c4_x_speed, c4_y_speed]]

# setup
fig, ax = plt.subplots()
ax.set_aspect('equal')
ax.set_xlim(-20, 20)
ax.set_ylim(-20, 20)
ax.axis('on')

c_poss = [[random.randint(0, 17), random.randint(0, 17)],
          [random.randint(-17, 0), random.randint(0, 17)],
          [random.randint(-17, 0), random.randint(-17, 0)],
          [random.randint(0, 17), random.randint(-17, 0)]]

c1 = patches.Circle((c_poss[0]), 2.0, color='red', alpha=0.6)
c2 = patches.Circle((c_poss[1]), 2.0, color='green', alpha=0.6)
c3 = patches.Circle((c_poss[2]), 2.0, color='blue', alpha=0.6)
c4 = patches.Circle((c_poss[3]), 2.0, color='black', alpha=0.6)

ax.add_patch(c1)
ax.add_patch(c2)
ax.add_patch(c3)
ax.add_patch(c4)

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

def update(frame):
    global c_poss, c_vels

    # handle all unique pair of circle's collision
    c_vels[0], c_vels[1] = circle_collision_handler(c_poss[0], c_poss[1], c_vels[0], c_vels[1])
    c_vels[0], c_vels[2] = circle_collision_handler(c_poss[0], c_poss[2], c_vels[0], c_vels[2])
    c_vels[0], c_vels[3] = circle_collision_handler(c_poss[0], c_poss[3], c_vels[0], c_vels[3])
    c_vels[1], c_vels[2] = circle_collision_handler(c_poss[1], c_poss[2], c_vels[1], c_vels[2])
    c_vels[1], c_vels[3] = circle_collision_handler(c_poss[1], c_poss[3], c_vels[1], c_vels[3])
    c_vels[2], c_vels[3] = circle_collision_handler(c_poss[2], c_poss[3], c_vels[2], c_vels[3])

    for x in range(4):
        c_vels[x] = wall_collision_handler(c_poss[x], c_vels[x])
        c_poss[x] = [c_poss[x][0] + c_vels[x][0], c_poss[x][1] + c_vels[x][1]]

    c1.set_center(c_poss[0])
    c2.set_center(c_poss[1])
    c3.set_center(c_poss[2])
    c4.set_center(c_poss[3])
    return c1, c2, c3, c4

ani = FuncAnimation(fig, update, frames=np.arange(0, 200), interval=20, blit=True)
plt.show()
