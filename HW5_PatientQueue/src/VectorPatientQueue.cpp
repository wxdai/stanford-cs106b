// This is the vector implementation of priority queue.

#include "VectorPatientQueue.h"
#include "vector.h"
#include "strlib.h"

// Initialize a new empty queue.
VectorPatientQueue::VectorPatientQueue() : currentTime(0) {}

// Free up any memory used.
VectorPatientQueue::~VectorPatientQueue() {}

// Add the given person into patient queue with the given priority.
// Duplicate names and priorities are allowed.
// Any string is a legal value, and any integer is a legal priority; there are no invalid values that can be passed.
void VectorPatientQueue::newPatient(string name, int priority) {
    patient p;
    p.name = name;
    p.priority = priority;
    p.arrivalTime = currentTime;
    queue.add(p);
    currentTime++;
}

// Remove the patient with the most urgent priority from your queue, and return their name as a string.
// Throw a string exception if the queue does not contain any patients.
string VectorPatientQueue::processPatient() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    patient nextP = queue[0];
    int nextId = 0;
    for (int i = 0; i < queue.size(); i++) {
        if ((nextP.priority > queue[i].priority) ||
            ((nextP.priority == queue[i].priority) && (nextP.arrivalTime > queue[i].arrivalTime))) {
            nextP = queue[i];
            nextId = i;
        }
    }
    queue.remove(nextId);
    return nextP.name;
}

// Return the name of the most urgent patient, without removing it or altering the state of the queue.
// Throw a string exception if the queue does not contain any patients.
string VectorPatientQueue::frontName() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    patient nextP = queue[0];
    for (int i = 0; i < queue.size(); i++) {
        if ((nextP.priority > queue[i].priority) ||
            ((nextP.priority == queue[i].priority) && (nextP.arrivalTime > queue[i].arrivalTime))) {
            nextP = queue[i];
        }
    }
    return nextP.name;
}

// Return the integer priority of the most urgent patient, without removing it or altering the state of the queue.
// Throw a string exception if the queue does not contain any patients.
int VectorPatientQueue::frontPriority() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    patient nextP = queue[0];
    for (int i = 0; i < queue.size(); i++) {
        if ((nextP.priority > queue[i].priority) ||
            ((nextP.priority == queue[i].priority) && (nextP.arrivalTime > queue[i].arrivalTime))) {
            nextP = queue[i];
        }
    }
    return nextP.priority;
}

// Modify the priority of a given existing patient in the queue.
// The intent is to change the patient's priority to be more urgent (a smaller integer) than its current value,
// perhaps because the patient's condition has gotten worse.
// If the given patient is present in the queue and already has a more urgent priority than the given new priority,
// or if the given patient is not already in the queue, throw a string exception.
// If the given patient name occurs multiple times in the queue,
// alter the priority of the highest priority person with that name that was placed into the queue.
void VectorPatientQueue::upgradePatient(string name, int newPriority) {
    int patientId = -1;
    int curPriority = numeric_limits<int>::max();
    for (int i = 0; i < queue.size(); i++) {
        if (queue[i].name == name && queue[i].priority > newPriority && queue[i].priority < curPriority) {
            patientId = i;
            curPriority = queue[i].priority;
        }
    }
    if (patientId == -1) {
        throw ("Exception: cannot find a patient with lower priority in the queue!");
    }
    queue[patientId].priority = newPriority;
}

// Return true if your patient queue does not contain any elements and false if it does contain at least one patient.
bool VectorPatientQueue::isEmpty() {
    return queue.isEmpty();
}

// Remove all elements from the patient queue, freeing memory for all nodes that are removed.
void VectorPatientQueue::clear() {
    queue.clear();
}

string VectorPatientQueue::toString() {
    string result = "";
    for (int i = 0; i < queue.size(); i++) {
        result += integerToString(queue[i].priority);
        result += ":";
        result += queue[i].name;
        if (i < queue.size() -1) {
            result += " ";
        }
    }

    return "{" + result + "}";
}
