#ifndef AF4EVALMACROS_H
#define AF4EVALMACROS_H

/*
 * Removes copy and assignment operators
 * explicitly from a class definition
 */
#define NO_COPY_ASSIGNMENT_CTORS(THIS_CLASS)\
   THIS_CLASS( const       THIS_CLASS&  src ) = delete;\
   THIS_CLASS& operator= ( THIS_CLASS&  src ) = delete;\
   THIS_CLASS(             THIS_CLASS&& src ) = delete;\
   THIS_CLASS& operator= ( THIS_CLASS&& src ) = delete;
#endif // AF4EVALMACROS_H
