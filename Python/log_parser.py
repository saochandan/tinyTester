#!/usr/bin/env 
# -*- coding: utf-8 -*-

import sys
import os

from logParserClass import logParser
import config
from console_log import LOG

"""
This function prints help message
"""
def print_help ():
    print ("This tool prints test metrics from log files to test_metrics.txt.")
    print ("Location of this file would be same as tool's location.")
    print ("Enter below command to generate the file.")
    print ("python log_parser.py <log folder root path>")

'''
cmd : python log_parser.py <log folder root path>
'''
if __name__ == "__main__":

    if len(sys.argv) < 2:
        LOG ("ERROR", "Log path is not provided");
        print_help ()
        exit (1)
    
    if sys.argv[1] == "-h":
        print_help ()
        exit (0)
        
    log_dir = sys.argv[1]
    if not os.path.exists (log_dir):
        LOG ("ERROR", "Provided log path " + log_dir + " doesn't exist");
        exit (1)
    
    # perform xml log parsing from given path and generate detailed test report
    lp = logParser("xml")
    lp.process_logs(log_dir)
    LOG ("INFO", "total passed: " +str(lp.get_result_by_type(config.TEST_RES_PASS)))
    LOG ("INFO", "total failed: " +str(lp.get_result_by_type(config.TEST_RES_FAIL)))
    LOG ("INFO", "total skipped: " +str(lp.get_result_by_type(config.TEST_RES_SKIP)))
    lp.generate_detailed_report()

 # End of file 
 