import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation
import numpy as np
import random

# setup
fig, ax = plt.subplots()
ax.set_aspect('equal')
ax.set_xlim(-20, 20)
ax.set_ylim(-20, 20)
ax.axis('on')

circle1_pos = [random.randint(0, 18), random.randint(0, 18)]
circle1_vel = [random.random(), random.random()]

circle2_pos = [random.randint(-18, 0), random.randint(-18, 0)]
circle2_vel = [random.random(), random.random()]

circle1 = patches.Circle((circle1_pos[0], circle1_pos[1]), 2.0, color='blue', alpha=0.6)
circle2 = patches.Circle((circle2_pos[0], circle2_pos[1]), 2.0, color='red', alpha=0.6)

circles = [circle1, circle2]
ax.add_patch(circle1)
ax.add_patch(circle2)

def check_circle_collision(pos1, pos2, rad=2.0):
    distance = ((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])) ** 0.5
    if distance <= 2 * rad:
        return True
    else:
        return False

def check_handle_wall_collision(pos, vel):
    for n in range(2):
        if pos[n] <= -18 or pos[n] >= 18:
            vel[n] = vel[n] * -1
    return vel

def update(frame):
    global circle1_pos, circle1_vel, circle2_pos, circle2_vel

    if check_circle_collision(circle1_pos, circle2_pos):
        circle1_vel, circle2_vel = circle2_vel, circle1_vel

    circle1_vel = check_handle_wall_collision(circle1_pos, circle1_vel)
    circle2_vel = check_handle_wall_collision(circle2_pos, circle2_vel)

    for n in range(2):
        circle1_pos[n] = circle1_pos[n] + circle1_vel[n]
        circle2_pos[n] = circle2_pos[n] + circle2_vel[n]

    circle1.set_center(circle1_pos)
    circle2.set_center(circle2_pos)
    return circle1, circle2
ani = FuncAnimation(fig, update, frames=np.arange(0, 200), interval=20, blit=True)
plt.show()
