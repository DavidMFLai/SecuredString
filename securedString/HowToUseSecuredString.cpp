// securedString.cpp : Defines the entry point for the console application.
//

#include "securedString.h"
#include <string>

#include "gtest\gtest.h"
#include "gmock/gmock.h"


class HowToUseSecuredStringTest : public ::testing::Test {
protected:
	wchar_t *rawString;
	wchar_t *rawString2;
	HowToUseSecuredStringTest()
	{
		wchar_t tmp[8] = L"rawdata";
		rawString = new wchar_t[sizeof(tmp)];
		memcpy(rawString, tmp, sizeof(tmp));

		wchar_t tmp2[9] = L"rawdata2";
		rawString2 = new wchar_t[sizeof(tmp2)];
		memcpy(rawString2, tmp2, sizeof(tmp2));
	}
};

TEST_F(HowToUseSecuredStringTest, Contruction)
{
	SecuredString<wchar_t> securedString{ rawString };
	ASSERT_THAT(rawString, ::testing::Ne(securedString.c_str()));
}

TEST_F(HowToUseSecuredStringTest, GainControl)
{
	SecuredString<wchar_t> securedString;
	securedString.gainControl(rawString);
	ASSERT_THAT(rawString, ::testing::Eq(securedString.c_str()));
}

TEST_F(HowToUseSecuredStringTest, compare)
{
	SecuredString<wchar_t> securedString1{ rawString };
	SecuredString<wchar_t> securedString2{ rawString };
	ASSERT_THAT(securedString1 == securedString2, ::testing::Eq(true));
	ASSERT_THAT(securedString1 != securedString2, ::testing::Eq(false));

	SecuredString<wchar_t> securedString3{ rawString2 };
	ASSERT_THAT(securedString1 == securedString3, ::testing::Eq(false));
	ASSERT_THAT(securedString1 != securedString3, ::testing::Eq(true));
}

TEST_F(HowToUseSecuredStringTest, append)
{
	SecuredString<wchar_t> securedString1{ rawString };
	SecuredString<wchar_t> securedString2{ rawString2 };

	securedString1 += securedString2;

	std::wstring mergedstring{ std::wstring{ rawString } +std::wstring{ rawString2 } };
	std::wstring mergedsecuredstring = securedString1.c_str();
	ASSERT_THAT(mergedstring == mergedsecuredstring, ::testing::Eq(true));
}

TEST_F(HowToUseSecuredStringTest, concatenate)
{
	SecuredString<wchar_t> securedString1{ rawString };
	SecuredString<wchar_t> securedString2{ rawString2 };

	SecuredString<wchar_t> securedString3 = securedString1 + securedString2;
	
	std::wstring mergedstring{ std::wstring{ rawString } + std::wstring{ rawString2 } };
	std::wstring mergedsecuredstring = securedString3.c_str();
	ASSERT_THAT(mergedstring == mergedsecuredstring, ::testing::Eq(true));
}
