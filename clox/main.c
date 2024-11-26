//
//  main.c
//  clox
//
//  Created by Olaifa Darasimi on 23/08/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

void repl(void) {
    char line[1024];
    
    for (;;) {
        printf("> ");
        
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        
        interpret(line);
    }
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    
    // scan the whole file to the end
    // TODO: handle error
    fseek(file, 0L, SEEK_END);
    
    // calculate the size by reading number of bytes scanned from the beginning of the file
    // TODO: handle error
    size_t fileSize = ftell(file);
    
    // rewind the handle to the beginning of file
    // TODO: handle error
    rewind(file);
    
    char* buffer = (char*) malloc(fileSize + 1); // an extra byte for the null byte signalling EOF
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    
    buffer[bytesRead] = '\0'; // ALWAYS! allocate the null byte
    
    fclose(file);
    return buffer;
}

static void runFile(const char* path) {
    char* source = readFile(path);
    
    InterpretResult result = interpret(source);
    free(source);
    
    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}


int main(int argc, const char * argv[]) {
    // insert code here...
    initVM();

    Chunk chunk;
    initChunk(&chunk);
    
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
    }
    
    freeVM();
    freeChunk(&chunk);

    return 0;
}
