/*

c_json_stream.c

Simple streaming JSON write library. Contains minimal procedures to support the
production of a JSON-formatted data stream.

This library is closely patterned off the javax.json.stream JsonGenerator 
Interface. This is intended to serve the same purpose; a tool exists solely  
to easily write JSON-formatted data. 

This library enforces the structural constraints of JSON (objects, arrays, members, 
pairs, elements), but does not enforce JSON string or number formatting rules. 

*/

#include <stdio.h>
#include <string.h>
#include "c_json_stream.h"



/* Helper function to initialize a stream tracking object. */
void json_init_stream(json_stream_struct *js, int human_readable, FILE *out_file) {
  js->human_readable = human_readable;
  strcpy(js->indent_token, "  ");
  js->file_started = 0;
  js->prior_element = JSON_NULL; /* Indicates no prior element in object, array, or file. */
  js->stack_depth = 0; /* Stack depth counting starts at 1. */
  js->out = out_file;
  strcpy(js->error_string, "");
  js->string_sanitize_fn = NULL;
};



/* Utility function to print human-readable indentation. */
void js_newline(json_stream_struct *js) {
  /* If this is the start of a new file, do not preceed with a newline. */
  if(js->file_started != 0 && js->human_readable != 0) {
    fprintf(js->out, "\n");
  }
}



/* Utility function to print human-readable indentation. */
void do_indent(json_stream_struct *js) {
  int ii;
  if(js->file_started != 0 && js->human_readable != 0) {
    for(ii = 0; ii < js->stack_depth; ++ii) {
      fprintf(js->out, js->indent_token);
    }
  }
}



/* Utility function to print a comma when needed and record that an element has 
   been printed inside the current context. */
void new_element(json_stream_struct *js) {
  /* Element names from the JSON structure are abused here because they make
     readable labels. */

  /* JSON_ELEMENT indicates that this is not the first element of an object or array. */
  if(js->prior_element == JSON_ELEMENT) {
    fprintf(js->out, ",");
  } else { /* JSON_NULL indicates that there was not a prior element. */
    js->prior_element = JSON_ELEMENT;
  }
  js_newline(js);
}



/* Utility function. Permit a callback for string sanitization to be defined. 
   NULL by default. (TODO: Are there C/C++ issues here?) */
/*void (*string_sanitize_fn)(char *str) = NULL;*/

void sanitize_string(json_stream_struct *js, char *str) {
  if(js->string_sanitize_fn) {
    (*(js->string_sanitize_fn))(str);
  }
}



/* Valid JSON must begin with an object or an array.  */

/* Start a brace-enclosed object. 
   An object may only begin in an array context or when no context has yet been started. (The beginning of a file) */
int json_start_object(json_stream_struct *js) {
  if(js->file_started != 0) {
    if(js->stack_depth <= 0 || js->object_array_stack[js->stack_depth - 1] != JSON_ARRAY) {
      strcpy(js->error_string, "Attempted to open an object when not starting a file or in array context.");
      return -1;
    } 
  } else {
    if(js->prior_element != JSON_NULL) {
      strcpy(js->error_string, "Attempted to open an object when file is already complete.");
      return -1;
    }
  }

  new_element(js); /* Print a comma if this follows a previous element. */

  /* Indent and print the open brace. */
  do_indent(js); 
  fprintf(js->out, "{");

  /* Record that a new object has been opened. */
  js->prior_element = JSON_NULL;
  js->stack_depth++;
  js->object_array_stack[js->stack_depth - 1] = JSON_OBJECT;
  js->file_started = 1;

  return 0;
};


  
/* Start an named object. (A name: value pair where the value is a new object.)
   Must be in an object or array context. */
int json_start_object_named(json_stream_struct *js, char *name) {
  if(js->stack_depth <= 0) {
    strcpy(js->error_string, "Attempted to start a named object when no context is open.");
    return -1;
  }

  if(js->object_array_stack[js->stack_depth - 1] != JSON_OBJECT) {
    strcpy(js->error_string, "Attempted to open a named object outside an object context.");
    return -1;
  }

  new_element(js); /* Print a comma if this follows a previous element. */

  /* Indent and print the name and open brace. */
  do_indent(js); 
  sanitize_string(js, name);
  fprintf(js->out, "\"%s\": {", name);

  /* Record that a new object has been opened. */
  js->prior_element = JSON_NULL;
  js->stack_depth++;
  js->object_array_stack[js->stack_depth - 1] = JSON_OBJECT;

  return 0;
};



/* Start a bracket-enclosed array.
   An array may only begin in an array context or when no context has yet been started. (The beginning of a file) */
int json_start_array(json_stream_struct *js) {
  if(js->file_started != 0) {
    if(js->stack_depth <= 0 || js->object_array_stack[js->stack_depth - 1] != JSON_ARRAY) {
      strcpy(js->error_string, "Attempted to open an array when not starting a file or in array context.");
      return -1;
    } 
  } else {
    if(js->prior_element != JSON_NULL) {
      strcpy(js->error_string, "Attempted to open an array when file is already complete.");
      return -1;
    }
  }

  new_element(js); /* Print a comma if this follows a previous element. */

  /* Indent and print the open bracket. */
  do_indent(js); 
  fprintf(js->out, "[");

  /* Record that a new array has been opened. */
  js->prior_element = JSON_NULL;
  js->stack_depth++;
  js->object_array_stack[js->stack_depth - 1] = JSON_ARRAY;
  js->file_started = 1;

  return 0;
};



/* Start an named array. (A name: value pair where the value is an array.)
   Must be in an object context. */
int json_start_array_named(json_stream_struct *js, char *name) {
  if(js->stack_depth <= 0) {
    strcpy(js->error_string, "Attempted to start a named array without a top-level object.");
    return -1;
  }

  if(js->object_array_stack[js->stack_depth - 1] != JSON_OBJECT) {
    strcpy(js->error_string, "Attempted to open a named array outside an object context.");
    return -1;
  }

  new_element(js); /* Print a comma if this follows a previous element. */

  /* Indent and print the name and open bracket. */
  do_indent(js); 
  sanitize_string(js, name);
  fprintf(js->out, "\"%s\": [", name);

  /* Record that a new array has been opened. */
  js->prior_element = JSON_NULL;
  js->stack_depth++;
  js->object_array_stack[js->stack_depth - 1] = JSON_ARRAY;

  return 0;
};


  
/* Close an array or object. */
int json_end_context(json_stream_struct *js) {
  JSON_TYPE open_context;

  if(js->stack_depth <= 0) {
    strcpy(js->error_string, "Attempted to close a context when none is open.");
    return -1;
  }

  open_context = js->object_array_stack[js->stack_depth - 1];

  /*printf("DEBUG open_context %d\n", open_context);*/

  js_newline(js);

  js->stack_depth--; /* Record that the object has been closed. */

  do_indent(js);

  if(open_context == JSON_OBJECT) {
    fprintf(js->out, "}");
  }
  if(open_context == JSON_ARRAY) {
    fprintf(js->out, "]");
  }
  
  /* The newly closed element is an element in its parent context. */
  js->prior_element = JSON_ELEMENT;

  return 0;
};


  
/* Write a singleton value. Must be in an array context.
   String values will have enclosing quotes added. 
   Number values should be serialized as a character array prior to passing. */
int json_write_value(json_stream_struct *js, JSON_TYPE value_type, char *value) {
  if(js->stack_depth <= 0) {
    strcpy(js->error_string, "Attempted to print a single value when no contect is open.");
    return -1;
  }

  if(js->object_array_stack[js->stack_depth - 1] != JSON_ARRAY) {
    strcpy(js->error_string, "Attempted to print a single value outside an array context.");
    return -1;
  }

  new_element(js); /* Print a comma if this follows a previous element. */
  do_indent(js); 

  switch(value_type) {
  case JSON_STRING:
    sanitize_string(js, value);
    fprintf(js->out, "\"%s\"", value);
    break;
  case JSON_NUMBER:
    sanitize_string(js, value);
    fprintf(js->out, "%s", value);
    break;
  case JSON_TRUE:
    fprintf(js->out, "true");
    break;
  case JSON_FALSE:
    fprintf(js->out, "false");
    break;
  case JSON_NULL:
    fprintf(js->out, "null");
    break;
  default:
    strcpy(js->error_string, "Attempted to print a single value of an invalid type.");
    return -1;
  }
  return 0;
};


  
/* Write a name: value pair. Must be in an object context.
   String values will have enclosing quotes added. 
   Number values should be serialized as a character array prior to passing. */
int json_write_pair(json_stream_struct *js, char *name, JSON_TYPE value_type, char *value) {
  if(js->stack_depth <= 0) {
    strcpy(js->error_string, "Attempted to print a pair when no context is open.");
    return -1;
  }

  if(js->object_array_stack[js->stack_depth - 1] != JSON_OBJECT) {
    strcpy(js->error_string, "Attempted to print a name: value pair outside an object context.");
    return -1;
  }

  new_element(js); /* Print a comma if this follows a previous element. */
  do_indent(js); 

  switch(value_type) {
  case JSON_STRING:
    sanitize_string(js, name);
    sanitize_string(js, value);
    fprintf(js->out, "\"%s\": \"%s\"", name, value);
    break;
  case JSON_NUMBER:
    sanitize_string(js, name);
    sanitize_string(js, value);
    fprintf(js->out, "\"%s\": %s", name, value);
    break;
  case JSON_TRUE:
    fprintf(js->out, "\"%s\": true", name);
    break;
  case JSON_FALSE:
    fprintf(js->out, "\"%s\": false", name);
    break;
  case JSON_NULL:
    fprintf(js->out, "\"%s\": null", name);
    break;
  default:
    strcpy(js->error_string, "Attempted to print a name: value pair value of an invalid value type.");
    return -1;
  }

  return 0;
};



/* Close all open objects and arrays, terminating the file. */
int json_end_file(json_stream_struct *js) {
  int status;

  /*printf("DEBUG stack depth %d\n", js->stack_depth);*/
  if(js->stack_depth <= 0) {
    return 0; /* File is empty or already closed. Ignore. */
  }

  while(js->stack_depth > 0) {
    status = json_end_context(js);
    if(status) return status;
  }

  return 0;
};



