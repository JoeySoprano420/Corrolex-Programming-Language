struct RefCountedObject {
    void* data;
    int ref_count;
};

void increase_ref_count(struct RefCountedObject* obj) {
    obj->ref_count++;
}

void decrease_ref_count(struct RefCountedObject* obj) {
    obj->ref_count--;
    if (obj->ref_count == 0) {
        free(obj->data);
        free(obj);
    }
}


if (operand1_is_constant && operand2_is_constant) {
    // Perform the operation at compile time
    result = operand1 + operand2;
}

void report_error(const char* message, const char* token) {
    fprintf(stderr, "Error: %s, near '%s'.\n", message, token);
}

// Example of generating simple WebAssembly code for an addition
void generate_addition_code(const char* result, const char* operand1, const char* operand2) {
    printf("(local.set %s (i32.add (local.get %s) (local.get %s)))\n", result, operand1, operand2);
}
#include <pthread.h>

void* thread_function(void* arg) {
    printf("Thread started\n");
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);
    return 0;
}

