# A simple black hole visualization
This simple project is based on chapter 9.4 from J. B. Hartle's "Gravity - An Introduction to Einstein's General Relativity". The main equation used to calculate a bending angle of the light rays is an integral shown below:

![equation](https://latex.codecogs.com/svg.image?\Delta\phi=2&space;\int_{0}^{w_1}\text{d}w[1-w^2(1-\frac{2M}{b}w)]^{-1/2},)

where

![equation](https://latex.codecogs.com/svg.image?b^2=27M^2,)

which is a value determining an event horizon.

You can move around with RMB and change the distance from the singularity or the mass to see how they influence the result as shown below: 

![Opening media files](https://github.com/Bakaczor/Images/blob/master/Black-Hole/black_hole.gif)

### Technical details
The core of the visualization is a single shader, where we calculate the $w_1$ using Newton–Raphson method and then the whole integral using midpoint method.


---
*Copyright © 2026 Bartosz Kaczorowski*
