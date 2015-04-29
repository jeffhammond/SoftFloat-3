
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014 The Regents of the University of California
(Regents).  All Rights Reserved.  Redistribution and use in source and binary
forms, with or without modification, are permitted provided that the following
conditions are met:

Redistributions of source code must retain the above copyright notice,
this list of conditions, and the following two paragraphs of disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions, and the following two paragraphs of disclaimer in the
documentation and/or other materials provided with the distribution.  Neither
the name of the Regents nor the names of its contributors may be used to
endorse or promote products derived from this software without specific prior
written permission.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS".  REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "internals.h"

void
 softfloat_normRoundPackMToF128M(
     bool sign, int32_t exp, uint32_t *extSigPtr, uint32_t *zWPtr )
{
    const uint32_t *ptr;
    int_fast16_t shiftCount;
    uint32_t wordSig;

    ptr = extSigPtr + indexWordHi( 5 );
    shiftCount = 0;
    for (;;) {
        wordSig = *ptr;
        if ( wordSig ) break;
        shiftCount += 32;
        if ( 160 <= shiftCount ) {
            zWPtr[indexWordHi( 4 )] = packToF128UI96( sign, 0, 0 );
            zWPtr[indexWord( 4, 2 )] = 0;
            zWPtr[indexWord( 4, 1 )] = 0;
            zWPtr[indexWord( 4, 0 )] = 0;
            return;
        }
        ptr -= wordIncr;
    }
    shiftCount += softfloat_countLeadingZeros32( wordSig ) - 15;
    if ( shiftCount ) {
        exp -= shiftCount;
        softfloat_shiftLeft160M( extSigPtr, shiftCount, extSigPtr );
    }
    softfloat_roundPackMToF128M( sign, exp, extSigPtr, zWPtr );

}

