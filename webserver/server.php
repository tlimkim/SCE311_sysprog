/*
 * Developed by Jiwon Jeon
 *
 * Simple php script for checking photo shot result.
 */
<html>
<head>

</head>
<body>

<?php
  $imageDir = "./";
  $images=glob($imageDir.'2018*');
  foreach($images as $image) {
      echo '<h1>'.basename($image).'</h1>';
    echo '<div><img id="img'.'"src="'.$image.'" width=40%/></div>';
  }
?>
</body>
</html>