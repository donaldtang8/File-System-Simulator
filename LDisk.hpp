#ifndef _LDisk_HPP_
#define _LDisk_HPP_

#include <iostream>
#include "DNode.hpp"

using namespace std;

class LDisk{
    public:
        LDisk(int blocks){
            this->totalBlocks = blocks;
            this->head = new DNode(0, blocks, 0);
        }
        int free_blocks(){
            // check the number of free blocks
            DNode * current = this->head;
            int num_free = 0;
            while(current!=NULL){
                if (current->usedBit == 0){
                    num_free += current->end - current->start;
                }
                current = current->next;
            }
            return num_free;
        }
        int insert(){
            /*
                Insert a block into the diskspace

                Return:
                    The id of the inserted block
            */
            cout<< "LDisk " << endl;
            if (this== NULL) cout << "this is NULL" << endl;
            DNode * current = this->head;
            int successInsert = 0;
            int blockID = 0;
            while (current != NULL){
                if (current->usedBit == 0){
                    // When we have an available block
                    DNode * next = current->next;
                    blockID = current->start;
                    if (current->end - current->start == 1){
                        // This set contains only 1 block
                        current->usedBit = 1;
                    }else{
                        // This set contains multiple blocks and we need to split this set
                        DNode * cdr = new DNode(current->start + 1, current->end, current->usedBit);
                        current->end = current->start + 1;
                        current->usedBit = 1;
                        current->next = cdr;
                        // if (current->next == NULL) cout << "line 37";
                        current->next->next = next; 
                        // if (current->next == NULL) cout << "line 39";
                    }                    
                    successInsert = 1;
                    return blockID;
		    break;
                }
                current = current->next;
            }    
            return -1;
        }

        void remove(int blockID){
            /*
            Free the block starting at the blockAddress
            */

            DNode * current = this->head;
            // Traverse the list to find the block at the specific blockID
            while(blockID >= current->end - current->start){
                blockID -= current->end - current->start;
                current = current->next;
            }
            DNode * next = current->next;
            if (current->end - current->start == 1){
                // The memory chunk has only 1 block
                current->usedBit = 0;
            }else{
                // The memory chunk has multiple blocks
                if (blockID == current->start){
                    // The blockID is at the front of the DNode
                    DNode * cdr = new DNode(current->start + 1, current->end, current->usedBit);
                    current->end = current->start + 1;
                    current->usedBit = 0;
                    current->next = cdr;
                    current->next->next = next;
                }else if(blockID = current->end - 1){
                    // The blockID is at the end of the DNode
                    DNode * last = new DNode(current->end - 1, current->end, 0);
                    current->end -= 1;
                    current->next = last;
                    current->next->next = next;
                }else{
                    // The blockID is in the middle of the DNode
                    DNode * cddr = new DNode(blockID + 1, current->end, current->usedBit);
                    DNode * cadr = new DNode(blockID, blockID + 1, 0);
                    current->end = blockID;
                    current->next = cadr;
                    current->next->next = cddr;
                    current->next->next->next = next;
                }
            }
        }
        void update(){
            DNode * current = this->head;
            while(current != NULL && current->next != NULL){
                if (current->usedBit == 0 && current->next->usedBit == 0
                    || current->usedBit>0 && current->next->usedBit > 0)
                {
                    current->end = current->next->end;
                    current->usedBit += current->next->usedBit;
                    current->next = current->next->next;
                }else{
                    current = current->next;
                }
            }
        }
        void print(int blockSize){
            DNode * current = this->head;
            int fragmentation = 0;
            while (current != NULL){
                if (current->usedBit == 0){
                    std::cout << "Free: " << current->start * blockSize + 1 << "-" 
                    << current->end * blockSize << std::endl;
                    fragmentation += (current->end - current->start) * blockSize;                     
                }else{
                    std::cout << "In use: " << current->start * blockSize + 1 << "-" 
                    << current->end * blockSize << std::endl;        
                }
                current = current->next;
            }
            std:: cout << "Fragmentation: " << fragmentation << std::endl;
        }
        int totalBlocks;
        DNode* getHead() {
            return head;
        }
        ~LDisk();
    private:
        DNode * head;
};


#endif
