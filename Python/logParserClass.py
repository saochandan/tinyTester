#!/usr/bin/env 
# -*- coding: utf-8 -*-

import os
from abc import ABC, abstractmethod

import xml.etree.ElementTree as ET

import config
from console_log import LOG


class AbstractLogsParser ( ABC ):
    @abstractmethod
    
    
    def __init__ ( self, logs_extension ):
        """
        Base class constructor.
        @param logs_extension Extension of log files to parse.
        """
        self._logs_ext = '.'+logs_extension
        
    def get_result_by_type( self, result_type ):
        """
        Returns number of passed, failed or skipped tests.
        @param result_type Type of results to return. 
        """
        return -1
        
    def generate_detailed_report ( self ):
        """
        Generates detailed report on each test suite.
        """
        raise Exception("generate_detailed_report is not implemented")
        
    def process_logs ( self, folder ):
        """
        Parses all log files with target extension in the specified folder.
        @param folder Folder to look up for log files.
        """
        raise Exception("process_logs is not implemented")
        

class logParser ( AbstractLogsParser ):
    
    def __init__ ( self, logs_extension ):
        """
        Class constructor.
        @param logs_extension Extension of log files to parse.
        """
        self._logs_ext = '.'+logs_extension
        self.test_res_log = list()
        self.passed = 0
        self.failed = 0
        self.skipped = 0
        
    def get_result_by_type( self, result_type ):
        """
        Returns number of passed, failed or skipped tests.
        @param result_type Type of results to return. 
        """
        if result_type == config.TEST_RES_PASS:
            return self.passed
        elif result_type == config.TEST_RES_FAIL:
            return self.failed
        elif result_type == config.TEST_RES_SKIP:
            return self.skipped
        else:
            return -1
        
        
    def generate_detailed_report ( self ):
        """
        Generates detailed report on each test suite.
        
        This function parses a list of dictionaries that contains test details for each
        test suites. It obtains the info and prints them to "test_report.txt" to prepare
        a detailed report.
        """
        with open("test_report.txt",'w', encoding = 'utf-8') as f:
            
            # Create report heading
            f.write("----------------------------------------------------\r")
            f.write("                   Test Report\r")
            f.write("----------------------------------------------------\r\n")
            
            # Add report summary
            f.write("--------------------  Summary ----------------------\r")
            f.write("Total tests: "+str(self.passed+self.failed+self.skipped)+"\r")
            f.write("Passed: "+str(self.passed)+"\r")
            f.write("Failed: "+str(self.failed)+"\r")
            f.write("Skipped: "+str(self.skipped)+"\r")
            f.write("----------------------------------------------------\r\n")
            
            # Add detailed report for eac test suite
            for d in self.test_res_log:
                f.write("Below test details created using: ")
                f.write(d["file"])
                f.write("\r\n")
                
                # Skip detailed reporting in case there was an error and just print 
                # an error message obtained while processing log files
                if "error_log" in d.keys():
                    f.write(d["error_log"])
                    f.write("----------------------------------------------------\r")
                else: 
                    # Prepare a detailed report for test suite
                    test_suite_id = d["test_suite_id"]
                    f.write("Test Suite ID: "+str(test_suite_id)+"\r\n")
                    f.write("----------------------------------------------------\r")
                    f.write("    Test ID			Test Result     Comment\r")
                    f.write("----------------------------------------------------\r")
                    # Prepare test case level report
                    for tcr in d["tc_results"]:
                        test_id = tcr["test_id"]
                        test_result = tcr["test_result"]
                        comment = tcr["comment"]
                        f.write("    "+str(test_id)+"        "+str(test_result)+"        "+str(comment)+"\r\n")
                
                    f.write("----------------------------------------------------\r\n")
                    
            f.write("----------------------------------------------------\r")
            f.write("                   End of Report\r")
            f.write("----------------------------------------------------\r")
        
        LOG("INFO", "Test report created at " + os.path.join( os.getcwd(), "test_report.txt") )

        
    def process_logs(self, folder):
        """
        Parses all log files with target extension in the specified folder.
        @param folder Folder to look up for log files.
        """
        LOG("INFO", "Parsing XML log files from "+folder)
        
        self.test_res_log = [] # this list holds list of test suite result details
        
        # The below loop traverses through all subfolders and root folder to find
        # target extension files 
        for root_dir, dirs, files in os.walk(folder):
            for file in files:
                if (file.endswith(self._logs_ext)):
                    ts_res_log = dict()
                    ts_res_log["file"] = file
                    
                    # Full log file path
                    log_file = os.path.join(root_dir, file)
                    
                    # Parses log file
                    try:
                        tree = ET.parse(log_file)
                    except:
                        msg = "An exception occurred while parsing xml log.\r\n"
                        LOG ( "ERROR", msg )
                        ts_res_log["error_log"] = msg
                    else:
                        root = tree.getroot()
                        ts_res_log["test_suite_id"] = root.get('test_suite')
                        
                        # This list hold list of test case result details
                        tc_res_list = list() 
                        
                        for result in root.findall('tc_result'):
                        
                            # This dict holds test case result details
                            tc_res = dict() 
                            
                            tc_res["test_id"] = result.get('id')
                            tc_res["test_result"] = result.get('result')
                            
                            if tc_res["test_result"] == "SKIP":
                                comment = result.find('reason')
                            else:
                                comment = result.find('debug')
                                
                            if not comment == None:
                                tc_res["comment"] = comment.text
                            else:
                                tc_res["comment"] = "" 
                            
                            test_result = tc_res["test_result"]
                            if test_result == "PASS":
                                self.passed = self.passed + 1
                            elif test_result == "FAIL":
                                self.failed = self.failed + 1
                            elif test_result == "SKIP":
                                self.skipped = self.skipped + 1
                            else:
                                pass
                            
                            # Add this test case result to list
                            tc_res_list.append(tc_res)
                        
                        # Add list of test case results to test suite  
                        ts_res_log["tc_results"] = tc_res_list
                    
                    # Add test suite result to final list of test results
                    self.test_res_log.append(ts_res_log)
    
    #for testing purpose, return test result log
    def get_process_logs(self):
        return self.test_res_log
    
# End of file        
  