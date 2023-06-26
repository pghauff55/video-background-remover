# video-background-remover
Webcam Video Backgrounder Remover



Video Background Remover

P.  Hauff
Version  1.0

## Video Background Remover
This code is a C++ program that uses the OpenCV library to perform video camera processing. It begins by creating a VideoCapture object with the parameter 0 that indicates that the camera device number 0 should be used. It then checks to see if the camera was opened successfully, and if not, exits the program. It then gets the width and height of the frame from the video capture and stores them in dWidth and dHeight. After that, it creates a window named MyVideo.

It then reads the first frame from the camera into frame_still and checks again to see if the read was successful. If not, it breaks out of the loop. It then sets the size of the template that will be used when performing template matching and gets the number of columns and rows for the image by dividing the frame width and height by the size of the template respectively.

It then has a while loop which reads a new frame from the camera and stores it in frame. It then applies a Gaussian blur to the frame and performs template matching to find the best matches for the given template. 

If a match is found, it is highlighted in the frame. It then checks each pixel in the frame against the pixel in the original frame_still to find the delta color for each pixel. If the delta color is smaller than a given threshold, it will change the pixel color to black. It then displays the frame in the MyVideo window and waits for a key press. If the key is Esc, it will break out of the loop. If the key is space, it will read a new frame into frame_still, apply the same processing as before and store the results into a vector. 

After that, the loop will start again. Finally, it returns 0 when the loop has completed.

##Introduction
This code is written in C++ and uses OpenCV to process a Mat frame_still.
The code is used to "map" the image by dividing it into smaller pieces,
by setting the crop width (crop_width) and crop height (crop_height) to the
given template size and finding the number of pieces N and M in the image.

The code then starts a loop from 0 to M (rows) and inside this loop, another loop from 0 to N (columns) is created.
The offset_x and offset_y values are used to define a rectangular region of interest (ROI) which is used to crop
the image into pieces. The cropped pieces are then pushed into a vector (result) and the total number of pieces
is returned. 

The code also checks whether any features are detected using feature detection and skips any pieces with no features. 

## Class Index
## Class List
Here are the classes, structs, unions and interfaces with brief descriptions:

## Class Documentation
### BackGroundRemover Class Reference
#### Public Member Functions
### BackGroundRemover (int WIDTH, int HEIGHT)
void UpdateBackground ()
void ColoriseBackground ()
void Show ()
void Process (int tile_size)
void Save_Tiles (int tile_size)
void Create_Tiles (int tile_size)
void Colorise (float value0, float value1, float value2, int *value_out0, int *value_out1, int *value_out2)
void Colorise2 (float value0, float value1, float value2, int *value_out0, int *value_out1, int *value_out2)

### Constructor & Destructor Documentation
### BackGroundRemover::BackGroundRemover (int    WIDTH, int    HEIGHT)[inline]

Open video capture device 0.
if not success, exit program
get the width of frames of the video
get the height of frames of the video
read a new frame from video
if not success, break loop

## Member Function Documentation
### void BackGroundRemover::Process (int    tile_size)[inline]

read a new frame from video
if not success, break loop
Fill Rectangles from returned vector list_tile_size of rectangles from grid
show the frame in "MyVideo" window
#### void BackGroundRemover::Save_Tiles (int    tile_size)[inline]
### Save Tiles 
#### void BackGroundRemover::Show ()[inline]

read a new frame from video
if not success, break loop
show the frame in "MyVideo" window
#### void BackGroundRemover::UpdateBackground ()[inline]

read a new frame from video
if not success, break loop

The documentation for this class was generated from the following file:
image-cap.cpp
