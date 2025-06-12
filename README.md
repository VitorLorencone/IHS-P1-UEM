# IHS-P1-UEM
Programming for Hardware and Software Interfacing project 1 from computer science course in State University of Maringá

**Student**: Vitor Madeira Lorençone  
**RA**: 132788  
**Professor**: Marcelo Malheiros

## Additional Informations

I used [Duktape](https://duktape.org/) for an embeddable Javascript engine to compile.

The Makefile does have support for Windows and Linux

I used Duktape v2.7.0, but versions v2.x does not have native support for `print()` function (related to stdin) nor to include full `.js` files (related to file syscalls) because of compatibility with other systems. Because of that, I needed to implement those and I am not 100% sure if they will work in other environments or OS. Please, do contact me if anything goes out of expected.

## Files Information
- `duk_config.h` -> needed for duktape
- `duktape.h` -> needed for duktape
- `duktape.c` -> needed for duktape
- `edit_distance.h` -> header for the edit distances algorithms such as the levenshtein distance.
- `edit_distance.c` -> the c file and implementation for the algorithms
- `main.c` -> main file with the wrapper functions and eval execution
- `src/*.js` -> javascript files for executing with duktape


## Implemented Edit Distance Algorithms:
- Levenshtein Distance
- Longest Common Subsequence (lcs)
- Hamming Distance
- Damerau–Levenshtein Distance (dld)
- Jaro distance

## Execute Instructions

> \> cd ../IHS-P1-UEM/src  
> \> make  
> \> make run

If you want to change the `.js` scripts for another algorithm, there are commented lines in the `src/main.c` for you to pick.