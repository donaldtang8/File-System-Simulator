#ifndef _LFILE_HPP_
#define _LFILE_HPP_

#include "FNode.hpp"
#include "LDisk.hpp"

using namespace std;

class LFile
{
public:
    LFile(int ts, int bs, LDisk * d){
        this->totalSize = ts;
        this->blockSize = bs;
        this->freeBytes = 0;
        this->disk = d;
        this->start = new FNode();
    }
    void append(int numBytes) {
        // check if this->freeBytes can allocate all of numBytes
	cout << "Free bytes: " << this->freeBytes << endl;
        if (this->freeBytes >= numBytes) {
            this->freeBytes = this->freeBytes - numBytes;
        } else {
            int blocksReq = (numBytes - this->freeBytes) / this->blockSize;     // get number of bytes needed after accounting for this->freeBytes
            int extraBytes = (numBytes - this->freeBytes) % this->blockSize;    // keep track of any extra bytes left over at the end
            if (extraBytes > 0) ++blocksReq;
            if (this->disk->free_blocks() < blocksReq){ // Check if there is enough memory
                cout << "Cannot append any more, disk is full" << endl;
            }else{                
                cout << "blocksReq: " << blocksReq << endl;
                int blockId;                                            // blockId will be the id of the DNode just inserted
                int blockAddress;                                       // blockAddress will be the address of the FNode to add
                FNode* lastNode;                                        // lastNode is the last node of LFile
                // for each block needed to add, we update LDisk and add a new FNode
                for (int i = 0; i < blocksReq; i++) {
                    lastNode = getLastNode();               // get last node
                    cout << "Insert iteration: " << i << endl;
                    blockId = this->disk->insert();               // get blockId of block just used
                    blockAddress = blockId * this->blockSize;     // get blockAddress
		    if (blockId < 0) {
			    cout << "Cannot allocate block, not enough memory" << endl;
			    break;
		    }
		    if (lastNode->blockAddress == -1) {
			    cout << "This is a new file" << endl;
			    cout << "Block id: " << blockId << endl;
			    cout << "Block address: " << blockAddress << endl;
			    lastNode->blockAddress = blockAddress;
		    }
		    if (lastNode == NULL) {
			    cout << "This is a file we just removed" << endl;
		    } 
		    else {
                    	FNode* newNode = new FNode();           // create new FNode
                    	newNode->blockAddress = blockAddress;   // update block address of new FNode
                    	lastNode->next = newNode;               // set next pointer of lastnode equal to new FNode just created
		    }
                }
                this->disk->update();
                this->freeBytes = this->blockSize - extraBytes;
                this->totalSize += blocksReq;
            }
        }
    }
    void remove(int numBytes) {
        int blocksReq = numBytes / blockSize;   // get number of bytes needed
        int blockId;                            // blockId will be the id of the DNode just inserted
        int blockAddress;                       // blockAddress will be the address of the FNode to add
        int bytesRemaining = 0;                 // free bytes remaining after removing blocks that are partially full
        FNode* lastNode;                        // lastNode is the last node of LFile
        FNode* secondToLastNode;
        
        // check if there is a block that is partially filled
        // if there is, remove bytes from partially filled block first
	cout << "Free bytes to remove: " << this->freeBytes << endl;
        if (this->freeBytes > 0) {
            bytesRemaining += (numBytes - (blockSize - this->freeBytes));	// update number of bytes left to remove after removing partially filled block
	    this->freeBytes = 0;						// update number of free bytes now that we removed partially filled block
	    lastNode = getLastNode();						// get last node
	    blockAddress = lastNode->blockAddress;				// get block address of last node
	    cout << "Removing block at address: " << blockAddress << endl;
	    cout << "Removing block at id: " << blockAddress / blockSize << endl;
	    this->disk->remove(blockAddress/blockSize);				// remove last node
            FNode* newLastNode = getSecondToLastNode();                		 // update last node pointer
            newLastNode->next = NULL;
	    totalSize--;
        }
	// if there is only one block in LFile
	if (this->totalSize == 1) {
		if (numBytes < blockSize) {
			this->freeBytes += numBytes;
		} else {
			blockAddress = this->start->blockAddress;	// get block address of the FNode
			this->disk->remove(blockAddress/blockSize);	// remove block at that id
			this->start==NULL;				// set LFile to null
			totalSize--;					// decrement size
			bytesRemaining -= blockSize;
		}
	} else {
        	// for each block we need to remove, we update LDisk and remove the last FNode in LFile
        	for (int i = 0; i < blocksReq; i++) {
			if (this->totalSize == 1) {
				blockAddress = this->start->blockAddress;
				this->disk->remove(blockAddress/blockSize);
				this->start==NULL;
				totalSize--;
				bytesRemaining -= blockSize;
				break;
			}
            		lastNode = getLastNode();                   // get the last node
           		secondToLastNode = getSecondToLastNode();   // get second to last node
            		blockAddress = lastNode->blockAddress;      // update the blockAddress
			cout << "Removing block at address: " << blockAddress << endl;
			cout << "Removing block at id: " << blockAddress / blockSize << endl;
            		this->disk->remove(blockAddress/blockSize); // remove node at that blockAddress from disk
            		secondToLastNode->next = NULL;              // update last node pointer
            		totalSize--;                                // update total size
			bytesRemaining-= blockSize;
        	}
	}
        // if there are still bytes leftover to remove, we will remove them
        if (bytesRemaining > 0) {
            this->freeBytes = bytesRemaining;
        }
	cout << "Free bytes left after removing: " << this->freeBytes << endl;
    }
    void print() {
        FNode* curr = start;
        while (curr != NULL) {
            cout << "Block address is: " << curr->blockAddress << endl;
            curr = curr->next;
            
        }
    }
    int getSize() {
        return totalSize;
    }
    FNode* getFirstNode() {
        return start;
    }
    FNode* getSecondToLastNode() {
        FNode* curr = start;
        FNode* tmp = start;
        while (curr->next != NULL) {
            tmp = curr;
            curr = curr->next;
        }
        return tmp;
    }
    FNode* getLastNode() {
        FNode* curr = start;
        FNode* tmp = start;
        while (curr != NULL) {
            tmp = curr;
            curr = curr->next;
        }
        return tmp;
    }
    ~LFile();
private:
    FNode* start;
    int totalSize;
    int blockSize;
    int freeBytes;
    LDisk * disk;
    
};

#endif
