#!/usr/bin/env 
# -*- coding: utf-8 -*-

import os, subprocess

import logParserClass
import config
from console_log import LOG
from logParserClass import logParser

LOG_FOLDER = "C:\\repos\\trash\\nordic\\Python\\logs"   # root location for test logs


# Test with non-existing path
def test_nonexisting_path ():
    ret_code = subprocess.call(["python", "log_parser.py", os.path.join(LOG_FOLDER, "Invalid")])
    if ( ret_code != 0 ):
        LOG ( "TEST", "Non-existing path test: Passed" )
    else:
        LOG ( "TEST", "Non-existing path test: Failed" )

# Test with existing path, create log for all xml files
def test_generate_report_all ():    
    ret_code = subprocess.call(["python", "log_parser.py", LOG_FOLDER])
    if ( ret_code == 0 ):
        LOG ( "TEST", "Existing path test: Passed" )
    else:
        LOG ( "TEST", "Existing path test: Failed" )


# Create log only for xml files from /gap/sec folder
def test_generate_report_single ():  
    ret_code = subprocess.call(["python", "log_parser.py", os.path.join(LOG_FOLDER, "gap", "sec")])
    if ( ret_code == 0 ):
        LOG ( "TEST", "Single folder path test: Passed" )
    else:
        LOG ( "TEST", "Single folder path test: Failed" )


# Test pass, fail, skip counts using get_result_by_type()
def test_get_result_by_type (): 
    lp = logParser("xml")
    lp.process_logs(os.path.join(LOG_FOLDER, "gatts", "gaw"))
    test_result = 0
    if not lp.get_result_by_type(config.TEST_RES_PASS) == 3:
        LOG ( "TEST", "pass count test: failed" )
        test_result = 1
        
    if not lp.get_result_by_type(config.TEST_RES_FAIL) == 2:
        LOG ( "TEST", "fail count test: failed" )
        test_result = 1

    if not lp.get_result_by_type(config.TEST_RES_SKIP) == 1:
        LOG ( "TEST", "skip count test: failed" )
        test_result = 1

    if test_result == 0:
        LOG ( "TEST", "pass/fail/skip count test: passed" )

# Test process_logs()
def test_process_logs (): 
    lp = logParser("xml")
    lp.process_logs(os.path.join(LOG_FOLDER, "gatts", "gaw"))
    test_dict = lp.get_process_logs()

    """
     expectations:
     1. Only 1 test suite info expected as the specified log folder 
     contains only one xml file with exactly one test suite
     2. expected test_suite_id = "GATTS_GAW_001"
     3. expected test count 
     5. expected pass, fail and skip counts
     
    """
    exp_test_suite_count = 1
    exp_test_suite_id = "GATTS_GAW_001"
    exp_total_tests = 6
    exp_passed = 3
    exp_failed = 2
    exp_skipped = 1

    test_suite_count = 0
    test_suite_id = ""
    total_tests = 0
    passed = 0
    failed = 0
    skipped = 0

    for d in test_dict:
        test_suite_id = d["test_suite_id"]
        test_suite_count = test_suite_count + 1
        for tcr in d["tc_results"]:
            test_result = tcr["test_result"]
            
            total_tests = total_tests + 1
            if test_result == "PASS":
                passed = passed + 1
            if test_result == "FAIL":
                failed = failed + 1
            if test_result == "SKIP":
                skipped = skipped + 1
            
    # verify the contents
    test_result = 0
    if not exp_test_suite_count == test_suite_count:
        LOG ( "TEST", "test suite count test: failed" )
        test_result = 1

    if not exp_test_suite_id == test_suite_id:
        LOG ( "TEST", "test suite ID test: failed" )
        test_result = 1

    if not exp_total_tests == total_tests:
        LOG ( "TEST", "total test count test: failed" )
        test_result = 1

    if not exp_passed == passed:
        LOG ( "TEST", "pass count test: failed" )
        test_result = 1

    if not exp_failed == failed:
        LOG ( "TEST", "fail count test: failed" )
        test_result = 1

    if not exp_skipped == skipped:
        LOG ( "TEST", "skip count test: failed" )
        test_result = 1


    if test_result == 0:
        LOG ( "TEST", "process_logs() test: passed" )

#------------------------------------------------------------------------------

# Execute tests
LOG ( "TEST", "Running Test 1" )
test_nonexisting_path () 
LOG ( "TEST", "Running Test 2" )
test_get_result_by_type ()
LOG ( "TEST", "Running Test 3" )
test_process_logs () 
LOG ( "TEST", "Running Test 4" )
test_generate_report_single () 
LOG ( "TEST", "Running Test 5" )
test_generate_report_all ()

# End of file  

