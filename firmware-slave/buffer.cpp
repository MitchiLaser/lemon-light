#include "config.h"
#include "Arduino.h"

#include "buffer.h"

// Implementation of the buffer
// It only supports basic Operations: pop(), push() and empty()
// the whole buffer works as a FIFO


// class which contains the implementation of a single node in the buffer
// similar to the implementation of a linked list

BNode::BNode( unsigned char d ) {
  Data = d; // when a new node is initialised it has to have a value
  Next = NULL; // no next
}

BNode::~BNode( void ) { // it seems like theres no special destructor needed

}

unsigned char BNode::getData( void ) {
  return Data;
}

void BNode::setData( unsigned char d) {
  Data = d;
}

BNode * BNode::getNext( void ) {
  return Next;
}

void BNode::setNext( BNode *New ) {
  Next = New;
}



// class which contains the implementation of the buffer

Buffer::Buffer() { // contructor: create empty buffer
  front = back = NULL;
}

Buffer::~Buffer() { // destructor: delete buffer and all values
  while (front != NULL) { // while there are some elements in the queue: remove them all
    unsigned char _ = this->pop();
  }
}

void Buffer::push( unsigned char Data ) { // add a new entry in the queue
  BNode *temp = new BNode( Data ); // create new node

  if (back == NULL) { // if queue is empty than the new element is front and back both
    front = back = temp;
    return;
  }

  // add the new node at the end of the queue
  back->setNext( temp );
  back = temp;
}

unsigned char Buffer::pop( void ) { // remove entry from the queue and return its value
  if ( front == NULL) // if queue is empty there is nothing to delete
    return 0;

  BNode *temp = front;
  front = front->getNext();

  if (front == NULL) // if front is NULL, back has to become NULL as well
    back = NULL;

  return temp->getData();
}

// return true if queue is empty
bool Buffer::empty( void ) {
  if ( front == NULL ) { // if the front-pointer refers to no element then the queue has to be empty
    return true;

  } else {
    return false;

  }
}
