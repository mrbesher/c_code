# C Code
This repository contains some projects that I wrote / am writing for university assignments. Folders are named after their relative courses / projects. There is a huge room for improvments and there are a lot of things that I know now and didn't when I wrote those. I don't plan on doing anything with these projects anyway, so I am making them open source for fun.

## Tetris
A simple implementation of the popular TETRIS game lacking a lot of functionality. It works best on unix-like operating systems.

## Numerical Methods
Those are assignments to reinvent some calculation methods. Beware of memory leaks! I wasn't paying attention to those while writing the code.

## Histogram equalization
Contains `histogram_equalization.c` which is a simple code for small file inputs (photos in pgm ASCII format) to apply histogram equalization to them. Note that the code will work only if the header comments are removed. An updated version `equalize.c` is also included which removes comments at the beginning of the file and can deal with images of any size.
Here is a video on this project: https://youtu.be/OWPc9IVfdLQ
Compile by typing `gcc equalize.c -o equalize` if you use gcc and run by typing: `./equalize input.pgm [output.pgm]` in the directory that you compiled the source code.

## Database
Contains `main.c` which is a database management project requested to be coded in a single file.
The database is aimed at storing information about the students and the lecturers of an arbitrary university and tracking their course enrollment, assignment and class statuses.The folder has some files created during a test of the program.
Also `report.pdf` [file](./database/report.pdf) contains more detailed information in Turkish.
