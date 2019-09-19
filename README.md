# File-System-Simulator

### Explanation

The goal of the assignment is to emulate a file system.

1) Tree.hpp represents the Hierarchical Tree G
2) The tree is made of GNodes which represent directories/files, they have a parent and a list of children of GNodes.
3) GNodes also have pointer to LFile which shows where the blocks of memory are stored in Disk.
4) LFile.hpp represents the linked list container to show where the block adresses which make up the file are stored. The linked list itself is made up of FNodes which stores the block address of the memory. LFile also points to the LDisk so that it can add or shrink the disk space as needed.
5) LDisk.hpp is the representation of disk memory, it is the container for the Disk linked list. The linked list itself is made of DNodes. DNodes have a start,end, and used bit so you can tell which addresses in memory are free.   

### How to run this program:
Open up terminal and run the command `./test.sh` to run program with pre-defined arguments in test script.
