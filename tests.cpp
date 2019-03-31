/*
 * tests.cpp
 *
 *  Created on: 27.03.2019
 *      Author: Boss
 */

#include <iomanip>
#include <ctime>
#include <constants.h>
#include <ucos.h>
#include <mcf5441x_rtc.h>
#include "lest/lest.hpp"
#include "tests.h"

using namespace std;

namespace {
	std::string incrementRTC(const char* setValue, uint16_t delay) {
		struct tm set_tm, incremented_tm;
		if(strptime(setValue, "%D %T %w %j", &set_tm) == nullptr)
				return "";
		OSTimeDly(1); // Ensure we set the RTC as close to tick as possible
		MCF541X_RTCSetTime(set_tm);
		OSTimeDly(delay * TICKS_PER_SECOND);
		MCF541X_RTCGetTime(incremented_tm);
		std::ostringstream oss;
		oss << std::put_time(&incremented_tm, "%D %T %w %j");
		return oss.str();
	}
}

const lest::test specification[] = {

    CASE( "Empty string has length zero (succeed)" ) {
        EXPECT(     0u == string(  ).length() );
        EXPECT(     0u == string("").length() );
        EXPECT_NOT( 0u <  string("").length() );
    },

    CASE( "Text compares lexically (fail)" ) {
        EXPECT( string("hello") == string("world") );
    },

    CASE( "Unexpected exception is reported" ) {
        EXPECT( (throw std::runtime_error("surprise!"), true) );
    },

    CASE( "Unspecified expected exception is captured" ) {
        EXPECT_THROWS( throw std::runtime_error("surprise!") );
    },

    CASE( "Specified expected exception is captured" ) {
        EXPECT_THROWS_AS( throw std::bad_alloc(), std::bad_alloc );
    },

    CASE( "Expected exception is reported missing" ) {
        EXPECT_THROWS( true );
    },

    CASE( "Specific expected exception is reported missing" ) {
        EXPECT_THROWS_AS( true, std::runtime_error );
    },

    CASE( "Expected minute increment" "[RTC]" ) {
    	EXPECT(incrementRTC(
    			"05/24/13 16:45:58 5 144", 4) == "05/24/13 16:46:02 5 144");
    },

    CASE( "Expected hour increment" "[RTC]"  ) {
    	EXPECT(incrementRTC(
    			"05/24/13 16:45:58 5 144", 4) == "05/24/13 16:46:02 5 144");
    	EXPECT(incrementRTC(
    			"12/10/14 12:59:58 3 344", 4) == "12/10/14 13:00:02 3 344");
    },

	CASE( "Expected day increment" "[RTC]" ) {
		EXPECT(incrementRTC(
				"02/15/17 23:59:57 3 46", 4) == "02/16/17 00:00:01 4 047");
		EXPECT(incrementRTC(
				"04/24/16 23:59:57 6 115", 4) == "04/25/16 00:00:01 0 116");
	},

	CASE( "Expected month increment" "[RTC]" ) {
		EXPECT(incrementRTC(
				"01/31/12 23:59:59 2 31", 4) == "02/01/12 00:00:03 3 032");
		EXPECT(incrementRTC(
				"11/30/12 23:59:59 5 335", 4) == "12/01/12 00:00:03 6 336");
	},

	CASE( "Expected year increment" "[RTC]" ) {
		EXPECT(incrementRTC(
				"12/31/16 23:59:58 6 366", 4) == "01/01/17 00:00:02 0 001");
		EXPECT(incrementRTC(
				"12/31/85 23:59:58 6 365", 4) == "01/01/86 00:00:02 0 001");
	},

	CASE( "Expected leap day increment" "[RTC]" ) {
		EXPECT(incrementRTC(
				"02/28/12 23:59:59 2 59", 4) == "02/29/12 00:00:03 3 060");
		EXPECT(incrementRTC(
				"02/29/12 23:59:59 3 60", 4) == "03/01/12 00:00:03 4 061");
		EXPECT(incrementRTC(
				"02/28/13 23:59:59 4 59", 4) == "03/01/13 00:00:03 5 060");
		EXPECT(incrementRTC(
				"02/28/14 23:59:59 5 59", 4) == "03/01/14 00:00:03 6 060");
		EXPECT(incrementRTC(
				"02/28/15 23:59:59 6 59", 4) == "03/01/15 00:00:03 0 060");
	},

};

void test_main() {
    const std::vector<std::string> args{"-v", "-p", "" };
    lest::run( specification, args, std::cout );
}
