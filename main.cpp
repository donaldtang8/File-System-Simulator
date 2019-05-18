#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "GNode.hpp"
#include "DNode.hpp"
#include "FNode.hpp"
#include "Tree.hpp"
#include "LDisk.hpp"
#include "LFile.hpp"

void useStatement(){
    std::cout<<"./driver -f input_file -d input_dir -s disk_size -b block_size"<<std::endl;
}

int main(int argc, char* argv[]){
    
    //PARSE ARGS
    std::string fileList = "";
    std::string dirList = "";
    int diskSize = -1;
    int blockSize = -1;
    char flag;
    
    while((flag = getopt(argc, argv, "f:d:s:b:")) != -1)
    {
        switch(flag)
        {
            case 'f':
                fileList = optarg;
                break;
            case 'd':
                dirList = optarg;
                break;
            case 's':
                diskSize = atoi(optarg);
                break;
            case 'b':
                blockSize = atoi(optarg);
                break;
            case '?':
                useStatement();
                exit(1);
            default:
                useStatement();
                exit(1);
        }
    }
    
    // ERROR CHECKING
    if(diskSize < 0 || blockSize < 0 || fileList.length() < 1 || dirList.length() < 1)
    {
        useStatement();
        exit(1);
    }
    
    //READING STREAMS
    std::ifstream fileStream(fileList.c_str());
    std::ifstream directoryStream(dirList.c_str());
    std::string line;
    size_t index=0;
    
    if (!directoryStream){
        std::cerr<< "Cannot open file " <<std::endl;
        exit(1);
    }
    
    int numBlocks= diskSize/blockSize;
    
    // CREATE a linked-list data structure Ldisk representation of disk space - free and in use
    LDisk * diskSpace = new LDisk(numBlocks);    
    //CREATE THE DIRECTORY G
    Tree * directory = new Tree(blockSize, numBlocks);
    directory->set_disk(diskSpace);
    
    
    
    //READ THE DIRECTORIES
    while(std::getline(directoryStream, line)){
        //ADD EACH DIRECTORY AS A NODE
        cout  << "Adding directory for line: " << line << endl;
        directory->addNode(-1,line);
    }
    directoryStream.close();
    
    directory->prePrint();
    
    if (!fileStream){
        std::cerr<< "Cannot open file" <<std::endl;
    }
    
    //READ THE FILES
    while (std::getline(fileStream,line)){
        //PARSE THE LINE INTO NAME AND SIZE
        //MAY VARY
        std::string name;
        int size=0;
        
        std::string buffer;
        std::stringstream stream(line);
        int ctr=0;
        
        while (stream >> buffer){
            if (ctr==6){
                size= atoi(buffer.c_str());
            }
            if (ctr == 10){
                name= buffer;
            }
            ctr++;
        }
        // ADD EACH TO THE DIRECTORY
	if (line.length() > 0) {
		cout << "Adding file for line: " << line << endl;
        	directory->addNode(size,name);
		//directory->setCurr();
		deque<string> getDirs = directory->parsePath(name);
		//getDirs.pop_back();
		cout << getDirs.back() << endl;
        	directory->append(getDirs.back(), size);
	}
    }
    fileStream.close();
    
    directory->prePrint();
    
    /* SET COMMAND PARAMETERS */
    directory->setCurr();   // set currentDir of tree equal to root to start tracking working directory
    GNode* curr;            // curr will track the working directory
    //HANDLE USER INPUT
    while(1) {
        std::cout << "FileSystem/Shell" << endl;    // print line
        curr = directory->getCurrentDir();          // we keep getting the most current working directory
        cout << curr->name << "/";                  // print line
        std::string command;
        std::string arg1;
        std::string arg2;
        getline(std::cin,command);
        
        std::stringstream stream(command);
        
        stream >> command;
        stream >> arg1;
        stream >> arg2;
        
        if (!command.compare("cd")) {
            // if there is no argument passed
            if (!arg1.length() > 0) {
                cout << "Please add a directory argument" << endl;
                continue;
            }
            directory->cd(arg1);
        }
        else if(!command.compare("cd..")) {
            directory->cdOut();
        }
        else if(!command.compare("ls")) {
            // if there is no argument passed
            if (!arg1.length() >0) {
                directory->ls();
            } else {
                directory->cd(arg1);
                directory->ls();
                directory->cdOut();
            }
        }
        else if(!command.compare("mkdir")) {
            if (arg1.compare("")) {
                directory->mkdir(arg1);
            }
            else {
                std::cout << "Need more arguments" <<std::endl;
            }
        }
        else if(!command.compare("create")) {
            if (arg1.compare("")) {
                directory->create(arg1);
            }
            else {
                std::cout << "Need more arguments" <<std::endl;
            }
        }
        else if(!command.compare("append")) {
            if (arg1.compare("") && arg2.compare("")) {
                directory->append(arg1,stoi(arg2));
            }
            else {
                std::cout << "Need more arguments" <<std::endl;
            }
        }
         
        else if(!command.compare("remove")) {
            if (arg1.compare("") && arg2.compare("")) {
                directory->remove(arg1,stoi(arg2));
            }
            else {
                std::cout << "Need more arguments" <<std::endl;
            }
        }

        else if(!command.compare("delete")) {
            directory->deleteNode(arg1);
        }
         
        else if(!command.compare("exit")) {
            std::cout << "Bye" << std::endl;
            break;
        }
         
        else if(!command.compare("dir")) {
            directory->dir();
        }
         
        else if(!command.compare("prdisk")) {
             directory->printDisk();
        }
         
        else if(!command.compare("prfiles")) {
            directory->printFiles();
        }

        else {
            std::cout << "Command does not exist" <<std::endl;
        }
    }
}
