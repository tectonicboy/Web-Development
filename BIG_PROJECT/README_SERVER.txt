This is the most advanced project I've ever done. It saw me create a custom database management system from scratch in C++ (including me creating a custom SQL language
and writing an interpreter for it, to be used as the language for the database system), giving it a GUI with wxWidgets in C++,then creating a great-looking website 
and hooking it to said database system using jQuery and PHP to generate SQL commands conforming to the SQL language syntax I created & wrote an Interpreter for.

It involves both client- and server-side web development + Desktop development, in 7 different languages/APIs: 

- C++ for the custom database management system from scratch, including the Interpreter for the custom SQL language syntax.
- wxWidgets to give the database system a GUI in C++.
- HTML for website content.
- CSS for website appearance.
- jQuery for website interactivity + gathering server data from browser & sending it to another remote PHP file.
- PHP for communication between the website and the custom database management system.
- SQL language I created & wrote an interpreter for myself, part of the custom RDBMS from scratch in C++. Used as the database system for the website, instead of MySQL or other.


Overview:

An SVG slider reveals a registration area when the mouse cursor goes there (with mouse tracking in jQuery). Users can input their data and if it's incorrect, errors are shown.

Once they have entered data that's all correct, a Success message is shown, upon which a jQuery AJAX function creates POST variables from said input data, and sends them
to a different .php file.

This file uses these variables to generate an SQL command conforming to the custom SQL language I created, and outputs it to a text file.

Every 10 seconds, the running DBMS (that I created from scratch in C++) opens this file to check whether an SQL command has been added to it. If it detects one, it processes
it accordingly, in most cases calling the SQL Command Interpreter with it, which reports whether it has been successful and alters the DBMS as needed if it was.

The DBMS itself includes both a command-line edition and a GUI edition, the latter of which is included in this web development project. It contains a regular
text area for entering a single SQL command, and a "Schema+" section, which can be used to enter many SQL commands at once, and not only creating a skeleton database
from a schema, but also entering data in it at the same time, hence the "+". Two output windows are present, one for error/success messages resulting from SQL commands,
and one for main output, such as viewing tables from databases, outputting the names of all databases present in the system, etc. A Save/Load subsystem has been written for it,
which writes all vital information about all databases and their contents to a text file using a custom syntax I came up with, and this file is read next time the DBMS fires up,
to load it properly from the state it was in the last time it was used.

This project is mainly for the purposes of increasing my programming knowledge and skills. It has taught me quite a bit about C++, GUI programming with wxWidgets, and both 
front-end and back-end web development.

UPDATE: Apache + PHP are no longer used as backend technologies. Instead, I have gone ahead and written my own HTTP server from scratch in C, which is able to process server side variables from methods such as GET/POST.
