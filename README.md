# Parallel Programming Intro for Physics People

I am, unfortunately, not a professional physicist.  I am a computational scientist or even, I hate to admit, a computer scientist.

But my passion is physics.   My degrees say chemistry but if you look at what I actually did in Graduate School, it was physics.   So while I am not a physicist, I am a member of a community I'll call "Physics People".  For us, Physics defines the lens through which we view the world.   It colors how we think about science and gives us a unique perspective on computing.

In this repository, you'll find materials to support courses on parallel computing specifically tuned to the needs of physics people.  It emphasizes OpenMP since that's the best place to start when learning parallel computing.  Over time, however, it will grow to include MPI, SYCL, PyOMP and other approaches to parallel computing that physics people use.  I've also added content to support a lecture on floating point arithmetic since, if you do physics on a computer, then you are using float point arithmetic.  There are consequences to approximating real arithmetic with finite precision numbers.  You need to know about these consequences and unfortunately, they are not widely taught in school.

For the parallel programming courses, always start with the file *OpenMP_CPU.pdf*.  This segment of the course introduces key concepts in parallel programming through a series of exercises in parallel programming on a CPU. After that CPU focussed introduction to parallel computing, the sequence splits.  If there is little time, there is a lecture in the file *Other_par_prog_envs.pdf* that summarizes programming for GPUs and clusters.  If time is not an issue, there are hands-on courses for cluster computing (*MPI_intro_hands_on.pdf*) and GPU computing (*OpenMP_GPU.pdf*).  

This content is under a creative commons license.  You can do whatever you want with it, but please cite me (Tim Mattson).


