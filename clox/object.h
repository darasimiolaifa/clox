//
//  object.h
//  clox
//
//  Created by Olaifa Darasimi on 10/09/2024.
//

#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "table.h"
#include "chunk.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

#define IS_STRING(value)        isObjType(value, OBJ_STRING)
#define IS_NATIVE(value)        isObjType(value, OBJ_NATIVE)
#define IS_FUNCTION(value)      isObjType(value, OBJ_FUNCTION)
#define IS_CLOSURE(value)       isObjType(value, OBJ_CLOSURE)
#define IS_CLASS(value)         isObjType(value, OBJ_CLASS)
#define IS_INSTANCE(value)      isObjType(value, OBJ_INSTANCE)
#define IS_BOUND_METHOD(value)  isObjType(value, OBJ_BOUND_METHOD)

#define AS_FUNCTION(value)      ((ObjFunction*)AS_OBJ(value))
#define AS_CLOSURE(value)       ((ObjClosure*)AS_OBJ(value))
#define AS_CLASS(value)         ((ObjClass*) AS_OBJ(value))
#define AS_INSTANCE(value)      ((ObjInstance*) AS_OBJ(value))
#define AS_BOUND_METHOD(value)  ((ObjBoundMethod*) AS_OBJ(value))
#define AS_NATIVE(value)        (((ObjNative*)AS_OBJ(value))->function)
#define AS_STRING(value)        ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)       (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION,
    OBJ_INSTANCE,
    OBJ_CLASS,
    OBJ_BOUND_METHOD,
    OBJ_CLOSURE,
    OBJ_UPVALUE,
    OBJ_NATIVE
} ObjType;

struct Obj {
    ObjType type;
    bool isMarked;
    struct Obj* next;
};

typedef struct {
    Obj obj;
    int arity;
    int upvalueCount;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

typedef struct {
    Obj obj;
    Value* location;
    Value closed;
    struct ObjUpvalue* next;
} ObjUpvalue;

typedef struct {
    Obj obj;
    ObjFunction* function;
    ObjUpvalue** upvalues;
    int upvalueCount;
} ObjClosure;

typedef struct {
    Obj obj;
    ObjString* name;
    Table methods;
} ObjClass;

typedef struct {
    Obj obj;
    ObjClass* klass;
    Table fields;
} ObjInstance;

typedef struct {
    Obj obj;
    Value receiver;
    ObjClosure* method;
} ObjBoundMethod;

ObjClass* newClass(ObjString* name);
ObjInstance* newInstance(ObjClass* klass);
ObjBoundMethod* newBoundMethod(Value receiver, ObjClosure* method);
ObjClosure* newClosure(ObjFunction* function);

typedef Value (*NativeFn)(int argCount, Value* args);

typedef struct {
    Obj obj;
    NativeFn function;
} ObjNative;

struct ObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

ObjFunction* newFunction(void);
ObjNative* newNative(NativeFn function);
ObjString* copyString(const char* chars, int length);
ObjUpvalue* newUpvalue(Value* slot);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

void printObject(Value value);
ObjString* takeString(char* chars, int length);

#endif /* object_h */
