README_SHAPE by Joshua Freilich aka jfreilic

About the program:

I chose to store the triangle data in a 1D array. Each item in the array is a triangle struct.
A triangle struct contains two arrays of nine doubles.
One array gets values x y z x2 y2 z2 x3 y3 z3, the coordinates of each vertex in a triangle in counterclockwise order.
I chose not to draw the triangles in each Shape's subclass, instead tessellate just generates these points and the normals.
I chose this implementation because the triangles do not necessarilly know about the place in which they will be drawn.
Also, this way a bunch of shapes can be tessellated and stored and then drawn, possibly simultaneously.
Normals:
I calculated normals for the cube using cross products.
I also used cross products to determine what should happen with normals at edges of the cylinder and cone.
For the cylinder, I used 2x,0, 2z as the normal for the sides and 0,2y,0 as the normal for the top.
For the cone, I used 0,2y,0 as the normal for the bottom, and 2x/(r*root 5), 1/root 5, 2z/(r*root 5) for the sides. r= (x^2+z^2)^.5 and I did something similar for the tip by setting x, z, and r =1
For the sphere, I just used the point as the normal and normalized it.


Extra credit stuff:
Special 1: Failed attempt at a klein bottle. Still looks interesting so I kept it. Uses face normals.
Special 2: Mobius strip. Just used cross product across each triangle face for normals so it doesn't look great.
Special 3: N64 Cube. If you don't know what this is, your childhood probably sucked, no offense. This was hard as hell to get working right and is still not perfect but it looks good.

No bugs in the required shapes. For default/minimum parameters see each .cpp file.
