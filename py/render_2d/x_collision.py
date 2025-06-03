import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation
import numpy as np
import random

def generate_two_floats(total_sum, max_difference_ratio):
    max_diff = total_sum * max_difference_ratio
    diff = random.uniform(0, max_diff)
    
    a = (total_sum + diff) / 2
    b = total_sum - a
    return [a, b]

def update_x_speed(x_pos, x_speed):
    if x_pos <= -18.9 or x_pos >= 18.9:
        return x_speed * -1
    else:
        return x_speed

# setup
sum_speed = 1.2
fig, ax = plt.subplots()
ax.set_aspect('equal')
ax.set_xlim(-20, 20)
ax.set_ylim(-14, 14)
ax.axis('on')

# create circle
circle_midpoint = [-4, 4]
circle_speed = generate_two_floats(sum_speed, 0.4)

circle1 = patches.Circle((circle_midpoint[0], 0), 1, color='blue', alpha=0.6)
circle2 = patches.Circle((circle_midpoint[1], 0), 1, color='red', alpha=0.6)
ax.add_patch(circle1)
ax.add_patch(circle2)

# Animation update function
def update(frame):
    for n in range(0, 2):
        circle_speed[n] = update_x_speed(circle_midpoint[n], circle_speed[n])
        circle_midpoint[n] = circle_midpoint[n] + circle_speed[n]

    if abs(circle_midpoint[0] - circle_midpoint[1]) <= 2:
        print((circle_midpoint[0] + circle_midpoint[1])/2)
        temp_x = circle_speed[0]
        circle_speed[0] = circle_speed[1]
        circle_speed[1] = temp_x

    circle1.set_center((circle_midpoint[0], 0))
    circle2.set_center((circle_midpoint[1], 0))
    return circle1, circle2

ani = FuncAnimation(fig, update, frames=np.arange(0, 200), interval=20, blit=True)

plt.show()
