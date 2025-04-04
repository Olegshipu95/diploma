#ifndef SHIP_JVM_ATTR_INFO_H
#define SHIP_JVM_ATTR_INFO_H

#include <stdint.h>
#include "classfile.h"


/* Verification type tags */
#define ITEM_Top 0
#define ITEM_Integer 1
#define ITEM_Float 2
#define ITEM_Long 4
#define ITEM_Double 3
#define ITEM_Null 5
#define ITEM_UninitializedThis 6
#define ITEM_Object 7
#define ITEM_Uninitialized 8

/* Stack Map Frame type constants */
#define SAME_FRAME_MIN 0
#define SAME_FRAME_MAX 63
#define SAME_LOCALS_1_STACK_ITEM_MIN 64
#define SAME_LOCALS_1_STACK_ITEM_MAX 127
#define SAME_LOCALS_1_STACK_ITEM_EXTENDED 247
#define CHOP_FRAME_MIN 248
#define CHOP_FRAME_MAX 250
#define SAME_FRAME_EXTENDED 251
#define APPEND_FRAME_MIN 252
#define APPEND_FRAME_MAX 254
#define FULL_FRAME 255

/* Parameter access flags */
#define ACC_FINAL        0x0010
#define ACC_SYNTHETIC    0x1000
#define ACC_MANDATED     0x8000

/* Inner class access flags */
#define ACC_PUBLIC      0x0001
#define ACC_PRIVATE     0x0002
#define ACC_PROTECTED   0x0004
#define ACC_STATIC      0x0008
#define ACC_FINAL       0x0010
#define ACC_INTERFACE   0x0200
#define ACC_ABSTRACT    0x0400
#define ACC_SYNTHETIC   0x1000
#define ACC_ANNOTATION  0x2000
#define ACC_ENUM        0x4000

#define ATTRIBUTE_ConstantValue 0
#define ATTRIBUTE_Code 1
#define ATTRIBUTE_StackMapTable 2
#define ATTRIBUTE_Exceptions 3
#define ATTRIBUTE_InnerClasses 4
#define ATTRIBUTE_EnclosingMethod 5
#define ATTRIBUTE_Synthetic 6
#define ATTRIBUTE_Signature 7
#define ATTRIBUTE_SourceFile 8
#define ATTRIBUTE_SourceDebugExtension 9
#define ATTRIBUTE_LineNumberTable 10
#define ATTRIBUTE_LocalVariableTable 11
#define ATTRIBUTE_LocalVariableTypeTable 12
#define ATTRIBUTE_Deprecated 13
#define ATTRIBUTE_RuntimeVisibleAnnotations 14
#define ATTRIBUTE_RuntimeInvisibleAnnotations 15
#define ATTRIBUTE_RuntimeVisibleParameterAnnotations 16
#define ATTRIBUTE_RuntimeInvisibleParameterAnnotations 17
#define ATTRIBUTE_RuntimeVisibleTypeAnnotations 18
#define ATTRIBUTE_RuntimeInvisibleTypeAnnotations 19
#define ATTRIBUTE_AnnotationDefault 20
#define ATTRIBUTE_BootstrapMethods 21
#define ATTRIBUTE_MethodParameters 22

#define ATTRIBUTE_INVALID 99


// All attributes

struct constant_value_attribute;
struct Code_attribute;
struct StackMapTable_attribute;
struct BootstrapMethods_attribute;
struct NestHost_attribute;
struct NestMembers_attribute;
struct PermittedSubclasses_attribute;
struct Exceptions_attribute;
struct InnerClasses_attribute;
struct EnclosingMethod_attribute;
struct Synthetic_attribute;
struct Signature_attribute;
struct Record_attribute;
struct SourceFile_attribute;
struct LineNumberTable_attribute;
struct LocalVariableTable_attribute;
struct LocalVariableTypeTable_attribute;
struct SourceDebugExtension_attribute;
struct Deprecated_attribute;
struct RuntimeVisibleAnnotations_attribute;
struct RuntimeInvisibleAnnotations_attribute;
struct RuntimeVisibleParameterAnnotations_attribute;
struct RuntimeInvisibleParameterAnnotations_attribute;
struct RuntimeVisibleTypeAnnotations;
struct RuntimeInvisibleTypeAnnotations_attribute;
struct AnnotationDefault_attribute;
struct MethodParameters_attribute;
struct Module_attribute;
struct ModulePackages_attribute;
struct ModuleMainClass_attribute;

struct attribute_info {
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint8_t *info;  // size = attribute_length
};

struct constant_value_attribute {
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t constant_value_index;
};

struct Code_attribute {
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t max_stack;
  uint16_t max_locals;
  uint32_t code_length;
  uint8_t* code; /* массив размера code_length */
  uint16_t exception_table_length;
  struct {
      uint16_t start_pc;
      uint16_t end_pc;
      uint16_t handler_pc;
      uint16_t catch_type;
  }* exception_table; /* массив размера exception_table_length */
  uint16_t attributes_count;
  struct attribute_info* attributes; /* массив размера attributes_count */
};


/**
 * Union representing verification type info in StackMapTable
 * Uses tag byte to determine which type is actually stored
 */
union verification_type_info {
  struct {
      uint8_t tag; /* = ITEM_Top (0) */
  } Top_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_Integer (1) */
  } Integer_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_Float (2) */
  } Float_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_Long (4) */
  } Long_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_Double (3) */
  } Double_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_Null (5) */
  } Null_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_UninitializedThis (6) */
  } UninitializedThis_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_Object (7) */
      uint16_t cpool_index; /* Constant pool index */
  } Object_variable_info;
  
  struct {
      uint8_t tag; /* = ITEM_Uninitialized (8) */
      uint16_t offset; /* Bytecode offset */
  } Uninitialized_variable_info;
};

/**
 * Union representing different stack map frame types
 */
union stack_map_frame {

  /**
   * Frame type where the frame has exactly the same locals as the previous frame
   * and the operand stack is empty (frame_type = 0-63)
   */
  struct {
      uint8_t frame_type; /* SAME (0-63) */
  } same_frame;

  /**
   * Frame type where the frame has exactly the same locals as the previous frame
   * and the operand stack has one entry (frame_type = 64-127)
   */
  struct {
      uint8_t frame_type; /* SAME_LOCALS_1_STACK_ITEM (64-127) */
      union verification_type_info stack[1];
  } same_locals_1_stack_item_frame;

  /**
   * Frame type where the frame has exactly the same locals as the previous frame
   * except one additional verification type on the stack, with explicit offset delta
   */
  struct {
      uint8_t frame_type; /* SAME_LOCALS_1_STACK_ITEM_EXTENDED (247) */
      uint16_t offset_delta;
      union verification_type_info stack[1];
  } same_locals_1_stack_item_frame_extended;
  
  /**
   * Frame type where the frame has the same locals as the previous frame
   * except that the last k locals are absent (frame_type = 248-250)
   */
  struct {
      uint8_t frame_type; /* CHOP (248-250) */
      uint16_t offset_delta;
  } chop_frame;

  /**
   * Frame type that has exactly the same locals as the previous frame and
   * the operand stack is empty, with explicit offset delta
   */
  struct {
      uint8_t frame_type; /* SAME_FRAME_EXTENDED (251) */
      uint16_t offset_delta;
  } same_frame_extended;

  /**
   * Frame type where the frame has the same locals as the previous frame
   * except that k additional locals are defined (frame_type = 252-254)
   */
  struct {
      uint8_t frame_type; /* APPEND (252-254) */
      uint16_t offset_delta;
      union verification_type_info* locals; /* size = frame_type - 251 */
  } append_frame;

  /**
   * Complete frame data with explicit locals and stack info
   */
  struct {
      uint8_t frame_type; /* FULL_FRAME (255) */
      uint16_t offset_delta;
      uint16_t number_of_locals;
      union verification_type_info* locals; /* size = number_of_locals */
      uint16_t number_of_stack_items;
      union verification_type_info* stack; /* size = number_of_stack_items */
  } full_frame;
};

/**
 * StackMapTable attribute structure as defined in JVM specification
 */
struct StackMapTable_attribute {
  /**
   * Index into the constant pool table pointing to
   * the UTF8 string "StackMapTable"
   */
  uint16_t attribute_name_index;
  
  /**
   * Length of the attribute excluding initial 6 bytes
   * (name_index + length fields)
   */
  uint32_t attribute_length;
  
  /**
   * Number of stack map frame entries in this attribute
   */
  uint16_t number_of_entries;
  
  /**
   * Array of stack map frames describing the state of
   * the local variables and operand stack at specific
   * bytecode offsets
   */
  union stack_map_frame* entries; /* array of size number_of_entries */
};

/**
 * BootstrapMethods attribute structure
 * Used for invokedynamic instruction support
 */
struct BootstrapMethods_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "BootstrapMethods"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute excluding the initial 6 bytes
   */
  uint32_t attribute_length;

  /**
   * Number of bootstrap methods in this attribute
   */
  uint16_t num_bootstrap_methods;

  /**
   * Array of bootstrap method entries
   */
  struct {
      /**
       * Index into the constant pool of a CONSTANT_MethodHandle
       * representing the bootstrap method
       */
      uint16_t bootstrap_method_ref;

      /**
       * Number of arguments for this bootstrap method
       */
      uint16_t num_bootstrap_arguments;

      /**
       * Array of constant pool indices (each a CONSTANT_* entry)
       * representing the bootstrap arguments
       */
      uint16_t* bootstrap_arguments; /* array of size num_bootstrap_arguments */
  }* bootstrap_methods; /* array of size num_bootstrap_methods */
};

/**
 * NestHost attribute structure (Java 11+)
 * Indicates which class is the host of the nest to which the current class belongs
 */
struct NestHost_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "NestHost"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (must be 2)
   */
  uint32_t attribute_length;

  /**
   * Index into the constant pool of a CONSTANT_Class_info
   * representing the nest host class
   */
  uint16_t host_class_index;
};

/**
 * NestMembers attribute structure (Java 11+)
 * Lists all classes that are members of the nest hosted by the current class
 */
struct NestMembers_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "NestMembers"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (2 + 2*number_of_classes)
   */
  uint32_t attribute_length;

  /**
   * Number of classes in the nest
   */
  uint16_t number_of_classes;

  /**
   * Array of constant pool indices (each a CONSTANT_Class_info)
   * representing the nest member classes
   */
  uint16_t* classes; /* array of size number_of_classes */
};

/**
 * PermittedSubclasses attribute structure (Java 17+)
 * Specifies the allowed direct subclasses of a sealed class
 */
struct PermittedSubclasses_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "PermittedSubclasses"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (2 + 2*number_of_classes)
   */
  uint32_t attribute_length;

  /**
   * Number of permitted subclasses
   */
  uint16_t number_of_classes;

  /**
   * Array of constant pool indices (each a CONSTANT_Class_info)
   * representing the permitted subclasses
   */
  uint16_t* classes; /* array of size number_of_classes */
};

/**
 * Exceptions attribute structure
 * Lists checked exceptions that may be thrown by a method
 */
struct Exceptions_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "Exceptions"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (2 + 2*number_of_exceptions)
   */
  uint32_t attribute_length;

  /**
   * Number of exceptions in the exception table
   */
  uint16_t number_of_exceptions;

  /**
   * Array of constant pool indices (each a CONSTANT_Class_info)
   * representing the exception classes
   */
  uint16_t* exception_index_table; /* array of size number_of_exceptions */
};

/**
 * InnerClasses attribute structure
 * Contains information about inner classes of a class
 */
struct InnerClasses_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "InnerClasses"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (2 + 8*number_of_classes)
   */
  uint32_t attribute_length;

  /**
   * Number of entries in the classes table
   */
  uint16_t number_of_classes;

  /**
   * Array of inner class entries
   */
  struct {
      /**
       * Index into the constant pool of a CONSTANT_Class_info
       * representing the inner class
       */
      uint16_t inner_class_info_index;

      /**
       * Index into the constant pool of a CONSTANT_Class_info
       * representing the outer class (0 if not member)
       */
      uint16_t outer_class_info_index;

      /**
       * Index into the constant pool of a CONSTANT_Utf8_info
       * representing the original name of the inner class (0 if anonymous)
       */
      uint16_t inner_name_index;

      /**
       * Access flags of the inner class (ACC_PUBLIC, ACC_PRIVATE, etc.)
       */
      uint16_t inner_class_access_flags;
  }* classes; /* array of size number_of_classes */
};

/**
 * EnclosingMethod attribute structure
 * Indicates the immediately enclosing method of a nested class
 */
struct EnclosingMethod_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "EnclosingMethod"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (must be 4)
   */
  uint32_t attribute_length;

  /**
   * Index into the constant pool of a CONSTANT_Class_info
   * representing the enclosing class
   */
  uint16_t class_index;

  /**
   * Index into the constant pool of a CONSTANT_NameAndType_info
   * representing the enclosing method (0 if class is not enclosed by a method)
   */
  uint16_t method_index;
};

/**
 * Synthetic attribute structure
 * Marks synthetic elements generated by the compiler
 */
struct Synthetic_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "Synthetic"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (must be 0)
   */
  uint32_t attribute_length;
};

/**
 * Signature attribute structure
 * Stores generic type information for classes, methods, and fields
 */
struct Signature_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "Signature"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (must be 2)
   */
  uint32_t attribute_length;

  /**
   * Index into the constant pool of a CONSTANT_Utf8_info
   * containing the generic signature string
   */
  uint16_t signature_index;
};

/**
 * Record component information structure
 * Represents a single component in a record declaration
 */
struct record_component_info {
  /**
   * Index into the constant pool of a CONSTANT_Utf8_info
   * representing the component name
   */
  uint16_t name_index;

  /**
   * Index into the constant pool of a CONSTANT_Utf8_info
   * representing the component descriptor
   */
  uint16_t descriptor_index;

  /**
   * Number of attributes for this component
   */
  uint16_t attributes_count;

  /**
   * Array of attributes associated with this component
   */
  struct attribute_info* attributes; /* array of size attributes_count */
};

/**
* Record attribute structure (Java 14+)
* Contains metadata about record class components
*/
struct Record_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "Record"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute excluding the initial 6 bytes
   */
  uint32_t attribute_length;

  /**
   * Number of components in this record
   */
  uint16_t components_count;

  /**
   * Array of record component information structures
   */
  struct record_component_info* components; /* array of size components_count */
};

/**
 * SourceFile attribute structure
 * Records the source file from which this class was compiled
 */
struct SourceFile_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "SourceFile"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (must be 2)
   */
  uint32_t attribute_length;

  /**
   * Index into the constant pool of a CONSTANT_Utf8_info
   * representing the source file name
   */
  uint16_t sourcefile_index;
};

/**
 * LineNumberTable attribute structure
 * Maps bytecode offsets to source code line numbers
 */
struct LineNumberTable_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "LineNumberTable"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of entries in the line number table
   */
  uint16_t line_number_table_length;

  /**
   * Array of line number entries
   */
  struct {
      /**
       * Bytecode offset (program counter) where this line starts
       */
      uint16_t start_pc;

      /**
       * Corresponding source file line number
       */
      uint16_t line_number;
  }* line_number_table; /* array of size line_number_table_length */
};

/**
 * LocalVariableTable attribute structure
 * Stores debug information about local variables
 */
struct LocalVariableTable_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "LocalVariableTable"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of entries in the local variable table
   */
  uint16_t local_variable_table_length;

  /**
   * Array of local variable entries
   */
  struct {
      /**
       * Bytecode offset where variable scope begins
       */
      uint16_t start_pc;

      /**
       * Length of variable scope (in bytes)
       */
      uint16_t length;

      /**
       * Index into the constant pool of a CONSTANT_Utf8_info
       * representing the variable name
       */
      uint16_t name_index;

      /**
       * Index into the constant pool of a CONSTANT_Utf8_info
       * representing the variable descriptor
       */
      uint16_t descriptor_index;

      /**
       * Index in the local variable array of this variable
       */
      uint16_t index;
  }* local_variable_table; /* array of size local_variable_table_length */
};

/**
 * LocalVariableTypeTable attribute structure
 * Stores generic type information for local variables
 */
struct LocalVariableTypeTable_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "LocalVariableTypeTable"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of entries in the local variable type table
   */
  uint16_t local_variable_type_table_length;

  /**
   * Array of local variable type entries
   */
  struct {
      /**
       * Bytecode offset where variable scope begins
       */
      uint16_t start_pc;

      /**
       * Length of variable scope (in bytes)
       */
      uint16_t length;

      /**
       * Index into the constant pool of a CONSTANT_Utf8_info
       * representing the variable name
       */
      uint16_t name_index;

      /**
       * Index into the constant pool of a CONSTANT_Utf8_info
       * representing the generic signature
       */
      uint16_t signature_index;

      /**
       * Index in the local variable array of this variable
       */
      uint16_t index;
  }* local_variable_type_table; /* array of size local_variable_type_table_length */
};

/**
 * SourceDebugExtension attribute structure
 * Contains extended debugging information in implementation-defined format
 */
struct SourceDebugExtension_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "SourceDebugExtension"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the debug extension data
   */
  uint32_t attribute_length;

  /**
   * Array of bytes containing implementation-specific debug information
   * (typically SMAP data for JSR-045)
   */
  uint8_t* debug_extension; /* array of size attribute_length */
};

/**
 * Deprecated attribute structure
 * Marks deprecated class, field, or method
 */
struct Deprecated_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "Deprecated"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (must be 0)
   */
  uint32_t attribute_length;
};

struct annotation;

/**
 * Element_value structure
 * Represents the value of an annotation element
 */
struct element_value {
  /**
   * Tag indicating the type of this element value
   * Possible values:
   *   'B', 'C', 'D', 'F', 'I', 'J', 'S', 'Z' - primitive
   *   's' - String
   *   'e' - enum constant
   *   'c' - Class
   *   '@' - Annotation
   *   '[' - Array
   */
  uint8_t tag;

  /**
   * Union of possible value types
   */
  union {
      /**
       * For primitive and String values (tags 'B','C','D','F','I','J','S','Z','s')
       * Index into the constant pool of appropriate type
       */
      uint16_t const_value_index;

      /**
       * For enum constants (tag 'e')
       */
      struct {
          uint16_t type_name_index;  /* Index of enum type descriptor */
          uint16_t const_name_index; /* Index of enum constant name */
      } enum_const_value;

      /**
       * For Class values (tag 'c')
       * Index into the constant pool of class descriptor
       */
      uint16_t class_info_index;

      /**
       * For nested annotations (tag '@')
       */
      struct annotation* annotation_value;

      /**
       * For arrays (tag '[')
       */
      struct {
          uint16_t num_values;              /* Number of elements */
          struct element_value* values;     /* Array of element values */
      } array_value;
  } value;
};

/**
* Annotation structure
* Represents a single annotation
*/
struct annotation {
  /**
   * Index into the constant pool of a CONSTANT_Utf8_info
   * representing the annotation type descriptor
   */
  uint16_t type_index;

  /**
   * Number of element-value pairs
   */
  uint16_t num_element_value_pairs;

  /**
   * Array of annotation element-value pairs
   */
  struct {
      /**
       * Index into the constant pool of a CONSTANT_Utf8_info
       * representing the element name
       */
      uint16_t element_name_index;

      /**
       * The element value
       */
      struct element_value value;
  }* element_value_pairs; /* array of size num_element_value_pairs */
};



/**
 * RuntimeVisibleAnnotations attribute structure
 * Stores annotations that should be visible at runtime
 */
struct RuntimeVisibleAnnotations_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "RuntimeVisibleAnnotations"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of annotations
   */
  uint16_t num_annotations;

  /**
   * Array of annotation structures
   */
  struct annotation* annotations; /* array of size num_annotations */
};

/**
 * RuntimeInvisibleAnnotations attribute structure
 * Stores annotations that should not be visible at runtime
 */
struct RuntimeInvisibleAnnotations_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "RuntimeInvisibleAnnotations"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of annotations
   */
  uint16_t num_annotations;

  /**
   * Array of annotation structures
   */
  struct annotation* annotations; /* array of size num_annotations */
};

/**
 * RuntimeVisibleParameterAnnotations attribute structure
 * Stores parameter annotations visible at runtime
 */
struct RuntimeVisibleParameterAnnotations_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "RuntimeVisibleParameterAnnotations"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of method parameters
   */
  uint8_t num_parameters;

  /**
   * Array of parameter annotation collections
   */
  struct {
      /**
       * Number of annotations on this parameter
       */
      uint16_t num_annotations;

      /**
       * Array of annotations for this parameter
       */
      struct annotation* annotations; /* array of size num_annotations */
  }* parameter_annotations; /* array of size num_parameters */
};

/**
 * Type parameter target (for class/method type parameters)
 */
struct type_parameter_target {
  uint8_t type_parameter_index;  /* 0-based type parameter index */
};

/**
* Supertype target (for extends/implements clauses)
*/
struct supertype_target {
  uint16_t supertype_index;      /* Index in interfaces/superclass list */
};

/**
* Type parameter bound target
*/
struct type_parameter_bound_target {
  uint8_t type_parameter_index;  /* 0-based type parameter index */
  uint8_t bound_index;           /* 0-based bound index */
};

/**
* Empty target (for field declarations, method return types, etc.)
*/
struct empty_target {
  /* No additional data */
};

/**
* Formal parameter target
*/
struct formal_parameter_target {
  uint8_t formal_parameter_index; /* 0-based parameter index */
};

/**
* Throws target
*/
struct throws_target {
  uint16_t throws_type_index;    /* Index in exception table */
};

/**
* Local variable target
*/
struct localvar_target {
  uint16_t table_length;         /* Number of scope entries */
  struct {
      uint16_t start_pc;        /* Start of scope (bytecode offset) */
      uint16_t length;          /* Scope length */
      uint16_t index;           /* Local variable index */
  }* table;                     /* Array of scope entries */
};

/**
* Catch target (exception handler)
*/
struct catch_target {
  uint16_t exception_table_index; /* Index in exception table */
};

/**
* Offset target (various bytecode offsets)
*/
struct offset_target {
  uint16_t offset;               /* Bytecode offset */
};

/**
* Type argument target (generic type arguments)
*/
struct type_argument_target {
  uint16_t offset;               /* Bytecode offset */
  uint8_t type_argument_index;   /* 0-based type argument index */
};

/**
* Type path (path to nested type)
*/
struct type_path {
  uint8_t path_length;           /* Number of path entries */
  struct {
      uint8_t type_path_kind;    /* Path step kind (0=array,1=inner,etc) */
      uint8_t type_argument_index; /* For parameterized types */
  }* path;                       /* Array of path steps */
};


/**
 * RuntimeInvisibleParameterAnnotations attribute structure
 * Stores parameter annotations that should not be visible at runtime
 */
struct RuntimeInvisibleParameterAnnotations_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "RuntimeInvisibleParameterAnnotations"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of method parameters
   */
  uint8_t num_parameters;

  /**
   * Array of parameter annotation collections
   */
  struct {
      /**
       * Number of annotations on this parameter
       */
      uint16_t num_annotations;

      /**
       * Array of annotations for this parameter
       */
      struct annotation* annotations; /* array of size num_annotations */
  }* parameter_annotations; /* array of size num_parameters */
};


/**
 * Type annotation structure
 * Extended annotation format for Java type annotations
 */
struct type_annotation {
  /**
   * Type of target being annotated (item 17.1 in JVMS)
   * Possible values:
   * 0x00-0x01: Type parameter declarations
   * 0x10: Class extends/implements
   * 0x11-0x12: Type parameter bounds
   * 0x13: Field declaration
   * 0x14-0x16: Method return/receiver/parameters
   * 0x17: Throws clause
   * etc.
   */
  uint8_t target_type;

  /**
   * Target-specific information
   */
  union {
      struct type_parameter_target type_parameter;
      struct supertype_target supertype;
      struct type_parameter_bound_target type_parameter_bound;
      struct empty_target empty;
      struct formal_parameter_target formal_parameter;
      struct throws_target throws;
      struct localvar_target localvar;
      struct catch_target catch;
      struct offset_target offset;
      struct type_argument_target type_argument;
  } target_info;

  /**
   * Path to the exact annotated type in a compound type
   */
  struct type_path target_path;

  /**
   * Index into the constant pool of the annotation type
   */
  uint16_t type_index;

  /**
   * Number of element-value pairs
   */
  uint16_t num_element_value_pairs;

  /**
   * Array of annotation element-value pairs
   */
  struct {
      /**
       * Index into the constant pool of element name
       */
      uint16_t element_name_index;

      /**
       * The element value
       */
      struct element_value value;
  }* element_value_pairs; /* array of size num_element_value_pairs */
};

/**
 * RuntimeVisibleTypeAnnotations attribute structure
 * Stores type annotations that should be visible at runtime
 */
struct RuntimeVisibleTypeAnnotations_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "RuntimeVisibleTypeAnnotations"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of type annotations
   */
  uint16_t num_annotations;

  /**
   * Array of type annotation structures
   */
  struct type_annotation* annotations; /* array of size num_annotations */
};

/**
 * RuntimeInvisibleTypeAnnotations attribute structure
 * Stores type annotations that should not be visible at runtime
 */
struct RuntimeInvisibleTypeAnnotations_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "RuntimeInvisibleTypeAnnotations"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (excluding initial 6 bytes)
   */
  uint32_t attribute_length;

  /**
   * Number of type annotations
   */
  uint16_t num_annotations;

  /**
   * Array of type annotation structures
   */
  struct type_annotation* annotations; /* array of size num_annotations */
};

/**
 * AnnotationDefault attribute structure
 * Specifies default value for an annotation type element
 */
struct AnnotationDefault_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "AnnotationDefault"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (depends on default_value size)
   */
  uint32_t attribute_length;

  /**
   * The default value for this annotation element
   */
  struct element_value default_value;
};

/**
 * MethodParameters attribute structure
 * Contains metadata about method parameters
 */
struct MethodParameters_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "MethodParameters"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (1 + 4*parameters_count)
   */
  uint32_t attribute_length;

  /**
   * Number of parameter entries
   */
  uint8_t parameters_count;

  /**
   * Array of parameter entries
   */
  struct {
      /**
       * Index into the constant pool of a CONSTANT_Utf8_info
       * representing the parameter name (0 if unnamed)
       */
      uint16_t name_index;

      /**
       * Parameter access flags (e.g., ACC_FINAL, ACC_SYNTHETIC, ACC_MANDATED)
       */
      uint16_t access_flags;
  }* parameters; /* array of size parameters_count */
};

/**
 * Module attribute structure (Java 9+)
 * Contains module definition metadata
 */
struct Module_attribute {
  /* Attribute header */
  uint16_t attribute_name_index;  /* Points to "Module" in constant pool */
  uint32_t attribute_length;      /* Total attribute size excluding header */

  /* Module basic info */
  uint16_t module_name_index;     /* CONSTANT_Module_info index */
  uint16_t module_flags;          /* ACC_OPEN, ACC_SYNTHETIC, etc. */
  uint16_t module_version_index;  /* CONSTANT_Utf8_info index (0 if none) */

  /* Requires section */
  uint16_t requires_count;
  struct {
      uint16_t requires_index;       /* CONSTANT_Module_info index */
      uint16_t requires_flags;       /* ACC_TRANSITIVE, etc. */
      uint16_t requires_version_index; /* CONSTANT_Utf8_info (0 if none) */
  }* requires; /* array of size requires_count */

  /* Exports section */
  uint16_t exports_count;
  struct {
      uint16_t exports_index;         /* CONSTANT_Package_info index */
      uint16_t exports_flags;         /* ACC_SYNTHETIC, etc. */
      uint16_t exports_to_count;
      uint16_t* exports_to_index;     /* CONSTANT_Module_info indices */
  }* exports; /* array of size exports_count */

  /* Opens section */
  uint16_t opens_count;
  struct {
      uint16_t opens_index;           /* CONSTANT_Package_info index */
      uint16_t opens_flags;           /* ACC_SYNTHETIC */
      uint16_t opens_to_count;
      uint16_t* opens_to_index;       /* CONSTANT_Module_info indices */
  }* opens; /* array of size opens_count */

  /* Uses section */
  uint16_t uses_count;
  uint16_t* uses_index;               /* CONSTANT_Class_info indices */

  /* Provides section */
  uint16_t provides_count;
  struct {
      uint16_t provides_index;        /* CONSTANT_Class_info index */
      uint16_t provides_with_count;
      uint16_t* provides_with_index;  /* CONSTANT_Class_info indices */
  }* provides; /* array of size provides_count */
};

/**
 * ModulePackages attribute structure (Java 9+)
 * Lists all packages owned by this module
 */
struct ModulePackages_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "ModulePackages"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (2 + 2*package_count)
   */
  uint32_t attribute_length;

  /**
   * Number of packages in this module
   */
  uint16_t package_count;

  /**
   * Array of constant pool indices (each a CONSTANT_Package_info)
   * representing the packages in this module
   */
  uint16_t* package_index; /* array of size package_count */
};

/**
 * ModuleMainClass attribute structure (Java 9+)
 * Specifies the main class for executable modules
 */
struct ModuleMainClass_attribute {
  /**
   * Index into the constant pool pointing to
   * the UTF8 string "ModuleMainClass"
   */
  uint16_t attribute_name_index;

  /**
   * Length of the attribute (must be 2)
   */
  uint32_t attribute_length;

  /**
   * Index into the constant pool of a CONSTANT_Class_info
   * representing the main class
   */
  uint16_t main_class_index;
};

#endif