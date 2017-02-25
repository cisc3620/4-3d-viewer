# Lab 4: Create a 3D viewer

# Part 1: Create a 3d model

## Due date: Thurs. 3/2

As usual, 1 point extra credit will be awarded for completed assignment or pull request fixing error before Sat. 2/25.

Extra credit will also be given for opening and especially answering issues on the original repository.

## Task

Create and render a 3d model. Make the necessary changes in your vertices array, your shaders, and the vertex attribute commands. In addition, make your model continuously rotate (slowly) around the **y-axis**.

## Submit

Your pull request should contain the modified code and a clip of the rendered animation (like last time).

# Part 2: Create a viewer.

## Due date: Thurs. 3/9

## Task

Create appropriate view and projection matrices to position and clip your model. Link to keyboard callbacks so that pressing an arrow key moves the "camera" in the appropriate direction. Pressing an arrow with the shift key should rotate the camera in the appropriate direction.

Each time the camera is moved, also print an update to the terminal with the new position and orientation. E.g. "Camera location now at (3, 2, 1)." "Camera gaze now at (3, 2, 1)." "Camera up direction now (3, 2, 1)."

## Submit

Submit a screen recording of you moving the viewer around the scene. Both the terminal with the logging output and the opengl window should be shown.

The pull request should show the screen recording and the updated code (like lab 3 and part 1).
