#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>



int path_is_in_dir (const char *path) {  /* To check if a path is a directory*/
    struct stat statbuf;
    if (stat(path, &statbuf) !=0)  {
        return 0;  /* Not valid path*/
    }
    return S_ISDIR(statbuf.st_mode);
}



void filetofile_copy(const char *source, const char *destination) {
/* Copiess the file content from source to destintion*/
/* opens the source file for reading in binary mode */

FILE *src_file = fopen(source, "rb");
if (src_file == NULL)    {
    perror( "Sorry, error in opening source file");
    exit(EXIT_FAILURE);
} 

/* Check if the destination file already ecists*/
FILE *destination_file = fopen(destination, "rb");
if (destination_file != NULL) {
    fclose(destination_file);
    printf("Warning, the destination file %s already exist. Do you wish to overwrite (y/n):", destination);
    char response;
    scanf(" %c", &response); 
    if (response != 'y' && response != 'Y') {
        printf("Sorry, the operation has been canceled for %s.\n",destination);
        fclose(src_file);
        return;
    }
    }

/*Opens the destination file for writing in binary mode*/
destination_file = fopen(destination, "wb");
if (destination_file == NULL) {
    perror("Sorry, there has been an error in openinng/creating the destination file");
    fclose(src_file);
    exit(EXIT_FAILURE);
}

    

/* function for the buffer for reading and writing*/
char buffer[1024];
size_t bytes;

/*Read from the source and then write to destination*/
while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
   if (fwrite (buffer, 1, bytes, destination_file) != bytes) {
    perror("Sorry, there has been an error in writing to the destination file");
    fclose(src_file);
    fclose(destination_file);
    exit(EXIT_FAILURE);
}
}

/* Check to see if an error occured during the reading*/
if (ferror(src_file)) {
    perror("Sorry, there has been an error in reading fr0m the source file");
}


/* CLoses the files after operation*/
fclose(src_file);
fclose(destination_file);
printf( "Sucess! the file has been copied from %s to %s\n ", source, destination);
}



int main (int argc, char *argv[]) {
    /* check if at least the two arguements are provided; source file and destination*/

    if (argc <3) {
        printf("please run the program as: %s <source-file> <target-directory>\n", argv[0]);
        return 1;      /* exits with error code */
    
    }

/* capture the destination arguement*/
const char *destination = argv [argc-1];  /*The last arguemnt is the destination*/

/*Check if the destination is a directory*/
if (!path_is_in_dir(destination)) {
    printf(" There is an Error, The Destination %s is not a valid directory\n", destination);
    return 1;  /*Error Code*/
}

/* Loops over all the source files (the arguments before the last one)*/
for (int i=1; i < argc -1; ++i) {
    const char *source = argv[i];


/* Checks if the source file exist and also if its is a regular file*/
struct stat stat_source;
if (stat(source, & stat_source) !=0 || !S_ISREG(stat_source.st_mode)) {
    printf("There is an error; the source file %s does not exist or is not a regular file.\n", source);
    continue;   /*Then skips to the next file*/
}

/*Creates the full destination path*/
char destination_p[512];
const char *lastSlash = strrchr(source, '/');
snprintf(destination_p,sizeof(destination_p), "%s/%s", destination, lastSlash ? lastSlash +1: source);
    /* data */


/*copy the file*/
filetofile_copy(source, destination_p);
}


return 0; 
}

