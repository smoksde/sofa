### Novel approach to the sofa problem using graphics processing and backpropagation free optimization algorithms

This project implements a novel approach on the well known yet unsolved sofa problem.

An implemented optimization algorithm is supposed to converge against the
shape of the optimal sofa for the vanilla sofa problem.

The optimizer can query scores for different parametersets that describe, how
the sofa has to be moved through space over time.
The score just delivers a measurement for the area of the sofa that got through.

A self implemented graphics engine computes the score that can be queried.