/**
This engine build config is a definition-only file that contains a number of switches used to 
configure the output of a build of the nebula engine.

Note:
All definitions should prefix with either 'SWITCH' or 'NEB'

@date edited 25/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef ENGINE_BUILD_CONFIG_H
#define ENGINE_BUILD_CONFIG_H

///////////////////////////////////////////////////////////////////////////////
// Switches

#define SWITCH_CONFIGURATION_DEBUG
#define SWITCH_USE_LOGGING
#define SWITCH_USE_STAT_TRACKING
#define SWITCH_USE_LAUNCH_MENU
#define SWITCH_USE_CONTAINER_CHECKS

///////////////////////////////////////////////////////////////////////////////
// Defines based on switches

#ifdef SWITCH_CONFIGURATION_DEBUG
	#define NEB_CONFIG_DEBUG
	#ifdef SWITCH_USE_LOGGING
		#define NEB_USE_LOGGING
	#endif
	#ifdef SWITCH_USE_STAT_TRACKING
		#define NEB_USE_STAT_TRACKING
	#endif
	#ifdef SWITCH_USE_LAUNCH_MENU
		#define NEB_USE_LAUNCH_MENU
	#endif
	#ifdef SWITCH_USE_CONTAINER_CHECKS
		#define NEB_USE_CONTAINER_CHECKS	
	#endif
#endif

#ifndef SWITCH_CONFIGURATION_DEBUG
	#define NEB_CONFIG_RELEASE
#endif

#endif