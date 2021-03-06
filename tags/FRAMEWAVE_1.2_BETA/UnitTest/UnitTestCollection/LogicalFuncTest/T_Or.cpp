/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation
class TestOr_8u_C1R : public SrcSrcDstRoi< U8, 1, U8, 1 >
{
public:
    TestOr_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 1, U8, 1 >( parent, "fwiOr_8u_C1R", fwiOr_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "0 0 11 33 34 56 123 MAX", "0 1 31 63 98 56 255 MAX");
    }
};

class TestOr_8u_C3R : public SrcSrcDstRoi< U8, 3, U8, 3 >
{
public:
    TestOr_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 3, U8, 3 >( parent, "fwiOr_8u_C3R", fwiOr_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX MAX", "0 0 11 33 34 56 123 MAX 134", "0 1 31 63 98 56 255 MAX MAX");
    }
};

class TestOr_8u_C4R : public SrcSrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestOr_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 4, U8, 4 >( parent, "fwiOr_8u_C4R", fwiOr_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "0 0 11 33 34 56 123 MAX", "0 1 31 63 98 56 255 MAX");
    }
};

class TestOr_8u_AC4R : public SrcSrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestOr_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 4, U8, 4 >( parent, "fwiOr_8u_AC4R", fwiOr_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "0 0 11 33 34 56 123 MAX", "0 1 31 0 98 56 255 0");
    }
};

class TestOr_16u_C1R : public SrcSrcDstRoi< U16, 1, U16, 1 >
{
public:
    TestOr_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 1, U16, 1 >( parent, "fwiOr_16u_C1R", fwiOr_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "0 0 1111 3311 34112 56123 123 MAX", 
			"0 1 3327 7423 64855 56123 255 MAX");
    }
};

class TestOr_16u_C3R : public SrcSrcDstRoi< U16, 3, U16, 3 >
{
public:
    TestOr_16u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 3, U16, 3 >( parent, "fwiOr_16u_C3R", fwiOr_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX 78", "0 0 1111 3311 34112 56123 123 MAX 123", 
			"0 1 3327 7423 64855 56123 255 MAX 127");
    }
};

class TestOr_16u_C4R : public SrcSrcDstRoi< U16, 4, U16, 4 >
{
public:
    TestOr_16u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 4, U16, 4 >( parent, "fwiOr_16u_C4R", fwiOr_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "0 0 1111 3311 34112 56123 123 MAX", 
			"0 1 3327 7423 64855 56123 255 MAX");
    }
};

class TestOr_16u_AC4R : public SrcSrcDstRoi< U16, 4, U16, 4 >
{
public:
    TestOr_16u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 4, U16, 4 >( parent, "fwiOr_16u_AC4R", fwiOr_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "0 0 1111 3311 34112 56123 123 MAX", 
			"0 1 3327 0 64855 56123 255 0");
    }
};

class TestOr_32s_C1R : public SrcSrcDstRoi< S32, 1, S32, 1 >
{
public:
    TestOr_32s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 1, S32, 1 >( parent, "fwiOr_32s_C1R", fwiOr_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN", "0 -1 -1111 3311 -34112 -56123 1234567890 MAX", 
			"0 -1 -1105 -4113 -297 -39483 1234567934 -1");
    }
};

class TestOr_32s_C3R : public SrcSrcDstRoi< S32, 3, S32, 3 >
{
public:
    TestOr_32s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 3, S32, 3 >( parent, "fwiOr_32s_C3R", fwiOr_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN 1234567890", 
			"0 -1 -1111 3311 -34112 -56123 1234567890 MAX 987654321", 
			"0 -1 -1105 -4113 -297 -39483 1234567934 -1 2078173939");
    }
};

class TestOr_32s_C4R : public SrcSrcDstRoi< S32, 4, S32, 4 >
{
public:
    TestOr_32s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 4, S32, 4 >( parent, "fwiOr_32s_C4R", fwiOr_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN", "0 -1 -1111 3311 -34112 -56123 1234567890 MAX", 
			"0 -1 -1105 -4113 -297 -39483 1234567934 -1");
    }
};

class TestOr_32s_AC4R : public SrcSrcDstRoi< S32, 4, S32, 4 >
{
public:
    TestOr_32s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 4, S32, 4 >( parent, "fwiOr_32s_AC4R", fwiOr_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN", "0 -1 -1111 3311 -34112 -56123 1234567890 MAX", 
			"0 -1 -1105 0 -297 -39483 1234567934 0");
    }
};

//Case 2: In-place operation
class TestOr_8u_C1IR : public SrcSDstRoi< U8, 1, U8, 1 >
{
public:
    TestOr_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 1, U8, 1 >( parent, "fwiOr_8u_C1IR", fwiOr_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "0 0 11 33 34 56 123 MAX", "0 1 31 63 98 56 255 MAX");
    }
};

class TestOr_8u_C3IR : public SrcSDstRoi< U8, 3, U8, 3 >
{
public:
    TestOr_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 3, U8, 3 >( parent, "fwiOr_8u_C3IR", fwiOr_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX MAX", "0 0 11 33 34 56 123 MAX 134", "0 1 31 63 98 56 255 MAX MAX");
    }
};

class TestOr_8u_C4IR : public SrcSDstRoi< U8, 4, U8, 4 >
{
public:
    TestOr_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 4, U8, 4 >( parent, "fwiOr_8u_C4IR", fwiOr_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "0 0 11 33 34 56 123 MAX", "0 1 31 63 98 56 255 MAX");
    }
};

class TestOr_8u_AC4IR : public SrcSDstRoi< U8, 4, U8, 4 >
{
public:
    TestOr_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 4, U8, 4 >( parent, "fwiOr_8u_AC4IR", fwiOr_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "0 0 11 33 34 56 123 MAX", "0 1 31 33 98 56 255 MAX");
    }
};

class TestOr_16u_C1IR : public SrcSDstRoi< U16, 1, U16, 1 >
{
public:
    TestOr_16u_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 1, U16, 1 >( parent, "fwiOr_16u_C1IR", fwiOr_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "0 0 1111 3311 34112 56123 123 MAX", 
			"0 1 3327 7423 64855 56123 255 MAX");
    }
};

class TestOr_16u_C3IR : public SrcSDstRoi< U16, 3, U16, 3 >
{
public:
    TestOr_16u_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 3, U16, 3 >( parent, "fwiOr_16u_C3IR", fwiOr_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX 78", "0 0 1111 3311 34112 56123 123 MAX 123", 
			"0 1 3327 7423 64855 56123 255 MAX 127");
    }
};

class TestOr_16u_C4IR : public SrcSDstRoi< U16, 4, U16, 4 >
{
public:
    TestOr_16u_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 4, U16, 4 >( parent, "fwiOr_16u_C4IR", fwiOr_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "0 0 1111 3311 34112 56123 123 MAX", 
			"0 1 3327 7423 64855 56123 255 MAX");
    }
};

class TestOr_16u_AC4IR : public SrcSDstRoi< U16, 4, U16, 4 >
{
public:
    TestOr_16u_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 4, U16, 4 >( parent, "fwiOr_16u_AC4IR", fwiOr_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "0 0 1111 3311 34112 56123 123 MAX", 
			"0 1 3327 3311 64855 56123 255 MAX");
    }
};

class TestOr_32s_C1IR : public SrcSDstRoi< S32, 1, S32, 1 >
{
public:
    TestOr_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 1, S32, 1 >( parent, "fwiOr_32s_C1IR", fwiOr_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN", "0 -1 -1111 3311 -34112 -56123 1234567890 MAX", 
			"0 -1 -1105 -4113 -297 -39483 1234567934 -1");
    }
};

class TestOr_32s_C3IR : public SrcSDstRoi< S32, 3, S32, 3 >
{
public:
    TestOr_32s_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 3, S32, 3 >( parent, "fwiOr_32s_C3IR", fwiOr_32s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN 1234567890", 
			"0 -1 -1111 3311 -34112 -56123 1234567890 MAX 987654321", 
			"0 -1 -1105 -4113 -297 -39483 1234567934 -1 2078173939");
    }
};

class TestOr_32s_C4IR : public SrcSDstRoi< S32, 4, S32, 4 >
{
public:
    TestOr_32s_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 4, S32, 4 >( parent, "fwiOr_32s_C4IR", fwiOr_32s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN", "0 -1 -1111 3311 -34112 -56123 1234567890 MAX", 
			"0 -1 -1105 -4113 -297 -39483 1234567934 -1");
    }
};

class TestOr_32s_AC4IR : public SrcSDstRoi< S32, 4, S32, 4 >
{
public:
    TestOr_32s_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 4, S32, 4 >( parent, "fwiOr_32s_AC4IR", fwiOr_32s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN", "0 -1 -1111 3311 -34112 -56123 1234567890 MAX", 
			"0 -1 -1105 3311 -297 -39483 1234567934 MAX");
    }
};

DEFINE_TEST_TABLE( OrTestCatalog )
//Case 1
TEST_ENTRY( TestOr_8u_C1R )
TEST_ENTRY( TestOr_8u_C3R )
TEST_ENTRY( TestOr_8u_C4R )
TEST_ENTRY( TestOr_8u_AC4R )
TEST_ENTRY( TestOr_16u_C1R )
TEST_ENTRY( TestOr_16u_C3R )
TEST_ENTRY( TestOr_16u_C4R )
TEST_ENTRY( TestOr_16u_AC4R )
TEST_ENTRY( TestOr_32s_C1R )
TEST_ENTRY( TestOr_32s_C3R )
TEST_ENTRY( TestOr_32s_C4R )
TEST_ENTRY( TestOr_32s_AC4R )

//Case 2
TEST_ENTRY( TestOr_8u_C1IR )
TEST_ENTRY( TestOr_8u_C3IR )
TEST_ENTRY( TestOr_8u_C4IR )
TEST_ENTRY( TestOr_8u_AC4IR )
TEST_ENTRY( TestOr_16u_C1IR )
TEST_ENTRY( TestOr_16u_C3IR )
TEST_ENTRY( TestOr_16u_C4IR )
TEST_ENTRY( TestOr_16u_AC4IR )
TEST_ENTRY( TestOr_32s_C1IR )
TEST_ENTRY( TestOr_32s_C3IR )
TEST_ENTRY( TestOr_32s_C4IR )
TEST_ENTRY( TestOr_32s_AC4IR )

END_TEST_TABLE()
