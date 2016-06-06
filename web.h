/**
 *  showlinks: Extract URLs from a webpage
 *  Copyright (C) 2013  Matthijs van Drunen <wvdrunen@liacs.nl>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 **/

// Return the webpage located by myurl
char *GetWebPage(char *myurl);

// Retrieve the last cURL error
const char *GetLastWebError();

// Get all URLs from webpage (as absolute URLs)
char *GetLinksFromWebPage(char *mywebpage, char *base);

// Get all image URLs from webpage (as absolute URLs)
char *GetImageLinksFromWebPage(char *mywebpage, char *base);
