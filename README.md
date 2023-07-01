# Malloc

building my own malloc API to dynamically replace the std version in a project.
I have set the requirement that it is easy to implement different versions so the optimal solution can be chosen for the project at hand.
To effectively test it with a large set of smaller allocations I am using an updated version of the push_swap project
witch an adition to the makefiel which can compile the project with this library.

## Todo:

split the LARGE and SMALL / TINY allocation strategies apart and change the makefiel that they can both be swapped around as desired.