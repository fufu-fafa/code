import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation
import numpy as np
import random

# adjustment
frame_max_col = 5       # max collision in one frame
circle_speed = 1.4

def start_vel(circle_speed, x=False, y=False):
    temp_vel = []
    temp_vel.append(circle_speed - random.random() * circle_speed)
    temp_vel.append(circle_speed - temp_vel[0])
    if x:
        temp_vel[0] = temp_vel[0] * -1
    if y:
        temp_vel[1] = temp_vel[1] * -1
    return temp_vel

def generate_positions():
    new_poss = [[random.randint(0, 17), random.randint(0, 17)],
                [random.randint(-17, 0), random.randint(0, 17)],
                [random.randint(-17, 0), random.randint(-17, 0)],
                [random.randint(0, 17), random.randint(-17, 0)]]
    return new_poss

def generate_velocities():
    new_vels = [start_vel(circle_speed, True, True),
                start_vel(circle_speed, False, True),
                start_vel(circle_speed, False, False),
                start_vel(circle_speed, True, False)]
    return new_vels

def calculate(pos1, pos2, vel1, vel2):
    # circle2 relative position and speed to circle1
    vector_12 = [pos2[0] - pos1[0], pos2[1] - pos1[1]]
    relative_vel_12 = [vel2[0] - vel1[0], vel2[1] - vel1[1]]

    # the distance of circle1 to circle2 (always positive)
    vec_length = ((pos2[0] - pos1[0])**2 + (pos2[1] - pos1[1])**2)**0.5

    # normalize vector12 (make the length = 1)
    normalized_vector = [vector_12[0] / vec_length, vector_12[1] / vec_length]

    # handle if the circles are moving apart
    relative_vel_along_normal = (relative_vel_12[0] * normalized_vector[0] +
                                 relative_vel_12[1] * normalized_vector[1])
    if relative_vel_along_normal > 0:
        return vel1, vel2

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

def check_circle_collision(pos1, pos2, rad=2.0):
    distance = (pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2
    return distance <= (2 * (rad + 0.3))**2      # 0.3 is margin for error

def check_wall_collision(pos, bounds=17.8):
    temp = []
    for n in range(2):
        temp.append(pos[n] <= -bounds or pos[n] >= bounds)
    return temp

def update(frame):
    global c_poss, c_vels
    collision_count = 0

    # handle all unique pair of circle's collision
    pairs = [[0, 1], [0, 2], [0, 3], [1, 2], [1, 3], [2, 3]]
    for i, j in pairs:
        if check_circle_collision(c_poss[i], c_poss[j], 2.0):
            c_vels[i], c_vels[j] = calculate(c_poss[i], c_poss[j], c_vels[i], c_vels[j])
            collision_count += 1

    for x in range(4):
        wall_collision = check_wall_collision(c_poss[x], 17.8)
        for n in range(2):
            if wall_collision[n]:
                c_vels[x][n] = -c_vels[x][n]
                collision_count += 1
        c_poss[x] = [c_poss[x][0] + c_vels[x][0], c_poss[x][1] + c_vels[x][1]]

    if collision_count > frame_max_col:
        c_poss = generate_positions()
        c_vels = generate_velocities()

    c1.set_center(c_poss[0])
    c2.set_center(c_poss[1])
    c3.set_center(c_poss[2])
    c4.set_center(c_poss[3])
    return c1, c2, c3, c4

# setup
c_poss = generate_positions()
c_vels = generate_velocities()

fig, ax = plt.subplots()
fig.canvas.manager.set_window_title('collision simulation')
ax.set_aspect('equal')
ax.set_xlim(-20, 20)
ax.set_ylim(-20, 20)
ax.axis('on')

c1 = patches.Circle((c_poss[0]), 2.0, color='red', alpha=0.6)
c2 = patches.Circle((c_poss[1]), 2.0, color='green', alpha=0.6)
c3 = patches.Circle((c_poss[2]), 2.0, color='blue', alpha=0.6)
c4 = patches.Circle((c_poss[3]), 2.0, color='black', alpha=0.6)

ax.add_patch(c1)
ax.add_patch(c2)
ax.add_patch(c3)
ax.add_patch(c4)

ani = FuncAnimation(fig, update, frames=np.arange(0, 200), interval=20, blit=True)
plt.show()
