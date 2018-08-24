// This is the linked list implementation of patient queue.

#include "LinkedListPatientQueue.h"
#include "VectorPatientQueue.h"
#include "vector.h"
#include "strlib.h"

// Initialize a new empty queue.
LinkedListPatientQueue::LinkedListPatientQueue() : queue(NULL) {}

// Free up any memory used.
LinkedListPatientQueue::~LinkedListPatientQueue() {
    PatientNode* cur;
    while (queue) {
        cur = queue;
        queue = cur->next;
        delete cur;
    }
}

// Return true if your patient queue does not contain any elements and false if it does contain at least one patient.
bool LinkedListPatientQueue::isEmpty() {
    return queue == NULL;
}

// Add the given person into patient queue with the given priority.
// Duplicate names and priorities are allowed.
// Any string is a legal value, and any integer is a legal priority; there are no invalid values that can be passed.
void LinkedListPatientQueue::newPatient(string name, int priority) {
    // create new node.
    PatientNode* newPatient = new PatientNode(name, priority, NULL);
    if (isEmpty()) {
        queue = newPatient;
    } else {
        PatientNode* current = queue;
        // compare with first node.
        if (current->priority > priority) {
            newPatient->next = current;
            queue = newPatient;
        } else {
            // move forward to find the insertion place.
            while ((current->next != NULL) && (current->next->priority <= priority)) {
                current = current->next;
            }
            // insert new node.
            newPatient->next = current->next;
            current->next = newPatient;
        }
    }
}

// Remove all elements from the patient queue, freeing memory for all nodes that are removed.
void LinkedListPatientQueue::clear() {
    PatientNode* cur;
    while (queue) {
        cur = queue;
        queue = cur->next;
        delete cur;
    }
}

// Return the name of the most urgent patient, without removing it or altering the state of the queue.
// Throw a string exception if the queue does not contain any patients.
string LinkedListPatientQueue::frontName() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    return queue->name;
}

// Return the integer priority of the most urgent patient, without removing it or altering the state of the queue.
// Throw a string exception if the queue does not contain any patients.
int LinkedListPatientQueue::frontPriority() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    return queue->priority;
}

// Remove the patient with the most urgent priority from your queue, and return their name as a string.
// Throw a string exception if the queue does not contain any patients.
string LinkedListPatientQueue::processPatient() {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    string name = queue->name;
    queue = queue->next;
    return name;
}

// Modify the priority of a given existing patient in the queue.
// The intent is to change the patient's priority to be more urgent (a smaller integer) than its current value,
// perhaps because the patient's condition has gotten worse.
// If the given patient is present in the queue and already has a more urgent priority than the given new priority,
// or if the given patient is not already in the queue, throw a string exception.
// If the given patient name occurs multiple times in the queue,
// alter the priority of the highest priority person with that name that was placed into the queue.
void LinkedListPatientQueue::upgradePatient(string name, int newPriority) {
    if (isEmpty()) {
        throw ("Exception: no patient in the queue!");
    }
    PatientNode* cur = queue;
    while (cur->name != name && cur->next != NULL) {
        cur = cur->next;
    }
    if (cur->name == name && cur->priority > newPriority) {
        cur->priority = newPriority;
    } else {
        throw ("Exception: cannot find a patient with lower priority in the queue!");
    }
    sortQueue();
}

void LinkedListPatientQueue::sortQueue() {
    PatientNode* cur = queue;
    while (cur != NULL and cur->next != NULL) {
        if (cur->priority > cur->next->priority) {
            string tempName = cur->next->name;
            int tempPriority = cur->next->priority;
            cur->next->name = cur->name;
            cur->next->priority = cur->priority;
            cur->name = tempName;
            cur->priority = tempPriority;
        }
        cur = cur->next;
    }
}

string LinkedListPatientQueue::toString() {
    PatientNode* cur = queue;
    string result = "";
    while (cur != NULL and cur->next != NULL) {
        result = result + integerToString(cur->priority) + ":" +  cur->name + " ";
        cur = cur->next;
    }
    if (cur != NULL) {
        result = result + integerToString(cur->priority) + ":" +  cur->name;
    }
    return "{" + result + "}";
}
