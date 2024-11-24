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
