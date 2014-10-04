﻿//======================================================================
//-----------------------------------------------------------------------
/**
 * @file		iutest_defs.hpp
 * @brief		iris unit test definition
 *
 * @author		t.shirayanagi
 * @par			copyright
 * Copyright (C) 2011-2014, Takazumi Shirayanagi\n
 * This software is released under the new BSD License,
 * see LICENSE
*/
//-----------------------------------------------------------------------
//======================================================================
#ifndef INCG_IRIS_IUTEST_DEFS_HPP_839F06DB_E0B6_4E6A_84F2_D99C0A44E06C_
#define INCG_IRIS_IUTEST_DEFS_HPP_839F06DB_E0B6_4E6A_84F2_D99C0A44E06C_

//======================================================================
// include
#include "internal/iutest_compiler.hpp"
#include "internal/iutest_pp.hpp"
#include "internal/iutest_stdlib.hpp"
#include "iutest_config.hpp"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "internal/iutest_debug.hpp"

#if IUTEST_HAS_TYPED_TEST_P
#  if IUTEST_TYPED_TEST_P_STRICT
#    include <set>
#  endif
#endif

namespace iutest
{

//======================================================================
// struct
namespace detail
{

/**
 * @brief	type_least_t
*/
template<int SIZE>
struct type_least_t {};

/** type_least_t<1> */
template<>
struct type_least_t<1>
{
	typedef char				Int;
	typedef unsigned char		UInt;
};

/** type_least_t<2> */
template<>
struct type_least_t<2>
{
	typedef short				Int;
	typedef unsigned short		UInt;
};

/** type_least_t<4> */
template<>
struct type_least_t<4>
{
	typedef int					Int;
	typedef unsigned int		UInt;
};

/** type_least_t<8> */
template<>
struct type_least_t<8>
{
#if defined(_MSC_VER)
	typedef __int64				Int;
	typedef unsigned __int64	UInt;
#else
	typedef long long			Int;
	typedef unsigned long long	UInt;
#endif
};

}

namespace internal
{

typedef void* TypeId;		//!< テスト識別型

namespace helper
{

//======================================================================
// struct
/**
 * @brief	TypeId Generator Implementation
*/
template<typename T>
struct TestTypeIdHelper { public: static bool _dummy; };

template<typename T>bool TestTypeIdHelper<T>::_dummy = false;

}

//======================================================================
// function
/**
 * @internal
 * @brief	TypeId Generator
*/
template<typename T>
inline TypeId GetTypeId(void)
{
	return &(helper::TestTypeIdHelper<T>::_dummy);
}

/**
 * @internal
 * @brief	TypeId Generator
*/
inline IUTEST_CXX_CONSTEXPR TypeId GetTestTypeId(void)
{
	return 0;
}

}

//======================================================================
// class
namespace detail
{

/**
 * @internal
 * @brief	IEEE754 floating point bits
*/
template<typename T>
struct ieee754_bits {};

template<>
struct ieee754_bits<float>
{
	enum {
		  EXP = 8
		, FRAC = 23
	};
};

template<>
struct ieee754_bits<double>
{
	enum {
		  EXP = 11
		, FRAC = 52
	};
};

}

/**
 * @brief	浮動小数点数
*/
template<typename RawType>
class floating_point
{
private:
	typedef floating_point<RawType> _Myt;

	typedef typename detail::type_least_t<sizeof(RawType)> type;
	typedef typename type::Int	Int;
	typedef typename type::UInt	UInt;
	union FInt
	{
		Int		iv;
		UInt	uv;
		RawType	fv;
	};

public:
	/**
	 * @brief	コンストラクタ
	*/
	floating_point(void)
	{
		m_v.uv = 0;
	}

	/**
	 * @brief	コンストラクタ
	 * @param [in]	f	= 浮動小数点数
	*/
	floating_point(RawType f)
	{
		m_v.fv = f;
	}

	/**
	 * @brief	コンストラクタ
	*/
	floating_point(const _Myt& rhs)
		: m_v(rhs.m_v)
	{
	}

public:
	/**
	 * @brief	浮動小数点数がほぼ一致するかどうか
	*/
	bool	AlmostEquals(const _Myt& rhs) const
	{
		if( is_nan() || rhs.is_nan() ) return false;
		return NanSensitiveAlmostEquals(rhs);
	}

	/**
	 * @brief	浮動小数点数がほぼ一致するかどうか
	 * @sa		http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
	*/
	bool	NanSensitiveAlmostEquals(const _Myt& rhs) const
	{
		const UInt v1 = norm(m_v.uv);
		const UInt v2 = norm(rhs.m_v.uv);
		const UInt diff = (v1 > v2) ? v1 - v2 : v2 - v1;
		if( diff <= kMaxUlps )
		{
			return true;
		}
		return false;
	}

public:
	/**
	 * @brief	ビット列の取得
	*/
	UInt	bits(void) const { return m_v.uv; }

	/**
	 * @brief	raw データの取得
	*/
	RawType	raw(void) const { return m_v.fv; }

	/**
	 * @brief	exponent
	*/
	UInt	exponent_bits(void) const { return m_v.uv & kExpMask; }

	/**
	 * @brief	fraction
	*/
	UInt	fraction_bits(void) const { return m_v.uv & kFracMask; }

	/**
	 * @brief	sign
	*/
	UInt	sign_bit(void) const { return m_v.uv & kSignMask; }

	/**
	 * @brief	is nan
	*/
	bool	is_nan(void) const { return exponent_bits() == kExpMask && fraction_bits() != 0; }

public:
	//! plus inf
	static _Myt PINF(void)
	{
		_Myt f;
		f.m_v.uv = kExpMask;
		return f;
	}
	//! minus inf
	static _Myt NINF(void)
	{
		_Myt f = PINF();
		f.m_v.uv |= kSignMask;
		return f;
	}
	//! plus nan
	static _Myt PNAN(void)
	{
		_Myt f;
		f.m_v.uv = kExpMask | 1;
		return f;
	}
	//! minus nan
	static _Myt NNAN(void)
	{
		_Myt f = PNAN();
		f.m_v.uv |= kSignMask;
		return f;
	}
	//! plus qnan
	static _Myt PQNAN(void)
	{
		_Myt f;
		f.m_v.uv = ((1 << (kEXP + 1)) - 1);
		f.m_v.uv <<= kFRAC - 1;
		return f;
	}
	//! minus qnan
	static _Myt NQNAN(void)
	{
		_Myt f = PQNAN();
		f.m_v.uv |= kSignMask;
		return f;
	}

public:
#if !defined(_MSC_VER) || _MSC_VER >= 1310
	operator RawType (void) const	{ return m_v.fv; }	//!< cast to RawType
#else
	operator float  (void) const	{ return m_v.fv; }
	operator double (void) const	{ return m_v.fv; }
#endif
	_Myt&	operator = (RawType f)	{ m_v.fv = f; return *this; }	//!< 代入

	bool	operator == (const _Myt& rhs) const	{ return m_v.uv == rhs.m_v.uv; }	//!< 比較

private:
	enum
	{
		  kEXP = detail::ieee754_bits<RawType>::EXP
		, kFRAC = detail::ieee754_bits<RawType>::FRAC
		, kMaxUlps = 4
	};

private:

	static UInt norm(UInt v) { return v & kSignMask ? ~v + 1 : v | kSignMask; }

#if !defined(IUTEST_NO_INCLASS_MEMBER_INITIALIZATION)
	static const UInt kSignMask = static_cast<UInt>(1u) << (kEXP + kFRAC);
	static const UInt kExpMask = ((static_cast<UInt>(1u) << kEXP) - 1) << kFRAC;
	static const UInt kFracMask = (static_cast<UInt>(1u) << kFRAC) - 1;
#else
	static const UInt kSignMask;
	static const UInt kExpMask;
	static const UInt kFracMask;
#endif

private:
	FInt m_v;
};

#if defined(IUTEST_NO_INCLASS_MEMBER_INITIALIZATION)

template<typename T>
const typename floating_point<T>::UInt floating_point<T>::kSignMask
	= static_cast<typename floating_point<T>::UInt>(1u) << (kEXP + kFRAC);
template<typename T>
const typename floating_point<T>::UInt floating_point<T>::kExpMask
	= ((static_cast<typename floating_point<T>::UInt>(1u) << floating_point<T>::kEXP) - 1) << floating_point<T>::kFRAC;
template<typename T>
const typename floating_point<T>::UInt floating_point<T>::kExpMask
	= ((static_cast<typename floating_point<T>::UInt>(1u) << floating_point<T>::kFRAC) - 1);

#endif

//======================================================================
// typedef
typedef detail::type_least_t<4>::Int	Int32;	//!< 32 bit 符号付き整数型
typedef detail::type_least_t<4>::UInt	UInt32;	//!< 32 bit 符号なし整数型
typedef detail::type_least_t<8>::Int	Int64;	//!< 64 bit 符号付き整数型
typedef detail::type_least_t<8>::UInt	UInt64;	//!< 64 bit 符号なし整数型

typedef internal::TypeId TestTypeId;	//!< テスト識別型

typedef void (*SetUpMethod)(void);		//!< SetUp 関数型
typedef void (*TearDownMethod)(void);	//!< TearDown 関数型

typedef detail::type_least_t<8>::UInt	TimeInMillisec;	//!< ミリ秒単位を扱う型
typedef detail::type_least_t<8>::Int	BiggestInt;		//!< Biggest Int

}	// end of namespace iutest

#endif // INCG_IRIS_IUTEST_DEFS_HPP_839F06DB_E0B6_4E6A_84F2_D99C0A44E06C_
