#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberOfPages; // taken from the file indicate how much page i have in memory
char outputFileName[25];

void ReadingFile(char *fileName, char *method, int *pagesIn);
void OPTIMALPolicy(int *pagesIn);
void FIFOPolicy(int *pagesIn);
void LRUPolicy(int *pagesIn);
void CLOCKPolicy(int *pagesIn);
void WriteFile(int flag, char *strOutput);
int isEmpty(int *buffer);
void initalize(int *buffer);
int isExists(int search, int *buffer);

int main()
{

    char method[7];      // show which policy we use [OPTIMAL,FIFO,LRU,CLOCK]
    char nameOfFile[30]; // File Name of Input
    // int pages[100];
    int i = 0;
    strcpy(outputFileName,"output");

    // printf("Enter The memory size: ");
    scanf("%d",&numberOfPages);

    // printf("Enter The Method: ");
    scanf("%s", method);

    // printf("Enter The pages: ");
    int * pages = malloc(100*sizeof(int));
    do{
        scanf("%d",&pages[i]);
        i++;
        if(i%100 == 0){
            pages = (int*)realloc(pages, 100*sizeof(int));
        }


    }while(pages[i-1]!=-1);

    // scanf("%s", nameOfFile);
    // ReadingFile(nameOfFile, method, pages); //Reading all Information from input file (file name ,method,arrofpageIN)
    strcat(outputFileName,method);
    strcat(outputFileName,".txt\0");

    WriteFile(0, method);

    // Fetching Policy
    if (strcmp("OPTIMAL", method) == 0)
    {
        OPTIMALPolicy(pages);
    }
    else if (strcmp("FIFO", method) == 0)
    {
        FIFOPolicy(pages);
    }
    else if (strcmp("LRU", method) == 0)
    {
        LRUPolicy(pages);
    }
    else if (strcmp("CLOCK", method) == 0)
    {
        CLOCKPolicy(pages);
    }

}


// Read input file and get the fetch policy and the pages that will be fetched

void ReadingFile(char *fileName, char *method, int *pagesIn)
{

    FILE *fptr;
    fptr = fopen(fileName, "r"); // read mode
    int parse;
    int i = 0;
    if (fptr == NULL)
    {
        perror("The Requested File Not Found \n");
        exit(-1);
    }
    fscanf(fptr, "%d", &numberOfPages); // Scan frist integer indicate the pages in memory
    fscanf(fptr, "%s[^\n]", method);    // scan the policy to use
    // printf("%s",method);

    while (!feof(fptr))
    {
        fscanf(fptr, "%d", &pagesIn[i]);
        i++;
    }
    fclose(fptr);
}


void WriteFile(int flag, char *strOutput)
{
    FILE *pFile;
    if (flag == 0)
    {
        pFile = fopen(outputFileName, "w");
        fprintf(pFile, "Replacement Policy = %s\n", strOutput);
        printf("Replacement Policy = %s\n", strOutput);

        fprintf(pFile, "-------------------------------------\n");
        printf("-------------------------------------\n");

        fprintf(pFile, "Page   Content of Frames\n");
        printf("Page   Content of Frames\n");

        fprintf(pFile, "----   -----------------\n");
        printf("----   -----------------\n");
    }
    else if (flag == 1)
    {
        pFile = fopen(outputFileName, "a");
        fprintf(pFile, "%s", strOutput);
        printf("%s",strOutput);
    }
    else
    {
        pFile = fopen(outputFileName, "a");
        fprintf(pFile, "-------------------------------------\n");
        printf("-------------------------------------\n");


        int intValue=atoi(strOutput);
        fprintf(pFile, "Number of page faults = %d\n", intValue);
        printf("Number of page faults = %d\n", intValue);
    }
    fclose(pFile);
}

int isEmpty(int *buffer)
{
    int i = 0;
    for (i = 0; i < numberOfPages; i++)
    {
        if (buffer[i] == -1)
            return i;
    }
    return -1;
}

void initalize(int *buffer)
{
    int i;
    //Initialize the buffer
    for (i = 0; i < numberOfPages; i++)
        buffer[i] = -1;
}

int isExists(int search, int *buffer)
{
    int i = 0;
    for (i = 0; i < numberOfPages; i++)
        if (buffer[i] == search)
            return i;
    return -1;
}


void OPTIMALPolicy(int *pagesIn)
{

    int i = 0, j = 0, k = 0;
    int fault = 0;
    int buffer[numberOfPages];
    int index;
    char output[100];
    initalize(buffer);

    // Policy
    while (pagesIn[i] != -1)
    {
        // make the pagesIn[i] from int to string and put it in output
        // check the next empty space
        sprintf(output, "0%d ", pagesIn[i]);
        index = isEmpty(buffer);

        // Check if the page already exists in the memory
        if (isExists(pagesIn[i], buffer) != -1)
        {
            /*
               Go to the next fetch page
               Write in the file
             */
            i++;
            char bufferstr[10];
            j = 0;
            // no page fault
            strcat(output, "    ");

            // Loop on the buffer to print it in file
            for (j = 0; j < numberOfPages; j++)
            {
                /*
                    if the buffer has empty places neglect it
                    convert the int to string to be easily put in the output string
                    contact the the pageNo  with the buffer
                */
                if (buffer[j] != -1)
                {
                    sprintf(bufferstr, "0%d ", buffer[j]);
                    strcat(output, bufferstr);             // contact the the pageNo  with the buffer
                }
            }
            /*
                Write in the file 1 flag indicate append and the output string to be written
                Go To next Page that need to be feteched
            */
            strcat(output, "\n");
            WriteFile(1, output);
            continue;
        }
        // if there is no Empty Space
        // increase el fault occur
        if (index == -1)
        {
            fault++;
            strcat(output, "F   "); //put in the output line F to indicate there is fault
            j = i + 1;
            k = 0;

            //same size as memory indicate which page that Least used
            int taken[numberOfPages];

           // Loop Until i have found pages equal to size of the memory -1 or i reach the last feteched page
           // Check if this page in the memory return the index inside the memory if found else return -1
            while (pagesIn[j] != -1 && k < numberOfPages - 1)
            {

                index = isExists(pagesIn[j], buffer);

                // if the page found in the memory and this page will be feteched soon
                // indicate that this page will be used near
                if (index != -1 && taken[index] != 1)
                {
                    taken[index] = 1;
                    k++;
                }
                j++;
            }

            // Check which page in the memory that wont be fetech or the least feteched page which have value of 0 in the array taken
            for (k = 0; k < numberOfPages; k++)
            {
                 // if taken[k]!=1 means that this page prority value = 0 then will be replaced
                 // found the index that will be replaced in memory
                if (taken[k] != 1)
                {
                    index = k;
                    break;
                }
            }
        }
        else
        {
            //No Fault there is empty space in memory
            strcat(output, "    ");  // for Printing in File indicate no Fault
        }

        // put the page in the next emptyspace or the next page accorrding to the policy
        buffer[index] = pagesIn[i];
        i++;

        // Printing the whole line in file  pageNo   F/NF    content of Frames
        char bufferstr[10];
        for (j = 0; j < numberOfPages; j++)
        {
            // if the buffer has empty places neglect it
            if (buffer[j] != -1)
            {
                // convert the int to string to be easily put in the output string
                // contact the the pageNo  with the buffer
                sprintf(bufferstr, "0%d ", buffer[j]);
                strcat(output, bufferstr);
            }
        }

        strcat(output, "\n");
        WriteFile(1, output);
    }
    sprintf(output, "%d ", fault);
    WriteFile(3, output); // Write to file the number of faults
}


void FIFOPolicy(int *pagesIn){
    int i = 0, j = 0;
    int buffer[numberOfPages];
    int fault = 0;
    int index;
    char output[100];

    initalize(buffer);

    int *FIFO;
    FIFO = buffer;

    while (pagesIn[i] != -1)
    {
        // make the pagesIn[i] from int to string and put it in output
        sprintf(output, "0%d ", pagesIn[i]);
        index = isEmpty(buffer);

        if (isExists(pagesIn[i], buffer) != -1)
        {
            // Go to the next fetch page
            i++;
            // Write in the file
            char bufferstr[10];
            j = 0;
            strcat(output, "    ");       // no page fault

            // Loop on the buffer to print it in file
            for (j = 0; j < numberOfPages; j++)
            {
                // if the buffer has empty places neglect it
                if (buffer[j] != -1)
                {
                    // convert the int to string to be easily put in the output string
                    // contact the the pageNo  with the buffer
                    sprintf(bufferstr, "0%d ", buffer[j]);
                    strcat(output, bufferstr);
                }
            }

            // Write in the file 1 flag indicate append and the output string to be written
            strcat(output, "\n");
            WriteFile(1, output);
            continue; // Go To next Page that need to be feteched
        }

        // memeory Full and page[i] not page inside the memeory
        if (index == -1)
        {
            // increase el fault occur
            fault++;
            strcat(output, "F   "); //put in the output line F to indicate there is fault
            index = isExists(*FIFO,buffer);
            if(isExists(*FIFO,buffer) == numberOfPages-1){
                FIFO = buffer;
            }else{
                FIFO++;
            }
        }
        else  strcat(output, "    ");   // for Printing in File indicate no Fault



        buffer[index] = pagesIn[i];
        i++;
        char bufferstr[10];
        for (j = 0; j < numberOfPages; j++)
        {
            // if the buffer has empty places neglect it
            if (buffer[j] != -1)
            {
                // convert the int to string to be easily put in the output string
                // contact the the pageNo  with the buffer
                sprintf(bufferstr, "0%d ", buffer[j]);
                strcat(output, bufferstr);
            }
        }
        strcat(output, "\n");
        WriteFile(1, output);
    }

    sprintf(output, "%d ", fault);
    WriteFile(3, output); // Write to file the number of faults
}

void LRUPolicy(int *pagesIn){
    int i = 0, j = 0, k = 0;
    int buffer[numberOfPages];
    int fault = 0;
    int turn=0;
    int index;
    char output[100];
    initalize(buffer);
    while (pagesIn[i] != -1)
    {

        sprintf(output, "0%d ", pagesIn[i]); // make the pagesIn[i] from int to string and put it in output
        index = isEmpty(buffer);

        // Check if the page already exists in the memory
        if (isExists(pagesIn[i], buffer) != -1)
        {
            i++; // Go to the next fetch page

            // Write in the file
            char bufferstr[10];
            j = 0;
            // no page fault
            strcat(output, "    ");

            // Loop on the buffer to print it in file
            for (j = 0; j < numberOfPages; j++)
            {
                // if the buffer has empty places neglect it
                if (buffer[j] != -1)
                {
                    // convert the int to string to be easily put in the output string
                     // contact the the pageNo  with the buffer
                    sprintf(bufferstr, "0%d ", buffer[j]);
                    strcat(output, bufferstr);
                }
            }
            // Write in the file 1 flag indicate append and the output string to be written
            strcat(output, "\n");
            WriteFile(1, output);
            continue; // Go To next Page that need to be feteched
        }

        if (index == -1)
        {
            // increase el fault occur
            fault++;
            strcat(output, "F   "); //put in the output line F to indicate there is fault

            j = i - 1;
            k = 0;
            //same size as memory indicate which page that Least used
            int LRU[numberOfPages];

            // Loop Until i have found pages equal to size of the memory -1 or i reach the last feteched page
            while (j >= 0 && k < numberOfPages - 1)
            {

                index = isExists(pagesIn[j], buffer);  // Check if this page in the memory return the index inside the memory if found else return -1

                // if the page found in the memory and this page will be feteched soon
                // indicate that this page will be least used

                if (index != -1 && LRU[index] != 1)
                {
                    LRU[index] = 1;
                    k++;
                }

                j--;
            }

            // Check which page in the memory that wont be fetech or the least feteched page which have value of 0 in the array LRU
            for (k = 0; k < numberOfPages; k++)
            {
                 // if LRU[k]!=1 means that this page prority value = 0 then will be replaced
                  // found the index that will be replaced in memory
                if (LRU[k] != 1)
                {
                    index = k;
                    break;
                }
            }
        }
        else  strcat(output, "    ");   // for Printing in File indicate no Fault


        // put the page in the next emptyspace or the next page accorrding to the policy
        buffer[index] = pagesIn[i];
        i++;

        // Printing the whole line in file  pageNo   F/NF    content of Frames
        char bufferstr[10];
        for (j = 0; j < numberOfPages; j++)
        {
            // if the buffer has empty places neglect it
            if (buffer[j] != -1)
            {
                 // convert the int to string to be easily put in the output string
                 // contact the the pageNo  with the buffer
                sprintf(bufferstr, "0%d ", buffer[j]);
                strcat(output, bufferstr);
            }
        }

        strcat(output, "\n");
        WriteFile(1, output);
    }

    sprintf(output, "%d ", fault);
    WriteFile(3, output); // Write to file the number of faults
}


void CLOCKPolicy(int *pagesIn){

    int i = 0, j = 0, k = 0;
    int buffer[numberOfPages];
    int fault = 0;
    int index;
    char output[100];

    // CLOCK have 0 or 1 to indicate which element will be rememoved
    int CLOCK[numberOfPages];
    int *clockPtr;

    initalize(buffer);

    // Start Clock on the first element in memory
    clockPtr = buffer;
    while (pagesIn[i] != -1)
    {
        sprintf(output, "0%d ", pagesIn[i]); // make the pagesIn[i] from int to string and put it in output
        index = isEmpty(buffer);
        // Check if the page already exists in the memory
        if (isExists(pagesIn[i], buffer) != -1)
        {
            index =isExists(pagesIn[i], buffer);
            i++;
            CLOCK[index]=1;
            char bufferstr[10];
            j = 0;
            // no page fault
            strcat(output, "    ");
            // Loop on the buffer to print it in file
            for (j = 0; j < numberOfPages; j++)
            {
                // if the buffer has empty places neglect it
                if (buffer[j] != -1)
                {
                    // convert the int to string to be easily put in the output string
                    // contact the the pageNo  with the buffer
                    sprintf(bufferstr, "0%d ", buffer[j]);
                    strcat(output, bufferstr);
                }
            }
            // Write in the file 1 flag indicate append and the output string to be written
            strcat(output, "\n");
            WriteFile(1, output);
            continue; // Go To next Page that need to be feteched
        }

        if (index == -1)
        {
            fault++;                 // increase el fault occur
            strcat(output, "F   "); //put in the output line F to indicate there is fault
            j = i+1;
            k = 0;

            // loop until i reach the numberofpages

            index = isExists(*clockPtr,buffer);
            for (k = 0; k < numberOfPages; k++)
            {
                 // if clock = 0 break loop
                if (CLOCK[index] == 0)
                    break;

                // if i reach last element in the clock cycle start from the first of the memory again
                if(clockPtr == &buffer[numberOfPages-1]){
                    clockPtr = buffer;
                    CLOCK[index]=0;
                    index=0;

                }
                else {
                    clockPtr++;
                    CLOCK[index] = 0;
                    index++;
                }


            }
        }
        else
        {
            //No Fault there is empty space in memory
            strcat(output, "    "); // for Printing in File indicate no Fault
        }

        CLOCK[index]=1;

        // Change clock pointer to the next element
        if(clockPtr == &buffer[numberOfPages-1])
            clockPtr = buffer;
        else
            clockPtr++;


        // put the page in the next emptyspace or the next page accorrding to the policy
        buffer[index] = pagesIn[i];
        i++;
        // Printing the whole line in file  pageNo   F/NF    content of Frames
        char bufferstr[10];
        for (j = 0; j < numberOfPages; j++)
        {
            // if the buffer has empty places neglect it
            if (buffer[j] != -1)
            {

                sprintf(bufferstr, "0%d ", buffer[j]); // convert the int to string to be easily put in the output string
                strcat(output, bufferstr);             // contact the the pageNo  with the buffer
            }
        }
        strcat(output, "\n");
        WriteFile(1, output);
    }
    sprintf(output, "%d ", fault);
    WriteFile(3, output); // Write to file the number of faults

}
