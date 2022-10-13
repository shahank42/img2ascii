# img2ascii
A program to render bitmap image files as ASCII characters.

# Usage
Download the latest version from the releases page.
Simply drag-and-drop the bitmap image file onto the executable file, and fill in the appropriate parameters.

![Demo](https://github.com/shahank42/img2ascii/blob/main/demos/demo-v0.1.gif?raw=true)

* Image Contrast Factor should be a value between 1 and 255.

# Implementation Details
This project was really more of an effort to show that no external libraries should be necessary for image processing; that everything can be achieved through vanilla C++17.

In hindsight this philosophy was erroneous and it is definitely better to use an existing library such as OpenCV for this purpose, and in general any other computer vision related projects.

Some of the resources and repositories I used are listed under:
* https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/reading-writing-images: To get an idea of the general projet structure, and find out the various techniques used in computer vision.
* https://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-5-contrast-adjustment/: For the contrast adjustment algorithm.
* https://github.com/kbuffardi/Bitmap: For the code on loading bitmap images. Thanks a bunch!
* https://bitesofcode.wordpress.com/2017/01/19/converting-images-to-ascii-art-part-1/ and https://bitesofcode.wordpress.com/2017/05/27/converting-images-to-ascii-art-part-2/: Super helpful write-ups that explain the entire workflow of the program in an easy-to-understand fashion.

# Known Issues
* No user input validation done yet (so it's super easy to break the program).
* Providing an ASCII Image Width longer than the actual number of horizontal pixels in the image causes a memory leak, and temporary freezing of the OS.
* Slight perspective issue for vertical images (width < height), still thinking of an algorithm that can adjust ASCII-Image proportions to look natural according to a provided constant factor (squishification factor?).
