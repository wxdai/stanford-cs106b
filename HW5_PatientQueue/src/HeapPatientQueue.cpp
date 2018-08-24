// heappatientqueue.cpp
// THIS IS AN EXTENSION AND NOT REQUIRED FOR THE ASSIGNMENT
// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "HeapPatientQueue.h"
#include "VectorPatientQueue.h"
#include "vector.h"
#include "strlib.h"

// Initialize a new empty queue.
HeapPatientQueue::HeapPatientQueue() : capacity(4), size(0), queue(nullptr) {
    // new returns a pointer.
    // patient() initializes a default struct.
    queue = new patient [capacity];
}

// Free up any memory used.
HeapPatientQueue::~HeapPatientQueue() {
    delete [] queue;
}

// Remove all elements from the patient queue, freeing memory for all nodes that are removed.
void HeapPatientQueue::clear() {
    patient* temp = queue;
    delete [] temp;
    capacity = 4;
    size = 0;
    queue = new patient [capacity];
}

// Return the name of the most urgent patient, without removing it or altering the state of the queue.
// Throw a string exception if the queue does not contain any patients.
string HeapPatientQueue::frontName() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    return queue[1].name;
}

// Return the integer priority of the most urgent patient, without removing it or altering the state of the queue.
// Throw a string exception if the queue does not contain any patients.
int HeapPatientQueue::frontPriority() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    return queue[1].priority;
}

// Return true if your patient queue does not contain any elements and false if it does contain at least one patient.
bool HeapPatientQueue::isEmpty() {
    return size == 0;
}

// Add the given person into patient queue with the given priority.
// Duplicate names and priorities are allowed.
// Any string is a legal value, and any integer is a legal priority; there are no invalid values that can be passed.
void HeapPatientQueue::newPatient(string name, int priority) {
    patient p;
    p.name = name;
    p.priority = priority;
    // Add new patient to the end of queue.
    size += 1;
    if (size == capacity) {
        // resize and copy the old one to the new one.
        capacity *= 2;
        patient* temp = queue;
        queue = new patient [capacity];
        for (int i = 1; i < size; i++) {
            queue[i] = temp[i];
        }
        delete [] temp;
    }
    queue[size] = p;

    // bubble up last element to find the right place.
    bubbleUp(size);
}

void HeapPatientQueue::bubbleUp(const int input_idx) {
    int idx = input_idx;
    while (idx / 2 > 0) {
        if (queue[idx/2].priority > queue[idx].priority ||
            (queue[idx/2].priority == queue[idx].priority &&
             queue[idx/2].name > queue[idx].name)) {
            patient pt = queue[idx];
            queue[idx] = queue[idx/2];
            queue[idx/2] = pt;
            idx = idx / 2;
        } else {
            break;
        }
    }
}

// Remove the patient with the most urgent priority from your queue, and return their name as a string.
// Throw a string exception if the queue does not contain any patients.
string HeapPatientQueue::processPatient() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    string name = queue[1].name;
    queue[1] = queue[size];
    size = size - 1;
    int idx = 1;
    int idxToComp;
    patient pt;
    while (2 * idx <= size) {
        if (2 * idx + 1 <= size &&
           (queue[2*idx].priority > queue[2*idx+1].priority ||
            (queue[2*idx].priority == queue[2*idx+1].priority &&
             queue[2*idx].name > queue[2*idx+1].name))) {
            idxToComp = 2 * idx + 1;
        } else {
            idxToComp = 2 * idx;
        }
        if (queue[idx].priority > queue[idxToComp].priority ||
            (queue[idx].priority == queue[idxToComp].priority &&
             queue[idx].name > queue[idxToComp].name)) {
            pt = queue[idxToComp];
            queue[idxToComp] = queue[idx];
            queue[idx] = pt;
            idx = idxToComp;
        } else {
            break;
        }
    }
    return name;
}

// Modify the priority of a given existing patient in the queue.
// The intent is to change the patient's priority to be more urgent (a smaller integer) than its current value,
// perhaps because the patient's condition has gotten worse.
// If the given patient is present in the queue and already has a more urgent priority than the given new priority,
// or if the given patient is not already in the queue, throw a string exception.
// If the given patient name occurs multiple times in the queue,
// alter the priority of the highest priority person with that name that was placed into the queue.
void HeapPatientQueue::upgradePatient(string name, int newPriority) {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    int minIdx = 0;
    int minPriority = numeric_limits<int>::max();
    for (int i = 1; i <= size; i++) {
        if (queue[i].name == name &&
            queue[i].priority > newPriority &&
            queue[i].priority < minPriority) {
            minIdx = i;
        }
    }
    if (minIdx == 0) {
        throw ("Exception: cannot find a patient with lower priority in the queue!");
    } else {
        queue[minIdx].priority = newPriority;
    }
    bubbleUp(minIdx);
}

string HeapPatientQueue::toString() {
    string result;
    for (int i = 1; i <= size; i++) {
        result = result + integerToString(queue[i].priority) + ":" + queue[i].name;
        if (i < size) {
            result += " ";
        }
    }
    return "{" + result + "}";
}
