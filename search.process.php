#!/usr/bin/php5-cgi


<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="/favicon.ico">

    <title>LIACS SEARCH</title>

    <!-- Bootstrap core CSS -->
    <link href="dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
    <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
    <script src="assets/js/ie-emulation-modes-warning.js"></script>

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->

    <!-- Custom styles for this template -->
    <link href="dist/css/carousel.css" rel="stylesheet">
    <link href="dist/css/cover.css" rel="stylesheet">
  </head>
<!-- NAVBAR
================================================== -->
  <body>
    <div class="navbar-wrapper">
      <div class="container">

        <nav class="navbar navbar-inverse navbar-static-top">
          <div class="container">
            <div class="navbar-header">
              <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
                <span class="sr-only">Toggle navigation</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
              </button>
              <a class="navbar-brand" href="#" style="color: white">LIACS Search Engine</a>
            </div>
            <div id="navbar" class="navbar-collapse collapse">
              
                <div class="container" style="float:right; margin-top: 5px; color: white">
                <h4>Oliver Becher</h4></div>
            </div>

            </div>
          </div>
        </nav>

      </div>
    </div>


    <!-- Carousel
    ================================================== -->
   

    <!-- Marketing messaging and featurettes
    ================================================== -->
    <!-- Wrap the rest of the page in another container to center all the content. -->
 <div class="container marketing">
    <hr class="featurette-divider" style="visibility:hidden">

     <div class="row featurette">
        <div class="col-md-10 col-md-push-1">
            
      <form class="form-signin" enctype="multipart/form-data" action="./search.process.php" method="post">
        <h2 class="form-signin-heading"></h2>
        <label for="inputEmail" class="sr-only">Enter your search</label>
        <input type="input" name="myquery" class="form-control" placeholder="Enter your search" required autofocus>
        
        </br>
        <button class="btn btn-lg btn-primary btn-block" type="submit" value="search">Search</button>
      </form> 
          <h3 class="featurette-heading"><span class="text-muted"></span></h3>
          <p class="lead" align="left"></p>
        </div>
      </div>
<div class="row featurette">
        <div class="col-md-1 col-md-push-5">
          <h3 class="featurette-heading"><span class="text-muted"></span></h3>
          <p class="lead" align="left"></p>
        </div>
      </div>
     <!-- START THE FEATURETTES -->
<div class="row featurette">
        <div class="col-md-10 col-md-push-1">
<?php

echo '<hr><p><font size=6 color=darkblue>Results</font><p>';

// $myquery contains the query from the search engine
$myquery = $_POST['myquery']; 

// Write the query terms to a local file to minimize security problems
$handle  = fopen("queryterms.txt","w");
fwrite($handle,$myquery);
fclose($handle);

// $commandstring = './myprog ';
$commandstring = './webquery ';

//Using backticks one way for PHP to call an external program and return the output
$myoutput =`$commandstring`;

echo '<p>(1) ';
echo $myoutput.'<p>';
?>
</div>
      </div>
      <!-- FOOTER -->
      <footer>
        <p class="pull-right"><a href="#">Back to top</a></p>
        <p>&copy; 2016 Becher &middot; <a href="#">Privacy</a> &middot; <a href="#">Terms</a></p>
      </footer>

    </div><!-- /.container -->


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script>
    <script src="dist/js/bootstrap.min.js"></script>
    <!-- Just to make our placeholder images work. Don't actually copy the next line! -->
    <script src="assets/js/vendor/holder.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="assets/js/ie10-viewport-bug-workaround.js"></script>
  </body>
</html>



