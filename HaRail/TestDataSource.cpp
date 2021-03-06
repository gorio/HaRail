﻿/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestDataSource.h"

namespace HaRail {
	void TestDataSource::initTrain(int train_id, const stop_arr_t& stops)
	{
		if (stops.size() < 1) {
			return;
		}
		for (unsigned int i = 0; i < stops.size() - 1; i++) {
			createTrain(train_id, stops[i].first, stops[i + 1].first, stops[i].second, stops[i + 1].second);
		}
	}

	stop_t TestDataSource::makeStop(int station_id, const string& time_str) const
	{
		return stop_t(getStationById(station_id), Utils::parseTime(time_str));
	}

	void TestDataSource::initStations()
	{
		createStation(100, "stat_a");
		createStation(200, "stat_b");
		createStation(300, "stat_c");
		createStation(400, "stat_d");
		createStation(500, "stat_e");
		createStation(600, "stat_f");
	}

	void TestDataSource::initTrains()
	{
		switch (test) {
		case 1:
			// Tests basic train switch cost logic. Expected result is to ride train 1 all the way to station 600, 
			// even though some of the route appears to be faster if we switch train.
			// run with -t 1 100 10:00:00 600
			initTrain(3, stop_arr_t{
				  makeStop(500, "12:01:00"), makeStop(600, "12:30:00")
		});
			initTrain(1, stop_arr_t{
				makeStop(100, "10:00:00"), makeStop(200, "10:30:00"), makeStop(300, "11:00:00"), makeStop(400, "11:30:00"), makeStop(500, "12:00:00"), makeStop(600, "12:30:00")
			});
			initTrain(2, stop_arr_t{
				makeStop(200, "10:31:00"), makeStop(400, "10:32:00")
			});
			break;
		case 2:
			// Basic shortest-path test, expected result is to ride train 2 from 100 to 400 and then go back to 300 using train 3
			// run with -t 2 100 10:00:00 300
			initTrain(1, stop_arr_t{
				  makeStop(100, "10:00:00"), makeStop(200, "10:30:00"), makeStop(300, "11:00:00"), makeStop(400, "11:30:00")
		});
			initTrain(2, stop_arr_t{
				makeStop(100, "10:00:00"), makeStop(400, "10:30:00")
			});
			initTrain(3, stop_arr_t{
				makeStop(400, "10:30:00"), makeStop(300, "10:40:00")
			});
			break;
		case 3:
			// Test train switch minimization in a more complicated case. Expected result is to only use train 1.
			// run with -t 3 100 10:00:00 400
			initTrain(1, stop_arr_t{
				  makeStop(100, "10:10:00"), makeStop(200, "10:30:00"), makeStop(300, "11:00:00"), makeStop(400, "11:30:00")
		});
			initTrain(2, stop_arr_t{
				makeStop(100, "10:00:00"), makeStop(300, "10:30:00")
			});
			break;
		case 4:
			// Test trains with WAIT_ON_TRAIN and alt-route finding
			// expected result is to use train 2 and then switch to 3
			// run with -t 4 100 10:00 300
			createTrain(1, getStationById(100), getStationById(200), Utils::parseTime("10:00"), Utils::parseTime("10:20"));
			createTrain(1, getStationById(200), getStationById(300), Utils::parseTime("10:30"), Utils::parseTime("11:00"));
			
			createTrain(2, getStationById(100), getStationById(200), Utils::parseTime("10:10"), Utils::parseTime("10:20"));
			
			createTrain(3, getStationById(200), getStationById(300), Utils::parseTime("10:30"), Utils::parseTime("10:40"));
			break;
		case 5:
			// Test trains with WAIT_ON_TRAIN
			// expected result is to use train 1 only in train switching mode, train 2 then 1 in delayed leaving
			// run with -t 5 100 10:00 300
			createTrain(1, getStationById(100), getStationById(200), Utils::parseTime("10:00"), Utils::parseTime("10:20"));
			createTrain(1, getStationById(200), getStationById(300), Utils::parseTime("10:30"), Utils::parseTime("11:00"));

			createTrain(2, getStationById(100), getStationById(200), Utils::parseTime("10:10"), Utils::parseTime("10:20"));

			createTrain(3, getStationById(200), getStationById(300), Utils::parseTime("10:30"), Utils::parseTime("11:00"));
			break;
		default:
			throw HaException("Test case not implemented", HaException::UNIMPLEMENTED_ERROR);
		}
	}
}
