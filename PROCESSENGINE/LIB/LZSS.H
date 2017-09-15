//---------------------------------------------------------------------------
#ifndef LZSSH
#define LZSSH
#include "mystream.h"

#define LZSSN		 4096
#define LZSSF		 18
class LZSS {
unsigned long CurSize;
unsigned long Total;
unsigned char * Buffer;
unsigned long
		textsize,	/* text size counter */
		codesize,	/* code size counter */
		printcount;	/* counter for reporting progress every 1K bytes */
unsigned char
		text_buf[LZSSN + LZSSF - 1];	/* ring buffer of size N,
			with extra F-1 bytes to facilitate string comparison */
long		match_position, match_length,  /* of longest match.  These are
			set by the InsertNode() procedure. */
		lson[LZSSN + 1], rson[LZSSN + 257], dad[LZSSN + 1];  /* left & right children &
			parents -- These constitute binary search trees. */

void InitBuffer();
void InitTree(void);
void InsertNode(long r);
void DeleteNode(long p);
long GetNextChar(const unsigned char * Buf, long Len,long *CurLen);
void PutNextChar(unsigned char * c);
DataBuffer *MakeBuffer();
public:

LZSS();
~LZSS();
DataBuffer * Encode(const unsigned char *Buf,long Len);
DataBuffer * Decode(const unsigned char *Buf,long Len);
};
//---------------------------------------------------------------------------
#endif
