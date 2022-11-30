#include "stdio.h"
#include <stdlib.h>

int getPage(int pageSize, int memoryRef){
    int pageNbr = memoryRef / pageSize;
    return pageNbr;
}

//check if page is in memory, otherwise replace
int checkPage(int page, int phyPages, int phyMemory[], int OldestPageIndex){
    int found = 0;

    
    // Check if the page already exists in memory
    for (int i = 0; i < phyPages; i++)
    {
        // If page was found we can break out
        if (page == phyMemory[i])
        {
            found = 1;
            break;
        }
    }

    // Only replace page if it wasn't already present in the memory
    if (found == 0)
    {
        phyMemory[OldestPageIndex] = page;

        // Return 1 if the page was replaced because we got a page fault
        return 1;
    }

    // Return 0 if there was no page fault
    return 0;
}

int main(int argc, char *argv[]){
    //read parameters
    int phyPages = atoi(argv[1]);
    int pageSize = atoi(argv[2]);
    char* fileName = argv[3];
    printf("No physical pages = %d, Page size = %d\n", phyPages, pageSize);

    //phyMemory[page] = loadedPage
    int phyMemory[phyPages];
    for(int i = 0; i < phyPages; i++){
        phyMemory[i] = -1;
    }

    int pageFault = 0;
    int memoryRef = 0;
    int amount = 0;
    int OldestPageIndex = 0;
    FILE * memReader;
    memReader = fopen(fileName, "r");
    while(!feof(memReader)) {
        //if succesfully read integer -> put in memoryRef
        if(fscanf(memReader, "%d", &memoryRef) == 1){
            amount++;

            // If check page returned 1 we had a page fault
            if (checkPage(getPage(pageSize, memoryRef), phyPages, phyMemory, OldestPageIndex) == 1)
            {
                pageFault++;

                // The oldest page was replaced so the oldest will now be at the next index
                OldestPageIndex = (OldestPageIndex + 1) % phyPages;
            }
        }
        else{
            printf("Line: %d\t| Failed to read integer.\n", amount);
        }
    }
    fclose(memReader);

    printf("Read memory trace from %s...  Read %d memory references\n", fileName, amount);
    printf("Result: %d page faults\n", pageFault);
    return 0;
}