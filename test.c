#include <stdio.h>
#include <string.h>

#include "c_json_stream.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

void basic_test_sample(); /* Write a simple file that uses all of the included routines. */
void test_buffer_write(); /* Write the same data, first writing to a string buffer. */
void test_error_cases(); /* Deliberately induce all currently handled error cases to verify correct handling. */

int main() {
  printf("Testing writing to a file.\n");
  basic_test_sample();
  printf("Complete.\n\n");

  printf("Testing writing to the buffer. (Then writing that to a file)\n");
  test_buffer_write();
  printf("Complete.\n\n");

  printf("Testing handled error cases.\n");
  test_error_cases();
  printf("Complete.\n\n");

  return 0;
}



// Test harness function.
void test_json(int json_result, json_stream_struct *js, const char *expected_error_str) {
  if(!expected_error_str) {
    if(json_result) {
      printf("Got error: %s\n", js->error_string);
    }
  } else {
    if(!json_result) {
      printf("No error reported. Expecting: \"%s\"\n", expected_error_str);
    } else {
      if(strcmp(js->error_string, expected_error_str) != 0) {
        printf("Got: \"%s\", Expecting: \"%s\"\n", js->error_string, expected_error_str);
      } else {
        printf("Correctly reported error: \"%s\"\n", expected_error_str);
      }
    }
  }
}



void basic_test_sample() {
  FILE *out;
  json_stream_struct json_stream;
  json_stream_struct *js;

  /* Helpful if inspecting the object stack. */
  /*printf("KEY: \n");
  printf("JSON_OBJECT:  %d\n", JSON_OBJECT);
  printf("JSON_MEMBER:  %d\n", JSON_MEMBER);
  printf("JSON_PAIR:    %d\n", JSON_PAIR);
  printf("JSON_ARRAY:   %d\n", JSON_ARRAY);
  printf("JSON_ELEMENT: %d\n", JSON_ELEMENT);
  printf("JSON_VALUE:   %d\n", JSON_VALUE);
  printf("JSON_STRING:  %d\n", JSON_STRING);
  printf("JSON_NUMBER:  %d\n", JSON_NUMBER);
  printf("JSON_TRUE:    %d\n", JSON_TRUE);
  printf("JSON_FALSE:   %d\n", JSON_FALSE);
  printf("JSON_NULL:    %d\n", JSON_NULL); */


  /* Test basic functionality of included functions. */
  out = fopen("test.json", "w");

  js = &json_stream;

  json_init_stream(js, true, out);

  test_json(json_start_object(js), js, NULL);

  test_json(json_start_object_named(js, "Gooble"), js, NULL);

  test_json(json_write_pair(js, "Awesome", JSON_STRING, "Possum"), js, NULL);
  test_json(json_write_pair(js, "Answer", JSON_NUMBER, "42"), js, NULL);
  test_json(json_write_pair(js, "Incredible", JSON_TRUE, "ERROR IF YOU SEE THIS"), js, NULL);
  test_json(json_write_pair(js, "Redundant", JSON_FALSE, "ERROR IF YOU SEE THIS"), js, NULL);
  test_json(json_write_pair(js, "DBA Word", JSON_NULL, "ERROR IF YOU SEE THIS"), js, NULL);

  test_json(json_end_context(js), js, NULL);

  test_json(json_write_pair(js, "Another", JSON_STRING, "Element"), js, NULL);

  test_json(json_start_array_named(js, "Arrrr-EH?"), js, NULL);

  test_json(json_start_array(js), js, NULL);

  test_json(json_write_value(js, JSON_STRING, "Possum"), js, NULL);
  test_json(json_write_value(js, JSON_NUMBER, "42"), js, NULL);
  test_json(json_write_value(js, JSON_TRUE, "ERROR IF YOU SEE THIS"), js, NULL);
  test_json(json_write_value(js, JSON_FALSE, "ERROR IF YOU SEE THIS"), js, NULL);
  test_json(json_write_value(js, JSON_NULL, "ERROR IF YOU SEE THIS"), js, NULL);

  test_json(json_end_context(js), js, NULL);

  test_json(json_start_object(js), js, NULL);

  test_json(json_start_object_named(js, "Objectify this"), js, NULL);

  test_json(json_write_pair(js, "Luggage combo", JSON_NUMBER, "12345"), js, NULL);

  test_json(json_end_file(js), js, NULL);
}



void test_buffer_write() {
  FILE *out;
  json_stream_struct json_stream;
  json_stream_struct *js;

  /* Test basic functionality of included functions. */
  out = fopen("test_buffer.json", "w");

  js = &json_stream;

  json_init_stream(js, true, NULL);

  test_json(json_start_object(js), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_start_object_named(js, "Gooble"), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_write_pair(js, "Awesome", JSON_STRING, "Possum"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_pair(js, "Answer", JSON_NUMBER, "42"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_pair(js, "Incredible", JSON_TRUE, "ERROR IF YOU SEE THIS"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_pair(js, "Redundant", JSON_FALSE, "ERROR IF YOU SEE THIS"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_pair(js, "DBA Word", JSON_NULL, "ERROR IF YOU SEE THIS"), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_end_context(js), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_write_pair(js, "Another", JSON_STRING, "Element"), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_start_array_named(js, "Arrrr-EH?"), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_start_array(js), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_write_value(js, JSON_STRING, "Possum"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_value(js, JSON_NUMBER, "42"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_value(js, JSON_TRUE, "ERROR IF YOU SEE THIS"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_value(js, JSON_FALSE, "ERROR IF YOU SEE THIS"), js, NULL);
  fprintf(out, js->stream_buffer);
  test_json(json_write_value(js, JSON_NULL, "ERROR IF YOU SEE THIS"), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_end_context(js), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_start_object(js), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_start_object_named(js, "Objectify this"), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_write_pair(js, "Luggage combo", JSON_NUMBER, "12345"), js, NULL);
  fprintf(out, js->stream_buffer);

  test_json(json_end_file(js), js, NULL);
  fprintf(out, js->stream_buffer);
}



void test_error_cases() {
  json_stream_struct json_stream;
  json_stream_struct *js;

  js = &json_stream;
  json_init_stream(js, true, NULL);

  test_json(json_start_object(js), js, NULL);

  // In an Object 
  printf("\nIn an Object: \n");
  test_json(json_start_object(js), js, "Attempted to open an object when not starting a file or in array context.");
  test_json(json_start_array(js), js, "Attempted to open an array when not starting a file or in array context.");
  test_json(json_write_value(js, JSON_STRING, "nope"), js, "Attempted to print a single value outside an array context.");
  test_json(json_write_pair(js, "nope", 
                            ~(JSON_OBJECT || JSON_MEMBER || JSON_PAIR || JSON_ARRAY || JSON_ELEMENT || JSON_VALUE || 
                            JSON_STRING || JSON_NUMBER || JSON_TRUE || JSON_FALSE || JSON_NULL),
                            "nope"), js, "Attempted to print a name: value pair value of an invalid value type.");
  test_json(json_end_file(js), js, NULL);

  // After finishing a file
  printf("\nAfter finishing a file: \n");
  test_json(json_start_object(js), js, "Attempted to open an object when file is already complete.");
  test_json(json_start_array(js), js, "Attempted to open an array when file is already complete.");
  test_json(json_end_context(js), js, "Attempted to close a context when none is open.");
  test_json(json_write_value(js, JSON_STRING, "nope"), js, "Attempted to print a single value when no context is open.");
  test_json(json_write_pair(js, "nope", JSON_STRING, "nope"), js, "Attempted to print a pair when no context is open.");


  json_init_stream(js, true, NULL);

  // Before starting a file:
  printf("\nBefore starting a file: \n");
  test_json(json_start_object_named(js, "nope"), js, "Attempted to start a named object when no context is open.");
  test_json(json_start_array_named(js, "nope"), js, "Attempted to start a named array when no context is open.");
  test_json(json_end_context(js), js, "Attempted to close a context when none is open.");
  test_json(json_write_value(js, JSON_STRING, "nope"), js, "Attempted to print a single value when no context is open.");

  test_json(json_start_array(js), js, NULL);
  // In an Array
  printf("\nIn an Array: \n");
  test_json(json_start_object_named(js, "nope"), js, "Attempted to open a named object outside an object context.");
  test_json(json_start_array_named(js, "nope"), js, "Attempted to open a named array outside an object context.");
  test_json(json_write_value(js, 
                             ~(JSON_OBJECT || JSON_MEMBER || JSON_PAIR || JSON_ARRAY || JSON_ELEMENT || JSON_VALUE || 
                             JSON_STRING || JSON_NUMBER || JSON_TRUE || JSON_FALSE || JSON_NULL),
                             "nope"), js, "Attempted to print a single value of an invalid type.");
  test_json(json_write_pair(js, "nope", JSON_STRING, "nope"), js, "Attempted to print a name: value pair outside an object context.");

  test_json(json_end_file(js), js, NULL);
}