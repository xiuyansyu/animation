# animation
CSC418 A3 Part B
Run using: baymax 900 700
# completed
obtained model from http://www.turbosquid.com/3d-models/free-max-mode-baymax-big-hero-6/881191
OBJ file loader added to dir objloader.cpp objloader.h
parsed into 7 body parts and rendered parts separately
added articulation of parts
added DoF light and camera movement
# need to add
cubic interpolation ( currently is linear ): edit Vector getInterpolatedJointDOFS(float time)
Skybox (optional? a3 did not require it but all the examples have it)
keyframes (can save using GLUI)
