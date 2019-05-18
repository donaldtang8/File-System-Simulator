#ifndef _Tree_HPP_
#define _Tree_HPP_

#include "GNode.hpp"
#include "LDisk.hpp"
#include <deque>
#include <queue>
#include <vector>

using namespace std;

class Tree {
    public:
        Tree(int bs,int b) {
            this->blockSize = bs;
            this->numBlocks = b;
            this->root = new GNode(".", 0, -1, NULL);
        }
        void set_disk(LDisk * d){
            this->disk = d;
        }
        //FOR YOUR DIRECTORIES AND FILES
        void addNode(int size, std::string name) {
            // subDirs will be a list of all directories listed in the path
            deque<string> subDirs = parsePath(name);
            /* DEBUGGING
            cout << "subDirs: " << endl;
            for (int i = 0; i < subDirs.size(); i++) {
                cout << subDirs[i] << endl;
            }
            */
            // if size is less than 0, then we are adding a directory
            if (size < 0) {
                subDirs.pop_front();            // we pop the front of subDirs so we start at the direct children of root
                GNode* parent = root;           // set parent to start at root
                currentDir = root;              // set currentDir to start at root, will be traversing with currentDir
                bool isChild;                   // isChild checks whether the current directory is a child of the parent
                string curr;                    // get string of current directory
                //cout << "Looping through subdirectories" << endl;
                while (subDirs.size() > 0) {
                    curr = subDirs.front();     // get current directory
                    subDirs.pop_front();        // remove first directory so move on in the list
                    isChild = false;
                    // we loop through the children of parent node and look for node in tree
                    for (list<GNode*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it) {
                        //cout << "Finding children nodes" << endl;
                        // if found set currentDir as the that node
                        if ((*it)->name == curr) {
                            currentDir = *it;   // update currentDir pointer when we have found node in tree
                            isChild = true;     // if name of current node is found in parent directory, then it is a child of parent directory
                        }
                    }
                    //cout << "Creating new child node" << endl;
                    // if directory is not a child and has not been found, then we create a new node for it
                    if (!isChild) {
                        GNode* newNode = new GNode(curr, 0, size, parent);  // create new GNode
                        parent->children.push_back(newNode);                // push new GNode to parent's children list
                        newNode->parent = parent;                           // set newNode's parent to parent node
                    }
                    //cout << "Traversing next level" << endl;
                    parent = currentDir;        // keep traversing tree
                }
            }
            // if size is greater than or equal to 0, then we are adding a file
            if (size != -1 && subDirs.size() > 0) {
                GNode* parent = root;           // set parent to start at root
                currentDir = root;              // set currentDir to start at root, will be traversing with currentDir                 // isChild checks whether the current directory is a child of the parent
                string curr;                    // get string of current directory
                // the final index of subDirs will be the name of the file, so we stop looping once we reach the last sub directory
                /*
                cout << subDirs.size() << endl;
                cout << name << endl;
                cout << "Looping through subdirectories" << endl;
                */
                while (subDirs.size() != 1) {
                    curr = subDirs.front();
                    subDirs.pop_front();
                    // we loop through the children of parent node and look for node in tree
                    for (list<GNode*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it) {
                        //cout << "Finding children nodes" << endl;
                        // if found set currentDir as the that node
                        if ((*it)->name == curr) {
                            currentDir = *it;   // update currentDir pointer when we have found node in tree
                        }
                    }
                    //cout << "Traversing next level" << endl;
                    parent = currentDir;        // keep traversing tree
                }
                GNode* newNode = new GNode(subDirs[0], 0, size, parent);
                parent->children.push_back(newNode);
                newNode->parent = parent;
		newNode->file = new LFile(0, this->blockSize, this->disk);
		newNode->file->append(size);
            }
        }
        // parsePath is a helper function that takes in directory paths in dir_list and returns a list of directories
        deque<string> parsePath(string path) {
            deque<string> subDirs;
            int i = 0;
            char c;
            string currDir;
            while (i < path.length()) {
                c = path[i];
                if (c == '/') {
                    subDirs.push_back(currDir);
                    currDir = "";
                }
                else if (i == path.length()-1) {
                    currDir += c;
                    if (currDir.length() > 0) {
                        subDirs.push_back(currDir);
                    }
                }
                else currDir += c;
                i++;
            }
            return subDirs;
        }
        //FOR YOUR DEBUGGING
        void prePrint() {
            cout << "DEBUGGING" << endl;
            queue<GNode*> current;
            list<GNode*> children;
            current.push(root);
            while (!current.empty()) {
                currentDir = current.front();
                cout << currentDir->name << endl;
                current.pop();
                children = currentDir->children;
                for (list<GNode*>::iterator it = children.begin(); it != children.end(); ++it) {
                     current.push(*it);
                }
            }
        }
        /* COMMAND IMPLEMENTATIONS */
        // set current directory as root
        void setCurr() {
            this->currentDir = this->root;                                                                                                                                                                          
        }
        // get root
        GNode* getRoot() {
            return this->root;
        }
        // get current working directory
        GNode* getCurrentDir() {
            return this->currentDir;
        }
        // change working directory to requested direectory
        void cd(std::string name) {
            bool foundNode = false;
            // traverse current working directory's children and look for requested directory
            for (list<GNode*>::iterator it = currentDir->children.begin(); it != currentDir->children.end(); ++it) {
                // if name of requested directory is found and is a directory (size < 0)
                if ((*it)->name == name && (*it)->size < 0) {
                    currentDir = *it;   // update currentDir pointer when we have found node in tree
                    foundNode = true;   // update foundNode
                    break;
                }
            }
            // if node is not found, cout error
            if (foundNode == false) {
                cout << "No directory found" << endl;
            }
        }
        // change working directory to parent
        void cdOut() {
            // if parent is null, then we are at root
            if (currentDir->parent == NULL) {
                currentDir = root;                  // set working directory to root
            } else {
                currentDir = currentDir->parent;    // set working directory to parent
            }
        }
        void ls() {
            // cout << currentDir->name << endl;
            // cout << currentDir->children.size() << endl;
            for (list<GNode*>::iterator it = currentDir->children.begin(); it != currentDir->children.end(); ++it) {
                // print all the children of current directory
                cout << (*it)->name << " " << endl;
            }
        }
        void mkdir(std::string name) {
            GNode* newDir = new GNode(name, 0, -1, currentDir);
            currentDir->children.push_back(newDir);
        }
        void create(std::string name) {
            GNode* newFile = new GNode(name, 0, 0, currentDir);
            newFile->file = new LFile(0, this->blockSize, this->disk);
            currentDir->children.push_back(newFile);
        }
        void append(std::string name, int bytes) {
	    cout << "In append function" << endl;
            GNode* nodeToAppend;
            bool foundNode = false;
		cout << "Appending to existing directory" << endl;
            	// traverse current working directory's children and look for file to append to
            	for (list<GNode*>::iterator it = currentDir->children.begin(); it != currentDir->children.end(); ++it) {
            	    // if name of requested directory is found and is a file (size >= 0)
			cout << "Looking for node: " << name << endl;
			cout << "Current node: " << (*it)->name << endl;
             	   if ((*it)->name == name && (*it)->size >= 0) {
             	       nodeToAppend = *it; // set nodeToAppend when we have found node in tree
            	        foundNode = true;   // update foundNode
             	       break;
             	   }
           	 }
            	// if node is not found, cout error
           	 if (foundNode == false) {
           	     cout << "No directory found" << endl;
           	 }
            	// else if node is found, get LFile of that node and call its append function 
           	 else {
		    	cout << "Appending bytes for file" << endl;
                	if(nodeToAppend==NULL) cout<< "nodeToAppend is NULL" <<endl;
                	if(nodeToAppend->file==NULL) cout<< "file is NULL" <<endl;
               	 	nodeToAppend->file->append(bytes);          // append bytes to this node's LFile
                	int size = (nodeToAppend->size) + bytes;    // calculate new size of node
                	nodeToAppend->size = size;                  // update size of node
                	//cout << nodeToAppend->size << endl;
            	}
        }
        void remove(std::string name,int bytes) {
            GNode* nodeToRemove;
            bool foundNode = false;
            // traverse current working directory's children and look for file to remove to
            for (list<GNode*>::iterator it = currentDir->children.begin(); it != currentDir->children.end(); ++it) {
                // if name of requested directory is found and is a file (size >= 0)
                if ((*it)->name == name && (*it)->size >= 0) {
                    nodeToRemove = *it; // set nodeToRemove when we have found node in tree
                    foundNode = true;   // update foundNode
                    break;
                }
            }
            // if node is not found, cout error
            if (foundNode == false) {
                cout << "No directory found" << endl;
            }
            // else if node is found, get LFile of that node and call its remove function 
            else {
		if (bytes > nodeToRemove->size || bytes <= 0) {
			cout << "Error: Please enter a valid amount of bytes to remove" << endl;
		} else {
                	nodeToRemove->file->remove(bytes);          // remove bytes of this node's LFile
                	int size = (nodeToRemove->size) - bytes;    // calculate new size of node
                	nodeToRemove->size = size;                  // update size of node
			disk->update();
                	//cout << nodeToRemove->size << endl;
		}
            }
        }
        void deleteNode(std::string name) {
            GNode* nodeToDelete;
            bool foundNode = false;
            // traverse current working directory's children and look for directory/file
            for (list<GNode*>::iterator it = currentDir->children.begin(); it != currentDir->children.end(); ++it) {
                // if name of requested directory/file
                if ((*it)->name == name) {
                    nodeToDelete = *it;                 // set nodeToDelete when we have found node in tree
                    currentDir->children.remove(*it);   // remove node as children of currentDir
                    foundNode = true;                   // update foundNode
                    break;
                }
            }
            // if node is not found, cout error
            if (foundNode == false) {
                cout << "No directory found" << endl;
            }
            // else if node is found, delete 
            else {
                // if node is a file, remove all blocks in memory that is associated with that file
                if (nodeToDelete->size >= 0) {
                    LFile* currLFile = nodeToDelete->file;                  // get file's LFile
		    /*
                    FNode* curr = currLFile->getFirstNode();                // get first node in LFile
                    // keep looping LFile until we reach the last node
                    while (curr != NULL) {
                        int blockId = (curr->blockAddress) / blockSize;     // get block ID from FNode
                        disk->remove(blockId);                              // remove that block from LDisk
                        curr = curr->next;                                  // move to next FNode
                    }
		    */
		    currLFile->remove(nodeToDelete->size);		    // remove entire file
		    disk->update();					    // update LDisk
                }
                nodeToDelete->parent = NULL;                                // set parent to NULL
            }
        }
        void dir() {
	    GNode* initial = currentDir;
            prePrint();
	    currentDir = initial;
        }
        void printDisk() {
            int index = 0;
            DNode* curr = disk->getHead();
            while (curr != NULL) {
                if (index > 15) break;
                cout << "Node: " << index << endl;
                cout << "Start id: " << curr->start << endl;
                cout << "End id: " << curr->end << endl;
                if (curr->usedBit == 0) {
                    cout << "Use Status: " << "Free" << endl;
                } else {
                    cout << "Use Status: " << "Used" << endl;
                }
                index++;
                curr = curr->next;
            }
        }
        void printFiles() {
            // traverse each node in tree looking for files, if it is a file, print file information
            GNode* currentFile;
            queue<GNode*> current;
            list<GNode*> children;
            current.push(root);
            while (!current.empty()) {
                currentFile = current.front();
                if (currentFile->size >= 0) {
                    cout << "Name: " << currentFile->name << endl;
                    cout << "Size: " << currentFile->size << endl;
		    
		    if (currentFile->file->getSize() > 0) {
			    cout << "Size of LFile: " << currentFile->file->getSize() << endl;
		    	currentFile->file->print();
		    }
		    
                }
                current.pop();
                children = currentFile->children;
                for (list<GNode*>::iterator it = children.begin(); it != children.end(); ++it) {
                     current.push(*it);
                }
            }
        }
        // CLEANUP
        ~Tree();

    private:
        GNode * root;
        LDisk * disk;
        // USE currentDir as a handle
        GNode * currentDir;
        int numBlocks;
        int blockSize;
        void pFile(GNode * dir);
        int lookUp(std::string name);
        void step(std::string name);
        void deleteHelper(GNode * tmp);
        void deleteFile(GNode * f);
        int time;
};

#endif
