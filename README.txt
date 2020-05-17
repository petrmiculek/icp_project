Authors:
Kryštof Lavinger <xlavin00@stud.fit.vutbr.cz>
Petr Mičulek, <xmicul08@stud.fit.vutbr.cz>

User manual to ICP - the simulation of Shalingrad's subway system.

General:
    This application simulates public transport in a town.
    Vehicles spawn at their start stop at given departure time and despawn at their last stop when they reach it.
    Spawn times, vehicles' routes and stops are loaded from JSON files in examples/.

Graphics:
    Streets are represented by a line with a label along it (street name).
    Vehicles and stops are represented by a circle item with a character inside.
    For vehicles, the character is the number of their line (we use the term line and trip interchangably).
    All vehicles on the same line share a common color.
    For stops, the character is the first letter of the stop's name.

Time:
    Time speed can be changed from 0.0x to 1000x of the normal speed.
    Holding the Faster/Slower buttons keeps increasing/decreasing the time speed.
    The longer the button is held, the faster the speed changes.

    Vehicles stop at given stops to wait for a random amount of time.
    The first vehicle departs (spawns) at 00:00:05.

Interacting with the map:
    Streets can be selected to modify their traffic density, from no traffic to full on traffic jam.
    The traffic density is a linear factor to the speed of all vehicles on the given street.
    Maximum traffic density (traffic jam) means that the vehicles stop completely.
    More streets can be selected at once by holding the ctrl button while selecting them by clicking at the streets lines.

    Selecting a vehicle highlights its route in the map.
    Clicking anywhere else on the map (deselecting the vehicle) removes the highlight.

    Highlighting a line route can also be accomplished by clicking the line name in the list of lines on the right.
    Clicking anywhere else in the list (other than the Line X item) removes the highlight.

    There is no closing of streets and there are no detour routes.

Zoom and view control:

    When zoomed in, move up/down vertically by regular scrolling. Move horizontally through alt + scroll (unlike usual).
    Zooming can only be done through the + and - buttons.
    Changing zoom level keeps a constant size of all map elements (circles dimensions, text size, line thickness etc.).


