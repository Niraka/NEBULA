Nebula game engine

==================================================================================================
==================================================================================================
==================================================================================================
The what and the why

The Nebula game engine is a personal project that has been meandering about in the early design
stages for quite some time. I have finally decided to bring it to life for a few reasons:

- As a demonstration of my ability as a programmer
- To use as a basis for the production of my own computer games
- For fun :D

The full feature list (as of the time of writing) is detailed below, but generally speaking this
engine aims to be a middle of the pack engine that does everything reasonably well without going 
over the top.

=================================================
FAQ:

Q. Why 'Nebula'?
A. Mostly just because I like the word.

Q. Can I use this engine, or portions of it (commercially or otherwise)?
A. Not yet. I may attach an appropriate license in the future.

Q. Contributions?
A. Contributions currently not being accepted as this project is meant as an indicator of my
own ability. Sorry.


==================================================================================================
==================================================================================================
==================================================================================================
Github 

Each GitHub commit will use a prefix to quickly identify the purpose of the commit. Example usage:
[FTR] Implemented concurrent map
[BUG] Fixed out of bounds array access

The prefixes are:
[FTR] For the implementation of a new feature, regardless of size. Feature tags are also used for
      additions to in-development features.
[BUG] For fixing a bug
[MOD] For modification of existing features. Note that a modification differs from a feature because
      a modification involves a change in a features purpose.
[DOC] For documentation changes/updates
[MUL] For a commit that meets more than one criteria. In this case the other prefixes should be added
      to the commit message.
[MSC] A catch-all for miscellaneous updates that don't match any other categories

==================================================================================================
==================================================================================================
==================================================================================================
Design (in brief)

Engine layers

- The system layer
Contains a series of tools and core systems that are to be utilised by all other systems.

- The resource layer
Handles the storage, access and general management of engine resources. Provides easy,
centralised access to objects.

- The module layer
Contains the modules that make up the bulk of the engine functionality. For example, the graphics
module, audio module, artificial intelligence module and entity component module. Many modules
depend on one another, and so internally this layer contains its own layer structure.

- The content layer
Supports the creation of game content via a "modding" system. Mods contain a series of
configurations and scripts whilst scripts contain a series of instructions that translate
to engine actions. Scripts are just glorified text files.

=================================================
Libraries

Confirmed:
GLM         : Mathematics library that works well with GLSL
GLEW        : Extension acquisition for OpenGL
GLFW        : Windowing and input for OpenGL
FreeType2   : Font rendering

Awaiting approval:
PhysX       : GPU-based physics from Nvidia
POCO        : Networking library
FMOD        : Audio library

=================================================
Layer features

========================
System layer
> Task-based threading system
> Logging system
> Networking system
> Platform information
> File system I/O
> Property management
> Containers
> Events system
> Timing system
> A collection of tools that are used freely throughout the engine such as:
  > Language extensions
  > Containers
  > Common structures
  > Common function objects

========================
Resource layer
> Statistic tracking on resource usage over time
> Garbage collection
> Automatic container storage
> High performance container insert/delete/retrieve
> Caching scheme
> Memory pooling (potentially)

========================
Module layer
> Almost all modules use a 'frame' system that allows items to be allocated to one
  frame (or "scene"). E.G: Graphics has two frames, one for level_frame and one for
  loading_screen_frame
> Modules may specify dependencies
  > E.G PhysicsModule.addDependency(TransformModule)

> Graphics module
  > Using OpenGL
  > Shadows (shadow maps)
	> Can be enabled/disabled
	> Quality can be adjusted (Low/Med/High)
  > Multi-light with automatic management
	> (E.G: Enabling lights by distance to viewer)
  > Multi-pass rendering
  > Multiple cameras
  > Cube maps (Skyboxes)
  > Particle systems
  > Animation & models
  > Not going to bother with directX for now
  
> Audio module
  > Using FMOD
  > Multiple audio channels (named for user convenience)
  > 3D sound relative to listener
  > Limiting number of concurrent sounds
  > Background music with multiple modes
    > Music stack
	> Random play-list
	> Music queue
    
> Physics module
  > Very basic stuff using PhysX (Probably)
  > More details to come after research has been done
  
> Window module
  > Create window
  > Delete window
  > Query-able event queue
  > Basic window adjustments
    > Resolution
	> Position
	> Size
	> Full-screen Y/N

> Transformation module
  > Handles transformations. Depended upon by many other modules
    (Translation, scale, rotation)
	
> Entity component module
  > Entity defined as a series of parts
  > Entity type can be defined to duplicate entities easily
  > Multiple frames. Can move entities between frames
  > Entities have unique ids
  
> Artificial intelligence module
  > Forward chaining
  > Knowledge bases (shared and private)
  > Action outputs (Query-able queues)
  > State machine systems
    > States can contain other state machines or FC
  
> User interface module
  > Exclusively 2 dimensional
  > UI file format
  > UI file loader
  > UI file validator tool
  > Variety of standard components:
    > Frame (Structure component)
	> Image (Just an image)
	> Button (Clickable button)
	> Label (Text)
	> CheckBox (Yes/no)
	> TextBox (Single line of text)
	> TextField (Multi-line text)
	> RadioButton (One yes, many no)
	> Slider (Draggable)
	> ListBox (Scrollable listbox of other components)

========================	
Content layer
> Convert text files to mod/script objects
> Enable/disable individual mods/scripts and instruction sets (E.G: Disable all instance of GetRandom())
> Auto load mods that are placed in mod directory(windows only for now)
> Mod/script validation tool
> Mod/script statistic tracking (call count, processing time, vars allocated, etc)
> Mod/script shared data (option to make variables read-only from self and other scripts)

==================================================================================================
==================================================================================================
==================================================================================================

Approximate Schedule of Development (On-going modification):

See "TASKLIST.txt"