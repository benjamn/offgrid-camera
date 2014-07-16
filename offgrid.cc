#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <node.h>
#include <v8.h>

using namespace v8;

Handle<Value> Test(const Arguments& args) {
    return String::New("oyez");
}

static void cleanup(void *arg) {}

void init(Handle<Object> target) {
    node::AtExit(cleanup, NULL);
    NODE_SET_METHOD(target, "test", Test);
}

NODE_MODULE(offgrid, init);
