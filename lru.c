#include "stdio.h"
#include <stdlib.h>

int getPage(int pageSize, int memoryRef)
{
    int pageNbr = memoryRef / pageSize;
    return pageNbr;
}

// check if page is in memory, otherwise replace
int checkPage(int page, int phyPages, int phyMemory[][2])
{
    // page to be replaced
    int replaceIndex = 0;
    int leastUsed = 0;
    int found = 0;

    // add +1 for all unused pages, reset if wanted page exists
    for (int i = 0; i < phyPages; i++)
    {
        if (phyMemory[i][0] == page)
        {
            phyMemory[i][1] = 0;
            found = 1;
        }
        else
        {
            phyMemory[i][1] += 1;
        }
    }
    // do nothing if page was found
    if (found)
    {
        return 0;
    }
    // find the least used page
    for (int i = 0; i < phyPages; i++)
    {
        if (phyMemory[i][1] > leastUsed)
        {
            leastUsed = phyMemory[i][1];
            replaceIndex = i;
        }
    }
    // replace page in memory
    phyMemory[replaceIndex][0] = page;
    phyMemory[replaceIndex][1] = 0;
    return 1;
}

int main(int argc, char *argv[])
{
    // read parameters
    int phyPages = atoi(argv[1]);
    int pageSize = atoi(argv[2]);
    char *fileName = argv[3];
    printf("No physical pages = %d, Page size = %d\n", phyPages, pageSize);

    // phyMemory[page][0] = loadedPage, phyMemory[page][1] = pagePriority
    int phyMemory[phyPages][2];
    // set all mem-pages to -1 (unowned)
    for (int i = 0; i < phyPages; i++)
    {
        phyMemory[i][0] = -1;
        phyMemory[i][1] = 0;
    }

    int pageFault = 0;
    int memoryRef = 0;
    int amount = 0;
    FILE *memReader;
    memReader = fopen(fileName, "r");
    while (!feof(memReader))
    {
        // if succesfully read integer -> put in memoryRef
        if (fscanf(memReader, "%d", &memoryRef) == 1)
        {
            amount++;
            pageFault += checkPage(getPage(pageSize, memoryRef), phyPages, phyMemory);
        }
        else
        {
            // printf("Line: %d\t| Failed to read integer.\n", amount);
        }
    }
    fclose(memReader);

    printf("Read memory trace from %s...  Read %d memory references\n", fileName, amount);
    printf("Result: %d page faults\n", pageFault);
    return 0;
}