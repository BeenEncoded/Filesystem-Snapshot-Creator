#ifndef GLOBAL_DEFINES_H_INCLUDED
#define GLOBAL_DEFINES_H_INCLUDED
#include "fsysclass.h"

/* Character sets: */
#define LETTERS "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"
#define NUMBERS "1234567890"
#define SPECIALS "`~!@#$%^&*()_-+={[}]\\|;:\'\",<.>/? "

/* String cancel return value: */
#define GSTRING_CANCEL "erhfbnmco4vi7whjdmcdjrsfekhj84928hg"

/* --------------------------  KEY CODES ---------------------------------------
 */

//single key-codes:
#define ENTER_KEY 13
#define BACKSPACE_KEY 8
#define ESCAPE_KEY 27

//double key-codes
#define F1_KEY 59
#define F2_KEY 60
#define F3_KEY 61
#define F4_KEY 62
#define F5_KEY 63
#define DELETE_KEY 83
#define UP_KEY 72
#define DOWN_KEY 80
#define RIGHT_KEY 77
#define LEFT_KEY 75

//------------------------------------------------------------------------------

//set this to turn color-enabled program on/off
#define USECOLOR true

/* Display defines */
#define HCENTER 40
#define VCENTER 6

//------------------------------------------------------------------------------

#define SNAPSHOT_FILE (fsys_class().gpath() + "\\Your Snapshots.snap")

/* File stream defines.  These can be used to easily change the way the streams
 are opened.*/
#define INFILE in
#define OUTFILE out

typedef unsigned long long id_type;


#endif