﻿//======================================================================
//-----------------------------------------------------------------------
/**
 * @file		iutest_matcher.hpp
 * @brief		iris unit test matcher 定義 ファイル
 *
 * @author		t.shirayanagi
 * @par			copyright
 * Copyright (C) 2014, Takazumi Shirayanagi\n
 * This software is released under the new BSD License,
 * see LICENSE
*/
//-----------------------------------------------------------------------
//======================================================================
#ifndef INCG_IRIS_IUTEST_MATCHER_HPP_23746E00_1A4B_4778_91AD_45C6DEFEEFA7_
#define INCG_IRIS_IUTEST_MATCHER_HPP_23746E00_1A4B_4778_91AD_45C6DEFEEFA7_

#if IUTEST_HAS_MATCHERS

/**
 * @defgroup	MATCHERS 	MATCHERS
 * @brief		Matchers
*/

//======================================================================
// define
/**
 * @private
 * @{
*/
#define IUTEST_TEST_THAT(actual, matcher, on_failure)					\
	IUTEST_AMBIGUOUS_ELSE_BLOCKER_										\
	if( ::iutest::AssertionResult iutest_ar = matcher(actual) ) {		\
	} else																\
		on_failure(::iutest::detail::MatcherAssertionFailureMessage(	\
			::iutest::PrintToString(actual).c_str(), #matcher, iutest_ar))

/**
 * @}
*/

namespace iutest {
namespace detail
{

//======================================================================
// function
/**
 * @brief	Matcher Assertion Failure Message
*/
inline ::std::string MatcherAssertionFailureMessage(const char* actual, const char* matcher_str, const AssertionResult& ar)
{
	iu_global_format_stringstream strm;
	strm << "error: Expected: " << matcher_str
		<< "\n  Actual: " << actual
		<< "\nWhich is: " << ar.message();
	return strm.str();
}
	
//======================================================================
// class

/**
 * @brief	matcher interface
*/
class IMatcher
{
public:
	template<typename T>
	struct is_matcher : public iutest_type_traits::is_base_of<IMatcher, T> {};
public:
	virtual ~IMatcher(void) {}
	virtual ::std::string WhichIs(void) const = 0;
};

inline iu_ostream& operator << (iu_ostream& os, const IMatcher& msg)
{
	return os << msg.WhichIs();
}

/**
 * @private
 * @{
 */

#define DECL_COMPARE_MATCHER(name, op)	\
	template<typename T>class IUTEST_PP_CAT(name, Matcher): public IMatcher{\
	public:	IUTEST_PP_CAT(name, Matcher)(const T& v) : m_expected(v) {}		\
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE {					\
		iu_global_format_stringstream strm;									\
		strm << #name ": " << m_expected; return strm.str();				\
	}																		\
	template<typename U>AssertionResult operator ()(const U& actual) const {\
		if( actual op m_expected ) return AssertionSuccess();				\
		return AssertionFailure() << WhichIs();								\
	}																		\
	private: IUTEST_PP_DISALLOW_ASSIGN(IUTEST_PP_CAT(name, Matcher));		\
	const T& m_expected;													\
	}

#define DECL_COMPARE_MATCHER2(name, op)	\
	class IUTEST_PP_CAT(Twofold, IUTEST_PP_CAT(name, Matcher)): public IMatcher{		\
	public:	IUTEST_PP_CAT(Twofold, IUTEST_PP_CAT(name, Matcher))() {}					\
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE { return #name; }				\
	template<typename T, typename U>AssertionResult operator ()							\
		(const T& actual, const U& expected) const {									\
		if( actual op expected ) return AssertionSuccess();								\
		return AssertionFailure() << WhichIs() << ": " << actual << " vs " << expected;	\
	} private:																			\
		IUTEST_PP_DISALLOW_ASSIGN(IUTEST_PP_CAT(Twofold, IUTEST_PP_CAT(name, Matcher)));\
	}


IUTEST_PARGMA_WARN_PUSH()
IUTEST_PRAGMA_WARN_DISABLE_SIGN_COMPARE()

DECL_COMPARE_MATCHER(Ne, !=);
DECL_COMPARE_MATCHER(Le, <=);
DECL_COMPARE_MATCHER(Lt, < );
DECL_COMPARE_MATCHER(Ge, >=);
DECL_COMPARE_MATCHER(Gt, > );

DECL_COMPARE_MATCHER2(Eq, ==);
DECL_COMPARE_MATCHER2(Ne, !=);
DECL_COMPARE_MATCHER2(Le, <=);
DECL_COMPARE_MATCHER2(Lt, < );
DECL_COMPARE_MATCHER2(Ge, >=);
DECL_COMPARE_MATCHER2(Gt, > );

IUTEST_PARGMA_WARN_POP()

#undef DECL_COMPARE_MATCHER
#undef DECL_COMPARE_MATCHER2

#define DECL_STR_COMPARE_MATCHER(name)	\
	template<typename T>class IUTEST_PP_CAT(name, Matcher): public IMatcher {	\
	public: IUTEST_PP_CAT(name, Matcher)(const T& value) : m_expected(value) {}	\
	template<typename U>AssertionResult operator ()(const U& actual) const {	\
		if( internal::IUTEST_PP_CAT(name, Helper)::Compare(						\
			actual, m_expected) ) {	return AssertionSuccess(); }				\
		return AssertionFailure() << WhichIs();									\
	}																			\
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE {						\
		iu_global_format_stringstream strm; strm << #name ": " << m_expected;	\
		return strm.str();														\
	}																			\
	private: IUTEST_PP_DISALLOW_ASSIGN(IUTEST_PP_CAT(name, Matcher));			\
	const T& m_expected;														\
	}

DECL_STR_COMPARE_MATCHER(StrEq);
DECL_STR_COMPARE_MATCHER(StrNe);
DECL_STR_COMPARE_MATCHER(StrCaseEq);
DECL_STR_COMPARE_MATCHER(StrCaseNe);

#undef DECL_COMPARE_MATCHER

/**
 * @}
*/

/**
 * @brief	IsNull matcher
*/
class IsNullMatcher : public IMatcher
{
public:
	IsNullMatcher() {}
public:
	template<typename U>
	AssertionResult operator ()(const U* actual) const
	{
		if( actual == NULL ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return "Is Null";
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(IsNullMatcher);
};

/**
 * @brief	NotNull matcher
*/
class NotNullMatcher : public IMatcher
{
public:
	NotNullMatcher() {}
public:
	template<typename U>
	AssertionResult operator ()(const U* actual) const
	{
		if( actual != NULL ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return "Not Null";
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(NotNullMatcher);
};

/**
 * @brief	Floating point Eq matcher
*/
template<typename T>
class FloatingPointEqMatcher : public IMatcher
{
public:
	FloatingPointEqMatcher(const T& value) : m_expected(value) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual) const
	{
		floating_point<T> f2(actual);
		if( m_expected.AlmostEquals(f2) )
		{
			return AssertionSuccess();
		}
		return AssertionFailure() << WhichIs();
	}

	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Eq: " << PrintToString(m_expected);
		return strm.str();
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(FloatingPointEqMatcher);

	floating_point<T> m_expected;
};

/**
 * @brief	Floating point Eq matcher (NanSensitive)
*/
template<typename T>
class NanSensitiveFloatingPointEqMatcher : public IMatcher
{
public:
	NanSensitiveFloatingPointEqMatcher(const T& value) : m_expected(value) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual) const
	{
		floating_point<T> f2(actual);
		if( m_expected.NanSensitiveAlmostEquals(f2) )
		{
			return AssertionSuccess();
		}
		return AssertionFailure() << WhichIs();
	}

	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "NanSensitive Eq: " << PrintToString(m_expected);
		return strm.str();
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(NanSensitiveFloatingPointEqMatcher);

	floating_point<T> m_expected;
};

/**
 * @brief	StartsWith matcher
*/
template<typename T>
class StartsWithMatcher : public IMatcher
{
public:
	StartsWithMatcher(T str) : m_str(str) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual) const
	{
		if( StartsWith(actual, m_str) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "StartsWith: " << m_str;
		return strm.str();
	}
private:
	static bool StartsWith(const char* actual, const char* start)
	{
		return strstr(actual, start) == actual;
	}
	static bool StartsWith(const ::std::string& actual, const char* start)
	{
		const char* p = actual.c_str();
		return StartsWith(p, start);
	}
	static bool StartsWith(const char* actual, const ::std::string& start)
	{
		const char* p = start.c_str();
		return StartsWith(actual, p);
	}
	static bool StartsWith(const ::std::string& actual, const ::std::string& start)
	{
		const char* p = start.c_str();
		return StartsWith(actual, p);
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(StartsWithMatcher);

	T m_str;
};

/**
 * @brief	Has substr matcher
*/
template<typename T>
class HasSubstrMatcher : public IMatcher
{
public:
	HasSubstrMatcher(T expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual) const
	{
		if( HasSubstr(actual, m_expected) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "HasSubstr: " << m_expected;
		return strm.str();
	}
private:
	static bool HasSubstr(const char* actual, const char* expected)
	{
		return strstr(actual, expected) != NULL;
	}
	static bool HasSubstr(const ::std::string& actual, const char* expected)
	{
		const char* p = actual.c_str();
		return HasSubstr(p, expected);
	}
	static bool HasSubstr(const char* actual, const ::std::string& expected)
	{
		const char* p = expected.c_str();
		return HasSubstr(actual, p);
	}
	static bool HasSubstr(const ::std::string& actual, const ::std::string& expected)
	{
		const char* p = expected.c_str();
		return HasSubstr(actual, p);
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(HasSubstrMatcher);

	T m_expected;
};

/**
 * @brief	EndsWith matcher
*/
template<typename T>
class EndsWithMatcher : public IMatcher
{
public:
	EndsWithMatcher(T str) : m_str(str) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual) const
	{
		if( EndsWith(actual, m_str) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "EndsWith: " << m_str;
		return strm.str();
	}
private:
	static bool EndsWith(const char* actual, const char* end)
	{
		const size_t len = strlen(end);
		const size_t actual_len = strlen(actual);
		if( len > actual_len ) return false;
		const char* p = actual + actual_len - 1;
		const char* q = end + len - 1;
		for( size_t i=0; i < len; ++i, --p, --q )
		{
			if( *p != *q ) return false;
		}	
		return true;
	}
	static bool EndsWith(const ::std::string& actual, const char* end)
	{
		const char* p = actual.c_str();
		return EndsWith(p, end);
	}
	static bool EndsWith(const char* actual, const ::std::string& end)
	{
		const char* p = end.c_str();
		return EndsWith(actual, p);
	}
	static bool EndsWith(const ::std::string& actual, const ::std::string& end)
	{
		const char* p = end.c_str();
		return EndsWith(actual, p);
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(EndsWithMatcher);

	T m_str;
};

/**
 * @brief	Equals matcher
*/
template<typename T>
class EqMatcher : public IMatcher
{
public:
	EqMatcher(const T& expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual) const
	{
		if( Equals(actual, m_expected) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Eq: " << m_expected;
		return strm.str();
	}
private:

	template<typename A, typename B>
	static bool Equals(const A& actual, const B& expected)
	{
IUTEST_PARGMA_WARN_PUSH()
IUTEST_PRAGMA_WARN_DISABLE_SIGN_COMPARE()
		return actual == expected;
IUTEST_PARGMA_WARN_POP()
	}
	static bool Equals(const char* actual, const char* expected)
	{
		return strcmp(actual, expected) == 0;
	}
	static bool Equals(const ::std::string& actual, const char* expected)
	{
		const char* p = actual.c_str();
		return Equals(p, expected);
	}
	static bool Equals(const ::std::string& actual, const ::std::string& expected)
	{
		const char* p = expected.c_str();
		return Equals(actual, p);
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(EqMatcher);

	const T& m_expected;
};

/**
 * @brief	TypedEq matcher
*/
template<typename T>
class TypedEqMatcher : public EqMatcher<T>
{
public:
	TypedEqMatcher(T expected) : EqMatcher<T>(m_expected), m_expected(expected) {}
public:
	AssertionResult operator ()(const T& actual)
	{
		return EqMatcher<T>::operator ()(actual);
	}
	template<typename U>
	AssertionResult operator ()(const U&) const;

private:
	IUTEST_PP_DISALLOW_ASSIGN(TypedEqMatcher);

	T m_expected;
};

/**
 * @brief	Cast to matcher
*/
template<typename T>
T& CastToMatcher(T& matcher
	, typename detail::enable_if_t< IMatcher::is_matcher<T> >::type*& = detail::enabler::value)
{
	return matcher;
}
/** @overload */
template<typename T>
EqMatcher<T> CastToMatcher(const T& value
	, typename detail::disable_if_t< IMatcher::is_matcher<T> >::type*& = detail::enabler::value)
{
	return EqMatcher<T>(value);
}


/**
 * @brief	Contains matcher
*/
template<typename T>
class ContainsMatcher : public IMatcher
{
public:
	ContainsMatcher(const T& expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Contains(actual, m_expected) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Contains: " << m_expected;
		return strm.str();
	}
private:
	template<typename TT, typename Container>
	bool Contains(const Container& actual, TT& expected)
	{
		return ContainsContainer(actual.begin(), actual.end(), expected);
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename TT, typename U, size_t SIZE>
	bool Contains(const U(&actual)[SIZE], TT& expected)
	{
		return ContainsContainer(actual, actual + SIZE, expected);
	}
#endif

	template<typename TT, typename Ite>
	bool ContainsContainer(Ite begin, Ite end, TT& expected)
	{
		for( Ite it = begin; it != end; ++it )
		{
			if( CastToMatcher(expected)(*it) )
			{
				return true;
			}
		}
		return false;
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(ContainsMatcher);

	T m_expected;
};

/**
 * @brief	Each matcher
*/
template<typename T>
class EachMatcher : public IMatcher
{
public:
	EachMatcher(const T& expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Each(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Each: " << m_expected;
		return strm.str();
	}
private:
	template<typename Container>
	bool Each(const Container& actual)
	{
		return EachContainer(actual.begin(), actual.end());
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename U, size_t SIZE>
	bool Each(const U(&actual)[SIZE])
	{
		return EachContainer(actual, actual + SIZE);
	}
#endif

	template<typename Ite>
	bool EachContainer(Ite begin, Ite end)
	{
		for( Ite it = begin; it != end; ++it )
		{
			if( !CastToMatcher(m_expected)(*it) )
			{
				return false;
			}
		}
		return true;
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(EachMatcher);

	T m_expected;
};

/**
 * @brief	ContainerEq matcher
*/
template<typename T>
class ContainerEqMatcher : public IMatcher
{
public:
	ContainerEqMatcher(const T& expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Check(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "ContainerEq: " << PrintToString(m_expected);
		strm << " (" << m_whichIs << ")";
		return strm.str();
	}
private:
	template<typename Container>
	bool Check(const Container& actual)
	{
		return Check(m_expected, actual.begin(), actual.end());
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename U, size_t SIZE>
	bool Check(const U(&actual)[SIZE])
	{
		return Check(m_expected, actual, actual + SIZE);
	}
#endif

	template<typename Container, typename Ite>
	bool Check(const Container& expected, Ite b2, Ite e2)
	{
		return CheckContainer(expected.begin(), expected.end(), b2, e2);
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename U, size_t SIZE, typename Ite>
	bool Check(const U(&expected)[SIZE], Ite b2, Ite e2)
	{
		return CheckContainer(expected, expected + SIZE, b2, e2);
	}
#endif

	template<typename Ite1, typename Ite2>
	bool CheckContainer(Ite1 b1, Ite1 e1, Ite2 b2, Ite2 e2)
	{
		int elem=0;
		bool result = true;
		Message ar;
		for( elem=0; b1 != e1 && b2 != e2; ++b1, ++b2, ++elem )
		{
			if( !internal::EqHelper<false>::Compare("", "", *b1, *b2) )
			{
				result = false;
				ar << "\nMismatch in a position " << elem << ": "
					<< ::iutest::internal::FormatForComparisonFailureMessage(*b1, *b2)
					<< " vs " << ::iutest::internal::FormatForComparisonFailureMessage(*b2, *b1);
			}
		}
		if( b1 != e1 )
		{
			int elem1 = elem;
			for( ; b1 != e1; ++b1, ++elem1 )
				;
			result = false;
			ar << "\nMismatch element : " << elem1 << " vs " << elem;
		}
		if( b2 != e2 )
		{
			int elem2 = elem;
			for( ; b2 != e2; ++b2, ++elem2 )
				;
			result = false;
			ar << "\nMismatch element : " << elem << " vs " << elem2;
		}
		m_whichIs = ar.GetString();
		return result;
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(ContainerEqMatcher);

	const T& m_expected;
	::std::string m_whichIs;
};


/**
 * @brief	Pointwise matcher
*/
template<typename M, typename T>
class PointwiseMatcher : public IMatcher
{
public:
	PointwiseMatcher(const M& matcher, const T& expected) : m_matcher(matcher), m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Check(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Pointwise: " << m_matcher << ": " << PrintToString(m_expected);
		strm << " (" << m_whichIs << ")";
		return strm.str();
	}
private:
	template<typename Container>
	bool Check(const Container& actual)
	{
		return Check(m_expected, actual.begin(), actual.end());
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename U, size_t SIZE>
	bool Check(const U(&actual)[SIZE])
	{
		return Check(m_expected, actual, actual + SIZE);
	}
#endif

	template<typename Container, typename Ite>
	bool Check(const Container& expected, Ite b2, Ite e2)
	{
		return CheckContainer(expected.begin(), expected.end(), b2, e2);
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename U, size_t SIZE, typename Ite>
	bool Check(const U(&expected)[SIZE], Ite b2, Ite e2)
	{
		return CheckContainer(expected, expected + SIZE, b2, e2);
	}
#endif

	template<typename Ite1, typename Ite2>
	bool CheckContainer(Ite1 b1, Ite1 e1, Ite2 b2, Ite2 e2)
	{
		int elem=0;
		bool result = true;
		Message ar;
		for( elem=0; b1 != e1 && b2 != e2; ++b1, ++b2, ++elem )
		{
			const AssertionResult r = m_matcher(*b2, *b1);
			if( r.failed() )
			{
				result = false;
				ar << "\nMismatch in a position " << elem << ": " << r.message();
			}
		}
		if( b1 != e1 )
		{
			int elem1 = elem;
			for( ; b1 != e1; ++b1, ++elem1 )
				;
			result = false;
			ar << "\nMismatch element : " << elem1 << " vs " << elem;
		}
		if( b2 != e2 )
		{
			int elem2 = elem;
			for( ; b2 != e2; ++b2, ++elem2 )
				;
			result = false;
			ar << "\nMismatch element : " << elem << " vs " << elem2;
		}
		m_whichIs = ar.GetString();
		return result;
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(PointwiseMatcher);

	M m_matcher;
	const T& m_expected;
	::std::string m_whichIs;
};

/**
 * @brief	IsEmpty matcher
*/
class IsEmptyMatcher : public IMatcher
{
public:
	IsEmptyMatcher(void) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( (actual).empty() ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return "Is Empty";
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(IsEmptyMatcher);
};


/**
 * @brief	SizeIs matcher
*/
template<typename T>
class SizeIsMatcher : public IMatcher
{
public:
	SizeIsMatcher(const T& expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Check(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Size is: " << m_expected;
		return strm.str();
	}
private:
	template<typename Container>
	bool Check(const Container& actual)
	{
		return static_cast<bool>(CastToMatcher(m_expected)(actual.size()));
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename U, size_t SIZE>
	bool Check(const U(&)[SIZE])
	{
		return static_cast<bool>(CastToMatcher(m_expected)(SIZE));
	}
#endif

private:
	IUTEST_PP_DISALLOW_ASSIGN(SizeIsMatcher);

	T m_expected;
};

/**
 * @brief	At matcher
*/
template<typename T>
class AtMatcher : public IMatcher
{
public:
	AtMatcher(const size_t index, const T& expected) : m_index(index), m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( CastToMatcher(m_expected)(actual[m_index]) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "At " << m_index << ": " << m_expected;
		return strm.str();
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(AtMatcher);

	size_t m_index;
	T m_expected;
};

/**
 * @brief	ElementsAreArray matcher
*/
template<typename T>
class ElementsAreArrayMatcher : public IMatcher
{
public:
	template<typename It>
	ElementsAreArrayMatcher(It begin, It end)
	{
		m_expected.insert(m_expected.end(), begin, end);
	}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		return Check(actual);
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "ElementsAreArray: " << PrintToString(m_expected);
		return strm.str();
	}
private:
	template<typename Container>
	AssertionResult Check(const Container& actual)
	{
		return Check(actual.begin(), actual.end());
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<typename U, size_t SIZE>
	AssertionResult Check(const U(&actual)[SIZE])
	{
		return Check(actual, actual + SIZE);
	}
#endif

	template<typename Ite>
	AssertionResult Check(Ite actual_begin, Ite actual_end)
	{
		const size_t actual_cnt = ::std::distance(actual_begin, actual_end);
		const size_t expected_cnt = m_expected.size();
		if( actual_cnt < expected_cnt )
		{
			return AssertionFailure() << "ElementsAreArray: argument[" << actual_cnt << "] is less than " << expected_cnt;
		}

		Ite a=actual_begin;
		typename ::std::vector<T>::iterator e=m_expected.begin();
		for( int i=0; e != m_expected.end(); ++e, ++a, ++i )
		{
			if( *a != *e )
			{
				return AssertionFailure() << WhichIs();
			}
		}
		return AssertionSuccess();
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(ElementsAreArrayMatcher);

	::std::vector<T> m_expected;
};

#if IUTEST_HAS_MATCHER_ELEMENTSARE

/**
 * @brief	ElementsAre matcher base class
*/
class ElementsAreMatcherBase : public IMatcher
{
protected:
	template<typename T, typename U>
	static AssertionResult Check(T& matchers, const U& actual)
	{
		return Check_<0, tuples::tuple_size<T>::value - 1>(actual, matchers);
	}
	template<int N, typename T>
	static ::std::string WhichIs(const T& matchers)
	{
		::std::string str = "ElementsAre: {";
		str += WhichIs_<T, N, tuples::tuple_size<T>::value-1>(matchers);
		str += "}";
		return str;
	}
private:
	template<int N, int LAST, typename Container, typename M>
	static AssertionResult Check_(const Container& actual, M& matchers)
	{
		return CheckSize<N, LAST>(actual.begin(), actual.end(), matchers);
	}
#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
	template<int N, int LAST, typename U, size_t SIZE, typename M>
	static AssertionResult Check_(const U(&actual)[SIZE], M& matchers)
	{
		return CheckSize<N, LAST>(actual, actual + SIZE, matchers);
	}
#endif

	template<int N, int LAST, typename Ite, typename M>
	static AssertionResult CheckSize(Ite it, Ite end, M& matchers)
	{
		const size_t cnt = ::std::distance(it, end);
		if( cnt < LAST+1 )
		{
			return AssertionFailure() << "ElementsAre: argument[" << cnt << "] is less than " << LAST+1;
		}
		return CheckElem<N, LAST>(it, end, matchers);
	}
	
	template<int N, int LAST, typename Ite, typename M>
	static AssertionResult CheckElem(Ite it, Ite end, M& matchers
		, typename detail::enable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		for( int index=N; it != end; ++it, ++index )
		{
			AssertionResult ar = CastToMatcher(tuples::get<N>(matchers))(*it);
			if( !ar )
			{
				return AssertionFailure() << WhichIsElem<N>(matchers, index);
			}
		}
		return AssertionSuccess();
	}

	template<int N, int LAST, typename Ite, typename M>
	static AssertionResult CheckElem(Ite it, Ite end, M& matchers
		, typename detail::disable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		AssertionResult ar = CastToMatcher(tuples::get<N>(matchers))(*it);
		if( ar )
		{
			return CheckElem<N + 1, LAST>(++it, end, matchers);
		}
		return AssertionFailure() << WhichIsElem<N>(matchers, N);
	}

	template<int N, typename T>
	static ::std::string WhichIsElem(const T& matchers, int index)
	{
		iu_global_format_stringstream strm;
		strm << "ElementsAre(" << index << "): " << tuples::get<N>(matchers);
		return strm.str();
	}

	template<typename T, int N, int LAST>
	static ::std::string WhichIs_(const T& matchers, typename detail::enable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		return StreamableToString(tuples::get<N>(matchers));
	}
	template<typename T, int N, int LAST>
	static ::std::string WhichIs_(const T& matchers, typename detail::disable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		return StreamableToString(tuples::get<N>(matchers)) + ", " + WhichIs_<T, N + 1, LAST>(matchers);
	}
	IUTEST_PP_DISALLOW_ASSIGN(ElementsAreMatcherBase);
};

#if IUTEST_HAS_VARIADIC_TEMPLATES

/**
* @brief	ElementsAre matcher
*/
template<typename ...T>
class ElementsAreMatcher : public ElementsAreMatcherBase
{
public:
	ElementsAreMatcher(T... t) : m_matchers(t...) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		return Check(m_matchers, actual);
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return ElementsAreMatcherBase::WhichIs<0>(m_matchers);
	}

private:
	tuples::tuple<T...> m_matchers;
};

#else

/*
template<typename T0, typename T1>
class ElementsAreMatcher : public ElementsAreMatcherBase
{
public:
	ElementsAreMatcher(T0 m0, T1 m1) : m_matchers(m0, m1) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		return Check(m_matchers, actual);
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return ElementsAreMatcherBase::WhichIs<0>(m_matchers);
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(ElementsAreMatcher);

	tuples::tuple<T0, T1> m_matchers;
};
*/

#define IIUT_DECL_ELEMENTSARE_MATCHER(n)												\
	template< IUTEST_PP_ENUM_PARAMS(n, typename T) >									\
	class IUTEST_PP_CAT(ElementsAreMatcher, n) : public ElementsAreMatcherBase {		\
	public: IUTEST_PP_CAT(ElementsAreMatcher, n)(IUTEST_PP_ENUM_BINARY_PARAMS(n, T, m))	\
		: m_matchers(IUTEST_PP_ENUM_PARAMS(n, m)) {}									\
	template<typename U>AssertionResult operator ()(const U& actual) {					\
		return Check(m_matchers, actual); }												\
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE {								\
		return ElementsAreMatcherBase::WhichIs<0>(m_matchers); }						\
	private: IUTEST_PP_DISALLOW_ASSIGN(IUTEST_PP_CAT(ElementsAreMatcher, n));			\
	tuples::tuple< IUTEST_PP_ENUM_PARAMS(n, T) > m_matchers;							\
	}

IIUT_DECL_ELEMENTSARE_MATCHER(1);
IIUT_DECL_ELEMENTSARE_MATCHER(2);
IIUT_DECL_ELEMENTSARE_MATCHER(3);
IIUT_DECL_ELEMENTSARE_MATCHER(4);
IIUT_DECL_ELEMENTSARE_MATCHER(5);
IIUT_DECL_ELEMENTSARE_MATCHER(6);
IIUT_DECL_ELEMENTSARE_MATCHER(7);
IIUT_DECL_ELEMENTSARE_MATCHER(8);
IIUT_DECL_ELEMENTSARE_MATCHER(9);
IIUT_DECL_ELEMENTSARE_MATCHER(10);

#undef IIUT_DECL_ELEMENTSARE_MATCHER

#endif

#endif

/**
 * @brief	Field matcher
*/
template<typename F, typename T>
class FieldMatcher : public IMatcher
{
public:
	FieldMatcher(const F& field, const T& expected) : m_field(field), m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Check(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Field: " << m_expected;
		//strm << "Field: (" << detail::GetTypeName<F>() << ") " << m_expected;
		return strm.str();
	}
private:
	template<typename U>
	bool Check(const U& actual
		, typename detail::disable_if_t< detail::is_pointer<U> >::type*& = detail::enabler::value)
	{
		return static_cast<bool>(CastToMatcher(m_expected)(actual.*m_field));
	}
	template<typename U>
	bool Check(const U& actual
		, typename detail::enable_if_t< detail::is_pointer<U> >::type*& = detail::enabler::value)
	{
		return static_cast<bool>(CastToMatcher(m_expected)(actual->*m_field));
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(FieldMatcher);

	const F& m_field;
	T m_expected;
};

/**
 * @brief	Property matcher
*/
template<typename F, typename T>
class PropertyMatcher : public IMatcher
{
public:
	PropertyMatcher(const F& prop, const T& expected) : m_property(prop), m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Check(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Property: " << m_expected;
		//strm << "Property: (" << detail::GetTypeName<F>() << ") " << m_expected;
		return strm.str();
	}
private:
	template<typename U>
	bool Check(const U& actual
		, typename detail::disable_if_t< detail::is_pointer<U> >::type*& = detail::enabler::value)
	{
		return static_cast<bool>(CastToMatcher(m_expected)((actual.*m_property)()));
	}
	template<typename U>
	bool Check(const U& actual
		, typename detail::enable_if_t< detail::is_pointer<U> >::type*& = detail::enabler::value)
	{
		return static_cast<bool>(CastToMatcher(m_expected)((actual->*m_property)()));
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(PropertyMatcher);

	const F& m_property;
	T m_expected;
};

/**
 * @brief	Key matcher
*/
template<typename T>
class KeyMatcher : public IMatcher
{
public:
	KeyMatcher(const T& expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual) const
	{
		if( CastToMatcher(m_expected)(actual.first) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Key: " << m_expected;
		return strm.str();
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(KeyMatcher);

	const T& m_expected;
};

/**
 * @brief	Pair matcher
*/
template<typename T1, typename T2>
class PairMatcher : public IMatcher
{
public:
	PairMatcher(const T1& m1, const T2& m2) : m_m1(m1), m_m2(m2) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( !CheckElem(actual.first, m_m1) )
		{
			return AssertionFailure() << WhichIs();
		}
		if( !CheckElem(actual.second, m_m2) )
		{
			return AssertionFailure() << WhichIs();
		}
		return AssertionSuccess();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Pair: (" << m_m1 << ", " << m_m2 << ")";
		return strm.str();
	}
private:
	template<typename T, typename U>
	bool CheckElem(const T& actual, U& matcher)
	{
		return static_cast<bool>(CastToMatcher(matcher)(actual));
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(PairMatcher);

	T1 m_m1;
	T2 m_m2;
};

/**
 * @brief	ResultOf matcher
*/
template<typename F, typename T>
class ResultOfMatcher : public IMatcher
{
public:
	ResultOfMatcher(F& func, const T& expected) : m_func(func), m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Check(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Result of: " << m_expected;
		//strm << "Result of " << detail::GetTypeName<F>() << "(): " << m_expected;
		return strm.str();
	}
private:
	template<typename U>
	bool Check(const U& actual)
	{
		return static_cast<bool>(CastToMatcher(m_expected)((*m_func)(actual)));
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(ResultOfMatcher);

	F& m_func;
	T m_expected;
};

/**
 * @brief	Pointee matcher
*/
template<typename T>
class PointeeMatcher : public IMatcher
{
public:
	PointeeMatcher(const T& expected) : m_expected(expected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( Check(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Points To: " << m_expected;
		return strm.str();
	}
private:
	template<typename U>
	bool Check(const U& actual)
	{
		return static_cast<bool>(CastToMatcher(m_expected)(*actual));
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(PointeeMatcher);

	T m_expected;
};

/**
 * @brief	Not matcher
*/
template<typename T>
class NotMatcher : public IMatcher
{
public:
	NotMatcher(const T& unexpected) : m_unexpected(unexpected) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		if( !CastToMatcher(m_unexpected)(actual) ) return AssertionSuccess();
		return AssertionFailure() << WhichIs();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "Not: (" << m_unexpected << ")";
		return strm.str();
	}

private:
	IUTEST_PP_DISALLOW_ASSIGN(NotMatcher);

	T m_unexpected;
};

/**
 * @brief	Any matcher
*/
template<typename T>
class AnyMatcher : public IMatcher
{
public:
	AnyMatcher(void) {}
public:
	AssertionResult operator ()(const T&) const
	{
		return AssertionSuccess();
	}
	template<typename U>
	AssertionResult operator ()(const U&) const;

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		iu_global_format_stringstream strm;
		strm << "A: " << detail::GetTypeName<T>();
		return strm.str();
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(AnyMatcher);
};

/**
 * @brief	Anything matcher
*/
class AnythingMatcher : public IMatcher
{
public:
	AnythingMatcher(void) {}
public:
	template<typename U>
	AssertionResult operator ()(const U&) const
	{
		return AssertionSuccess();
	}

public:
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return "_";
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(AnythingMatcher);
};

#if IUTEST_HAS_MATCHER_ALLOF_AND_ANYOF

/**
 * @brief	AllOf matcher base class
*/
class AllOfMatcherBase : public IMatcher
{
protected:
	template<typename T, typename U>
	static AssertionResult Check(T& matchers, const U& actual)
	{
		return Check_<T, U, 0, tuples::tuple_size<T>::value-1>(matchers, actual);
	}
	template<int N, typename T>
	static ::std::string WhichIs(const T& matchers)
	{
		return WhichIs_<T, N, tuples::tuple_size<T>::value-1>(matchers);
	}
private:
	template<typename T, typename U, int N, int LAST>
	static AssertionResult Check_(T& matchers, const U& actual, typename detail::enable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		AssertionResult ar = tuples::get<N>(matchers)(actual);
		if( ar )
		{
			return ar;
		}
		return AssertionFailure() << WhichIs_<T, 0, N>(matchers);
	}
	template<typename T, typename U, int N, int LAST>
	static AssertionResult Check_(T& matchers, const U& actual, typename detail::disable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		AssertionResult ar = tuples::get<N>(matchers)(actual);
		if( ar )
		{
			return Check_<T, U, N + 1, LAST>(matchers, actual);
		}
		return AssertionFailure() << WhichIs_<T, 0, N>(matchers);
	}

	template<typename T, int N, int LAST>
	static ::std::string WhichIs_(const T& matchers, typename detail::enable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		return tuples::get<N>(matchers).WhichIs();
	}
	template<typename T, int N, int LAST>
	static ::std::string WhichIs_(const T& matchers, typename detail::disable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		return tuples::get<N>(matchers).WhichIs() + " and " + WhichIs_<T, N + 1, LAST>(matchers);
	}
	IUTEST_PP_DISALLOW_ASSIGN(AllOfMatcherBase);
};

#if IUTEST_HAS_VARIADIC_TEMPLATES

/**
 * @brief	AllOf matcher
*/
template<typename ...T>
class AllOfMatcher : public AllOfMatcherBase
{
public:
	AllOfMatcher(T... t) : m_matchers(t...) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		return Check(m_matchers, actual);
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return AllOfMatcherBase::WhichIs<0>(m_matchers);
	}

private:
	tuples::tuple<T...> m_matchers;
};

#else

/*
template<typename T0, typename T1>
class AllOfMatcher : public AllOfMatcherBase
{
public:
	AllOfMatcher(T0 m0, T1 m1) : m_matchers(m0, m1) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		return Check(m_matchers, actual);
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return AllOfMatcherBase::WhichIs<0>(m_matchers);
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(AllOfMatcher);

	tuples::tuple<T0, T1> m_matchers;
};
*/

#define IIUT_DECL_ALLOF_MATCHER(n)													\
	template< IUTEST_PP_ENUM_PARAMS(n, typename T) >								\
	class IUTEST_PP_CAT(AllOfMatcher, n) : public AllOfMatcherBase {				\
	public: IUTEST_PP_CAT(AllOfMatcher, n)(IUTEST_PP_ENUM_BINARY_PARAMS(n, T, m))	\
		: m_matchers(IUTEST_PP_ENUM_PARAMS(n, m)) {}								\
	template<typename U>AssertionResult operator ()(const U& actual) {				\
		return Check(m_matchers, actual); }											\
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE {							\
		return AllOfMatcherBase::WhichIs<0>(m_matchers); }							\
	private: IUTEST_PP_DISALLOW_ASSIGN(IUTEST_PP_CAT(AllOfMatcher, n));				\
	tuples::tuple< IUTEST_PP_ENUM_PARAMS(n, T) > m_matchers;						\
	}

IIUT_DECL_ALLOF_MATCHER(2);
IIUT_DECL_ALLOF_MATCHER(3);
IIUT_DECL_ALLOF_MATCHER(4);
IIUT_DECL_ALLOF_MATCHER(5);
IIUT_DECL_ALLOF_MATCHER(6);
IIUT_DECL_ALLOF_MATCHER(7);
IIUT_DECL_ALLOF_MATCHER(8);
IIUT_DECL_ALLOF_MATCHER(9);
IIUT_DECL_ALLOF_MATCHER(10);

#undef IIUT_DECL_ALLOF_MATCHER

#endif

/**
 * @brief	AnyOf matcher base class
*/
class AnyOfMatcherBase : public IMatcher
{
protected:
	template<typename T, typename U>
	static AssertionResult Check(T& matchers, const U& actual)
	{
		return Check_<T, U, 0, tuples::tuple_size<T>::value-1>(matchers, actual);
	}
	template<int N, typename T>
	static ::std::string WhichIs(const T& matchers)
	{
		return WhichIs_<T, N, tuples::tuple_size<T>::value-1>(matchers);
	}
private:
	template<typename T, typename U, int N, int LAST>
	static AssertionResult Check_(T& matchers, const U& actual, typename detail::enable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		AssertionResult ar = tuples::get<N>(matchers)(actual);
		if( ar )
		{
			return ar;
		}
		return AssertionFailure() << WhichIs_<T, 0, N>(matchers);
	}
	template<typename T, typename U, int N, int LAST>
	static AssertionResult Check_(T& matchers, const U& actual, typename detail::disable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		AssertionResult ar = tuples::get<N>(matchers)(actual);
		if( ar )
		{
			return ar;
		}
		return Check_<T, U, N + 1, LAST>(matchers, actual);
	}

	template<typename T, int N, int LAST>
	static ::std::string WhichIs_(const T& matchers, typename detail::enable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		return tuples::get<N>(matchers).WhichIs();
	}
	template<typename T, int N, int LAST>
	static ::std::string WhichIs_(const T& matchers, typename detail::disable_if<N == LAST, void>::type*& = detail::enabler::value)
	{
		return tuples::get<N>(matchers).WhichIs() + " or " + WhichIs_<T, N + 1, LAST>(matchers);
	}

	IUTEST_PP_DISALLOW_ASSIGN(AnyOfMatcherBase);
};

#if IUTEST_HAS_VARIADIC_TEMPLATES

/**
 * @brief	AnyOf matcher
*/
template<typename ...T>
class AnyOfMatcher : public AnyOfMatcherBase
{
public:
	AnyOfMatcher(T... t) : m_matchers(t...) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		return Check(m_matchers, actual);
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return AnyOfMatcherBase::WhichIs<0>(m_matchers);
	}

private:
	tuples::tuple<T...> m_matchers;
};

#else

/*
template<typename T0, typename T1>
class AnyOfMatcher : public AnyOfMatcherBase
{
public:
	AnyOfMatcher(T0 m0, T1 m1) : m_matchers(m0, m1) {}

public:
	template<typename U>
	AssertionResult operator ()(const U& actual)
	{
		return Check(m_matchers, actual);
	}
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE
	{
		return AnyOfMatcherBase::WhichIs<0>(m_matchers);
	}
private:
	IUTEST_PP_DISALLOW_ASSIGN(AnyOfMatcher);

	tuples::tuple<T0, T1> m_matchers;
};
*/

#define IIUT_DECL_ANYOF_MATCHER(n)													\
	template< IUTEST_PP_ENUM_PARAMS(n, typename T) >								\
	class IUTEST_PP_CAT(AnyOfMatcher, n) : public AnyOfMatcherBase {				\
	public: IUTEST_PP_CAT(AnyOfMatcher, n)(IUTEST_PP_ENUM_BINARY_PARAMS(n, T, m))	\
		: m_matchers(IUTEST_PP_ENUM_PARAMS(n, m)) {}								\
	template<typename U>AssertionResult operator ()(const U& actual) {				\
		return Check(m_matchers, actual); }											\
	::std::string WhichIs(void) const IUTEST_CXX_OVERRIDE {							\
		return AnyOfMatcherBase::WhichIs<0>(m_matchers); }							\
	private: IUTEST_PP_DISALLOW_ASSIGN(IUTEST_PP_CAT(AnyOfMatcher, n));				\
	tuples::tuple< IUTEST_PP_ENUM_PARAMS(n, T) > m_matchers;						\
	}

IIUT_DECL_ANYOF_MATCHER(2);
IIUT_DECL_ANYOF_MATCHER(3);
IIUT_DECL_ANYOF_MATCHER(4);
IIUT_DECL_ANYOF_MATCHER(5);
IIUT_DECL_ANYOF_MATCHER(6);
IIUT_DECL_ANYOF_MATCHER(7);
IIUT_DECL_ANYOF_MATCHER(8);
IIUT_DECL_ANYOF_MATCHER(9);
IIUT_DECL_ANYOF_MATCHER(10);

#undef IIUT_DECL_ANYOF_MATCHER

#endif

#endif

}	// end of namespace detail

namespace matchers
{

/**
 * @ingroup	MATCHERS
 * @brief	Make Equals matcher
 * @details	argument == expected
*/
template<typename T>
detail::EqMatcher<T> Equals(const T& expected) { return detail::EqMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Eq matcher
 * @details	argument == expected
*/
template<typename T>
detail::EqMatcher<T> Eq(const T& expected) { return detail::EqMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Ne matcher
 * @details	argument != expected
*/
template<typename T>
detail::NeMatcher<T> Ne(const T& expected) { return detail::NeMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Le matcher
 * @details	argument <= expected
*/
template<typename T>
detail::LeMatcher<T> Le(const T& expected) { return detail::LeMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Lt matcher
 * @details	argument < expected
*/
template<typename T>
detail::LtMatcher<T> Lt(const T& expected) { return detail::LtMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Ge matcher
 * @details	argument >= expected
*/
template<typename T>
detail::GeMatcher<T> Ge(const T& expected) { return detail::GeMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Gt matcher
 * @details	argument > expected
*/
template<typename T>
detail::GtMatcher<T> Gt(const T& expected) { return detail::GtMatcher<T>(expected); }


/**
 * @ingroup	MATCHERS
 * @brief	Make Twofold Eq matcher
 * @details	argument == expected
*/
inline detail::TwofoldEqMatcher Eq(void) { return detail::TwofoldEqMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Twofold Ne matcher
 * @details	argument != expected
*/
inline detail::TwofoldNeMatcher Ne(void) { return detail::TwofoldNeMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Twofold Le matcher
 * @details	argument <= expected
*/
inline detail::TwofoldLeMatcher Le(void) { return detail::TwofoldLeMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Twofold Lt matcher
 * @details	argument < expected
*/
inline detail::TwofoldLtMatcher Lt(void) { return detail::TwofoldLtMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Twofold Ge matcher
 * @details	argument >= expected
*/
inline detail::TwofoldGeMatcher Ge(void) { return detail::TwofoldGeMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Twofold Gt matcher
 * @details	argument > expected
*/
inline detail::TwofoldGtMatcher Gt(void) { return detail::TwofoldGtMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make IsNull matcher
 * @details	argument == nullptr
*/
inline detail::IsNullMatcher IsNull() { return detail::IsNullMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make NotNull matcher
 * @details	argument != nullptr
*/
inline detail::NotNullMatcher NotNull() { return detail::NotNullMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make TypedEq matcher
 * @details	argument == expected
*/
template<typename T, typename U>
detail::TypedEqMatcher<T> TypedEq(const U& expected) { return detail::TypedEqMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Float Eq matcher
 * @details	argument は expected とおよそ等しい
*/
inline detail::FloatingPointEqMatcher<float> FloatEq(float expected) { return detail::FloatingPointEqMatcher<float>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Double Eq matcher
 * @details	argument は expected とおよそ等しい
*/
inline detail::FloatingPointEqMatcher<double> DoubleEq(double expected) { return detail::FloatingPointEqMatcher<double>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make NanSensitive Float Eq matcher
 * @details	argument は expected とおよそ等しい（NaN 同士は等しいとされる）
*/
inline detail::NanSensitiveFloatingPointEqMatcher<float> NanSensitiveFloatEq(float expected) { return detail::NanSensitiveFloatingPointEqMatcher<float>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make NanSensitive Double Eq matcher
 * @details	argument は expected とおよそ等しい（NaN 同士は等しいとされる）
*/
inline detail::NanSensitiveFloatingPointEqMatcher<double> NanSensitiveDoubleEq(double expected) { return detail::NanSensitiveFloatingPointEqMatcher<double>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make StrEq matcher
 * @details	argument == expected
*/
template<typename T>
detail::StrEqMatcher<T> StrEq(const T& expected) { return detail::StrEqMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make StrNe matcher
 * @details	argument != expected
*/
template<typename T>
detail::StrNeMatcher<T> StrNe(const T& expected) { return detail::StrNeMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make StrCaseEq matcher
 * @details	argument == expected (ignore case)
*/
template<typename T>
detail::StrCaseEqMatcher<T> StrCaseEq(const T& expected) { return detail::StrCaseEqMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make StrCaseNe matcher
 * @details	argument != expected (ignore case)
*/
template<typename T>
detail::StrCaseNeMatcher<T> StrCaseNe(const T& expected) { return detail::StrCaseNeMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make StartsWith matcher
 * @details	argument の先頭が str である
*/
template<typename T>
detail::StartsWithMatcher<const T&> StartsWith(const T& str) { return detail::StartsWithMatcher<const T&>(str); }

/**
 * @ingroup	MATCHERS
 * @brief	Make HasSubstr matcher
 * @details	argument が str を含む
*/
template<typename T>
detail::HasSubstrMatcher<const T&> HasSubstr(const T& str) { return detail::HasSubstrMatcher<const T&>(str); }

/**
 * @ingroup	MATCHERS
 * @brief	Make EndsWith matcher
 * @details	argument の末尾が str である
*/
template<typename T>
detail::EndsWithMatcher<const T&> EndsWith(const T& str) { return detail::EndsWithMatcher<const T&>(str); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Contains matcher
 * @details	argument は expected にマッチする要素を含む
*/
template<typename T>
detail::ContainsMatcher<T> Contains(const T& expected) { return detail::ContainsMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Each matcher
 * @details	argument はすべての要素が expected にマッチする
*/
template<typename T>
detail::EachMatcher<T> Each(const T& expected) { return detail::EachMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make ContainerEq matcher
 * @details	argument コンテナは expected コンテナにマッチする
*/
template<typename T>
detail::ContainerEqMatcher<T> ContainerEq(const T& expected) { return detail::ContainerEqMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Pointwise matcher
 * @details	argument コンテナは expected コンテナの各要素と matcher にマッチする
*/
template<typename M, typename T>
detail::PointwiseMatcher<M, T> Pointwise(const M& matcher, const T& expected) { return detail::PointwiseMatcher<M, T>(matcher, expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make IsEmpty matcher
 * @details	argument.empty()
*/
inline detail::IsEmptyMatcher IsEmpty() { return detail::IsEmptyMatcher(); }

/**
 * @ingroup	MATCHERS
 * @brief	Make SizeIs matcher
 * @details	argument の要素数が expected にマッチする
*/
template<typename T>
detail::SizeIsMatcher<T> SizeIs(const T& expected) { return detail::SizeIsMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make At matcher
 * @details	argument[index] は expected にマッチする
*/
template<typename T>
detail::AtMatcher<T> At(size_t index, const T& expected) { return detail::AtMatcher<T>(index, expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make ElementsAreArray matcher
 * @details	argument はの各要素が a の要素とマッチする
*/
template<typename Container>
detail::ElementsAreArrayMatcher< typename Container::value_type > ElementsAreArray(Container container)
{
	return detail::ElementsAreArrayMatcher<typename Container::value_type>(container.begin(), container.end());
}

#if !defined(IUTEST_NO_FUNCTION_TEMPLATE_ORDERING)
/** @overload */
template<typename T, size_t SIZE>
detail::ElementsAreArrayMatcher<T> ElementsAreArray(const T(&v)[SIZE])
{
	return detail::ElementsAreArrayMatcher<T>(v, v + SIZE);
}

#if !defined(IUTEST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
/** @overload */
template<typename Ite>
detail::ElementsAreArrayMatcher< typename detail::IteratorTraits<Ite>::type > ElementsAreArray(Ite begin, Ite end)
{
	return new detail::ElementsAreArrayMatcher< typename detail::IteratorTraits<Ite>::type >(begin, end);
}
#endif

#if IUTEST_HAS_INITIALIZER_LIST
/** @overload */
template<typename T>
detail::ElementsAreArrayMatcher<T> ElementsAreArray(::std::initializer_list<T> l)
{
	return detail::ElementsAreArrayMatcher<T>(l.begin(), l.end());
}
#endif

#endif

/**
 * @ingroup	MATCHERS
 * @brief	Make ElementsAreArray matcher
 * @details	argument はの要素 count 個が a の要素とマッチする
*/
template<typename T>
detail::ElementsAreArrayMatcher<T> ElementsAreArray(const T* a, int count)
{
	return detail::ElementsAreArrayMatcher<T>(a, a+count);
}

#if IUTEST_HAS_MATCHER_ELEMENTSARE

#if IUTEST_HAS_VARIADIC_TEMPLATES

/**
 * @ingroup	MATCHERS
 * @brief	Make ElementsAre matcher
*/
template<typename ...T>
detail::ElementsAreMatcher<T...> ElementsAre(const T&... m) { return detail::ElementsAreMatcher<T...>(m...); }

#else

#define IIUT_ELEMENTSARE_MATCHER_NAME(n)	IUTEST_PP_CAT(ElementsAreMatcher, n)
#define IIUT_DECL_ELEMENTSARE(n)												\
	template< IUTEST_PP_ENUM_PARAMS(n, typename T) >							\
	detail:: IIUT_ELEMENTSARE_MATCHER_NAME(n)< IUTEST_PP_ENUM_PARAMS(n, T) >	\
	ElementsAre( IUTEST_PP_ENUM_BINARY_PARAMS(n, const T, &m) ) { return		\
	detail:: IIUT_ELEMENTSARE_MATCHER_NAME(n)< IUTEST_PP_ENUM_PARAMS(n, T) >	\
		( IUTEST_PP_ENUM_PARAMS(n, m) ); }


IIUT_DECL_ELEMENTSARE(1)
IIUT_DECL_ELEMENTSARE(2)
IIUT_DECL_ELEMENTSARE(3)
IIUT_DECL_ELEMENTSARE(4)
IIUT_DECL_ELEMENTSARE(5)
IIUT_DECL_ELEMENTSARE(6)
IIUT_DECL_ELEMENTSARE(7)
IIUT_DECL_ELEMENTSARE(8)
IIUT_DECL_ELEMENTSARE(9)
IIUT_DECL_ELEMENTSARE(10)

#undef IIUT_ELEMENTSARE_MATCHER_NAME
#undef IIUT_DECL_ELEMENTSARE
#endif

#endif

/**
 * @ingroup	MATCHERS
 * @brief	Make Key matcher
 * @details	argument.first は expedted にマッチする 
*/
template<typename T>
detail::KeyMatcher<T> Key(const T& expected) { return detail::KeyMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Pair matcher
 * @details	argument.first は m1 にマッチし、arugment.second が m2 にマッチする 
*/
template<typename T1, typename T2>
detail::PairMatcher<T1, T2> Pair(const T1& m1, const T2& m2) { return detail::PairMatcher<T1, T2>(m1, m2); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Field matcher
 * @details	argument.*field は expedted にマッチする 
*/
template<typename F, typename T>
detail::FieldMatcher<F, T> Field(const F& field, const T& expected) { return detail::FieldMatcher<F, T>(field, expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Property matcher
 * @details	argument.*property() は expedted にマッチする 
*/
template<typename P, typename T>
detail::PropertyMatcher<P, T> Property(const P& prop, const T& expected) { return detail::PropertyMatcher<P, T>(prop, expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make ResultOf matcher
 * @details	func(argument) の戻り値は expedted にマッチする 
*/
template<typename F, typename T>
detail::ResultOfMatcher<F, T> ResultOf(const F& func, const T& expected) { return detail::ResultOfMatcher<F, T>(func, expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Pointee matcher
*/
template<typename T>
detail::PointeeMatcher<T> Pointee(const T& expected) { return detail::PointeeMatcher<T>(expected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Not matcher
*/
template<typename T>
detail::NotMatcher<T> Not(const T& unexpected) { return detail::NotMatcher<T>(unexpected); }

/**
 * @ingroup	MATCHERS
 * @brief	Make Any matcher
*/
template<typename T>
detail::AnyMatcher<T> A(void) { return detail::AnyMatcher<T>(); }


/**
 * @ingroup	MATCHERS
 * @brief	Anything matcher
*/
const detail::AnythingMatcher _;

#if IUTEST_HAS_MATCHER_ALLOF_AND_ANYOF

#if IUTEST_HAS_VARIADIC_TEMPLATES

/**
 * @ingroup	MATCHERS
 * @brief	Make AllOf matcher
 * @details	argument が全ての matcher にマッチする
*/
template<typename ...T>
detail::AllOfMatcher<T...> AllOf(const T&... m) { return detail::AllOfMatcher<T...>(m...); }

/**
 * @ingroup	MATCHERS
 * @brief	Make AnyOf matcher
 * @details	argument がいずれかの matcher にマッチする
*/
template<typename ...T>
detail::AnyOfMatcher<T...> AnyOf(const T&... m) { return detail::AnyOfMatcher<T...>(m...); }

#else

#define IIUT_ANYOF_AND_ALLOF_MATCHER_NAME(name, n)	IUTEST_PP_CAT( IUTEST_PP_CAT(name, Matcher), n)
#define IIUT_DECL_ANYOF_AND_ALLOF(name, n)												\
	template< IUTEST_PP_ENUM_PARAMS(n, typename T) >									\
	detail:: IIUT_ANYOF_AND_ALLOF_MATCHER_NAME(name, n)< IUTEST_PP_ENUM_PARAMS(n, T) >	\
	name( IUTEST_PP_ENUM_BINARY_PARAMS(n, const T, &m) ) { return						\
	detail:: IIUT_ANYOF_AND_ALLOF_MATCHER_NAME(name, n)< IUTEST_PP_ENUM_PARAMS(n, T) >	\
		( IUTEST_PP_ENUM_PARAMS(n, m) ); }


IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 2)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 3)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 4)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 5)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 6)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 7)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 8)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 9)
IIUT_DECL_ANYOF_AND_ALLOF(AllOf, 10)

IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 2)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 3)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 4)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 5)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 6)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 7)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 8)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 9)
IIUT_DECL_ANYOF_AND_ALLOF(AnyOf, 10)

#undef IIUT_ANYOF_AND_ALLOF_MATCHER_NAME
#undef IIUT_DECL_ANYOF_AND_ALLOF

#endif

#endif

/**
 * @ingroup	MATCHERS
 * @brief	Value predicate
*/
template<typename T, typename M>
bool Value(const T& value, const M& expected) { return static_cast<bool>(detail::CastToMatcher(expected)(value)); }


}	// end of namespace matchers

using namespace matchers;

}	// end of namespace iutest

#endif	// IUTEST_HAS_MATCHERS

#endif // INCG_IRIS_IUTEST_MATCHER_HPP_23746E00_1A4B_4778_91AD_45C6DEFEEFA7_
