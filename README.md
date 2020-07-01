# image_browser
An image browser.
*browser* is an image browser to illustrate features of OpenCV.  In particular,
it shows how to read and display and image, as well as to apply affine transform
to reduce the pixel size of the image while preserving its aspect ratio.

browser is invoked by

	browser --rows=480 --cols=640 directory

where the parameters preceded by -- are optional, the values of the parameters
may be changed to other suitable numbers depending on screen size, and the
parameter directory is the name of directory containing all picture files to
be displayed.
