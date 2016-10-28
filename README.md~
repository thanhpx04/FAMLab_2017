# MAELab_2016

This software is implemented in 2016.
======================================================================================================
The processes followed:
	- Read image and store into matrix
	- Extract the edge of object from image
	- Break the edges into list of approximate lines.
	- Construct the shape histogram and calculate Bhattacharyya distance to detect the similar between two images.
	- Apply the PHT 
	- Template matching.
	- Build UI test
	- Build console test
======================================================================================================
In details:

16 Sept 2016
Finished:
	- Construct the project and worked with imageModel package
	- Build a matrix class with the same type with the Eigen (a little)

07 Oct 2016
Works finished:
	- Read JPG, save JPG
	- Load JPG to matrix
	- Compute histogram, median, mean of histogram
14 Oct 2016
Finished:
	- Canny algorithm
	- Find the edge (trying) without findcontours of OpenCV
Next:
	- Break the edge into list of lines
	- Compute Geometric Histogram
Problems detected :
	- Load JPG wrong when the input is not JFIF or JFXX (the XMP segment (APP1))

17 Oct 2016
Finished:
	- Apply Canny algorithm into Image class
	- Get the edge of image into list of edges
	- Break the edges into approximate lines.

Next:
	- Construct the shape histogram for image
	- Compare the shape histogram using Bhattacharyya distance

28 Oct 2016
Works:
	- (1) Write the methods to construct the ShapeHistogram of images
	- (2) Write the method to compute Bhattacharyya distance
	- (3) Verify the Canny result with the result from OpenCV. Noted: The Gaussian filter in OpenCV is combined into Sobel method (http://docs.opencv.org/trunk/d4/d86/group__imgproc__filter.html#gacea54f142e81b6758cb6f375ce782c8d)
	- (4) Extract the contours after apply the Canny algorithm
Finished: (1), (2)
Problems:
	- The edge after applying the Canny algorithm are thinner than the edge from OpenCV.
	  The reason: the conditions when considering the weak edge in double threshold stage.








