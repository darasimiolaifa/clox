//
//  compiler.h
//  clox
//
//  Created by Olaifa Darasimi on 24/08/2024.
//

#ifndef clox_compiler_h
#define clox_compiler_h

#include "vm.h"
#include "object.h"

ObjFunction* compile(const char* source);
void markCompilerRoots(void);
#endif /* compiler_h */
