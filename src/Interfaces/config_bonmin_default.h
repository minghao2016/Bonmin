
/***************************************************************************/
/*           HERE DEFINE THE PROJECT SPECIFIC PUBLIC MACROS                */
/*    These are only in effect in a setting that doesn't use configure     */
/***************************************************************************/

/* Version number of project */
#define BONMIN_VERSION  "trunk"

/* Major Version number of project */
#define BONMIN_VERSION_MAJOR   9999

/* Minor Version number of project */
#define BONMIN_VERSION_MINOR   9999

/* Release Version number of project */
#define BONMIN_VERSION_RELEASE 9999

#ifndef BONMINLIB_EXPORT
#if defined(_WIN32) && defined(DLL_EXPORT)
#define BONMINLIB_EXPORT __declspec(dllimport)
#else
#define BONMINLIB_EXPORT
#endif
#endif
