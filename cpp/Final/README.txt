CS417 Spring 2018
Final Exam
Author: Erik Keys
Date: April 22, 2018

*******IMPORTANT NOTE TO EXECUTORS*******
If you run the programs contained, ALWAYS USE THE RELEASE
CONFIGURATION! The debug configuration supplies additional
output that only slows down processing.


Special Notes (In question order)

All Questions
	WHERE TO FIND MY ANSWERS???!?!?
	In each of the question folders, there is an output
	folder. This output folder contains the text file and
	Excel file that was generated by my solvers. Also, a
	PNG file that is the generated chart that was created
	by the Excel file. Some answers to questions did not
	need an Excel file or PNG file. In these cases, those
	files were not supplied.

Q1..Q6
	I have modified the supplied input files. Originally,
	the first number was the square size the defined matrix,
	and the second number was the start of the matrix contents.
	I have changed this so that the first and second number
	are the row and column counts of the supplied matrix and
	the third number is the start of the matrix contents.
	All other file contents were not affected.
	
Q3..Q5
	Due to the smilarities of these solvers and because of how
	I coded their functionality, I made them all dependent on
	Jacobi. For that reason, I thought it better to do all these
	questions in one project file. The solutions for these solvers
	can still found in the output folder and their solutions have
	been seperated into distinct text, Excel, and PNG files that
	denote to which quetions the solutions belong.
	
Q4..Q5
	I'll freely admit, that I did not solve these questions 
	using the specified method. Due to scaling problems (that I
	still have yet to overcome), Gaus-Seidel and SOR could not
	solve the problems at the scale that was specified. I
	instead solved these problems using Jacobi. I suspect that
	the issue with my GS and SOR solvers is how the D and C
	matrices are generated.

Q7
	The question asks to stop at the 11th iteration; however,
	my library has a built in tolerance threshold of .001. My
	solution stopped after 7 iterations because it had reached
	the tolerance threshold.

Q9
	Even though the instructions said to set the initial guess to
	0.5, I had already coded by library to set the initial guess
	to a random value. I didn't see much reason to change the code
	to support the question.

