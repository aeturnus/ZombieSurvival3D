Here is a collection of useful code that I've written to make Lab10 the best Lab10 ever.

The Files:

"BrandonTypes":
A collection of types to represent fractional numbers.
Included are fixed point decimal types of various precisions and size, and the
multiplication and division functions for them. Absolutely necessary; it's a
cornerstone due to all the need for a fixed point. The decimal fixed points
are basically aliased int types using stdint


"BrandonMath":
A collection of good mathematical functions (especially trigonometry) as well
as point and vector types and functions. Ryan did the sqrt.


"BrandonBufferManager":
For graphics. Functions to manipulate a "Buffer" datatype for the purpose of
buffering a screen before sending it out. Will supports 8,16, and 32 bit buffers.
Only functions for 16bit buffers are implemented as of now. Ryan did the bitmap scaler!


"BrandonMML":
Brandon's implementation of the Music Macro Language. Extensions include
jumps, loop counters, ties and dotted notes.


"BrandonRayCaster":
Defines a world datatype and will produce raycast related data such as
distance away.

"BrandonFIFO":
An array-index FIFO implementation

"BrandonPhysics":
A simple physics engine that allows objects to move and check collisions.
Entities are in 3D.
