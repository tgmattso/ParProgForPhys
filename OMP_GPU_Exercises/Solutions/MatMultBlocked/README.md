# Blocked Matrix Multiplication

Minimizing memory movement is the foundation of performance. When memory movementy scales as N squared while computation scales as N cubed (where N is a "size metric" for a problem) you can typically design algorithms to hide memory movement and achieve near peak performance.  Matrix Multiplication is a classic problem where we can approach peak performance.  To do so, however, can take a lot of work.

An important technique for minimizing memory movement is to turn the famous triply nested loop matrix multiply into a blocked matrix multiply.  An easy (but to be clear, still not the best) way to do this is shown in the code in this directory.  We break out loops into loops over blocks.  The threads in each team work together to load the data into team-local memory.  Then when the N-cubed computation occurs, is operaties on this team-local memory (and hence from the GPU caches).

We have been using this example for many years (dating back to our old OpenCL tutorials).  The code in this directory comes to us from Tom Deakin when he was a lecturer at Bristol University.
