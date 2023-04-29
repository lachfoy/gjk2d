# 3dcollision
playground for trying (and most likely failing) to understand 3d collisions

My main goal is to get working AABB vs mesh continuous collision!

The idea as far as I'm aware is extending all geometry by the extents of the AABB (Minkowski sum), you can transform every check needed into point vs. plane.

