/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include the combined color conversion functions from JPEG Chapter
//	fwiRGBToYCbCr444LS_MCU_8u16s_C3P3R
//  fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R
//	fwiRGBToYCbCr411LS_MCU_8u16s_C3P3R
//	fwiBGRToYCbCr444LS_MCU_8u16s_C3P3R
//	fwiBGR565ToYCbCr444LS_MCU_16u16s_C3P3R
//	fwiBGR555ToYCbCr444LS_MCU_16u16s_C3P3R
//	fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R
//	fwiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R
//	fwiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R
//	fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R
//	fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R
//	fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R
//	fwiCMYKToYCCK444LS_MCU_8u16s_C4P4R
//	fwiCMYKToYCCK422LS_MCU_8u16s_C4P4R
//	fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R
//	fwiYCbCr444ToRGBLS_MCU_16s8u_P3C3R
//	fwiYCbCr422ToRGBLS_MCU_16s8u_P3C3R
//	fwiYCbCr411ToRGBLS_MCU_16s8u_P3C3R
//	fwiYCbCr444ToBGRLS_MCU_16s8u_P3C3R
//	fwiYCbCr444ToBGR565LS_MCU_16s16u_P3C3R
//	fwiYCbCr444ToBGR555LS_MCU_16s16u_P3C3R
//	fwiYCbCr422ToBGRLS_MCU_16s8u_P3C3R
//	fwiYCbCr422ToBGR565LS_MCU_16s16u_P3C3R
//	fwiYCbCr422ToBGR555LS_MCU_16s16u_P3C3R
//	fwiYCbCr411ToBGRLS_MCU_16s8u_P3C3R
//	fwiYCbCr411ToBGR565LS_MCU_16s16u_P3C3R
//	fwiYCbCr411ToBGR555LS_MCU_16s16u_P3C3R
//	fwiYCCK444ToCMYKLS_MCU_16s8u_P4C4R
//	fwiYCCK422ToCMYKLS_MCU_16s8u_P4C4R
//	fwiYCCK411ToCMYKLS_MCU_16s8u_P4C4R

//************************************************************************* 
#include "fwdev.h"
#include "fwJPEG.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )
#ifndef __JPEG_COLOR
#define __JPEG_COLOR
#define FWJ_LIMIT5(val) (((val)<=0)?0:(((val)>=0x1f)?0x1f:((unsigned short)(val))))
#define FWJ_LIMIT6(val) (((val)<=0)?0:(((val)>=0x3f)?0x3f:((unsigned short)(val))))
#endif

namespace OPT_LEVEL
{	
	#define STEPCHECK1(X) if (X<=0) return fwStsStepErr

  static  SYS_INLINE void  Mul_16s(__m128i const &s1,__m128i const &s2,__m128i &d1,__m128i &d2)
    {
        __m128i temp1;
        d1 = _mm_mullo_epi16(s1,s2);
        d2 = _mm_mulhi_epi16(s1,s2);
        temp1    = _mm_unpacklo_epi16(d1,d2); 
        d2    = _mm_unpackhi_epi16(d1,d2); 
		d1 = temp1;

        
    }


// Converts YCbCr 16s type to 16s type
static SYS_INLINE void YCbCrToRGBConv(__m128i &y,__m128i &cb,__m128i &cr)
{
                    __m128i r, g, b;
                    static const __m128i constant = CONST_SET1_32I((S16)(128*64.0));
                    static const __m128i val90 = CONST_SET1_16I		( (S16)90		);		// R = ( 1.402*2^6 )
                    static const __m128i val46 = CONST_SET1_16I		( (S16)(-46)	);		// G = ( -.71414*2^6 )
                    static const __m128i val22 = CONST_SET1_16I		( (S16)(-22)	);		// cr= ( -0.34414*(2^6) )
                    static const __m128i val113 = CONST_SET1_16I( (S16)(113)	);		// B = ( 1.772*(2^6) )


                    // RED
                    __m128i r1,r2;
                    Mul_16s( val90, cr,r1,r2);		// R = ( 1.402*Cr )*(2^6)
                    r1 = _mm_add_epi32		( r1, constant	);		// R = ( 128.5 + 1.402*Cr )*(2^6)
                    r2 = _mm_add_epi32		( r2, constant	);		// R = ( 128.5 + 1.402*Cr )*(2^6)

                    r1 = _mm_srai_epi32		( r1, 6			);		// R = ((128.5 + 1.402*Cr )*(2^6)) / (2^6)
                    r2 = _mm_srai_epi32		( r2, 6			);		// R = ((128.5 + 1.402*Cr )*(2^6)) / (2^6)
                    r  = _mm_packs_epi32    (r1,r2);
                    r  = _mm_adds_epi16		( r, y			);		// R = Y + 128.5 + 1.402*Cr

                    // GREEN
                    __m128i g1,g2,g3,g4;
                    Mul_16s( val46, cr,g1,g2);		// G = ( -.71414*Cr )*(2^6)
                    g1 = _mm_add_epi32		( g1, constant	);		// G = ( -.71414*Cr + 128.5 )*(2^6)
                    g2 = _mm_add_epi32		( g2, constant	);		// G = ( -.71414*Cr + 128.5 )*(2^6)

                    Mul_16s( val22, cb,g3,g4);		// cr= ( -0.34414*Cb )*(2^6)


                    g1 = _mm_add_epi32		( g1, g3			);		// G = ( -.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)
                    g2 = _mm_add_epi32		( g2, g4			);		// G = ( -.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)
                    g1 = _mm_srai_epi32		( g1, 6			);		// G = ((-.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)) / (2^6)
                    g2 = _mm_srai_epi32		( g2, 6			);		// G = ((-.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)) / (2^6)
        
                    __m128i y1=y,y2;

                    CBL_SSE2::Unpack16STo32S(y1,y2);
                    g1 = _mm_add_epi32		( g1, y1			);		// G = Y - 0.34414*Cb - 0.71414*Cr + 128.5 
                    g2 = _mm_add_epi32		( g2, y2			);		// G = Y - 0.34414*Cb - 0.71414*Cr + 128.5 

                    g = _mm_packs_epi32(g1,g2);

                    // BLUE
                    __m128i b1,b2;
                    Mul_16s		( val113, cb,b1,b2			);		// B = ( 1.772*Cb )*(2^6)
                    b1 = _mm_add_epi32		( b1, constant	);		// B = ( 1.772*Cb + 128.5 )*(2^6)
                    b2 = _mm_add_epi32		( b2, constant	);		// B = ( 1.772*Cb + 128.5 )*(2^6)

                    b1 = _mm_srai_epi16		( b1, 6			);		// B = ((1.772*Cb + 128.5 )*(2^6)) / (2^6)
                    b2 = _mm_srai_epi16		( b2, 6			);		// B = ((1.772*Cb + 128.5 )*(2^6)) / (2^6)
                    b = _mm_packs_epi32(b1,b2);
                    b = _mm_adds_epi16		( b, y			);		// B = Y + 1.772*Cb + 128.5

                    // CBL_SSE2::Convert_3P_to_3C_16bit( r, g, b);
                   ssp_convert_3p_3c_epi16( &r, &g, &b);
                    // r = {g2,r2,b1,g1,r1,b0,g0,r0}
                    // g = {r5,b4,g4,r4,b3,g3,r3,b2}
                    // b = {b7,g7,r7,b6,g6,r6,b5,g5}
					y = r;
					cb= g;
					cr = b;

}

static SYS_INLINE void YCbCrToRGB(__m128i &y,__m128i &cb,__m128i &cr)
{
					YCbCrToRGBConv(y,cb,cr);

                    y = _mm_packus_epi16 (y, cb);			// r = {r5,b4,g4,r4,b3,g3,r3,b2,g2,r2,b1,g1,r1,b0,g0,r0}
                    cb = _mm_packus_epi16 (cr, cr);			// b = {b7,g7,r7,b6,g6,r6,b5,g5,b7,g7,r7,b6,g6,r6,b5,g5}
}

static SYS_INLINE void YCbCrToBGR(__m128i &y,__m128i &cb,__m128i &cr)
{
					YCbCrToRGBConv(y,cb,cr);

                    cr = _mm_packus_epi16 (cr, cb);			// r = {r5,b4,g4,r4,b3,g3,r3,b2,g2,r2,b1,g1,r1,b0,g0,r0}
                    cb = _mm_packus_epi16 (y, y);			// b = {b7,g7,r7,b6,g6,r6,b5,g5,b7,g7,r7,b6,g6,r6,b5,g5}
					y = cr;
}

	static SYS_INLINE void fwiYCbCr411ToRGBLS_MCU_16s8u_P3C3R_8x8Block (Fw16s *pSrcY, Fw16s *pSrcCb, Fw16s *pSrcCr, Fw8u *pDstRGB, int dstStep)
	{
		A32S y, x;
		A32S dstPos, srcYPos=0, srcCPos;
		A16S yVal, crVal, cbVal;

        switch( Dispatch::Type<DT_SSE2>() )
	    {
	    case DT_SSE3:
	    case DT_SSE2:
            {    
                XMM128 yVal, cbVal, crVal;
		        for (y=0; y<8; ++y)
		            {
                        
			            dstPos = y*dstStep;
			            srcCPos = (y>>1)<<3; //(y>>1)*8;
			                yVal.i = _mm_loadu_si128((__m128i*)(pSrcY+srcYPos));
							cbVal.d = _mm_setzero_pd();
							crVal.d = _mm_setzero_pd();
							cbVal.d  = _mm_loadl_pd(cbVal.d,(double*) (pSrcCb+srcCPos));
							crVal.d  = _mm_loadl_pd(crVal.d,(double*) (pSrcCr+srcCPos));

                          //  cbVal.s64[0] = *((Fw64s*)(pSrcCb+srcCPos));
                          //  crVal.s64[0] = *((Fw64s*)(pSrcCr+srcCPos));

							cbVal.i = _mm_unpacklo_epi16(cbVal.i, cbVal.i);
                            crVal.i = _mm_unpacklo_epi16(crVal.i, crVal.i);
							
							YCbCrToRGB(yVal.i,cbVal.i,crVal.i);
                            _mm_storeu_si128((__m128i*) (pDstRGB + dstPos),yVal.i);
                           // *((Fw64s*)(pDstRGB + dstPos + 16)) = cbVal.s64[0];
							 _mm_storel_epi64 ( ((__m128i*)(pDstRGB + dstPos + 16)), cbVal.i );

                             srcYPos = srcYPos + 8;

			            //}
		            }
            }
            break;
	    default:
		        for (y=0; y<8; ++y)
		        {
			        dstPos = y*dstStep;
			        srcCPos = (y>>1)<<3; //(y>>1)*8;
			        for (x=0; x<8; x+=2)
			        {
				        yVal = pSrcY[srcYPos++];
				        cbVal = pSrcCb[srcCPos];
				        crVal = pSrcCr[srcCPos++];
				        // Compute RGB
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.402*crVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal - 0.34414*cbVal - 0.71414*crVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.772*cbVal + 128.5);

				        yVal = pSrcY[srcYPos++];
				        // Compute RGB
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.402*crVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal - 0.34414*cbVal - 0.71414*crVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.772*cbVal + 128.5);
			        }
		        }
	    }
		
	}

	static SYS_INLINE void fwiYCbCr411ToBGRLS_MCU_16s8u_P3C3R_8x8Block (Fw16s *pSrcY, Fw16s *pSrcCb, Fw16s *pSrcCr, Fw8u *pDstRGB, int dstStep)
	{
		A32S y, x;
		A32S dstPos, srcYPos=0, srcCPos;
		

        switch( Dispatch::Type<DT_SSE2>() )
	    {
	    case DT_SSE3:
	    case DT_SSE2:
            {    
                

		        for (y=0; y<8; ++y)
		            {
                        XMM128 yVal, cbVal, crVal;
			            dstPos = y*dstStep;
			            srcCPos = (y>>1)<<3; //(y>>1)*8;
			                yVal.i = _mm_loadu_si128((__m128i*)(pSrcY+srcYPos));
							cbVal.d = _mm_setzero_pd();
							crVal.d = _mm_setzero_pd();
							cbVal.d  = _mm_loadl_pd(cbVal.d,(double*) (pSrcCb+srcCPos));
							crVal.d  = _mm_loadl_pd(crVal.d,(double*) (pSrcCr+srcCPos));

							cbVal.i = _mm_unpacklo_epi16(cbVal.i, cbVal.i);
                            crVal.i = _mm_unpacklo_epi16(crVal.i, crVal.i);
							
							YCbCrToBGR(yVal.i,cbVal.i,crVal.i);
                            _mm_storeu_si128((__m128i*) (pDstRGB + dstPos),yVal.i);
                           // *((Fw64s*)(pDstRGB + dstPos + 16)) = cbVal.s64[0];
							_mm_storel_epi64 ( ((__m128i*)(pDstRGB + dstPos + 16)), cbVal.i );

                             srcYPos = srcYPos + 8;

			            //}
		            }
            }
            break;
	    default:
		        for (y=0; y<8; ++y)
		        {
					A16S yVal, crVal, cbVal;
			        dstPos = y*dstStep;
			        srcCPos = (y>>1)<<3; //(y>>1)*8;
			        for (x=0; x<8; x+=2)
			        {
				        yVal = pSrcY[srcYPos++];
				        cbVal = pSrcCb[srcCPos];
				        crVal = pSrcCr[srcCPos++];
				        // Compute RGB
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.772*cbVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal - 0.34414*cbVal - 0.71414*crVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.402*crVal + 128.5);

				        yVal = pSrcY[srcYPos++];
				        // Compute RGB
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.772*cbVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal - 0.34414*cbVal - 0.71414*crVal + 128.5);
				        pDstRGB[dstPos++] = FW_REF::Limits<A8U>::Sat( yVal + 1.402*crVal + 128.5);
			        }                       
		        }
	    }
		
	}

	
    SYS_INLINE FwStatus iYCbCr411ToRGBLS_MCU_16s8u_P3C3R( const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep )
	{
		Fw16s y, srcCOffset, *pSrcY, *pSrcCb, *pSrcCr;
		Fw8u *pDstRGB_T;
		
		// Y:				Cb:					Cr:									RGB:
		//		  ----8----		  ---4-----4---		  ---4-----4---		==>			  --8---8--
		//		8 |   Ay   |		4 |A_cb | B_cb|		4 |A_cr | B_cr|		==>			8 | A | B |
		//		8 |   By   |		  -------------		  -------------		==>			  ---------
		//		8 |   Cy   |		4 |C_cb | D_cb|		4 |C_cr | D_cr|		==>			8 | C | D |
		//		8 |   Dy   |		  -------------		  -------------		==>			  ---------
		//		  ----------

		// Proces each block independently
		for (y=0; y<4; ++y)
		{
			// Arithmetic to point to the correct chroma block.  
			// (y&0x1)*4 = (y&0x1)<<2   = skip the left 4 pixels
			// (y&0x2)*2*8 = (y&0x2)<<4 = Skip the top 16x8 pixels
			// if y is odd
			//		Convert RIGHT hand side block
			// else
			//		Convert LEFT hand side block	
			// if y >=2 (bit 1 of y is set)
			//		Convert TOP half of block
			// else
			//		Convert BOTTOM half of block
			srcCOffset = ((y&0x1)<<2) + ((y&0x2)<<4);

			pSrcY = (Fw16s*)pSrcMCU[0] + (y<<6);//y*8*8;			// Pointer to the next block to be processed
			pSrcCb = (Fw16s*)pSrcMCU[1] + srcCOffset;
			pSrcCr = (Fw16s*)pSrcMCU[2] + srcCOffset;
			pDstRGB_T = pDstRGB + (y&0x1)*8*3 + ((y>>1)*8*dstStep);
			//pDstRGB_T = pDstRGB + (y&0x1)*8*3 + ((y>>1)*8*dstStep);

			// Process each 8x8 block
			fwiYCbCr411ToRGBLS_MCU_16s8u_P3C3R_8x8Block (pSrcY, pSrcCb, pSrcCr, pDstRGB_T, dstStep);
		}

		return fwStsNoErr;
	}

	
    SYS_INLINE FwStatus iYCbCr411ToBGRLS_MCU_16s8u_P3C3R( const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep )
	{
		Fw16s y, srcCOffset, *pSrcY, *pSrcCb, *pSrcCr;
		Fw8u *pDstRGB_T;
		
		// Y:				Cb:					Cr:									RGB:
		//		  ----8----		  ---4-----4---		  ---4-----4---		==>			  --8---8--
		//		8 |   Ay   |		4 |A_cb | B_cb|		4 |A_cr | B_cr|		==>			8 | A | B |
		//		8 |   By   |		  -------------		  -------------		==>			  ---------
		//		8 |   Cy   |		4 |C_cb | D_cb|		4 |C_cr | D_cr|		==>			8 | C | D |
		//		8 |   Dy   |		  -------------		  -------------		==>			  ---------
		//		  ----------

		// Proces each block independently
		for (y=0; y<4; ++y)
		{
			// Arithmetic to point to the correct chroma block.  
			// (y&0x1)*4 = (y&0x1)<<2   = skip the left 4 pixels
			// (y&0x2)*2*8 = (y&0x2)<<4 = Skip the top 16x8 pixels
			// if y is odd
			//		Convert RIGHT hand side block
			// else
			//		Convert LEFT hand side block	
			// if y >=2 (bit 1 of y is set)
			//		Convert TOP half of block
			// else
			//		Convert BOTTOM half of block
			srcCOffset = ((y&0x1)<<2) + ((y&0x2)<<4);

			pSrcY = (Fw16s*)pSrcMCU[0] + (y<<6);//y*8*8;			// Pointer to the next block to be processed
			pSrcCb = (Fw16s*)pSrcMCU[1] + srcCOffset;
			pSrcCr = (Fw16s*)pSrcMCU[2] + srcCOffset;
			pDstRGB_T = pDstRGB + (y&0x1)*8*3 + ((y>>1)*8*dstStep);
			//pDstRGB_T = pDstRGB + (y&0x1)*8*3 + ((y>>1)*8*dstStep);

			// Process each 8x8 block
			fwiYCbCr411ToBGRLS_MCU_16s8u_P3C3R_8x8Block (pSrcY, pSrcCb, pSrcCr, pDstRGB_T, dstStep);
		}

		return fwStsNoErr;
	}

	
    //-----------------------------------------------------------------------
	//Creates a YCCK Image and then converts to CMYK format
	//-----------------------------------------------------------------------
	static SYS_INLINE void fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (
	const Fw16s *pSrcMCU[4], Fw8u *pDstCMYK, int dstStep, int blocknum,
	int xshift, int yshift)
	{
		//Reference code only.
		//SSE2 code need to shift 16 bit 
		int x, y;
		unsigned char RVal, GVal, BVal;
		int ysrcPos, csrcPos, coffset, cPos, doffset, dstPos;

		ysrcPos = blocknum<<6;//*64
		coffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		doffset = ((blocknum&0x1)<<5) + ((blocknum&0x2)<<2)* dstStep;

		for (y=0; y<8; y++) {
			csrcPos = (y>>yshift)*8 + coffset;
			dstPos = y*dstStep + doffset;
			for (x=0;x<8;x++) {
				//add 128 for each element of YCCK
				cPos = csrcPos + (x>>xshift);
				RVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos] + 
				1.402*pSrcMCU[2][cPos] + 128.5);
				GVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos] - 
				0.34414*pSrcMCU[1][cPos] - 0.71414*pSrcMCU[2][cPos]+ 128.5);
				BVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos] + 
				1.772*pSrcMCU[1][cPos] + 128.5);
				pDstCMYK[dstPos++] = ~RVal; //C=255-R
				pDstCMYK[dstPos++] = ~GVal; //M=255-G
				pDstCMYK[dstPos++] = ~BVal; //Y=255-B
				pDstCMYK[dstPos++] = (Fw8u)(pSrcMCU[3][ysrcPos++]+128);
			}
		}

		return;
	}

	static SYS_INLINE void fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block (
	const Fw16s *pSrcMCU[3], Fw16u *pDstBGR, int dstStep, int blocknum,
	int xshift, int yshift)
	{
		//Reference code only.
		//SSE2 code need to shift 16 bit 
		int x, y;
		unsigned short RVal, GVal, BVal;
		int ysrcPos, csrcPos, coffset, cPos, doffset, dstPos;
		double val;

		ysrcPos = blocknum<<6;//*64
		coffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		doffset = ((blocknum&0x1)<<3) + ((blocknum&0x2)<<2)* dstStep;

		for (y=0; y<8; y++) {
			csrcPos = (y>>yshift)*8 + coffset;
			dstPos = y*dstStep + doffset;
			for (x=0;x<8;x++) {
				//add 128 for each element of YCC
				cPos = csrcPos + (x>>xshift);
				val = (pSrcMCU[0][ysrcPos] + 1.402*pSrcMCU[2][cPos] + 128.5)/8;
				RVal = FWJ_LIMIT5(val) << 10;
				val = (pSrcMCU[0][ysrcPos] - 0.34414*pSrcMCU[1][cPos] - 
				0.71414*pSrcMCU[2][cPos]+ 128.5)/8;
				GVal = FWJ_LIMIT5(val) << 5;
				val = (pSrcMCU[0][ysrcPos] + 1.772*pSrcMCU[1][cPos] + 128.5)/8;
				BVal = FWJ_LIMIT5(val);

				ysrcPos++;
				pDstBGR[dstPos++]=RVal|GVal|BVal;
			}
		}

		return;
	}

	//-----------------------------------------------------------------------
	//Creates a YCBCr image from MCU and then converts it to RGB format
	//-----------------------------------------------------------------------
	SYS_INLINE static FwStatus iYCbCr444ToRGBLS_MCU_16s8u_P3C3R_REF(const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep)
	{

		if (pDstRGB==0 || pSrcMCU==0) return fwStsNullPtrErr;
		if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
		STEPCHECK1(dstStep);

		//Reference code only.
		//SSE2 code need to shift 16 bit 
		int x, y;
		int srcPos=0, dstPos;
		Fw16s cbVal, crVal;

		for (y=0;y<8; y++) {
			//srcPos = y*8;
			dstPos = y*dstStep;
			for (x=0;x<8;x++) {
				cbVal = pSrcMCU[1][srcPos];
				crVal = pSrcMCU[2][srcPos];
				pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcMCU[0][srcPos] + 1.402*crVal + 128.5);
				pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcMCU[0][srcPos] - 0.34414*cbVal - 0.71414*crVal+ 128.5);
				pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcMCU[0][srcPos++] + 1.772*cbVal + 128.5);
			}
		}
		return fwStsNoErr;
	}


	SYS_INLINE static FwStatus iYCbCr444ToRGBLS_MCU_16s8u_P3C3R_SSE2(const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep)
	{
		Fw16s *pSrcY=(Fw16s*)pSrcMCU[0], *pSrcCb=(Fw16s*)pSrcMCU[1], *pSrcCr=(Fw16s*)pSrcMCU[2];
		Fw8u *pDst;

			for (S32 j=0; j<8; ++j) {
				pDst = j*dstStep + pDstRGB;
                   
                    __m128i r = _mm_loadu_si128((__m128i *)pSrcY);
                    __m128i g = _mm_loadu_si128((__m128i *)pSrcCb);
                    __m128i b = _mm_loadu_si128((__m128i *)pSrcCr);


                    YCbCrToRGBConv(r,g,b);

                    r = _mm_packus_epi16 (r, g);			// r = {r5,b4,g4,r4,b3,g3,r3,b2,g2,r2,b1,g1,r1,b0,g0,r0}
                    _mm_storeu_si128 ((__m128i*)pDst, r);
                    b = _mm_packus_epi16 (b, b);			// b = {b7,g7,r7,b6,g6,r6,b5,g5,b7,g7,r7,b6,g6,r6,b5,g5}
                    _mm_storel_epi64 ( ((__m128i*)pDst+1), b ); 


                    pSrcY += 8; 
                    pSrcCb += 8; 
                    pSrcCr += 8;	
			}
		return fwStsNoErr;
	}

SYS_INLINE	static FwStatus iYCbCr444ToBGRLS_MCU_16s8u_P3C3R_SSE2(const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep)
	{
		Fw16s *pSrcY=(Fw16s*)pSrcMCU[0], *pSrcCb=(Fw16s*)pSrcMCU[1], *pSrcCr=(Fw16s*)pSrcMCU[2];
		Fw8u *pDst;

			for (S32 j=0; j<8; ++j) {
				pDst = j*dstStep + pDstRGB;
         
                    __m128i r = _mm_loadu_si128((__m128i *)pSrcY);
                    __m128i g = _mm_loadu_si128((__m128i *)pSrcCb);
                    __m128i b = _mm_loadu_si128((__m128i *)pSrcCr);


                    YCbCrToRGBConv(r,g,b);

                    b = _mm_packus_epi16 (b, g);			// r = {r5,b4,g4,r4,b3,g3,r3,b2,g2,r2,b1,g1,r1,b0,g0,r0}
                    _mm_storeu_si128 ((__m128i*)pDst, b);
                    r = _mm_packus_epi16 (r, r);			// b = {b7,g7,r7,b6,g6,r6,b5,g5,b7,g7,r7,b6,g6,r6,b5,g5}
                    _mm_storel_epi64 ( ((__m128i*)pDst+1), r );


                    pSrcY += 8; 
                    pSrcCb += 8; 
                    pSrcCr += 8;	
			}
		return fwStsNoErr;
	}

SYS_INLINE static void YCbCr422ToRGBLS_MCU_16s8u_P3C3R_Conv(const __m128i &y,__m128i &cb,__m128i &cr,__m128i &r,__m128i &g,__m128i &b)
{

                    cb = _mm_shufflelo_epi16(cb,_MM_SHUFFLE(3, 3, 2, 2));
                    g = _mm_shufflehi_epi16(cb,_MM_SHUFFLE(1, 1, 0, 0));

                    cr = _mm_shufflelo_epi16(cr,_MM_SHUFFLE(3, 3, 2, 2));
                    b = _mm_shufflehi_epi16(cr,_MM_SHUFFLE(1, 1, 0, 0));

                    r = y;
                    YCbCrToRGBConv(r,g,b);

}

	SYS_INLINE static FwStatus iYCbCr422ToBGRLS_MCU_16s8u_P3C3R_SSE2(const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep)
	{
		Fw16s *pSrcY=(Fw16s*)pSrcMCU[0], *pSrcCb=(Fw16s*)pSrcMCU[1], *pSrcCr=(Fw16s*)pSrcMCU[2];
		Fw8u *pDst;

			for (S32 j=0; j<8; ++j) {
				pDst = j*dstStep + pDstRGB;
                    __m128i y,y1,r,g,b,r1,g1,b1;
                    __m128i cb1,cr1,cb,cr;

                    pSrcY = (Fw16s*)pSrcMCU[0]+ j*8;
                    
                    y = _mm_loadu_si128((__m128i *)pSrcY);
                    cr = _mm_loadu_si128((__m128i *)pSrcCr);
                    cb = _mm_loadu_si128((__m128i *)pSrcCb);

                    cb1 = _mm_slli_si128(cb,4);
                    cr1 = _mm_slli_si128(cr,4);

                    YCbCr422ToRGBLS_MCU_16s8u_P3C3R_Conv(y,cb1,cr1,r,g,b);

                    b = _mm_packus_epi16 (b, g);			// r = {r5,b4,g4,r4,b3,g3,r3,b2,g2,r2,b1,g1,r1,b0,g0,r0}
                    _mm_storeu_si128 ((__m128i*)pDst, b);

                    pSrcY += 64;
                    y1 = _mm_loadu_si128((__m128i *)pSrcY);

                    cb1 = _mm_srli_si128(cb,4);
                    cr1 = _mm_srli_si128(cr,4);

                    YCbCr422ToRGBLS_MCU_16s8u_P3C3R_Conv(y1,cb1,cr1,r1,g1,b1);

                    r = _mm_packus_epi16 (r, b1);			
                    _mm_storeu_si128 ( ((__m128i*)pDst+1), r );
                    g1 = _mm_packus_epi16 (g1, r1);			
                    _mm_storeu_si128 ( ((__m128i*)pDst+2),g1 );

                    pSrcCb += 8; 
                    pSrcCr += 8;	

                   }
		return fwStsNoErr;
	}





SYS_INLINE static FwStatus iYCbCr422ToRGBLS_MCU_16s8u_P3C3R_SSE2(const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep)
	{
		Fw16s *pSrcY=(Fw16s*)pSrcMCU[0], *pSrcCb=(Fw16s*)pSrcMCU[1], *pSrcCr=(Fw16s*)pSrcMCU[2];
		Fw8u *pDst;

			for (S32 j=0; j<8; ++j) {
				pDst = j*dstStep + pDstRGB;
                    __m128i y,y1,r,g,b,r1,g1,b1;
                    __m128i cb1,cr1,cb,cr;

                    pSrcY = (Fw16s*)pSrcMCU[0]+ j*8;
                    
                    y = _mm_loadu_si128((__m128i *)pSrcY);
                    cr = _mm_loadu_si128((__m128i *)pSrcCr);
                    cb = _mm_loadu_si128((__m128i *)pSrcCb);

                    cb1 = _mm_slli_si128(cb,4);
                    cr1 = _mm_slli_si128(cr,4);
                    
                    YCbCr422ToRGBLS_MCU_16s8u_P3C3R_Conv(y,cb1,cr1,r,g,b);

                    r = _mm_packus_epi16 (r, g);			// r = {r5,b4,g4,r4,b3,g3,r3,b2,g2,r2,b1,g1,r1,b0,g0,r0}
                    _mm_storeu_si128 ((__m128i*)pDst, r);

                     pSrcY += 64;

                    y1 = _mm_loadu_si128((__m128i *)pSrcY);

                    cb1 = _mm_srli_si128(cb,4);
                    cr1 = _mm_srli_si128(cr,4);

                    YCbCr422ToRGBLS_MCU_16s8u_P3C3R_Conv(y1,cb1,cr1,r1,g1,b1);

                    b = _mm_packus_epi16 (b, r1);			
                    _mm_storeu_si128 ( ((__m128i*)pDst+1), b );
                    g1 = _mm_packus_epi16 (g1, b1);			
                    _mm_storeu_si128 ( ((__m128i*)pDst+2),g1 );

                    pSrcCb += 8; 
                    pSrcCr += 8;	

                   }
		return fwStsNoErr;
	}


	static SYS_INLINE void fwiYCbCr4xxToRGBLS_MCU_16s8u_P3C3R_8x8Block (
	const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep, int blocknum, 
	int xshift, int yshift)
	{
		//Reference code only.
		//SSE2 code need to shift 16 bit 
		int x, y;
		unsigned char RVal, GVal, BVal;
		int ysrcPos, csrcPos, coffset, cPos, doffset, dstPos;

		ysrcPos = blocknum<<6;//*64
		coffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		doffset = ((blocknum&0x1)<<3) * 3 + ((blocknum&0x2)<<2)* dstStep;

		for (y=0; y<8; y++) {
			csrcPos = (y>>yshift)*8 + coffset;
			dstPos = y*dstStep + doffset;
			for (x=0;x<8;x++) {
				//add 128 for each element of YCC
				cPos = csrcPos + (x>>xshift);
				RVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos] + 
				1.402*pSrcMCU[2][cPos] + 128.5);
				GVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos] - 
				0.34414*pSrcMCU[1][cPos] - 0.71414*pSrcMCU[2][cPos]+ 128.5);
				BVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos++] + 
				1.772*pSrcMCU[1][cPos] + 128.5);
				pDstRGB[dstPos++] = RVal; 
				pDstRGB[dstPos++] = GVal; 
				pDstRGB[dstPos++] = BVal; 
			}
		}

		return;
	}

	//-----------------------------------------------------------------------
	//Creates a YCbCr Image and then converts to BGR format
	//-----------------------------------------------------------------------
	static SYS_INLINE void fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block (
	const Fw16s *pSrcMCU[3], Fw8u *pDstBGR, int dstStep, int blocknum, 
	int xshift, int yshift)
	{
		//Reference code only.
		//SSE2 code need to shift 16 bit 
		int x, y;
		unsigned char RVal, GVal, BVal;
		int ysrcPos, csrcPos, coffset, cPos, doffset, dstPos;

		ysrcPos = blocknum<<6;//*64
		coffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		doffset = ((blocknum&0x1)<<3) * 3 + ((blocknum&0x2)<<2)* dstStep;

		for (y=0; y<8; y++) {
			csrcPos = (y>>yshift)*8 + coffset;
			dstPos = y*dstStep + doffset;
			for (x=0;x<8;x++) {
				//add 128 for each element of YCC
				cPos = csrcPos + (x>>xshift);
				RVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos] + 
				1.402*pSrcMCU[2][cPos] + 128.5);
				GVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos] - 
				0.34414*pSrcMCU[1][cPos] - 0.71414*pSrcMCU[2][cPos]+ 128.5);
				BVal = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos++] + 
				1.772*pSrcMCU[1][cPos] + 128.5);
				pDstBGR[dstPos++] = BVal; 
				pDstBGR[dstPos++] = GVal; 
				pDstBGR[dstPos++] = RVal; 
			}
		}

		return;
	}

	static SYS_INLINE void fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block (
	const Fw16s *pSrcMCU[3], Fw16u *pDstBGR, int dstStep, int blocknum,
	int xshift, int yshift)
	{
		//Reference code only.
		//SSE2 code need to shift 16 bit 
		int x, y;
		unsigned short RVal, GVal, BVal;
		int ysrcPos, csrcPos, coffset, cPos, doffset, dstPos;
		double val;

		ysrcPos = blocknum<<6;//*64
		coffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		doffset = ((blocknum&0x1)<<3) + ((blocknum&0x2)<<2)* dstStep;

		for (y=0; y<8; y++) {
			csrcPos = (y>>yshift)*8 + coffset;
			dstPos = y*dstStep + doffset;
			for (x=0;x<8;x++) {
				//add 128 for each element of YCC
				cPos = csrcPos + (x>>xshift);
				val = (pSrcMCU[0][ysrcPos] + 1.402*pSrcMCU[2][cPos] + 128.5)/8;
				RVal = FWJ_LIMIT5(val) << 11;
				val = (pSrcMCU[0][ysrcPos] - 0.34414*pSrcMCU[1][cPos] - 
				0.71414*pSrcMCU[2][cPos]+ 128.5)/4;
				GVal = FWJ_LIMIT6(val) << 5;
				val = (pSrcMCU[0][ysrcPos] + 1.772*pSrcMCU[1][cPos] + 128.5)/8;
				BVal = FWJ_LIMIT5(val);

				ysrcPos++;
				pDstBGR[dstPos++]=RVal|GVal|BVal;
			}
		}

		return;
	}

	static void fwiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R_8x8Block(
	const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3], int blocknum)
	{
		unsigned short RVal, GVal, BVal;
		int result, x, y;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb=0, elementCr=0;

		srcOffset  = blocknum << 3;
		dstCOffset = blocknum << 2;
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			dstCPos = y*8 + dstCOffset;
			for (x=0;x<8;x++) {		
				BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
				GVal=((pSrcBGR[srcPos]>>5)&0x1f)<<3;
				RVal=(pSrcBGR[srcPos++]>>10)<<3;

				result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
				pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

				if (!(x&1)){
					elementCb = -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr = 128 * RVal - 107 * GVal - 21 * BVal;
				} else {
					result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb + 256;
					pDstMCU [1][dstCPos] = (Fw16s)(result>>9);
					result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr + 256;
					pDstMCU [2][dstCPos++] = (Fw16s)(result>>9);
				}	
			}
		}

		return;
	}

	//-----------------------------------------------------------------------
	//Convert an 16*16 BGR image to the YCbCr color mode and create 411MCU
	//
	//-----------------------------------------------------------------------
	static void fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R_8x8Block(
	const Fw8u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3], int blocknum)
	{
		unsigned char RVal, GVal, BVal;
		int result, x, y, x1;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb[4]={0, 0, 0, 0}; 
		int elementCr[4]={0, 0, 0, 0};

		srcOffset  = ((blocknum&0x1)<<3)*3 + ((blocknum&0x2)<<2)*srcStep;
		dstCOffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			if (!(y&1)) {
				for (x=0;x<8;x++) {
					BVal = pSrcBGR[srcPos++];
					GVal = pSrcBGR[srcPos++];
					RVal = pSrcBGR[srcPos++];
					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

					x1=x>>1;
					elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
				}
			} else {
				dstCPos = (y>>1)*8 + dstCOffset;
				for (x=0;x<8;x++) {
					BVal = pSrcBGR[srcPos++];
					GVal = pSrcBGR[srcPos++];
					RVal = pSrcBGR[srcPos++];
					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

					x1=x>>1;
					if (!(x&1)) {
						elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
						elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
					} else {
						result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb[x1] + 512;
						pDstMCU [1][dstCPos] = (Fw16s)(result>>10);
						elementCb[x1] = 0; //clean after using the value
						result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr[x1] + 512;
						pDstMCU [2][dstCPos++] = (Fw16s)(result>>10);
						elementCr[x1] = 0;
					}	
				}
			}
		}

		return;
	}

    SYS_INLINE STATIC void MulBGR_Coef(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
    {
        __m128i s1hi,s2hi,s3hi;

        CBL_SSE2::Unpack8UTo16U(s1lo,s1hi);
        CBL_SSE2::Unpack8UTo16U(s2lo,s2hi);
        CBL_SSE2::Unpack8UTo16U(s3lo,s3hi);

        s1lo = _mm_mullo_epi16(s1lo,mCoef[0].i);
        s1hi = _mm_mullo_epi16(s1hi,mCoef[0].i);
        s2lo = _mm_mullo_epi16(s2lo,mCoef[1].i);
        s2hi = _mm_mullo_epi16(s2hi,mCoef[1].i);
        s3lo = _mm_mullo_epi16(s3lo,mCoef[2].i);
        s3hi = _mm_mullo_epi16(s3hi,mCoef[2].i);

        s1lo = _mm_srli_epi16(s1lo,8);
        s2lo = _mm_srli_epi16(s2lo,8);
        s3lo = _mm_srli_epi16(s3lo,8);

        s1lo = _mm_add_epi16(s1lo,s2lo);
        s1lo = _mm_add_epi16(s1lo,s3lo);
        
        s1hi = _mm_srli_epi16(s1hi,8);
        s2hi = _mm_srli_epi16(s2hi,8);
        s3hi = _mm_srli_epi16(s3hi,8);
        s1hi = _mm_add_epi16(s1hi,s2hi);
        s2lo = _mm_add_epi16(s1hi,s3hi);
    }

    SYS_INLINE STATIC void MulBGR_CoefLo(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
    {
        __m128i s1hi,s2hi,s3hi;

        CBL_SSE2::Unpack8UTo16U(s1lo,s1hi);
        CBL_SSE2::Unpack8UTo16U(s2lo,s2hi);
        CBL_SSE2::Unpack8UTo16U(s3lo,s3hi);

        s1lo = _mm_mullo_epi16(s1lo,mCoef[0].i);
        s2lo = _mm_mullo_epi16(s2lo,mCoef[1].i);
        s3lo = _mm_mullo_epi16(s3lo,mCoef[2].i);

        s1lo = _mm_srli_epi16(s1lo,8);
        s2lo = _mm_srli_epi16(s2lo,8);
        s3lo = _mm_srli_epi16(s3lo,8);

        s1lo = _mm_add_epi16(s1lo,s2lo);
        s1lo = _mm_add_epi16(s1lo,s3lo);
    }


    SYS_INLINE STATIC void MulCbCr_Coef(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
    {
        __m128i a1,a2,b1,b2,c1,c2;


        Mul_16s(s1lo,mCoef[0].i,a1,a2);
        Mul_16s(s2lo,mCoef[1].i,b1,b2);
        Mul_16s(s3lo,mCoef[2].i,c1,c2);
        
        a1 = _mm_add_epi32(a1,b1);
        a1 = _mm_add_epi32(a1,c1);

        a2 = _mm_add_epi32(a2,b2);
        a2 = _mm_add_epi32(a2,c2);

        a1 = _mm_srai_epi32(a1,10);
        a2 = _mm_srai_epi32(a2,10);

        s1lo = _mm_packs_epi32(a1,a2);

    }

    SYS_INLINE STATIC void MulCbCr422_Coef(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
    {
        __m128i a1,a2,b1,b2,c1,c2;


        Mul_16s(s1lo,mCoef[0].i,a1,a2);
        Mul_16s(s2lo,mCoef[1].i,b1,b2);
        Mul_16s(s3lo,mCoef[2].i,c1,c2);
        
        a1 = _mm_add_epi32(a1,b1);
        a1 = _mm_add_epi32(a1,c1);

        a2 = _mm_add_epi32(a2,b2);
        a2 = _mm_add_epi32(a2,c2);

        a1 = _mm_srai_epi32(a1,9);
        a2 = _mm_srai_epi32(a2,9);

        s1lo = _mm_packs_epi32(a1,a2);

    }

    SYS_INLINE STATIC void MulCbCr_CoefLo(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
    {
        __m128i a1,a2,b1,b2,c1,c2;

        CBL_SSE2::Unpack8UTo16U(s1lo,a1);
        CBL_SSE2::Unpack8UTo16U(s2lo,a2);
        CBL_SSE2::Unpack8UTo16U(s3lo,b1);

        Mul_16s(s1lo,mCoef[0].i,a1,a2);
        Mul_16s(s2lo,mCoef[1].i,b1,b2);
        Mul_16s(s3lo,mCoef[2].i,c1,c2);
        
        a1 = _mm_add_epi32(a1,b1);
        a1 = _mm_add_epi32(a1,c1);

        a2 = _mm_add_epi32(a2,b2);
        a2 = _mm_add_epi32(a2,c2);

        a1 = _mm_srai_epi32(a1,8);
        a2 = _mm_srai_epi32(a2,8);

        s1lo = _mm_packs_epi32(a1,a2);

    }


    SYS_INLINE STATIC void Add8u_to_16s(__m128i &src1,__m128i &src2,__m128i &src3,__m128i &src4,__m128i &src5,__m128i &src6)
    {
        __m128i s1lo = src1,s2lo=src2,s3lo=src3,s4lo=src4,s5lo=src5,s6lo=src6;
        __m128i s1hi,s2hi,s3hi,s4hi,s5hi,s6hi;
        CBL_SSE2::Unpack8UTo16U(s1lo,s1hi);
        CBL_SSE2::Unpack8UTo16U(s2lo,s2hi);
        CBL_SSE2::Unpack8UTo16U(s3lo,s3hi);

        CBL_SSE2::Unpack8UTo16U(s4lo,s4hi);
        CBL_SSE2::Unpack8UTo16U(s5lo,s5hi);
        CBL_SSE2::Unpack8UTo16U(s6lo,s6hi);

       s1lo =  _mm_add_epi16(s1lo,s4lo); // b
       s2lo =  _mm_add_epi16(s2lo,s5lo); // g
       s3lo =  _mm_add_epi16(s3lo,s6lo); // r
       s1hi =  _mm_add_epi16(s1hi,s4hi); // b 
       s2hi =  _mm_add_epi16(s2hi,s5hi); // g
       s3hi =  _mm_add_epi16(s3hi,s6hi); // r


        __m128i t1 = _mm_srli_epi32(s1lo,16);
        t1 = _mm_add_epi16(t1,s1lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        __m128i t2 = _mm_srli_si128(t1,8);
        
        s1lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s1hi,16);
        t1 = _mm_add_epi16(t1,s1hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s1hi = _mm_unpacklo_epi32(t1,t2);

        src1 = _mm_unpacklo_epi64(s1lo,s1hi);
//
        t1 = _mm_srli_epi32(s2lo,16);
        t1 = _mm_add_epi16(t1,s2lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s2lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s2hi,16);
        t1 = _mm_add_epi16(t1,s2hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s2hi = _mm_unpacklo_epi32(t1,t2);

        src2 = _mm_unpacklo_epi64(s2lo,s2hi);
//
        t1 = _mm_srli_epi32(s3lo,16);
        t1 = _mm_add_epi16(t1,s3lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s3lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s3hi,16);
        t1 = _mm_add_epi16(t1,s3hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s3hi = _mm_unpacklo_epi32(t1,t2);

        src3 = _mm_unpacklo_epi64(s3lo,s3hi);
    }

    SYS_INLINE STATIC void Add4228u_to_16s(__m128i &src1,__m128i &src2,__m128i &src3,__m128i &src4,__m128i &src5,__m128i &src6)
    {
        __m128i s1lo = src1,s2lo=src2,s3lo=src3,s4lo=src4,s5lo=src5,s6lo=src6;
        __m128i s1hi,s2hi,s3hi,s4hi,s5hi,s6hi;
        CBL_SSE2::Unpack8UTo16U(s1lo,s1hi);
        CBL_SSE2::Unpack8UTo16U(s2lo,s2hi);
        CBL_SSE2::Unpack8UTo16U(s3lo,s3hi);

        CBL_SSE2::Unpack8UTo16U(s4lo,s4hi);
        CBL_SSE2::Unpack8UTo16U(s5lo,s5hi);
        CBL_SSE2::Unpack8UTo16U(s6lo,s6hi);


        __m128i t1 = _mm_srli_epi32(s1lo,16);
        t1 = _mm_add_epi16(t1,s1lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        __m128i t2 = _mm_srli_si128(t1,8);
        
        s1lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s1hi,16);
        t1 = _mm_add_epi16(t1,s1hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s1hi = _mm_unpacklo_epi32(t1,t2);

        src1 = _mm_unpacklo_epi64(s1lo,s1hi);

//
        t1 = _mm_srli_epi32(s2lo,16);
        t1 = _mm_add_epi16(t1,s2lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s2lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s2hi,16);
        t1 = _mm_add_epi16(t1,s2hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s2hi = _mm_unpacklo_epi32(t1,t2);

        src2 = _mm_unpacklo_epi64(s2lo,s2hi);
//
        t1 = _mm_srli_epi32(s3lo,16);
        t1 = _mm_add_epi16(t1,s3lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s3lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s3hi,16);
        t1 = _mm_add_epi16(t1,s3hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s3hi = _mm_unpacklo_epi32(t1,t2);

        src3= _mm_unpacklo_epi64(s3lo,s3hi);
//
        t1 = _mm_srli_epi32(s4lo,16);
        t1 = _mm_add_epi16(t1,s4lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s4lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s4hi,16);
        t1 = _mm_add_epi16(t1,s4hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s4hi = _mm_unpacklo_epi32(t1,t2);

        src4= _mm_unpacklo_epi64(s4lo,s4hi);
//

        t1 = _mm_srli_epi32(s5lo,16);
        t1 = _mm_add_epi16(t1,s5lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s5lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s5hi,16);
        t1 = _mm_add_epi16(t1,s5hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s5hi = _mm_unpacklo_epi32(t1,t2);

        src5= _mm_unpacklo_epi64(s5lo,s5hi);
//
        t1 = _mm_srli_epi32(s6lo,16);
        t1 = _mm_add_epi16(t1,s6lo);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s6lo = _mm_unpacklo_epi32(t1,t2);

        t1 = _mm_srli_epi32(s6hi,16);
        t1 = _mm_add_epi16(t1,s6hi);
        t1 = _mm_shufflelo_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t1 = _mm_shufflehi_epi16(t1,_MM_SHUFFLE(3, 2, 2, 0));
        t2 = _mm_srli_si128(t1,8);
        
        s6hi = _mm_unpacklo_epi32(t1,t2);

        src6= _mm_unpacklo_epi64(s6lo,s6hi);
    }

    SYS_INLINE STATIC void fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R_SSE (const Fw8u *pSrcBGR,int srcStep,Fw16s *pDstMCU[3])
    {
        XMM128 coeffY[3],coeffCb[3],coeffCr[3];
        coeffY[0].i = _mm_set1_epi16(29);
        coeffY[1].i = _mm_set1_epi16(150);
        coeffY[2].i = _mm_set1_epi16(77);

        //elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
        coeffCb[0].i = _mm_set1_epi16(128);
        coeffCb[1].i = _mm_set1_epi16(-85);
        coeffCb[2].i = _mm_set1_epi16(-43);
        //elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
        coeffCr[0].i = _mm_set1_epi16(-21);
        coeffCr[1].i = _mm_set1_epi16(-107);
        coeffCr[2].i = _mm_set1_epi16(128);

        const __m128i val127 = _mm_set1_epi16(127);
        int dstYPos = 0;
        int cbcrpos = 0;

        for(int height = 0;height < 8; height+=2)
        {
                __m128i regB = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32; 

                __m128i regB1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32;

               // SSEPLUS Begin
               // CBL_SSE2::Convert_3C_to_3P_8bit(regB,regG,regR);
               // CBL_SSE2::Convert_3C_to_3P_8bit(regB1,regG1,regR1);
               
               ssp_convert_3c_3p_epi8(&regB, &regG, &regR);
               ssp_convert_3c_3p_epi8(&regB1, &regG1, &regR1);
               
               __m128i s1 = regB,s2=regG,s3=regR,s4=regB1,s5=regG1,s6=regR1;
               Add8u_to_16s(s1,s2,s3,s4,s5,s6);
               s4 = s1,s5 = s2,s6 = s3;
               MulCbCr_Coef(s1,s2,s3,coeffCb);                
               MulCbCr_Coef(s4,s5,s6,coeffCr);
               
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos),s1);
               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos),s4);

                cbcrpos+=8;
                //Y = 77 * RVal + 150 * GVal + 29 * BVal + 128;
                MulBGR_Coef(regB,regG,regR,coeffY);
                MulBGR_Coef(regB1,regG1,regR1,coeffY);

                regB = _mm_sub_epi16(regB,val127);
                regG = _mm_sub_epi16(regG,val127);

                regB1 = _mm_sub_epi16(regB1,val127);
                regG1 = _mm_sub_epi16(regG1,val127);

               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG);
                dstYPos+=8;
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB1);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG1);
                dstYPos+=8;
        }

       dstYPos = 128;
        for(int height = 0;height < 8; height+=2)
        {
                __m128i regB = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32; 

                __m128i regB1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32;

               // CBL_SSE2::Convert_3C_to_3P_8bit(regB,regG,regR);
               // CBL_SSE2::Convert_3C_to_3P_8bit(regB1,regG1,regR1);
               ssp_convert_3c_3p_epi8(&regB, &regG, &regR);
               ssp_convert_3c_3p_epi8(&regB1, &regG1, &regR1);
               
                __m128i s1 = regB,s2=regG,s3=regR,s4=regB1,s5=regG1,s6=regR1;
               Add8u_to_16s(s1,s2,s3,s4,s5,s6);
               s4 = s1,s5 = s2,s6 = s3;
               MulCbCr_Coef(s1,s2,s3,coeffCb);                
               MulCbCr_Coef(s4,s5,s6,coeffCr);
               
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos),s1);
               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos),s4);

                cbcrpos+=8;
                //Y = 77 * RVal + 150 * GVal + 29 * BVal + 128;
                MulBGR_Coef(regB,regG,regR,coeffY);
                MulBGR_Coef(regB1,regG1,regR1,coeffY);

                regB = _mm_sub_epi16(regB,val127);
                regG = _mm_sub_epi16(regG,val127);

                regB1 = _mm_sub_epi16(regB1,val127);
                regG1 = _mm_sub_epi16(regG1,val127);

               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG);
                dstYPos+=8;
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB1);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG1);
                dstYPos+=8;
      }
       
}
SYS_INLINE STATIC void fwiRGBToYCbCr411LS_MCU_8u16s_C3P3R_SSE (const Fw8u *pSrcBGR,int srcStep,Fw16s *pDstMCU[3])
    {
        XMM128 coeffY[3],coeffCb[3],coeffCr[3];
        coeffY[0].i = _mm_set1_epi16(77);
        coeffY[1].i = _mm_set1_epi16(150);
        coeffY[2].i = _mm_set1_epi16(29);

        //elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
        coeffCb[0].i = _mm_set1_epi16(-43);
        coeffCb[1].i = _mm_set1_epi16(-85);
        coeffCb[2].i = _mm_set1_epi16(128);
        //elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
        coeffCr[0].i = _mm_set1_epi16(128);
        coeffCr[1].i = _mm_set1_epi16(-107);
        coeffCr[2].i = _mm_set1_epi16(-21);

        const __m128i val127 = _mm_set1_epi16(127);
        int dstYPos = 0;
        int cbcrpos = 0;

        for(int height = 0;height < 8; height+=2)
        {
                __m128i regB = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32; 

                __m128i regB1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32;

               // CBL_SSE2::Convert_3C_to_3P_8bit(regB,regG,regR);
               // CBL_SSE2::Convert_3C_to_3P_8bit(regB1,regG1,regR1);
               ssp_convert_3c_3p_epi8(&regB, &regG, &regR);
               ssp_convert_3c_3p_epi8(&regB1, &regG1, &regR1);
               
               __m128i s1 = regB,s2=regG,s3=regR,s4=regB1,s5=regG1,s6=regR1;
               Add8u_to_16s(s1,s2,s3,s4,s5,s6);
               s4 = s1,s5 = s2,s6 = s3;
               MulCbCr_Coef(s1,s2,s3,coeffCb);                
               MulCbCr_Coef(s4,s5,s6,coeffCr);
               
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos),s1);
               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos),s4);

                cbcrpos+=8;
                //Y = 77 * RVal + 150 * GVal + 29 * BVal + 128;
                MulBGR_Coef(regB,regG,regR,coeffY);
                MulBGR_Coef(regB1,regG1,regR1,coeffY);

                regB = _mm_sub_epi16(regB,val127);
                regG = _mm_sub_epi16(regG,val127);

                regB1 = _mm_sub_epi16(regB1,val127);
                regG1 = _mm_sub_epi16(regG1,val127);

               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG);
                dstYPos+=8;
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB1);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG1);
                dstYPos+=8;
        }

       dstYPos = 128;
        for(int height = 0;height < 8; height+=2)
        {
                __m128i regB = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32; 

                __m128i regB1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32;

               // CBL_SSE2::Convert_3C_to_3P_8bit(regB,regG,regR);
               // CBL_SSE2::Convert_3C_to_3P_8bit(regB1,regG1,regR1);
               ssp_convert_3c_3p_epi8(&regB, &regG, &regR);
               ssp_convert_3c_3p_epi8(&regB1, &regG1, &regR1);
               
               __m128i s1 = regB,s2=regG,s3=regR,s4=regB1,s5=regG1,s6=regR1;
               Add8u_to_16s(s1,s2,s3,s4,s5,s6);
               s4 = s1,s5 = s2,s6 = s3;
               MulCbCr_Coef(s1,s2,s3,coeffCb);                
               MulCbCr_Coef(s4,s5,s6,coeffCr);
               
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos),s1);
               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos),s4);

                cbcrpos+=8;
                //Y = 77 * RVal + 150 * GVal + 29 * BVal + 128;
                MulBGR_Coef(regB,regG,regR,coeffY);
                MulBGR_Coef(regB1,regG1,regR1,coeffY);

                regB = _mm_sub_epi16(regB,val127);
                regG = _mm_sub_epi16(regG,val127);

                regB1 = _mm_sub_epi16(regB1,val127);
                regG1 = _mm_sub_epi16(regG1,val127);

               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG);
                dstYPos+=8;
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB1);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG1);
                dstYPos+=8;
      }
       
}

  SYS_INLINE STATIC void fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R_SSE (const Fw8u *pSrcBGR,int srcStep,Fw16s *pDstMCU[3])
    {
        XMM128 coeffY[3],coeffCb[3],coeffCr[3];
        coeffY[0].i = _mm_set1_epi16(29);
        coeffY[1].i = _mm_set1_epi16(150);
        coeffY[2].i = _mm_set1_epi16(77);

        //elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
        coeffCb[0].i = _mm_set1_epi16(128);
        coeffCb[1].i = _mm_set1_epi16(-85);
        coeffCb[2].i = _mm_set1_epi16(-43);
        //elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
        coeffCr[0].i = _mm_set1_epi16(-21);
        coeffCr[1].i = _mm_set1_epi16(-107);
        coeffCr[2].i = _mm_set1_epi16(128);

        const __m128i val127 = _mm_set1_epi16(127);
        int dstYPos = 0;
        int cbcrpos = 0;

        for(int height = 0;height < 8; height+=2)
        {
                __m128i regB = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32; 

                __m128i regB1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regG1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+=16;
                __m128i regR1 = _mm_loadu_si128((__m128i*) pSrcBGR);
                pSrcBGR+= srcStep-32;

               // CBL_SSE2::Convert_3C_to_3P_8bit(regB,regG,regR);
               // CBL_SSE2::Convert_3C_to_3P_8bit(regB1,regG1,regR1);
               ssp_convert_3c_3p_epi8(&regB, &regG, &regR);
               ssp_convert_3c_3p_epi8(&regB1, &regG1, &regR1);
               
               __m128i s1 = regB,s2=regG,s3=regR,s4=regB1,s5=regG1,s6=regR1;
               
               Add4228u_to_16s(s1,s2,s3,s4,s5,s6);
               __m128i ts1 = s1,ts2 = s2, ts3 = s3, ts4  = s4 , ts5 = s5, ts6 = s6;

               MulCbCr422_Coef(s1,s2,s3,coeffCb);      
               MulCbCr422_Coef(s4,s5,s6,coeffCb);  

               MulCbCr422_Coef(ts1,ts2,ts3,coeffCr);
               MulCbCr422_Coef(ts4,ts5,ts6,coeffCr);
               
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos),s1);
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos + 8),s4);

               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos),ts1);
               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos + 8),ts4);

                cbcrpos+=16;
                //Y = 77 * RVal + 150 * GVal + 29 * BVal + 128;
                MulBGR_Coef(regB,regG,regR,coeffY);
                MulBGR_Coef(regB1,regG1,regR1,coeffY);

                regB = _mm_sub_epi16(regB,val127);
                regG = _mm_sub_epi16(regG,val127);

                regB1 = _mm_sub_epi16(regB1,val127);
                regG1 = _mm_sub_epi16(regG1,val127);

               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG);
                dstYPos+=8;
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB1);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),regG1);
                dstYPos+=8;
        }
}

  SYS_INLINE STATIC void fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R_SSE (const Fw8u *pSrcRGB,int srcStep,Fw16s *pDstMCU[3])
    {
        XMM128 coeffY[3],coeffCb[3],coeffCr[3];
        coeffY[0].i = _mm_set1_epi16(77);
        coeffY[1].i = _mm_set1_epi16(150);
        coeffY[2].i = _mm_set1_epi16(29);

        //elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
        coeffCb[0].i = _mm_set1_epi16(-43);
        coeffCb[1].i = _mm_set1_epi16(-85);
        coeffCb[2].i = _mm_set1_epi16(128);
        //elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
        coeffCr[0].i = _mm_set1_epi16(128);
        coeffCr[1].i = _mm_set1_epi16(-107);
        coeffCr[2].i = _mm_set1_epi16(-21);

        const __m128i val127 = _mm_set1_epi16(127);
        int dstYPos = 0;
        int cbcrpos = 0;

        for(int height = 0;height < 8; height+=2)
        {
                __m128i colR = _mm_loadu_si128((__m128i*) pSrcRGB);
                pSrcRGB+=16;
                __m128i colG = _mm_loadu_si128((__m128i*) pSrcRGB);
                pSrcRGB+=16;
                __m128i colB = _mm_loadu_si128((__m128i*) pSrcRGB);
                pSrcRGB+= srcStep-32; 

                __m128i colR1 = _mm_loadu_si128((__m128i*) pSrcRGB);
                pSrcRGB+=16;
                __m128i colG1 = _mm_loadu_si128((__m128i*) pSrcRGB);
                pSrcRGB+=16;
                __m128i colB1 = _mm_loadu_si128((__m128i*) pSrcRGB);
                pSrcRGB+= srcStep-32;

               // CBL_SSE2::Convert_3C_to_3P_8bit(colR,colG,colB);
               // CBL_SSE2::Convert_3C_to_3P_8bit(colR1,colG1,colB1);
               ssp_convert_3c_3p_epi8(&colR, &colG, &colB);
               ssp_convert_3c_3p_epi8(&colR1, &colG1, &colB1);
               
               __m128i s1 = colR,s2=colG,s3=colB,s4=colR1,s5=colG1,s6=colB1;
               
               Add4228u_to_16s(s1,s2,s3,s4,s5,s6);
               __m128i ts1 = s1,ts2 = s2, ts3 = s3, ts4  = s4 , ts5 = s5, ts6 = s6;

               MulCbCr422_Coef(s1,s2,s3,coeffCb);      
               MulCbCr422_Coef(s4,s5,s6,coeffCb);  

               MulCbCr422_Coef(ts1,ts2,ts3,coeffCr);
               MulCbCr422_Coef(ts4,ts5,ts6,coeffCr);
               
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos),s1);
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos + 8),s4);

               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos),ts1);
               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos + 8),ts4);

                cbcrpos+=16;
                //Y = 77 * RVal + 150 * GVal + 29 * BVal + 128;
                MulBGR_Coef(colR,colG,colB,coeffY);
                MulBGR_Coef(colR1,colG1,colB1,coeffY);

                colR = _mm_sub_epi16(colR,val127);
                colG = _mm_sub_epi16(colG,val127);

                colR1 = _mm_sub_epi16(colR1,val127);
                colG1 = _mm_sub_epi16(colG1,val127);

               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),colR);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),colG);
                dstYPos+=8;
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),colR1);
               _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos + 64),colG1);
                dstYPos+=8;
        }
}


   SYS_INLINE STATIC void fwiBGRToYCbCr444LS_MCU_8u16s_C3P3R_SSE (const Fw8u *pSrcBGR,int srcStep,Fw16s *pDstMCU[3])
    {
        XMM128 coeffY[3],coeffCb[3],coeffCr[3];
        coeffY[0].i = _mm_set1_epi16(29);
        coeffY[1].i = _mm_set1_epi16(150);
        coeffY[2].i = _mm_set1_epi16(77);

        //elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
        coeffCb[0].i = _mm_set1_epi16(128);
        coeffCb[1].i = _mm_set1_epi16(-85);
        coeffCb[2].i = _mm_set1_epi16(-43);
        //elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
        coeffCr[0].i = _mm_set1_epi16(-21);
        coeffCr[1].i = _mm_set1_epi16(-107);
        coeffCr[2].i = _mm_set1_epi16(128);

        const __m128i val127 = _mm_set1_epi16(127);
        int dstYPos = 0;
        int cbcrpos = 0;




        for(int height = 0;height < 8; height++)
        {
                XMM128  regB,regG,regR;
                regB.i  = _mm_loadu_si128((__m128i*) pSrcBGR);
                regG.d = _mm_setzero_pd();
                regG.d  = _mm_loadl_pd(regG.d,(double*) (pSrcBGR + 16));
                pSrcBGR+=srcStep;

                // CBL_SSE2::Convert_3C_to_3P_8bit(regB.i,regG.i,regR.i);
                ssp_convert_3c_3p_epi8(&regB.i, &regG.i, &regR.i);
                
                __m128i s1 = regB.i,s2=regG.i,s3=regR.i,s4=regB.i,s5=regG.i,s6=regR.i;

               MulCbCr_CoefLo(s1,s2,s3,coeffCb);                
               MulCbCr_CoefLo(s4,s5,s6,coeffCr);
               
               _mm_storeu_si128((__m128i*) (pDstMCU[1] + cbcrpos),s1);
               _mm_storeu_si128((__m128i*) (pDstMCU[2] + cbcrpos),s4);

               cbcrpos+=8;
                //Y = 77 * RVal + 150 * GVal + 29 * BVal + 128;
                MulBGR_CoefLo(regB.i,regG.i,regR.i,coeffY);

                regB.i = _mm_sub_epi16(regB.i,val127);

                _mm_storeu_si128((__m128i*) (pDstMCU[0] + dstYPos),regB.i);
                dstYPos+=8;
        }

       
}


	static void fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block(
	const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3], int blocknum)
	{
		unsigned short RVal, GVal, BVal;
		int result, x, y, x1;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb[4]={0, 0, 0, 0}; 
		int elementCr[4]={0, 0, 0, 0};

		srcOffset  = ((blocknum&0x1)<<3) + ((blocknum&0x2)<<2)*srcStep;
		dstCOffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			if (!(y&1)) {
				for (x=0;x<8;x++) {				
					BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
					GVal=((pSrcBGR[srcPos]>>5)&0x3f)<<2;
					RVal=(pSrcBGR[srcPos++]>>11)<<3;

					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

					x1=x>>1;
					elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
				}
			} else {
				dstCPos = (y>>1)*8 + dstCOffset;
				for (x=0;x<8;x++) {
					BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
					GVal=((pSrcBGR[srcPos]>>5)&0x3f)<<2;
					RVal=(pSrcBGR[srcPos++]>>11)<<3;

					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

					x1=x>>1;
					if (!(x&1)) {
						elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
						elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
					} else {
						result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb[x1] + 512;
						pDstMCU [1][dstCPos] = (Fw16s)(result>>10);
						elementCb[x1] = 0; //clean after using the value
						result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr[x1] + 512;
						pDstMCU [2][dstCPos++] = (Fw16s)(result>>10);
						elementCr[x1] = 0;
					}	
				}
			}
		}

		return;
	}

	static void fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block(
	const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3], int blocknum)
	{
		unsigned short RVal, GVal, BVal;
		int result, x, y, x1;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb[4]={0, 0, 0, 0}; 
		int elementCr[4]={0, 0, 0, 0};

		srcOffset  = ((blocknum&0x1)<<3) + ((blocknum&0x2)<<2)*srcStep;
		dstCOffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			if (!(y&1)) {
				for (x=0;x<8;x++) {				
					BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
					GVal=((pSrcBGR[srcPos]>>5)&0x1f)<<3;
					RVal=(pSrcBGR[srcPos++]>>10)<<3;

					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

					x1=x>>1;
					elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
				}
			} else {
				dstCPos = (y>>1)*8 + dstCOffset;
				for (x=0;x<8;x++) {
					BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
					GVal=((pSrcBGR[srcPos]>>5)&0x1f)<<3;
					RVal=(pSrcBGR[srcPos++]>>10)<<3;

					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

					x1=x>>1;
					if (!(x&1)) {
						elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
						elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
					} else {
						result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb[x1] + 512;
						pDstMCU [1][dstCPos] = (Fw16s)(result>>10);
						elementCb[x1] = 0; //clean after using the value
						result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr[x1] + 512;
						pDstMCU [2][dstCPos++] = (Fw16s)(result>>10);
						elementCr[x1] = 0;
					}	
				}
			}
		}

		return;
	}

	static void fwiCMYKToYCCK422LS_MCU_8u16s_C4P4R_8x8Block(
	const Fw8u *pSrcCMYK, int srcStep, Fw16s *pDstMCU[4], int blocknum)
	{
		unsigned char RVal, GVal, BVal;
		int result, x, y;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb=0, elementCr=0;

		srcOffset  = blocknum << 5;
		dstCOffset = blocknum << 2;
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			dstCPos = y*8 + dstCOffset;
			for (x=0;x<8;x++) {
				RVal = ~(pSrcCMYK[srcPos++]);//R=255-C
				GVal = ~(pSrcCMYK[srcPos++]);//G=255-M
				BVal = ~(pSrcCMYK[srcPos++]);//B=255-Y
				result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
				pDstMCU [0][dstYPos] = (Fw16s)((result>>8)-128);
				pDstMCU [3][dstYPos++] = pSrcCMYK[srcPos++]-128;
				if (!(x&1)){
					elementCb = -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr = 128 * RVal - 107 * GVal - 21 * BVal;
				} else {
					result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb + 256;
					pDstMCU [1][dstCPos] = (Fw16s)(result>>9);
					result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr + 256;
					pDstMCU [2][dstCPos++] = (Fw16s)(result>>9);
				}	
			}
		}

		return;
	}

	static void fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R_8x8Block(
	const Fw8u *pSrcCMYK, int srcStep, Fw16s *pDstMCU[4], int blocknum)
	{
		unsigned char RVal, GVal, BVal;
		int result, x, y, x1;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb[4]={0, 0, 0, 0}; 
		int elementCr[4]={0, 0, 0, 0};

		srcOffset  = ((blocknum&0x1)<<5) + ((blocknum&0x2)<<2)*srcStep;
		dstCOffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			if (!(y&1)) {
				for (x=0;x<8;x++) {
					RVal = ~(pSrcCMYK[srcPos++]);//R=255-C
					GVal = ~(pSrcCMYK[srcPos++]);//G=255-M
					BVal = ~(pSrcCMYK[srcPos++]);//B=255-Y
					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos] = (Fw16s)((result>>8)-128);
					pDstMCU [3][dstYPos++] = pSrcCMYK[srcPos++]-128;
					x1=x>>1;
					elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
				}
			} else {
				dstCPos = (y>>1)*8 + dstCOffset;
				for (x=0;x<8;x++) {
					RVal = ~(pSrcCMYK[srcPos++]);//R=255-C
					GVal = ~(pSrcCMYK[srcPos++]);//G=255-M
					BVal = ~(pSrcCMYK[srcPos++]);//B=255-Y
					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstMCU [0][dstYPos] = (Fw16s)((result>>8)-128);
					pDstMCU [3][dstYPos++] = pSrcCMYK[srcPos++]-128;
					x1=x>>1;
					if (!(x&1)) {
						elementCb[x1] += -43 * RVal - 85 * GVal + 128 * BVal;
						elementCr[x1] += 128 * RVal - 107 * GVal - 21 * BVal;
					} else {
						result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb[x1] + 512;
						pDstMCU [1][dstCPos] = (Fw16s)(result>>10);
						elementCb[x1] = 0; //clean after using the value
						result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr[x1] + 512;
						pDstMCU [2][dstCPos++] = (Fw16s)(result>>10);
						elementCr[x1] = 0;
					}	
				}
			}
		}

		return;
	}

	//-----------------------------------------------------------------------
	//Convert an 16*8 RGB image to the YCbCr color mode and create 422MCU
	//
	//-----------------------------------------------------------------------
	static void fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R_8x8Block(
	const Fw8u *pSrcRGB, int srcStep, Fw16s *pDstMCU[3], int blocknum)
	{
		unsigned char RVal, GVal, BVal;
		int result, x, y;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb=0, elementCr=0;

		srcOffset  = (blocknum<<3)*3;
		dstCOffset = blocknum << 2;
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
		    for (y=0;y<8; y++) {
			    srcPos = y*srcStep + srcOffset;
			    dstCPos = y*8 + dstCOffset;
			    for (x=0;x<8;x++) {
				    RVal = pSrcRGB[srcPos++];
				    GVal = pSrcRGB[srcPos++];
				    BVal = pSrcRGB[srcPos++];
				    result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
				    pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

				    if (!(x&1)){
					    elementCb = -43 * RVal - 85 * GVal + 128 * BVal;
					    elementCr = 128 * RVal - 107 * GVal - 21 * BVal;
				    } else {
					    result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb + 256;
					    pDstMCU [1][dstCPos] = (Fw16s)(result>>9);
					    result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr + 256;
					    pDstMCU [2][dstCPos++] = (Fw16s)(result>>9);
				    }	
			    }
		    }
		return;
	}

	//-----------------------------------------------------------------------
	//Convert an 16*8 BGR image to the YCbCr color mode and create 422MCU
	//
	//-----------------------------------------------------------------------
	static void fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R_8x8Block(
	const Fw8u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3], int blocknum)
	{
		unsigned char RVal, GVal, BVal;
		int result, x, y;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb=0, elementCr=0;

		srcOffset  = (blocknum<<3)*3;
		dstCOffset = blocknum << 2;
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			dstCPos = y*8 + dstCOffset;
			for (x=0;x<8;x++) {
				BVal = pSrcBGR[srcPos++];
				GVal = pSrcBGR[srcPos++];
				RVal = pSrcBGR[srcPos++];
				result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
				pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

				if (!(x&1)){
					elementCb = -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr = 128 * RVal - 107 * GVal - 21 * BVal;
				} else {
					result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb + 256;
					pDstMCU [1][dstCPos] = (Fw16s)(result>>9);
					result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr + 256;
					pDstMCU [2][dstCPos++] = (Fw16s)(result>>9);
				}	
			}
		}

		return;
	}

	static void fwiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R_8x8Block(
	const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3], int blocknum)
	{
		unsigned short RVal, GVal, BVal;
		int result, x, y;
		int srcPos, srcOffset, dstYPos, dstCOffset, dstCPos;
		int elementCb=0, elementCr=0;

		srcOffset  = blocknum << 3;
		dstCOffset = blocknum << 2;
		dstYPos    = blocknum << 6;

		//DEV code use shift 8 bit data for coeffcients
		//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
		//We use 77, 150, 29 as the modified coeff, and then shift the result
		//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
		//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536

		for (y=0;y<8; y++) {
			srcPos = y*srcStep + srcOffset;
			dstCPos = y*8 + dstCOffset;
			for (x=0;x<8;x++) {		
				BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
				GVal=((pSrcBGR[srcPos]>>5)&0x3f)<<2;
				RVal=(pSrcBGR[srcPos++]>>11)<<3;

				result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
				pDstMCU [0][dstYPos++] = (Fw16s)((result>>8)-128);

				if (!(x&1)){
					elementCb = -43 * RVal - 85 * GVal + 128 * BVal;
					elementCr = 128 * RVal - 107 * GVal - 21 * BVal;
				} else {
					result = -43 * RVal - 85 * GVal + 128 * BVal + elementCb + 256;
					pDstMCU [1][dstCPos] = (Fw16s)(result>>9);
					result = 128 * RVal - 107 * GVal - 21 * BVal + elementCr + 256;
					pDstMCU [2][dstCPos++] = (Fw16s)(result>>9);
				}	
			}
		}

		return;
	}

};

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToRGBLS_MCU_16s8u_P3C3R)( const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep )
{
	if (pDstRGB==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 || pSrcMCU[1]==0 || pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	// will not allow dstStep overlap
	if (dstStep < 48) return fwStsStepErr;

	/*switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		return iYCbCr411ToRGBLS_MCU_16s8u_P3C3R_SSE2( pSrcMCU, pDstRGB, dstStep );
	default:*/
		return iYCbCr411ToRGBLS_MCU_16s8u_P3C3R( pSrcMCU, pDstRGB, dstStep );	
	//}
}

//-----------------------------------------------------------------------
//Convert an 8*8 RGB image to the YCbCr color mode and create 444MCU
//
//-----------------------------------------------------------------------

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr444LS_MCU_8u16s_C3P3R)(const Fw8u *pSrcRGB, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcRGB==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);
	unsigned short RVal, GVal, BVal;
	int x, y, srcPos, result;

    switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		//SSE Code goes here
        {
            XMM128 rVal = {0} , gVal = {0}, bVal = {0};
                XMM128 xmm1, xmm2, xmm3, xmm128;
                xmm128.i = _mm_set1_epi16(128);
		        for (y=0;y<8; y++) {
			        srcPos = y*srcStep;
			        for (x=0;x<8;x++) {
				        rVal.u16[x] = pSrcRGB[srcPos++];
				        gVal.u16[x] = pSrcRGB[srcPos++];
				        bVal.u16[x] = pSrcRGB[srcPos++];
                    }
			        //add 0.5 for nearest neighbor rounding
                    xmm1.i = _mm_set1_epi16(77);
                    xmm2.i = _mm_set1_epi16(150);
                    xmm3.i = _mm_set1_epi16(29);
                    
                    xmm1.i = _mm_mullo_epi16(xmm1.i, rVal.i);
                    xmm2.i = _mm_mullo_epi16(xmm2.i, gVal.i);

                    xmm3.i = _mm_mullo_epi16(xmm3.i, bVal.i);
                    xmm1.i = _mm_add_epi16(xmm1.i, xmm3.i);
                    xmm1.i = _mm_add_epi16(xmm1.i, xmm128.i);//adding 128
                    xmm1.i = _mm_srli_epi16(xmm1.i, 4);
                    xmm2.i = _mm_srli_epi16(xmm2.i, 4);

                    xmm1.i = _mm_add_epi16(xmm1.i, xmm2.i);
                    xmm1.i = _mm_srli_epi16(xmm1.i, 4);

                    xmm1.i = _mm_sub_epi16(xmm1.i, xmm128.i);

                    _mm_storeu_si128  ((__m128i *)(&pDstMCU[0][y*8]),xmm1.i);	

                    xmm1.i = _mm_set1_epi16(-43);
                    xmm2.i = _mm_set1_epi16(-85);

                    xmm1.i = _mm_mullo_epi16(xmm1.i, rVal.i);
                    xmm2.i = _mm_mullo_epi16(xmm2.i, gVal.i);

                    xmm3.i = _mm_mullo_epi16(xmm128.i, bVal.i);
                    xmm1.i = _mm_add_epi16(xmm1.i, xmm2.i);            
                    xmm1.i = _mm_add_epi16(xmm1.i, xmm3.i);
                    xmm1.i = _mm_add_epi16(xmm1.i, xmm128.i);//adding 128
                    xmm1.i = _mm_srai_epi16(xmm1.i, 8);

                    _mm_storeu_si128  ((__m128i *)(&pDstMCU[1][y*8]),xmm1.i);	

                    xmm2.i = _mm_set1_epi16(-107);
                    xmm3.i = _mm_set1_epi16(-21);

                    rVal.i = _mm_mullo_epi16(rVal.i, xmm128.i);
                    xmm2.i = _mm_mullo_epi16(xmm2.i, gVal.i);

                    xmm3.i = _mm_mullo_epi16(xmm3.i, bVal.i);
                    rVal.i = _mm_add_epi16(rVal.i, xmm2.i);            
                    rVal.i = _mm_add_epi16(rVal.i, xmm3.i);
                    rVal.i = _mm_add_epi16(rVal.i, xmm128.i);//adding 128
                    rVal.i = _mm_srai_epi16(rVal.i, 8);

                    _mm_storeu_si128  ((__m128i *)(&pDstMCU[2][y*8]),rVal.i);	
	        }
            return fwStsNoErr;
        }
        //break;
	default:

	    for (y=0;y<8; y++) {//8*8 image
		    srcPos = y*srcStep;
		    for (x=0;x<8;x++) {
			    RVal=pSrcRGB[srcPos++]; 
			    GVal=pSrcRGB[srcPos++];
			    BVal=pSrcRGB[srcPos++];
			    //add 0.5 for nearest neighbor rounding
			    result =  77 * RVal + 150 * GVal + 29 * BVal + 128;
			    pDstMCU [0][x+y*8] = (Fw16s)((result>>8)-128);
			    result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			    pDstMCU [1][x+y*8] = (Fw16s)(result>>8);
			    result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			    pDstMCU [2][x+y*8] = (Fw16s)(result>>8);
		    }
	    }
    }
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R)(const Fw8u *pSrcRGB, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcRGB==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	// RGB:				 Y, K:		 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//					  ----------
	// Proces each block independently

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R_SSE(pSrcRGB, srcStep, pDstMCU);
            break;
    default:
	fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R_8x8Block (pSrcRGB, srcStep, pDstMCU, 0);
	fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R_8x8Block (pSrcRGB, srcStep, pDstMCU, 1);
    }
	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//Convert an 16*16 RGB image to the YCbCr color mode and create 411MCU
//
//-----------------------------------------------------------------------
//Optimized version of the code


FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr411LS_MCU_8u16s_C3P3R)(const Fw8u *pSrcRGB, int srcStep, Fw16s *pDstMCU[3])
{

     if (pSrcRGB==0 || pDstMCU==0) return fwStsNullPtrErr;
	    if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		    return fwStsNullPtrErr;
	    STEPCHECK1(srcStep);
	    // 8x8 blocks:
	    //				---------
	    //				| A | B |
	    //				---------
	    //				| C | D |
	    //				---------
	    // Y:								Cb:								Cr:
	    //		---------						-------------					-------------
	    //		|   Ay  |						|A_cb | B_cb|					|A_cr | B_cr|
	    //		|   By  |						-------------					-------------
	    //		|   Cy  |						|C_cb | D_cb|					|C_cr | D_cr|
	    //		|   Dy  |						-------------					-------------
	    //		---------

    switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		//SSE Code goes here
        fwiRGBToYCbCr411LS_MCU_8u16s_C3P3R_SSE(pSrcRGB, srcStep, pDstMCU);
        break;
	default:

	   
	    A16U rVal, gVal, bVal;
	    A16S result;
	    A32U rSum[8] = {0,0,0,0,0,0,0,0}, gSum[8] = {0,0,0,0,0,0,0,0}, bSum[8] = {0,0,0,0,0,0,0,0};
	    A32S x, y, srcPos, dstYPos=0, dstCPos=0;

	    // Process 2 8x8 block: A & B
	    // Proces 2 rows at a time							//  Pixels
	    for (y=0; y<8; ++y)									//  -------			---------	------	------
	    {													//  | a b |	  -->	| ay by |	| cb |	| cr |
		    // First Row									//	| c d |			| cy dy	|	------	------
		    srcPos = y*srcStep;								//	-------			---------
		    for (x=0; x<8; x+=2)
		    {												
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1]+=rVal;
			    gSum[x>>1]+=gVal;
			    bSum[x>>1]+=bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1]+=rVal;
			    gSum[x>>1]+=gVal;
			    bSum[x>>1]+=bVal;
		    }
		    dstYPos = dstYPos+56;
		    for ( ; x<16; x+=2)
		    {
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
		    }
		    dstYPos = dstYPos-64;
    		
		    // Second Row
		    srcPos = (++y)*srcStep;
		    for (x=0; x<8; x+=2)
		    {												
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
		    }
		    dstYPos = dstYPos+56;
		    for ( ; x<16; x+=2)
		    {
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
		    }
		    dstYPos = dstYPos-64;
    		
		    // Compute and store Cb & Cr Values
		    for (x=0; x<8; ++x)
		    {
			    rSum[x] = rSum[x]>>2;	// Average of 4 values
			    gSum[x] = gSum[x]>>2;	// Average of 4 values
			    bSum[x] = bSum[x]>>2;	// Average of 4 values
			    result = (A16S)(-43 * rSum[x] - 85 * gSum[x] + 128 * bSum[x] + 128);
			    pDstMCU[1][dstCPos] = (result>>8);		// cb
			    result = (A16S)(128 * rSum[x] - 107 * gSum[x] - 21 * bSum[x] + 128);
			    pDstMCU[2][dstCPos++] = (result>>8);	// cr
			    rSum[x] = gSum[x] = bSum[x] = 0;
		    }
	    }
	    // Process 2 8x8 block: C & D
	    dstYPos = 128;
	    for ( ; y<16; ++y)
	    {
		    // First Row
		    srcPos = y*srcStep;
		    for (x=0; x<8; x+=2)
		    {												
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
		    }
		    dstYPos = dstYPos+56;
		    for ( ; x<16; x+=2)
		    {
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
		    }
		    dstYPos = dstYPos-64;
    		
		    // Second Row
		    srcPos = (++y)*srcStep;
		    for (x=0; x<8; x+=2)
		    {												
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
		    }
		    dstYPos = dstYPos+56;
		    for ( ; x<16; x+=2)
		    {
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (ay,cy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
    			
			    rVal = pSrcRGB[srcPos++];
			    gVal = pSrcRGB[srcPos++];
			    bVal = pSrcRGB[srcPos++];
			    result = (rVal*77 + gVal*150 + bVal*29) >> 8;
			    pDstMCU[0][dstYPos++] = (result - 128);			// (by,dy)
			    rSum[x>>1] += rVal;
			    gSum[x>>1] += gVal;
			    bSum[x>>1] += bVal;
		    }
		    dstYPos = dstYPos-64;
    		
		    // Compute and store Cb & Cr
		    for (x=0; x<8; ++x)
		    {
			    rSum[x] = rSum[x]>>2;	// Average of 4 values
			    gSum[x] = gSum[x]>>2;	// Average of 4 values
			    bSum[x] = bSum[x]>>2;	// Average of 4 values
			    result = (A16S)(-43 * rSum[x] - 85 * gSum[x] + 128 * bSum[x] + 128);
			    pDstMCU[1][dstCPos] = (result>>8);		// cb
			    result = (A16S)(128 * rSum[x] - 107 * gSum[x] - 21 * bSum[x] + 128);
			    pDstMCU[2][dstCPos++] = (result>>8);	// cr
			    rSum[x] = gSum[x] = bSum[x] = 0;
		    }
	    }
    }//end of switch
	    return fwStsNoErr;
}

//-----------------------------------------------------------------------
//Convert an 8*8 BGR image to the YCbCr color mode and create 444MCU
//
//-----------------------------------------------------------------------

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToYCbCr444LS_MCU_8u16s_C3P3R)(const Fw8u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);



	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		fwiBGRToYCbCr444LS_MCU_8u16s_C3P3R_SSE(pSrcBGR, srcStep, pDstMCU);
            break;
	default:
	    unsigned short RVal, GVal, BVal;
	    int x, y, srcPos, result;

	    for (y=0;y<8; y++) {//8*8 image
		    srcPos = y*srcStep;
		    for (x=0;x<8;x++) {
			    BVal=pSrcBGR[srcPos++]; 
			    GVal=pSrcBGR[srcPos++];
			    RVal=pSrcBGR[srcPos++];

			    //add 0.5 for nearest neighbor rounding
			    result =  77 * RVal + 150 * GVal + 29 * BVal + 128;
			    pDstMCU [0][x+y*8] = (Fw16s)((result>>8)-128);
			    result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			    pDstMCU [1][x+y*8] = (Fw16s)(result>>8);
			    result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			    pDstMCU [2][x+y*8] = (Fw16s)(result>>8);
		    }
	    }
    }
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR565ToYCbCr444LS_MCU_16u16s_C3P3R)(const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);
	unsigned short RVal, GVal, BVal;
	int x, y, srcPos, result;

	//srcStep is byte size
	srcStep = srcStep / sizeof (Fw16u);

	for (y=0;y<8; y++) {//8*8 image
		srcPos = y*srcStep;
		for (x=0;x<8;x++) {
			//extend the RGB range to [0-255]
			BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
			GVal=((pSrcBGR[srcPos]>>5)&0x3f)<<2;
			RVal=(pSrcBGR[srcPos++]>>11)<<3;

			//add 0.5 for nearest neighbor rounding
			result =  77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstMCU [0][x+y*8] = (Fw16s)((result>>8)-128);
			result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			pDstMCU [1][x+y*8] = (Fw16s)(result>>8);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstMCU [2][x+y*8] = (Fw16s)(result>>8);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR555ToYCbCr444LS_MCU_16u16s_C3P3R)(const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);
	unsigned short RVal, GVal, BVal;
	int x, y, srcPos, result;

	//srcStep is byte size
	srcStep = srcStep / sizeof (Fw16u);

	for (y=0;y<8; y++) {//8*8 image
		srcPos = y*srcStep;
		for (x=0;x<8;x++) {
			//extend the RGB range to [0-255]
			BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
			GVal=((pSrcBGR[srcPos]>>5)&0x1f)<<3;
			RVal=((pSrcBGR[srcPos++]>>10)&0x1f)<<3;

			//add 0.5 for nearest neighbor rounding
			result =  77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstMCU [0][x+y*8] = (Fw16s)((result>>8)-128);
			result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			pDstMCU [1][x+y*8] = (Fw16s)(result>>8);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstMCU [2][x+y*8] = (Fw16s)(result>>8);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R)(const Fw8u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	// RGB:				 Y, K:		 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//					  ----------
	// Proces each block independently

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R_SSE(pSrcBGR, srcStep, pDstMCU);
            break;
    default:
	    fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 0);
	    fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 1);
    }
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R)(const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	//srcStep is byte size
	srcStep = srcStep / sizeof (Fw16u);

	// RGB:				 Y, K:		 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//					  ----------
	// Proces each block independently

	fwiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 0);
	fwiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 1);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R)(const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	//srcStep is byte size
	srcStep = srcStep / sizeof (Fw16u);

	// RGB:				 Y, K:		 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//					  ----------
	// Proces each block independently

	fwiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 0);
	fwiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 1);

	return fwStsNoErr;
}




FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R)(const Fw8u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	// RGB:				 Y:			 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	4 |A_cb | B_cb|	4 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//	8 | C | D |	==>	8 |   Cy   |	4 |C_cb | D_cb|	4 |C_cr | D_cr|
	//	---------	==>	8 |   Dy   |	-------------	-------------	
	//					  ----------
	// Proces each block independently

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R_SSE(pSrcBGR, srcStep, pDstMCU);
            break;
	default:
        fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 0);
        fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 1);
        fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 2);
        fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 3);
	}




	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R)(const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	//srcStep is byte size
	srcStep = srcStep / sizeof (Fw16u);

	// RGB:				 Y:			 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	4 |A_cb | B_cb|	4 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//	8 | C | D |	==>	8 |   Cy   |	4 |C_cb | D_cb|	4 |C_cr | D_cr|
	//	---------	==>	8 |   Dy   |	-------------	-------------	
	//					  ----------
	// Proces each block independently

	fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 0);
	fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 1);
	fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 2);
	fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 3);

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//Converts a CMYK 8*8, 16*8, 16*16 image to YCCK and creates corresponding
//MCU image.
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK444LS_MCU_8u16s_C4P4R)(const Fw8u *pSrcCMYK, int srcStep, Fw16s *pDstMCU[4])
{

	if (pSrcCMYK==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0 ||
		pDstMCU[3]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	int result;
	unsigned char RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<8; y++) {
		srcPos = y*srcStep;
		dstPos = y*8;
		for (x=0;x<8;x++) {
			RVal = ~(pSrcCMYK[srcPos++]);//R=255-C
			GVal = ~(pSrcCMYK[srcPos++]);//G=255-M
			BVal = ~(pSrcCMYK[srcPos++]);//B=255-Y

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstMCU [0][dstPos] = (Fw16s)((result>>8)-128);
			result = -43 * RVal - 85 * GVal + 128 * BVal + 128;
			pDstMCU [1][dstPos] = (Fw16s)(result>>8);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstMCU [2][dstPos] = (Fw16s)(result>>8);
			pDstMCU [3][dstPos++] = pSrcCMYK[srcPos++]-128;
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R)(const Fw16u *pSrcBGR, int srcStep, Fw16s *pDstMCU[3])
{

	if (pSrcBGR==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	//srcStep is byte size
	srcStep = srcStep / sizeof (Fw16u);

	// RGB:				 Y:			 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	4 |A_cb | B_cb|	4 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//	8 | C | D |	==>	8 |   Cy   |	4 |C_cb | D_cb|	4 |C_cr | D_cr|
	//	---------	==>	8 |   Dy   |	-------------	-------------	
	//					  ----------
	// Proces each block independently

	fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 0);
	fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 1);
	fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 2);
	fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R_8x8Block (pSrcBGR, srcStep, pDstMCU, 3);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK422LS_MCU_8u16s_C4P4R)(const Fw8u *pSrcCMYK, int srcStep, Fw16s *pDstMCU[4])
{

	if (pSrcCMYK==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0 ||
		pDstMCU[3]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	// RGB:				 Y, K:		 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//					  ----------
	// Proces each block independently

	fwiCMYKToYCCK422LS_MCU_8u16s_C4P4R_8x8Block (pSrcCMYK, srcStep, pDstMCU, 0);
	fwiCMYKToYCCK422LS_MCU_8u16s_C4P4R_8x8Block (pSrcCMYK, srcStep, pDstMCU, 1);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R)(const Fw8u *pSrcCMYK, int srcStep, Fw16s *pDstMCU[4])
{

	if (pSrcCMYK==0 || pDstMCU==0) return fwStsNullPtrErr;
	if (pDstMCU[0]==0 ||pDstMCU[1]==0 ||pDstMCU[2]==0 ||
		pDstMCU[3]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(srcStep);

	// RGB:				 Y, K:		 Cb:				Cr:				
	//	--8---8--	==>	  ----8----	     ---4-----4---	---4-----4---	  
	//	8 | A | B |	==>	8 |   Ay   |	4 |A_cb | B_cb|	4 |A_cr | B_cr|
	//	---------	==>	8 |   By   |	-------------	-------------	  
	//	8 | C | D |	==>	8 |   Cy   |	4 |C_cb | D_cb|	4 |C_cr | D_cr|
	//	---------	==>	8 |   Dy   |	-------------	-------------	
	//					  ----------
	// Proces each block independently

	fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R_8x8Block (pSrcCMYK, srcStep, pDstMCU, 0);
	fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R_8x8Block (pSrcCMYK, srcStep, pDstMCU, 1);
	fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R_8x8Block (pSrcCMYK, srcStep, pDstMCU, 2);
	fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R_8x8Block (pSrcCMYK, srcStep, pDstMCU, 3);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToRGBLS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3], Fw8u *pDstRGB, int dstStep)
{
	if (pDstRGB==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 || pSrcMCU[1]==0 || pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	if (dstStep<24)		// Make sure destination buffer can write at least 8 pixels in one row.
		return fwStsStepErr;

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		return iYCbCr444ToRGBLS_MCU_16s8u_P3C3R_SSE2( pSrcMCU, pDstRGB, dstStep );
	default:
		return iYCbCr444ToRGBLS_MCU_16s8u_P3C3R_REF( pSrcMCU, pDstRGB, dstStep );	
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGBLS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw8u *pDstRGB, int dstStep)
{
	if (pDstRGB==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	// Y:			    	Cb:				Cr:					RGB:
	//		  ----8----	     ---4-----4---	---4-----4---	==>	  --8---8--
	//		8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|	==>	8 | A | B |
	//		8 |   By   |	-------------	-------------	==>	  ---------
	//		  ----------

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		return iYCbCr422ToRGBLS_MCU_16s8u_P3C3R_SSE2( pSrcMCU, pDstRGB, dstStep );
	default:
        // Proces each block independently
        fwiYCbCr4xxToRGBLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDstRGB, dstStep, 0, 1, 0);
        fwiYCbCr4xxToRGBLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDstRGB, dstStep, 1, 1, 0);
	}	



	return fwStsNoErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToBGRLS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw8u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	// Y:			    	Cb:			Cr:				RGB:
	//		  ----8----	     ---8---	---8----	==>	  --8--
	//		8 |   Ay   |	8 |A_cb ||	8 |A_cr |	==>	8 | A |
	//		  ---------	    ---------	--------	==>	  -----
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		return iYCbCr444ToBGRLS_MCU_16s8u_P3C3R_SSE2( pSrcMCU, pDstBGR, dstStep );
	default:
		fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 0, 0, 0);
	}	

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToBGR565LS_MCU_16s16u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw16u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	//dstStep is byte size
	dstStep = dstStep / sizeof (Fw16u);

	// Y:			    	Cb:			Cr:				RGB:
	//		  ----8----	     ---8---	---8----	==>	  --8--
	//		8 |   Ay   |	8 |A_cb ||	8 |A_cr |	==>	8 | A |
	//		  ---------	    ---------	--------	==>	  -----

	fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 0, 0, 0);

	return fwStsNoErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToBGR555LS_MCU_16s16u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw16u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	//dstStep is byte size
	dstStep = dstStep / sizeof (Fw16u);

	// Y:			    	Cb:			Cr:				RGB:
	//		  ----8----	     ---8---	---8----	==>	  --8--
	//		8 |   Ay   |	8 |A_cb ||	8 |A_cr |	==>	8 | A |
	//		  ---------	    ---------	--------	==>	  -----

	fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 0, 0, 0);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGRLS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw8u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	// Y:			    	Cb:				Cr:					RGB:
	//		  ----8----	     ---4-----4---	---4-----4---	==>	  --8---8--
	//		8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|	==>	8 | A | B |
	//		8 |   By   |	-------------	-------------	==>	  ---------
	//		  ----------


	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		return iYCbCr422ToBGRLS_MCU_16s8u_P3C3R_SSE2( pSrcMCU, pDstBGR, dstStep );
	default:
        // Proces each block independently
        fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 0, 1, 0);
        fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 1, 1, 0);
	}	


	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR565LS_MCU_16s16u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw16u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	//dstStep is byte size
	dstStep = dstStep / sizeof (Fw16u);

	// Y:			    	Cb:				Cr:					RGB:
	//		  ----8----	     ---4-----4---	---4-----4---	==>	  --8---8--
	//		8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|	==>	8 | A | B |
	//		8 |   By   |	-------------	-------------	==>	  ---------
	//		  ----------
	// Proces each block independently
	fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 0, 1, 0);
	fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 1, 1, 0);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR555LS_MCU_16s16u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw16u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	//dstStep is byte size
	dstStep = dstStep / sizeof (Fw16u);

	// Y:			    	Cb:				Cr:					RGB:
	//		  ----8----	     ---4-----4---	---4-----4---	==>	  --8---8--
	//		8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|	==>	8 | A | B |
	//		8 |   By   |	-------------	-------------	==>	  ---------
	//		  ----------
	// Proces each block independently
	fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 0, 1, 0);
	fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block (pSrcMCU, pDstBGR, dstStep, 1, 1, 0);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGRLS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw8u *pDstBGR, int dstStep)
{

	//if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	//if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
	//	return fwStsNullPtrErr;
	//STEPCHECK1(dstStep);
	//	
	//// Y:			    	CbCr:								RGB:
	////		  ----8----		  ---4-----4---		  	==>			  --8---8--
	////		8 |   Ay   |		4 |A_c | B_c|		==>			8 | A | B |
	////		8 |   By   |		  -------------		==>			  ---------
	////		8 |   Cy   |		4 |C_c | D_c|		==>			8 | C | D |
	////		8 |   Dy   |		  -------------		==>			  ---------
	////		  ----------
	//// Proces each block independently
	//fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 0, 1, 1);
	//fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 1, 1, 1);
	//fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 2, 1, 1);
	//fwiYCbCr4xxToBGRLS_MCU_16s8u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 3, 1, 1);


	//return fwStsNoErr;

    if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 || pSrcMCU[1]==0 || pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	// will not allow dstStep overlap
	if (dstStep < 48) return fwStsStepErr;
	return iYCbCr411ToBGRLS_MCU_16s8u_P3C3R( pSrcMCU, pDstBGR, dstStep );	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGR565LS_MCU_16s16u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw16u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	//dstStep is byte size
	dstStep = dstStep / sizeof (Fw16u);		

	// Y:   				CbCr:								RGB:
	//		  ----8----		  ---4-----4---		  	==>			  --8---8--
	//		8 |   Ay   |		4 |A_c | B_c|		==>			8 | A | B |
	//		8 |   By   |		  -------------		==>			  ---------
	//		8 |   Cy   |		4 |C_c | D_c|		==>			8 | C | D |
	//		8 |   Dy   |		  -------------		==>			  ---------
	//		  ----------
	// Proces each block independently

	fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 0, 1, 1);
	fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 1, 1, 1);
	fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 2, 1, 1);
	fwiYCbCr4xxToBGR565LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 3, 1, 1);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGR555LS_MCU_16s16u_P3C3R)(const Fw16s *pSrcMCU[3],
						Fw16u *pDstBGR, int dstStep)
{

	if (pDstBGR==0 || pSrcMCU==0) return fwStsNullPtrErr;
	if (pSrcMCU[0]==0 ||pSrcMCU[1]==0 ||pSrcMCU[2]==0)
		return fwStsNullPtrErr;
	STEPCHECK1(dstStep);

	//dstStep is byte size
	dstStep = dstStep / sizeof (Fw16u);

	// Y:    				CbCr:								RGB:
	//		  ----8----		  ---4-----4---		  	==>			  --8---8--
	//		8 |   Ay   |		4 |A_c | B_c|		==>			8 | A | B |
	//		8 |   By   |		  -------------		==>			  ---------
	//		8 |   Cy   |		4 |C_c | D_c|		==>			8 | C | D |
	//		8 |   Dy   |		  -------------		==>			  ---------
	//		  ----------
	// Proces each block independently

	fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 0, 1, 1);
	fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 1, 1, 1);
	fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 2, 1, 1);
	fwiYCbCr4xxToBGR555LS_MCU_16s16u_P3C3R_8x8Block(pSrcMCU, pDstBGR, dstStep, 3, 1, 1);

	return fwStsNoErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCK444ToCMYKLS_MCU_16s8u_P4C4R)(const Fw16s *pSrcMCU[4],
			Fw8u *pDstCMYK, int dstStep)
{
	if (pSrcMCU == 0 || pDstCMYK == 0) return fwStsNullPtrErr;
	if (pSrcMCU[0] == 0 || pSrcMCU[1] == 0 || pSrcMCU[2] == 0 || 
		pSrcMCU[3] == 0)
		return fwStsNullPtrErr;

	STEPCHECK1(dstStep);

	// Y:			    	Cb:			Cr:				RGB:
	//		  ----8----	     ---8---	---8----	==>	  --8--
	//		8 |   Ay   |	8 |A_cb ||	8 |A_cr |	==>	8 | A |
	//		  ---------	    ---------	--------	==>	  -----
	
	fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (pSrcMCU, pDstCMYK, dstStep, 0, 0, 0);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCK422ToCMYKLS_MCU_16s8u_P4C4R)(const Fw16s *pSrcMCU[4],
			Fw8u *pDstCMYK, int dstStep)
{
	if (pSrcMCU == 0 || pDstCMYK == 0) return fwStsNullPtrErr;
	if (pSrcMCU[0] == 0 || pSrcMCU[1] == 0 || pSrcMCU[2] == 0 || 
		pSrcMCU[3] == 0)
		return fwStsNullPtrErr;

	STEPCHECK1(dstStep);

	// Y, K:				Cb:				Cr:					RGB:
	//		  ----8----	     ---4-----4---	---4-----4---	==>	  --8---8--
	//		8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|	==>	8 | A | B |
	//		8 |   By   |	-------------	-------------	==>	  ---------
	//		  ----------
	// Proces each block independently
	fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (pSrcMCU, pDstCMYK, dstStep, 0, 1, 0);
	fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (pSrcMCU, pDstCMYK, dstStep, 1, 1, 0);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCK411ToCMYKLS_MCU_16s8u_P4C4R)(const Fw16s *pSrcMCU[4],
			Fw8u *pDstCMYK, int dstStep)
{
	if (pSrcMCU == 0 || pDstCMYK == 0) return fwStsNullPtrErr;
	if (pSrcMCU[0] == 0 || pSrcMCU[1] == 0 || pSrcMCU[2] == 0 || 
		pSrcMCU[3] == 0)
		return fwStsNullPtrErr;

	STEPCHECK1(dstStep);
		
	// Y, K:				CbCr:								RGB:
	//		  ----8----		  ---4-----4---		  	==>			  --8---8--
	//		8 |   Ay   |		4 |A_c | B_c|		==>			8 | A | B |
	//		8 |   By   |		  -------------		==>			  ---------
	//		8 |   Cy   |		4 |C_c | D_c|		==>			8 | C | D |
	//		8 |   Dy   |		  -------------		==>			  ---------
	//		  ----------
	// Proces each block independently
	fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (pSrcMCU, pDstCMYK, dstStep, 0, 1, 1);
	fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (pSrcMCU, pDstCMYK, dstStep, 1, 1, 1);
	fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (pSrcMCU, pDstCMYK, dstStep, 2, 1, 1);
	fwiYCCK4xxToCMYKLS_MCU_16s8u_P4C4R_8x8Block (pSrcMCU, pDstCMYK, dstStep, 3, 1, 1);

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
