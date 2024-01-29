# Book-contents
This project is a book chapters management system that allows users to read information about book chapters from an input file, perform searches, and make small changes concerning the book chapters. The program supports hierarchical structures with open and closed chapters.

**Goal:** To practice to read code written by another programmer and learn modularity. Use pointers and dynamic memory management and consider how to divide program into parts by using functions.
**Task:** Since this was part of my coursework some of the modules were already provided and the main task was implementing the public part of the book class and using the private part forany utility methods i saw fit. Also had to choose a suitable data structure for this.

**Features**
- Hierarchical Structure: Book chapters can have subchapters, forming a hierarchical structure.
- Commands: Users can execute various commands, including opening/closing chapters, viewing parents, subchapters, siblings, and more.
  
**Commands**
- QUIT: Closes the program.
- IDS: Prints all book chapters with their IDs.
- CONTENTS : Prints the contents of the book, excluding subchapters of closed chapters.
- CLOSE<ID>: Closes the given chapter and its subchapters.
- OPEN: Opens the given chapter.
- OPEN_ALL: Opens all chapters.
- PARENTS<ID> <N> : Prints parent chapters of ID up to level N. 
- SUBCHAPTERS <ID> <N>: Prints subchapters of ID up to level N.
- SIBLINGS<ID>:Prints sibling chapters (same parent chapter) of ID.
- LENGTH <ID>: Prints the total length of the given chapter, including subchapters.
- LONGEST <ID>: Prints the longest chapter in the hierarchy of the given chapter.
- SHORTEST <ID> Prints the shortest chapter in the hierarchy of the given chapter.
  

  
