/**
This file is an enum-only file that defines a number of extremely common responses for other 
classes to utilise. 

Most importantly, these responses alleviate the common issue of having only true and false to
return where a third response indicating error would be useful. For example, a 
"ConvertStringtoBool(string s)" function.

Note that some responses may seem synonymous such as 'true' and 'success'. This is because the
results are intended to read as close to english as possible.

@date edited 11/09/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

enum class StandardResponses
{
	/**
	The result of the function was true */
	TRUE,

	/**
	The result of the function was false */
	FALSE,

	/**
	An error occurred during the execution of the function */
	ERROR,

	/**
	The function succeeded */
	SUCCESS,

	/**
	The function failed */
	FAILURE,

	/**
	An invalid argument was supplied to the function. Whether or not the function succeeded
	is unspecified. */
	INVALID_ARG,

	/**
	An invalid number of arguments was supplied to the function. Whether or not the function
	succeeded is unspecified.  */
	INVALID_ARG_COUNT
};