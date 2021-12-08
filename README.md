# gui++

(Yet another) C++ GUI library.

## Motivation

There are plenty of C++ GUI libraries out there. So why another one?  
Well, most existing c++ libraries were started to be written a long time ago.  
And so they mostly are based on an old C++ standard (most of them before c++03,
some even before c++98).  
Most of them require to subclass, if you want to change the look or if you want to 
handle events.
Newer techniques, like lambdas, are added as an add-on to existing implementations.  
On the other hand, more and more GUI libraries tend to become more script based
and interpreted, like QML.
This results in slower starts, bigger executables (because they need the parser 
and implementation for every scriptable possibility) and lesser debuggable code.
And you need to learn another language (like QML or javascript).

So this library re-envent the wheel, by going forward and make usage of new 
C++11/14 techniques from base on and going backward to programmable interfaces 
instead scriptable.

And it claims to be *realy* completely model-view-control driven.  
No data is stored in a control (and has to be updated on change).
Instead data is always *provided* to the controls and a change requires only a 
redraw of the control.
The *view* or better the *look* is seperated in own functions, so they can be 
changed in whatever you want.
Btw. a basic look of windows, macos and GTK like X11 ist already shiped with.

As a result, the usage is different from other C++ GUI libraries.  
In most cases, there is no need to sub-class any GUI class.

Instead existing classes can be customized either by template parameters (for 
the visual difference) or by functors (for the data).
Indeed there are several classes, like split_views, who have to be customized 
before they can display anything.

And you can add handler for events to any existing windows class without 
the need of sub-classing it.

## Usage

### First steps

Let's start with an example. Of course: "Hello world"!

```C++

#include <gui/win/overlapped_window.h>

int gui_main(const std::vector<std::string>& /*args*/) {  // (1)
  using namespace gui::win;

  main_window main;                 // (2)

  main.create({50, 50, 800, 600});  // (3)
  main.on_destroy(&quit_main_loop); // (4)
  main.set_title("Hello world");    // (5)
  main.set_visible();               // (6)

  return run_main_loop();           // ()
}

```

Nothing realy special here. 

  1. There is a __gui_main__ instead a __main__ or __WinMain__ routine, for 
     platform independence.
  2. Therein a main window **main** is created (on stack) 
  3. And then created  (on screen) with a size.
  4. A handler (quit_main_loop) is registered to be called **on_destroy**.
  5. A title is set (ups, doesn't look like model-view, but let's see later).
  6. The window is make visible (on screen) 
  7. And then a run_main_loop is called (that runs until the window is closed).

### Second step

Let's do a second one.

```C++

#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>

int gui_main(const std::vector<std::string>& /*args*/) {

  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;

  using label_t = basic_label<text_origin_t::center,            // (1)
                              draw::frame::raised_deep_relief>;

  using client_t = vertical_split_view<label_t, label_t>;       // (2)

  client_t client;                                              // (3)

  layout_main_window<border::center_layout<client_t>,           // (4)
                     client_t&> main(client);

  main.on_create([&] () {                                       // (5)
    client.create(main);
    client.set_split_pos(0.5);
    client.first.set_text([&] () { return ostreamfmt("Left (" << client.first.geometry() << ")"); });
    client.second.set_text([&] () { return ostreamfmt("Right (" << client.second.geometry() << ")"); });
  });

  main.create({50, 50, 800, 600});                              // (6)
  main.on_destroy(&quit_main_loop);
  main.set_title("Split view");
  main.set_visible();

  return run_main_loop();
}

```

What's new?

  1. We declare a custom label type, based on a basic_label and we customize it
     with template parameters *\<text_origin_t::center, draw::frame::raised_deep_relief\>*.
  2. We declare a custom client type, based on a vertical_split_view and 
     customized it to hold two of the prior declared labels.
  3. We create our custom client object
  4. And we create our custom main window, based on a layout_main_window with a 
     center_layout that can layout a client_t.
  5. We register a handler (a lambda) to be called, when the main window was 
     created (on_create), and therein we create the client window and register
     text provider for the two labels.
  6. The remaining is the same as before.

So here you can see some of the principles in this library.

  - Customizing ist done by template parameters. Some for the look, some for the 
    layouting, some for the behaviour
  - Events are handled by register an event handler. In most cases a small lambda.

To be continued...

## Building your own app

I assume, you have created a blank project at your prefered git repository
and cloned it to a local directory.

1. Create a Application cpp file, f.e. call it MyApp.cpp and copy the content 
   of the "Hello World" example or the second sample above in it.

2. Add guipp as a submodule and initialize it.

   ```bash

   git submodule add --name guipp https://github.com/r3dl3g/guipp.git libs/guipp
   git submodule update --init --recursive

   ```

3. Create a CMakeLists.txt and add following:

   ```cmake

   cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

   project("MyApp" CXX)

   add_subdirectory(libs/guipp/gui gui)
   add_definitions(${GUIPP_CXX_FLAGS})
   include_directories(${PROJECT_BINARY_DIR} ${GUIPP_INCLUDE_DIRS})

   add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
   target_link_libraries(${PROJECT_NAME} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
   set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD ${GUIPP_CXX_STANDARD})

   ```

4. build it

For linux users:

   ```bash

   mkdir build
   cd build
   cmake ..
   make -j

   ```

For Windows users (using Visual Studio 2022):

   ```cmd

   mkdir build
   cd build
   cmake .. -G"Visual Studio 17 2022" -DCMAKE_CONFIGURATION_TYPES=Debug;Release
   cmake --build . --config Release

   ```


5. Run it

For linux users:

   ```bash

   ./MyApp

   ```

For Windows users

   ```cmd

   cd bin\Release
   MyApp

   ```
