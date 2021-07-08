#!/usr/bin/env 
# -*- coding: utf-8 -*-

import config 

"""
This function prints log message on console along with the Tag string
@param log_type - Tag string ("ERROR", "TEST", "INFO"). 
@param log_msg - log message to print. 
"""
def LOG ( log_type, log_msg ):
    if log_type == "TEST":
        print ("["+log_type+"] " + log_msg)
    elif not config.TEST_MODE == True:
        print ("["+log_type+"] log_parser: " + log_msg)
 
 # End of file 
 