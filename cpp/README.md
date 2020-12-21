# CS 417, something about theoretical computations?

Yes, this cpp code is a mess and I didn't bother deduplicating any of the files.

## The goal

As the course would progress, the class would be introduced to new methods for
Solving matricies. That's probably not a correct phrasing, but it's the best that
I can describe it. Early on, it was clear that these methods, referred to as
solvers built on top of the previous, not fully, but shared some similar step.
We were also told that the code we built, we'd have to use on our final. The
code is C++ becuase that was what the the professor was going to compile our
solutions. In the past, students got to choose either C++ or Java, but Java
would also struggle with these large matricies near the end of the course.

I've been dabbling more in Python recently and I know it gets use in machine
learning contexts. So that means it should be able to do this work too, no?

## The setup

As I created new solvers, I would put them into a lib folder so I could copy
them into future projects. I should've structured it so eveything could live
in a single lib folder, but I couldn't figure that out and the IDE wasn't
helpful either.

The are a bunch of `.layout` and `.depend` files which were created by the IDE.

Part of the project was too initially create an program, makemat, that would
be able to create square matricies of some specified size.