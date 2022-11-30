#include "stdio.h"
#include <stdlib.h>
#define ARRAYSIZE 100000


int futureMem[ARRAYSIZE];

int getPage(int pageSize, int memoryRef){
    int pageNbr = memoryRef/pageSize;
    return pageNbr;
}

//checks for the next time memref in current page is referenced
int checkOptimal(int index, int page, int pageSize){
    int startIndex = index;
    while(index < ARRAYSIZE){
        if(getPage(pageSize, futureMem[index]) == page){
            return index - startIndex;
        }
        index++;
    }
    //if no references are found, return arraysize + 1 
    //(will always be largest -> will get replaced)
    return ARRAYSIZE + 1;
}

int checkPage(int page, int phyPages, int pageSize, int index, int phyMemory[]){
    int farthestPage = 0;
    int farthestDist = 0;
    int found = 0;
    
    //check if page already is loaded
    for(int i = 0; i < phyPages; i++){
        if(phyMemory[i] == page){
            found = 1;
        }
    }
    //do nothing if page was found
    if(found){
        return 0;
    }

    //call checkOptimal for all physical pages
    //replace page with largest distance
    //(tempdist is used so that checkOptimal is only called once per loop)
    int tempDist = 0;
    for(int i = 0; i < phyPages; i++){
        tempDist = checkOptimal(index, phyMemory[i], pageSize);
        if(farthestDist < tempDist){
            farthestDist = tempDist;
            farthestPage = i;
        }
    }
    phyMemory[farthestPage] = page;
    return 1;
}

int main(int argc, char *argv[]){
    //read parameters
    int phyPages = atoi(argv[1]);
    int pageSize = atoi(argv[2]);
    char* fileName = argv[3];
    printf("No physical pages = %d, Page size = %d\n", phyPages, pageSize);

    //phyMemory[page][0] = loadedPage, phyMemory[page][1] = pagePriority
    int phyMemory[phyPages];
    //set all mem-pages to -1 (unowned)
    for(int i = 0; i < phyPages; i++){
        phyMemory[i] = -1;
    }

    int pageFault = 0;
    int memoryRef = 0;
    int index = 0;
    FILE * memReader;
    memReader = fopen(fileName, "r");

    //put all references into futureMem to read future
    while(!feof(memReader)) {
        //if succesfully read integer -> put in memoryRef
        if(fscanf(memReader, "%d", &memoryRef) == 1){
            futureMem[index] = memoryRef;
            index++;
        }
        else{
            //printf("Line: %d\t| Failed to read integer.\n", amount);
        }
    }

    index = 0;
    memReader = fopen(fileName, "r");
    while(!feof(memReader)) {
        //if succesfully read integer -> put in memoryRef
        if(fscanf(memReader, "%d", &memoryRef) == 1){
            index++;
            pageFault += checkPage(getPage(pageSize, memoryRef), phyPages, pageSize, index, phyMemory);
        }
        else{
            //printf("Line: %d\t| Failed to read integer.\n", amount);
        }
    }
    fclose(memReader);

    printf("Read memory trace from %s...  Read %d memory references\n", fileName, index);
    printf("Result: %d page faults\n", pageFault);
    return 0;
}