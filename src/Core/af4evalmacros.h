#ifndef AF4EVALMACROS_H
#define AF4EVALMACROS_H

/*
 * Removes the copy constructor and the
 * copy assignment operator explicitly
 * from a class definition
 */
#define NO_COPY_CTOR(THIS_CLASS)\
   THIS_CLASS(             THIS_CLASS& src ) = delete;\
   THIS_CLASS& operator= ( THIS_CLASS& src ) = delete;

/*
 * Removes the move constructor and the
 * move assignment operator explicitly
 * from a class definition
 */
#define NO_MOVE_CTOR(THIS_CLASS)\
   THIS_CLASS(             THIS_CLASS&& src ) = delete;\
   THIS_CLASS& operator= ( THIS_CLASS&& src ) = delete;

/*
 * Removes the copy constructor, the move constructor
 * and their associated assignment operators
 * explicitly from a class definition
 */
#define NO_COPY_MOVE_CTORS(THIS_CLASS)\
   THIS_CLASS( const       THIS_CLASS&  src ) = delete;\
   THIS_CLASS& operator= ( THIS_CLASS&  src ) = delete;\
   THIS_CLASS(             THIS_CLASS&& src ) = delete;\
   THIS_CLASS& operator= ( THIS_CLASS&& src ) = delete;

#endif // AF4EVALMACROS_H
