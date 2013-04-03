
#include <RomiboRobot.h>
#include <Timeline.h>


// constructor
 Timeline::Timeline()
{
  timer = ElapsedTimer ();
  timelineHead = 0;
  timelineNext = 0;
}

void Timeline::insertNode (timelineNode *newnode)
{
  if (timelineHead == 0) {
    timelineHead = newnode;
	return;
  }
  timelineNode *prev = 0;
  timelineNode *curr = timelineHead;
  while (curr != 0) {
    if (curr->time < newnode->time) {
	  if (prev == 0) {
	    newnode->next = curr;
		timelineHead = newnode;
		return;
	  } else {
	    newnode->next = prev->next;
	    prev->next = newnode;
		return;
	  }
	}
	prev = curr;
	curr = curr->next;
  }
}

void Timeline::addEntry (int time, void (*fun) ())
{
  timelineNode *newnode = new timelineNode;
  newnode->time = time;
  newnode->arglessfun = fun;
  newnode->arg = 0;
  newnode->next = 0;
  
  insertNode (newnode);
}

void Timeline::addEntry (int time, void (*argfun) (void *), void *arg) {
  timelineNode * newnode = new timelineNode;
  newnode->time = time;
  newnode->arg = arg;
  newnode->argfun = argfun;
  newnode->next = 0;
  
  insertNode (newnode);
}

void Timeline::resetEntries ()
{
  timelineNode *curr = timelineHead;
  timelineNode *tmp;
  while (curr != 0) {
    tmp = curr->next;
	delete curr;
	curr = tmp;
  }
  timelineHead = 0;
}

void Timeline::resetTimer ()
{
  timer.reset ();
  timelineNext = timelineHead;
}

void Timeline::executeNode (timelineNode *node)
{
  if (node->arg == 0) {
    node->arglessfun();
  } else {
    node->argfun (node->arg);
  }
}

void Timeline::iterateOnce ()
{

  while ((0 != timelineNext)
  && (timelineNext->time < timer.elapsedMilliseconds ()))
  {
    executeNode (timelineNext);
	timelineNext = timelineNext->next;
  }
}

void Timeline::executeSequentially ()
{
  
}
