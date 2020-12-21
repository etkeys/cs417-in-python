Iterative Solvers (by Keys, E.)

1) Makemat generates matrices A, b, and soln to the "files" directory
	named A.def, b.def, and soln.def respectively.
	
2) When running IterativeSolvers.cbp, be sure to RUN USING RELEASE
	CONFIGURATION. The DEBUG configuration does not print to console
	and there is a bunch of debug and trace garbage.
	
NOTES ABOUT RESULTS:

1) The Jacobi solver works well.

2) The Gaus-Seidel and SOR solvers have a flaw. For reasons I can't
	fully explain, the solvers work for smallish matrices but once I
	went to 20x20, these two solvers get into an infinite loop because
	the produced results eventually don't change enough to get close
	to true convergence. The best Two Norm I achieved was 0.17403.
	
	I call this condition a "Lame Convergence". The iterative solvers will
	detect this condition if the Two Norm is the same for 5 iterations. The
	program will also prin a message if a Lame Convergence was detected, but
	still consider the solve successful.
	
	I believe this condition is caused by the values used in the inverse matrix
	are so small that eventually the values don't change when it is significant
	enough. If you want to see what I mean, you can run USING THE DEBUG
	CONFIGURATION and review the RunResults.out file. You can disable the
	TRACE messages by editing the build arguments and removing the "TRACE" #define
	parameter.