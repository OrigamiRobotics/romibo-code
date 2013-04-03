/* This file defines a class meant for setting up and executing function calls on a timeline. */
#ifndef __TIMELINE_H_INCLUDED__
#define __TIMELINE_H_INCLUDED__

#include <ElapsedTimer.h>

class Timeline
{


protected:
// Timelines are stored in a linked list

  typedef struct timelineNode_t {
    int time; //the time at which this node is executed in milliseconds
    void (*argfun) (void *); // a pointer to the function to call, which takes a void pointer as an argument
	void (*arglessfun) (void); // a pointer to the function to call if there is no argument
    void *arg; // a pointer to the argument to send, or null
    timelineNode_t *next;
  } timelineNode;


  ElapsedTimer timer;
  timelineNode *timelineHead;
  timelineNode *timelineNext;
  
  void insertNode (timelineNode *newnode);
  
  void executeNode (timelineNode *node);
   
public:
  //default constructor, used to create a new timeline
  Timeline();
  
  //add an entry to the timeline, with time in ms, and fun a pointer 
  //to the function (of no arguments) to be called at time
  void addEntry (int time, void (*fun) ());
  
  //add an entry to the time line, with time in ms, fun a pointer to
  //a function taking a void * arg, with arg being the argument.
  void addEntry (int time, void (*argfun) (void *), void *arg);
  
  // add the final entry, which signals timeline end
  //void endEntry (int time); //TODO: implement
  
  // Removes all timeline entries.
  void resetEntries ();
  
  // resets the timer. Only necessary if iterateOnce will be called.
  void resetTimer ();
  
  // Checks the timer once and executes the next event if applicable.
  void iterateOnce ();
  
  // Runs through the entire timeline sequentially
  void executeSequentially ();
  
  // Runs through the entire timeline as part of the polling loop. 
  // Note that this will modify the polling callback
  void executeInBackground ();
  
  // Runs through the entire timeline as part of the polling loop.
  // Once the timeline is complete, the timer will reset and the
  // timeline will loop until stopExecution() is called
  void loopInBackground ();
  
  // Stops execution of the timeline if it is currently executing
  // as part of the polling loop
  void stopExecution ();
  
  // frees all dynamically allocated memory associated with the timeline
  // Call this function when done with the timeline
  void destroy ();

};



#endif