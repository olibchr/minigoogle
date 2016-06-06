# minigoogle

This is a simple Search Engine

It uses PHP to call an external program and then display the results.

The mongoose program is a lightweight and no-installation-required 
webserver which serves the files in the current directory it is in.
http://code.google.com/p/mongoose/

First, startup mongoose by

./mongoose

It will (by default) serve documents on port 8080

From a browser you can type:

http://localhost:8080/

You can do queries at this address. The interesting part tho is the webcrawling.

To build up a bag of words, run 

./webspider apple.com 

or any other URL to start from. The webcrawler parses the webpages, puts links in the pocket and continues on found links. (Don't forget to turn it off and be careful to not query one head to much or they will think that theyre under attack. No resposibility taken.)

