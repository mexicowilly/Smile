#if !defined(SMILE_FINALIZE_HPP__)
#define SMILE_FINALIZE_HPP__

/**
 * @file 
 * Smile's finalization functionality
 */

/**
 * The Smile namespace
 */
namespace smile
{

/**
 * Finalize the Smile library. This function should be called
 * if you believe your memory leak analysis tools when they tell 
 * you that static objects that live the life of the application 
 * are memory leaks. You should also use this function if you 
 * are using Smile as a shared object or within an enclosing
 * shared object. 
 *  
 * @note No Smile APIs may be used after calling this function.
 *       The result is extremely undefined.
 */
void finalize();

}

#endif
