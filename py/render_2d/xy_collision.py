import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation
import numpy as np

# setup
fig, ax = plt.subplots()
ax.set_aspect('equal')
ax.set_xlim(-20, 20)
ax.set_ylim(-20, 20)
ax.axis('on')

circle1_pos = [4.0, 4.0]
circle1_vel = [-1.0, -1.0]

circle2_pos = [-4.0, -4.0]
circle2_vel = [1.0, 1.0]

circle1 = patches.Circle((circle1_pos[0], circle1_pos[1]), 1, color='blue', alpha=0.6)
circle2 = patches.Circle((circle2_pos[0], circle2_pos[1]), 1, color='red', alpha=0.6)

circles = [circle1, circle2]
ax.add_patch(circle1)
ax.add_patch(circle2)

def check_handle_wall_collision(pos, vel):
    for n in range(2):
        if pos[n] <= -19 or pos[n] >= 19:
            vel[n] = vel[n] * -1
    return vel


def update(frame):
    global circle1_pos, circle1_vel, circle2_pos, circle2_vel

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
