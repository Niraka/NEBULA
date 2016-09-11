/**
This engine build config is a definition-only file that contains a number of switches used to 
configure the output of a build of the nebula engine.

Note:
All definitions should prefix with either 'SWITCH' or 'NEB'

@date edited 10/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef ENGINE_BUILD_CONFIG_H
#define ENGINE_BUILD_CONFIG_H

///////////////////////////////////////////////////////////////////////////////
// Switches

#define SWITCH_CONFIGURATION_DEBUG
#define SWITCH_NO_LOGGING
#define SWITCH_NO_STAT_TRACKING


///////////////////////////////////////////////////////////////////////////////
// Defines based on switches

#ifdef SWITCH_CONFIGURATION_DEBUG
	#define NEB_CONFIG_DEBUG
	#ifdef SWITCH_NO_LOGGING
		#define NEB_NO_LOGGING
	#endif
	#ifdef SWITCH_NO_STAT_TRACKING
		#define NEB_NO_STAT_TRACKING
	#endif
#endif

#ifndef SWITCH_CONFIGURATION_DEBUG
	#define NEB_CONFIG_RELEASE
	#define NEB_NO_LOGGING
	#define NEB_NO_STAT_TRACKING
#endif

#endif