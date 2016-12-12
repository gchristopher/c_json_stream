#include <stdio.h>

#include "c_json_stream.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


void test_json(int json_result, json_stream_struct *js) {
  if(json_result) {
    printf("Got error: %s\n", js->error_string);
  }
}

int main() {
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

  test_json(json_start_object(js), js);

  test_json(json_start_object_named(js, "Gooble"), js);

  test_json(json_write_pair(js, "Awesome", JSON_STRING, "Possum"), js);
  test_json(json_write_pair(js, "Answer", JSON_NUMBER, "42"), js);
  test_json(json_write_pair(js, "Incredible", JSON_TRUE, "ERROR IF YOU SEE THIS"), js);
  test_json(json_write_pair(js, "Redundant", JSON_FALSE, "ERROR IF YOU SEE THIS"), js);
  test_json(json_write_pair(js, "DBA Word", JSON_NULL, "ERROR IF YOU SEE THIS"), js);

  test_json(json_end_context(js), js);

  test_json(json_write_pair(js, "Another", JSON_STRING, "Element"), js);

  test_json(json_start_array_named(js, "Arrrr-EH?"), js);

  test_json(json_start_array(js), js);

  test_json(json_write_value(js, JSON_STRING, "Possum"), js);
  test_json(json_write_value(js, JSON_NUMBER, "42"), js);
  test_json(json_write_value(js, JSON_TRUE, "ERROR IF YOU SEE THIS"), js);
  test_json(json_write_value(js, JSON_FALSE, "ERROR IF YOU SEE THIS"), js);
  test_json(json_write_value(js, JSON_NULL, "ERROR IF YOU SEE THIS"), js);

  test_json(json_end_context(js), js);

  test_json(json_start_object(js), js);

  test_json(json_start_object_named(js, "Objectify this"), js);

  test_json(json_write_pair(js, "Luggage combo", JSON_NUMBER, "12345"), js);

  test_json(json_end_file(js), js);

  /* TODO: Verification tests of all handled error cases. */

  return 0;
}